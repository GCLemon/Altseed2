#pragma once

#include <hidapi.h>
#include <stdio.h>

#include <array>
#include <chrono>
#include <cmath>

#include "../Window/Window.h"
#include "ButtonState.h"

namespace Altseed {

enum class JoystickType : int32_t {
    Other,
    DualShock3 = 616,
    DualShock4 = 1476,
    DualShock4Slim = 2508,
    XBOX360 = 654,
    JoyconL = 8198,
    JoyconR = 8199,
};

enum class JoystickButtonType : int32_t {
    Start,  /// Joycon plus
    Select,  /// Joycon minus

    Home,
    Capture,

    LeftUp,
    LeftDown,
    LeftLeft,
    LeftRight,
    LeftPush,

    RightUp,  ///< PSx Triangle
    RightRight,  ///< PSx Circle
    RightLeft,  ///< PSx Square
    RightDown,  ///< PSx Cross
    RightPush,

    L1,
    R1,
    L2,  // Joycon ZL
    R2,  // Joycon ZR
    L3,  // Joycon SL
    R3,  // Joycon SR

    LeftStart,  ///< XBOX360 Start, PS4 Options
    RightStart,  ///< XBOX360 Select, PS4 TouchPad

    Max,
};

enum class JoystickAxisType : int32_t {
    LeftH,
    LeftV,
    RightH,
    RightV,
    L2,
    R2,

    Max,
};

class Joystick : public BaseObject {
private:
    static const int MAX_AXES_NUM = 10;
    static const int MAX_BUTTONS_NUM = 30;
    static const int MAX_JOYSTICKS_NUM = 16;

    static std::shared_ptr<Joystick> instance_;

    uint8_t globalCount_;

    std::array<hid_device*, MAX_JOYSTICKS_NUM> handler_;

    std::array<JoystickType, MAX_JOYSTICKS_NUM> types_;
    std::array<std::u16string, MAX_JOYSTICKS_NUM> names_;

    std::array<std::array<bool, MAX_BUTTONS_NUM>, MAX_JOYSTICKS_NUM> currentHit_;
    std::array<std::array<bool, MAX_BUTTONS_NUM>, MAX_JOYSTICKS_NUM> preHit_;
    std::array<std::array<float, MAX_AXES_NUM>, static_cast<int>(JoystickAxisType::Max)> currentAxis_;

    std::u16string ToU16(const std::wstring& wstr);

    void SendSubcommand(hid_device* dev, uint8_t command, uint8_t data[], int len);
    void HandleJoyconInput(int index, unsigned char* buff, bool is_left);

public:
    static bool Initialize();

    static void Terminate();

    static std::shared_ptr<Joystick>& GetInstance();

    void RefreshInputState();

    void RefreshConnectedState();

    bool IsPresent(int32_t joystickIndex);

    ButtonState GetButtonStateByIndex(int32_t joystickIndex, int32_t buttonIndex) const;
    ButtonState GetButtonStateByType(int32_t joystickIndex, JoystickButtonType type) const;

    JoystickType GetJoystickType(int32_t index) const;

    float GetAxisStateByIndex(int32_t joystickIndex, int32_t axisIndex) const;
    float GetAxisStateByType(int32_t joystickIndex, JoystickAxisType type) const;

    const char16_t* GetJoystickName(int32_t index) const;

    void Vibrate(int32_t joystickIndex, float frequency, float amplitude);
};

}  // namespace Altseed
