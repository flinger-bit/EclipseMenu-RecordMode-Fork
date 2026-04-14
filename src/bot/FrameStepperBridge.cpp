#include <bot/BotManager.hpp>

namespace eclipse::bot {

    class FrameStepperBridge {
    public:
        static FrameStepperBridge& get() {
            static FrameStepperBridge instance;
            return instance;
        }

        void setEnabled(bool enabled) {
            m_enabled = enabled;
            if (!enabled) {
                m_pendingStep = false;
            }
        }

        bool enabled() const {
            return m_enabled;
        }

        void requestStep() {
            m_pendingStep = true;
        }

        bool consumeStep() {
            if (!m_enabled || !m_pendingStep) {
                return false;
            }
            m_pendingStep = false;
            BotManager::get().advanceFrame();
            return true;
        }

        void notifyFrameAdvanced() {
            if (m_enabled) {
                BotManager::get().advanceFrame();
            }
        }

    private:
        bool m_enabled = false;
        bool m_pendingStep = false;
    };

    FrameStepperBridge& getFrameStepperBridge() {
        return FrameStepperBridge::get();
    }

    void setFrameStepperEnabled(bool enabled) {
        FrameStepperBridge::get().setEnabled(enabled);
    }

    void requestFrameStepperStep() {
        FrameStepperBridge::get().requestStep();
    }

    bool consumeFrameStepperStep() {
        return FrameStepperBridge::get().consumeStep();
    }

    void onFrameAdvancedByGame() {
        FrameStepperBridge::get().notifyFrameAdvanced();
    }
}
