#include <bot/BotManager.hpp>

#include <algorithm>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace eclipse::bot {

    namespace {
        constexpr std::size_t headerSize = sizeof(MacroFileHeader);
    }

    BotManager& BotManager::get() {
        static BotManager instance;
        return instance;
    }

    void BotManager::startRecording(RecordMode mode) {
        std::scoped_lock lock(m_mutex);
        m_mode = mode;
        m_state = ReplayState::Recording;
        m_macro.clear();
        m_engineFrame = 0;
        m_customFrame = 0;
    }

    void BotManager::stopRecording() {
        std::scoped_lock lock(m_mutex);
        if (m_state == ReplayState::Recording) {
            m_state = ReplayState::Idle;
        }
    }

    void BotManager::startReplay() {
        std::scoped_lock lock(m_mutex);
        m_state = ReplayState::Replaying;
        m_engineFrame = 0;
        m_customFrame = 0;
    }

    void BotManager::stopReplay() {
        std::scoped_lock lock(m_mutex);
        if (m_state == ReplayState::Replaying) {
            m_state = ReplayState::Idle;
        }
    }

    void BotManager::advanceFrame() {
        std::scoped_lock lock(m_mutex);
        ++m_engineFrame;
        if (m_state == ReplayState::Recording && m_mode == RecordMode::FrameStepper) {
            ++m_customFrame;
        }
    }

    void BotManager::resetFrameCounter() {
        std::scoped_lock lock(m_mutex);
        m_engineFrame = 0;
        m_customFrame = 0;
    }

    std::int32_t BotManager::effectiveFrame(std::int32_t engineFrame) const {
        return m_mode == RecordMode::FrameStepper ? m_customFrame : engineFrame;
    }

    void BotManager::appendOrReplace(BotInput input) {
        if (!m_macro.empty()) {
            auto& last = m_macro.back();
            if (last.frame == input.frame && last.button == input.button && last.player2 == input.player2) {
                last.kind = input.kind;
                return;
            }
            if (input.frame < last.frame) {
                input.frame = last.frame;
            }
            if (input.frame == last.frame && input.button == last.button && input.player2 == last.player2) {
                last.kind = input.kind;
                return;
            }
        }
        m_macro.push_back(input);
    }

    void BotManager::recordInput(std::int32_t engineFrame, std::int32_t button, bool player2, InputKind kind) {
        std::scoped_lock lock(m_mutex);
        if (m_state != ReplayState::Recording) {
            return;
        }

        if (engineFrame > m_engineFrame) {
            m_engineFrame = engineFrame;
        }

        auto frame = effectiveFrame(engineFrame);
        if (!m_macro.empty() && frame < m_macro.back().frame) {
            frame = m_macro.back().frame;
        }

        appendOrReplace(BotInput {
            .frame = frame,
            .button = button,
            .player2 = player2,
            .kind = kind,
        });
    }

    void BotManager::recordInput(std::int32_t button, bool player2, InputKind kind) {
        recordInput(m_engineFrame, button, player2, kind);
    }

    bool BotManager::isRecording() const {
        std::scoped_lock lock(m_mutex);
        return m_state == ReplayState::Recording;
    }

    bool BotManager::isReplaying() const {
        std::scoped_lock lock(m_mutex);
        return m_state == ReplayState::Replaying;
    }

    RecordMode BotManager::recordMode() const {
        std::scoped_lock lock(m_mutex);
        return m_mode;
    }

    ReplayState BotManager::state() const {
        std::scoped_lock lock(m_mutex);
        return m_state;
    }

    std::int32_t BotManager::currentFrame() const {
        std::scoped_lock lock(m_mutex);
        return m_mode == RecordMode::FrameStepper ? m_customFrame : m_engineFrame;
    }

    const std::vector<BotInput>& BotManager::macro() const {
        return m_macro;
    }

    void BotManager::clear() {
        std::scoped_lock lock(m_mutex);
        m_macro.clear();
        m_engineFrame = 0;
        m_customFrame = 0;
        m_state = ReplayState::Idle;
    }

    std::filesystem::path BotManager::defaultMacroPath() const {
        return std::filesystem::current_path() / "eclipse_record_mode.macro";
    }
}
