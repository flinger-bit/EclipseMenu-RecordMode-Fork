#include <bot/BotManager.hpp>

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace eclipse::bot {
    void onFrameAdvancedByGame();
}

class $modify(EclipsePlayLayerHooks, PlayLayer) {
    void update(float dt) {
        PlayLayer::update(dt);
        eclipse::bot::onFrameAdvancedByGame();
    }

    void pushButton(int button, bool player2) {
        PlayLayer::pushButton(button, player2);
        eclipse::bot::BotManager::get().recordInput(button, player2, eclipse::bot::InputKind::Press);
    }

    void releaseButton(int button, bool player2) {
        PlayLayer::releaseButton(button, player2);
        eclipse::bot::BotManager::get().recordInput(button, player2, eclipse::bot::InputKind::Release);
    }
};
