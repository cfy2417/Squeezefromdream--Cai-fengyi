// JoystickBlueprintLibrary.cpp
#include "JoystickBlueprintLibrary.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

// 静态变量定义
bool UJoystickBlueprintLibrary::LastFrameInDeadzone = true;
bool UJoystickBlueprintLibrary::CurrentFrameInDeadzone = true;
bool UJoystickBlueprintLibrary::LastButton1State = false;
bool UJoystickBlueprintLibrary::LastButton2State = false;
bool UJoystickBlueprintLibrary::LastButton3State = false;
bool UJoystickBlueprintLibrary::LastButton4State = false;

// 压力传感器状态跟踪
bool UJoystickBlueprintLibrary::LastPressure1Triggered = false;
bool UJoystickBlueprintLibrary::LastPressure2Triggered = false;
float UJoystickBlueprintLibrary::LastPressure1Value = 0.0f;
float UJoystickBlueprintLibrary::LastPressure2Value = 0.0f;

// 事件更新时间戳
float UJoystickBlueprintLibrary::LastUpdateTime = 0.0f;
bool UJoystickBlueprintLibrary::bEventStatesUpdatedThisFrame = false;

// === 基本数据获取函数 ===

int32 UJoystickBlueprintLibrary::GetArduinoMessageID()
{
    return AOSCReceiver::GetMessageID();
}

float UJoystickBlueprintLibrary::GetArduinoTimestamp()
{
    return AOSCReceiver::GetTimestamp();
}

FString UJoystickBlueprintLibrary::GetArduinoDeviceName()
{
    return AOSCReceiver::GetDeviceName();
}

int32 UJoystickBlueprintLibrary::GetArduinoIsActive()
{
    return AOSCReceiver::GetIsActive();
}

bool UJoystickBlueprintLibrary::IsArduinoDataReceived()
{
    return AOSCReceiver::GetDataReceived();
}

bool UJoystickBlueprintLibrary::IsArduinoConnected()
{
    return AOSCReceiver::IsJoystickConnected();
}

// === 摇杆数据获取函数 ===

float UJoystickBlueprintLibrary::GetArduinoJoystickX()
{
    return AOSCReceiver::GetJoystickX();
}

float UJoystickBlueprintLibrary::GetArduinoJoystickY()
{
    return AOSCReceiver::GetJoystickY();
}

FVector2D UJoystickBlueprintLibrary::GetArduinoJoystickVector()
{
    return AOSCReceiver::GetJoystickVector();
}

FVector UJoystickBlueprintLibrary::GetArduinoJoystickVector3D()
{
    FVector2D Vec2D = AOSCReceiver::GetJoystickVector();
    return FVector(Vec2D.X, Vec2D.Y, 0.0f);
}

float UJoystickBlueprintLibrary::GetArduinoJoystickMagnitude()
{
    FVector2D Vec = AOSCReceiver::GetJoystickVector();
    return Vec.Size();
}

float UJoystickBlueprintLibrary::GetArduinoJoystickAngle()
{
    FVector2D Vec = AOSCReceiver::GetJoystickVector();
    if (Vec.IsNearlyZero())
    {
        return 0.0f;
    }
    
    float AngleRad = FMath::Atan2(Vec.Y, Vec.X);
    float AngleDeg = FMath::RadiansToDegrees(AngleRad);
    
    if (AngleDeg < 0.0f)
    {
        AngleDeg += 360.0f;
    }
    
    return AngleDeg;
}

bool UJoystickBlueprintLibrary::IsArduinoJoystickInDeadzone(float DeadzoneRadius)
{
    float Magnitude = GetArduinoJoystickMagnitude();
    CurrentFrameInDeadzone = Magnitude <= DeadzoneRadius;
    return CurrentFrameInDeadzone;
}

// === 压力传感器数据获取函数 ===

float UJoystickBlueprintLibrary::GetArduinoPressure1()
{
    return AOSCReceiver::GetPressure1();
}

float UJoystickBlueprintLibrary::GetArduinoPressure2()
{
    return AOSCReceiver::GetPressure2();
}

FVector2D UJoystickBlueprintLibrary::GetArduinoPressureVector()
{
    return AOSCReceiver::GetPressureVector();
}

