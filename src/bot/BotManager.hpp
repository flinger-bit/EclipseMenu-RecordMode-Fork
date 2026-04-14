#pragma once

#include <bot/BotTypes.hpp>

#include <filesystem>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace eclipse::bot {

    class BotManager {
    public:
        static BotManager& get();

        void startRecording(RecordMode mode);
        void stopRecording();
        void startReplay();
        void stopReplay();

        void advanceFrame();
        void resetFrameCounter();

        void recordInput(std::int32_t engineFrame, std::int32_t button, bool player2, InputKind kind);
        void recordInput(std::int32_t button, bool player2, InputKind kind);

        [[nodiscard]] bool isRecording() const;
        [[nodiscard]] bool isReplaying() const;
        [[nodiscard]] RecordMode recordMode() const;
        [[nodiscard]] ReplayState state() const;
        [[nodiscard]] std::int32_t currentFrame() const;
        [[nodiscard]] const std::vector<BotInput>& macro() const;

        bool saveMacro(const std::filesystem::path& path) const;
        bool loadMacro(const std::filesystem::path& path);

        void clear();

        [[nodiscard]] std::filesystem::path defaultMacroPath() const;

    private:
        BotManager() = default;

        void appendOrReplace(BotInput input);
        std::int32_t effectiveFrame(std::int32_t engineFrame) const;

        mutable std::mutex m_mutex;
        std::vector<BotInput> m_macro;
        RecordMode m_mode = RecordMode::Normal;
        ReplayState m_state = ReplayState::Idle;
        std::int32_t m_engineFrame = 0;
        std::int32_t m_customFrame = 0;
        std::optional<std::filesystem::path> m_lastLoadedPath;
    };
}
