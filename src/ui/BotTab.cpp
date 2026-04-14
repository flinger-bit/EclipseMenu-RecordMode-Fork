#include <bot/BotManager.hpp>

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

namespace eclipse::bot {
    void setFrameStepperEnabled(bool enabled);
}

namespace eclipse::ui {
    namespace {
        void afterModeChosen(bool frameStepper) {
            auto& bot = eclipse::bot::BotManager::get();
            bot.startRecording(frameStepper ? eclipse::bot::RecordMode::FrameStepper
                                            : eclipse::bot::RecordMode::Normal);
            eclipse::bot::setFrameStepperEnabled(frameStepper);
        }
    }

    void openRecordModePopup() {
        auto popup = geode::createQuickPopup(
            "Record Mode",
            "Choose how this recording session should track frames.",
            "Normal",
            "Frame Stepper",
            280.f,
            [](FLAlertLayer*, bool secondButton) {
                afterModeChosen(secondButton);
            },
            true,
            true
        );
        if (popup) {
            popup->m_noElasticity = true;
        }
    }

    void openStopRecordingPopup() {
        auto popup = geode::createQuickPopup(
            "Stop Recording",
            "Save the current macro to disk?",
            "Discard",
            "Save",
            280.f,
            [](FLAlertLayer*, bool save) {
                auto& bot = eclipse::bot::BotManager::get();
                if (save) {
                    bot.saveMacro(bot.defaultMacroPath());
                }
                bot.stopRecording();
                eclipse::bot::setFrameStepperEnabled(false);
            },
            true,
            true
        );
        if (popup) {
            popup->m_noElasticity = true;
        }
    }

    void openLoadReplayPopup() {
        auto& bot = eclipse::bot::BotManager::get();
        bot.loadMacro(bot.defaultMacroPath());
        bot.startReplay();
    }
}