float UJoystickBlueprintLibrary::GetArduinoTotalPressure()
{
    return GetArduinoPressure1() + GetArduinoPressure2();
}

float UJoystickBlueprintLibrary::GetArduinoPressureDifference()
{
    return GetArduinoPressure1() - GetArduinoPressure2();
}

// === 加速度传感器数据获取函数 ===

float UJoystickBlueprintLibrary::GetArduinoAccelX()
{
    return AOSCReceiver::GetAccelX();
}

float UJoystickBlueprintLibrary::GetArduinoAccelY()
{
    return AOSCReceiver::GetAccelY();
}

float UJoystickBlueprintLibrary::GetArduinoAccelZ()
{
    return AOSCReceiver::GetAccelZ();
}

FVector UJoystickBlueprintLibrary::GetArduinoAccelVector()
{
    return AOSCReceiver::GetAccelVector();
}

float UJoystickBlueprintLibrary::GetArduinoAccelMagnitude()
{
    FVector Vec = GetArduinoAccelVector();
    return Vec.Size();
}

bool UJoystickBlueprintLibrary::IsArduinoMoving(float Threshold)
{
    FVector Accel = GetArduinoAccelVector();
    FVector Gravity = FVector(0.0f, 0.0f, 1.0f);
    FVector Motion = Accel - Gravity;
    return Motion.Size() > Threshold;
}

// === 陀螺仪数据获取函数 ===

float UJoystickBlueprintLibrary::GetArduinoGyroX()
{
    return AOSCReceiver::GetGyroX();
}

float UJoystickBlueprintLibrary::GetArduinoGyroY()
{
    return AOSCReceiver::GetGyroY();
}

float UJoystickBlueprintLibrary::GetArduinoGyroZ()
{
    return AOSCReceiver::GetGyroZ();
}

FVector UJoystickBlueprintLibrary::GetArduinoGyroVector()
{
    return AOSCReceiver::GetGyroVector();
}

float UJoystickBlueprintLibrary::GetArduinoGyroMagnitude()
{
    FVector Vec = GetArduinoGyroVector();
    return Vec.Size();
}

bool UJoystickBlueprintLibrary::IsArduinoRotating(float Threshold)
{
    return GetArduinoGyroMagnitude() > Threshold;
}

// === 按钮数据获取函数 ===

bool UJoystickBlueprintLibrary::GetArduinoButton1()
{
    return AOSCReceiver::GetButton1();
}

bool UJoystickBlueprintLibrary::GetArduinoButton2()
{
    return AOSCReceiver::GetButton2();
}

bool UJoystickBlueprintLibrary::GetArduinoButton3()
{
    return AOSCReceiver::GetButton3();
}

bool UJoystickBlueprintLibrary::GetArduinoButton4()
{
    return AOSCReceiver::GetButton4();
}

int32 UJoystickBlueprintLibrary::GetArduinoButtonsBitmask()
{
    int32 Bitmask = 0;
    if (GetArduinoButton1()) Bitmask |= (1 << 0);
    if (GetArduinoButton2()) Bitmask |= (1 << 1);
    if (GetArduinoButton3()) Bitmask |= (1 << 2);
    if (GetArduinoButton4()) Bitmask |= (1 << 3);
    return Bitmask;
}

bool UJoystickBlueprintLibrary::IsAnyArduinoButtonPressed()
{
    return GetArduinoButton1() || GetArduinoButton2() || GetArduinoButton3() || GetArduinoButton4();
}

int32 UJoystickBlueprintLibrary::GetArduinoButtonsPressed()
{
    int32 Count = 0;
    if (GetArduinoButton1()) Count++;
    if (GetArduinoButton2()) Count++;
    if (GetArduinoButton3()) Count++;
    if (GetArduinoButton4()) Count++;
    return Count;
}

// === 高级功能 ===

FJoystickData UJoystickBlueprintLibrary::GetAllArduinoData()
{
    return AOSCReceiver::GetAllJoystickData();
}

