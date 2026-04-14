#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace eclipse::bot {

    enum class RecordMode : std::uint8_t {
        Normal = 0,
        FrameStepper = 1,
    };

    enum class ReplayState : std::uint8_t {
        Idle = 0,
        Recording = 1,
        Replaying = 2,
    };

    enum class InputKind : std::uint8_t {
        Press = 0,
        Release = 1,
    };

    struct BotInput {
        std::int32_t frame = 0;
        std::int32_t button = 0;
        bool player2 = false;
        InputKind kind = InputKind::Press;
    };

    struct MacroFileHeader {
        std::uint32_t magic = 0x4D435242; // "BRCM"
        std::uint32_t version = 1;
        std::uint32_t mode = 0;
        std::uint32_t reserved = 0;
    };

    inline constexpr std::uint32_t macroMagic() {
        return 0x4D435242;
    }

    inline std::string toString(RecordMode mode) {
        switch (mode) {
            case RecordMode::Normal: return "Normal";
            case RecordMode::FrameStepper: return "FrameStepper";
        }
        return "Normal";
    }
}
