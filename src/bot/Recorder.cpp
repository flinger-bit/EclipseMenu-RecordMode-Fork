#include <bot/BotManager.hpp>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <limits>
#include <span>

namespace eclipse::bot {

    namespace {
        template <class T>
        void writePod(std::ostream& out, T const& value) {
            out.write(reinterpret_cast<char const*>(&value), static_cast<std::streamsize>(sizeof(T)));
        }

        template <class T>
        bool readPod(std::istream& in, T& value) {
            return static_cast<bool>(in.read(reinterpret_cast<char*>(&value), static_cast<std::streamsize>(sizeof(T))));
        }
    }

    bool BotManager::saveMacro(const std::filesystem::path& path) const {
        std::scoped_lock lock(m_mutex);

        std::ofstream out(path, std::ios::binary);
        if (!out.is_open()) {
            return false;
        }

        MacroFileHeader header;
        header.mode = static_cast<std::uint32_t>(m_mode);
        writePod(out, header);

        std::uint64_t count = static_cast<std::uint64_t>(m_macro.size());
        writePod(out, count);

        for (auto const& input : m_macro) {
            writePod(out, input.frame);
            writePod(out, input.button);
            std::uint8_t player2 = input.player2 ? 1u : 0u;
            std::uint8_t kind = static_cast<std::uint8_t>(input.kind);
            writePod(out, player2);
            writePod(out, kind);
        }

        return static_cast<bool>(out);
    }

    bool BotManager::loadMacro(const std::filesystem::path& path) {
        std::scoped_lock lock(m_mutex);

        std::ifstream in(path, std::ios::binary);
        if (!in.is_open()) {
            return false;
        }

        MacroFileHeader header;
        if (!readPod(in, header)) {
            return false;
        }
        if (header.magic != macroMagic() || header.version != 1) {
            return false;
        }

        std::uint64_t count = 0;
        if (!readPod(in, count)) {
            return false;
        }
        if (count > 1'000'000) {
            return false;
        }

        std::vector<BotInput> macro;
        macro.reserve(static_cast<std::size_t>(count));

        for (std::uint64_t i = 0; i < count; ++i) {
            BotInput input;
            std::uint8_t player2 = 0;
            std::uint8_t kind = 0;
            if (!readPod(in, input.frame) || !readPod(in, input.button) || !readPod(in, player2) || !readPod(in, kind)) {
                return false;
            }
            input.player2 = player2 != 0;
            input.kind = static_cast<InputKind>(kind);
            if (!macro.empty() && input.frame < macro.back().frame) {
                input.frame = macro.back().frame;
            }
            if (!macro.empty() && macro.back().frame == input.frame && macro.back().button == input.button && macro.back().player2 == input.player2) {
                macro.back().kind = input.kind;
                continue;
            }
            macro.push_back(input);
        }

        m_macro = std::move(macro);
        m_mode = static_cast<RecordMode>(std::min<std::uint32_t>(header.mode, 1u));
        m_state = ReplayState::Idle;
        m_engineFrame = 0;
        m_customFrame = 0;
        m_lastLoadedPath = path;
        return true;
    }
}