FString UJoystickBlueprintLibrary::GetArduinoConnectionInfo()
{
    if (!IsArduinoDataReceived())
    {
        return TEXT("Arduino: 未连接");
    }
    
    FString DeviceName = GetArduinoDeviceName();
    int32 MessageID = GetArduinoMessageID();
    float Timestamp = GetArduinoTimestamp();
    bool IsActive = GetArduinoIsActive() == 1;
    
    return FString::Printf(TEXT("设备: %s | 消息: #%d | 时间: %.1fs | 状态: %s"),
                          *DeviceName, MessageID, Timestamp,
                          IsActive ? TEXT("活跃") : TEXT("非活跃"));
}

float UJoystickBlueprintLibrary::GetArduinoNetworkLatency()
{
    if (!IsArduinoDataReceived())
    {
        return -1.0f;
    }
    
    if (GEngine && GEngine->GetWorld())
    {
        float CurrentGameTime = GEngine->GetWorld()->GetTimeSeconds();
        float ArduinoTime = GetArduinoTimestamp();
        return FMath::Abs(CurrentGameTime - ArduinoTime);
    }
    
    return 0.0f;
}

// === 摇杆事件检测 ===

bool UJoystickBlueprintLibrary::IsArduinoJoystickJustPressed()
{
    IsArduinoJoystickInDeadzone();
    
    bool JustPressed = LastFrameInDeadzone && !CurrentFrameInDeadzone;
    LastFrameInDeadzone = CurrentFrameInDeadzone;
    
    return JustPressed;
}

bool UJoystickBlueprintLibrary::IsArduinoJoystickJustReleased()
{
    IsArduinoJoystickInDeadzone();
    
    bool JustReleased = !LastFrameInDeadzone && CurrentFrameInDeadzone;
    LastFrameInDeadzone = CurrentFrameInDeadzone;
    
    return JustReleased;
}

// === 按钮事件检测 ===

bool UJoystickBlueprintLibrary::IsArduinoButtonJustPressed(int32 ButtonNumber)
{
    bool CurrentState = false;
    bool* LastState = nullptr;
    
    switch (ButtonNumber)
    {
    case 1:
        CurrentState = GetArduinoButton1();
        LastState = &LastButton1State;
        break;
    case 2:
        CurrentState = GetArduinoButton2();
        LastState = &LastButton2State;
        break;
    case 3:
        CurrentState = GetArduinoButton3();
        LastState = &LastButton3State;
        break;
    case 4:
        CurrentState = GetArduinoButton4();
        LastState = &LastButton4State;
        break;
    default:
        return false;
    }
    
    bool JustPressed = !(*LastState) && CurrentState;
    *LastState = CurrentState;
    
    return JustPressed;
}

bool UJoystickBlueprintLibrary::IsArduinoButtonJustReleased(int32 ButtonNumber)
{
    bool CurrentState = false;
    bool* LastState = nullptr;
    
    switch (ButtonNumber)
    {
    case 1:
        CurrentState = GetArduinoButton1();
        LastState = &LastButton1State;
        break;
    case 2:
        CurrentState = GetArduinoButton2();
        LastState = &LastButton2State;
        break;
    case 3:
        CurrentState = GetArduinoButton3();
        LastState = &LastButton3State;
        break;
    case 4:
        CurrentState = GetArduinoButton4();
        LastState = &LastButton4State;
        break;
    default:
        return false;
    }
    
    bool JustReleased = (*LastState) && !CurrentState;
    *LastState = CurrentState;
    
    return JustReleased;
}

bool UJoystickBlueprintLibrary::IsArduinoButtonDown(int32 ButtonNumber)
{
    switch (ButtonNumber)
    {
    case 1:
        return GetArduinoButton1();
    case 2:
        return GetArduinoButton2();
    case 3:
        return GetArduinoButton3();
    case 4:
        return GetArduinoButton4();
    default:
        return false;
    }
}

// === 压力传感器事件检测 ===

bool UJoystickBlueprintLibrary::IsPressure1Triggered(float Threshold)
{
    return GetArduinoPressure1() > Threshold;
}

bool UJoystickBlueprintLibrary::IsPressure2Triggered(float Threshold)
{
    return GetArduinoPressure2() > Threshold;
}

bool UJoystickBlueprintLibrary::IsPressure1JustTriggered(float Threshold)
{
    float CurrentValue = GetArduinoPressure1();
    bool CurrentTriggered = CurrentValue > Threshold;
    
    bool JustTriggered = !LastPressure1Triggered && CurrentTriggered;
    
    LastPressure1Triggered = CurrentTriggered;
    LastPressure1Value = CurrentValue;
    
    return JustTriggered;
}

bool UJoystickBlueprintLibrary::IsPressure2JustTriggered(float Threshold)
{
    float CurrentValue = GetArduinoPressure2();
    bool CurrentTriggered = CurrentValue > Threshold;
    
    bool JustTriggered = !LastPressure2Triggered && CurrentTriggered;
    
    LastPressure2Triggered = CurrentTriggered;
    LastPressure2Value = CurrentValue;
    
    return JustTriggered;
}

bool UJoystickBlueprintLibrary::IsPressure1JustReleased(float Threshold)
{
    float CurrentValue = GetArduinoPressure1();
    bool CurrentTriggered = CurrentValue > Threshold;
    
    bool JustReleased = LastPressure1Triggered && !CurrentTriggered;
    
    LastPressure1Triggered = CurrentTriggered;
    LastPressure1Value = CurrentValue;
    
    return JustReleased;
}

bool UJoystickBlueprintLibrary::IsPressure2JustReleased(float Threshold)
{
    float CurrentValue = GetArduinoPressure2();
    bool CurrentTriggered = CurrentValue > Threshold;
    
    bool JustReleased = LastPressure2Triggered && !CurrentTriggered;
    
    LastPressure2Triggered = CurrentTriggered;
    LastPressure2Value = CurrentValue;
    
    return JustReleased;
}

bool UJoystickBlueprintLibrary::IsAnyPressureTriggered(float Threshold)
{
    return IsPressure1Triggered(Threshold) || IsPressure2Triggered(Threshold);
}

bool UJoystickBlueprintLibrary::AreBothPressuresTriggered(float Threshold)
{
    return IsPressure1Triggered(Threshold) && IsPressure2Triggered(Threshold);
}

// === 组合事件检测 ===

bool UJoystickBlueprintLibrary::IsButtonAndPressureTriggered(int32 ButtonNumber, int32 PressureNumber, float Threshold)
{
    bool ButtonPressed = false;
    
    switch (ButtonNumber)
    {
    case 1:
        ButtonPressed = GetArduinoButton1();
        break;
    case 2:
        ButtonPressed = GetArduinoButton2();
        break;
    case 3:
        ButtonPressed = GetArduinoButton3();
        break;
    case 4:
        ButtonPressed = GetArduinoButton4();
        break;
    default:
        return false;
    }
    
    bool PressureTriggered = false;
    
    switch (PressureNumber)
    {
    case 1:
        PressureTriggered = IsPressure1Triggered(Threshold);
        break;
    case 2:
        PressureTriggered = IsPressure2Triggered(Threshold);
        break;
    default:
        return false;
    }
    
    return ButtonPressed && PressureTriggered;
}

// === 事件状态更新 ===

void UJoystickBlueprintLibrary::UpdateArduinoEventStates()
{
    float CurrentTime = 0.0f;
    if (GEngine && GEngine->GetWorld())
    {
        CurrentTime = GEngine->GetWorld()->GetTimeSeconds();
    }
    
    // 防止同一帧重复更新
    if (FMath::IsNearlyEqual(CurrentTime, LastUpdateTime, 0.001f))
    {
        return;
    }
    
    LastUpdateTime = CurrentTime;
    
    // 更新按钮状态
    LastButton1State = GetArduinoButton1();
    LastButton2State = GetArduinoButton2();
    LastButton3State = GetArduinoButton3();
    LastButton4State = GetArduinoButton4();
    
    // 更新压力传感器状态
    float Threshold = 100.0f;
    LastPressure1Triggered = GetArduinoPressure1() > Threshold;
    LastPressure2Triggered = GetArduinoPressure2() > Threshold;
    LastPressure1Value = GetArduinoPressure1();
    LastPressure2Value = GetArduinoPressure2();
    
    // 更新摇杆死区状态
    LastFrameInDeadzone = CurrentFrameInDeadzone;
    IsArduinoJoystickInDeadzone();
}
