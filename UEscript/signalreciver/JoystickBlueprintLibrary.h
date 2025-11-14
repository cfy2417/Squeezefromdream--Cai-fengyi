#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OSCReceiver.h"
#include "JoystickBlueprintLibrary.generated.h"

/**
 * 蓝图函数库，提供全局访问Arduino控制器数据的便捷函数
 * 在任何蓝图中都可以调用这些函数获取Arduino传感器数据
 */
UCLASS()
class WORKVOILENCEGAME_API UJoystickBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // === 基本数据获取函数 ===
    
    /** 获取消息ID（消息计数器） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Basic",
              meta = (Keywords = "arduino controller message id counter"))
    static int32 GetArduinoMessageID();

    /** 获取时间戳（Arduino运行时间，单位秒） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Basic",
              meta = (Keywords = "arduino controller timestamp time"))
    static float GetArduinoTimestamp();

    /** 获取设备名称 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Basic",
              meta = (Keywords = "arduino controller device name"))
    static FString GetArduinoDeviceName();

    /** 获取激活状态 (1=活跃, 0=非活跃) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Basic",
              meta = (Keywords = "arduino controller active status"))
    static int32 GetArduinoIsActive();

    /** 是否接收到数据 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Basic",
              meta = (Keywords = "arduino controller data received connected"))
    static bool IsArduinoDataReceived();

    /** 检查Arduino是否连接且活跃 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Basic",
              meta = (Keywords = "arduino controller connected online active"))
    static bool IsArduinoConnected();

    // === 摇杆数据获取函数 ===
    
    /** 获取摇杆X轴值 (-1.0 到 1.0) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Joystick",
              meta = (Keywords = "arduino joystick x axis horizontal"))
    static float GetArduinoJoystickX();

    /** 获取摇杆Y轴值 (-1.0 到 1.0) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Joystick",
              meta = (Keywords = "arduino joystick y axis vertical"))
    static float GetArduinoJoystickY();

    /** 获取摇杆2D向量 (X, Y) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Joystick",
              meta = (Keywords = "arduino joystick vector 2d xy"))
    static FVector2D GetArduinoJoystickVector();

    /** 获取摇杆3D向量 (X, Y, 0) - 便于直接用于角色移动 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Joystick",
              meta = (Keywords = "arduino joystick vector 3d movement"))
    static FVector GetArduinoJoystickVector3D();

    /** 获取摇杆距离中心的距离 (0.0 到 1.0) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Joystick",
              meta = (Keywords = "arduino joystick magnitude distance center"))
    static float GetArduinoJoystickMagnitude();

    /** 获取摇杆角度（度数，0-360） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Joystick",
              meta = (Keywords = "arduino joystick angle rotation degrees"))
    static float GetArduinoJoystickAngle();

    /** 检查摇杆是否在死区内 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Joystick",
              meta = (Keywords = "arduino joystick deadzone dead zone"))
    static bool IsArduinoJoystickInDeadzone(float DeadzoneRadius = 0.1f);

    // === 压力传感器数据获取函数 ===
    
    /** 获取第一个压力传感器值 (0.0 到 1.0) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Pressure",
              meta = (Keywords = "arduino pressure sensor 1 force"))
    static float GetArduinoPressure1();

    /** 获取第二个压力传感器值 (0.0 到 1.0) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Pressure",
              meta = (Keywords = "arduino pressure sensor 2 force"))
    static float GetArduinoPressure2();

    /** 获取压力传感器2D向量 (Pressure1, Pressure2) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Pressure",
              meta = (Keywords = "arduino pressure vector 2d sensors"))
    static FVector2D GetArduinoPressureVector();

    /** 获取总压力值（两个传感器之和） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Pressure",
              meta = (Keywords = "arduino pressure total combined sum"))
    static float GetArduinoTotalPressure();

    /** 获取压力差值（传感器1 - 传感器2） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Pressure",
              meta = (Keywords = "arduino pressure difference delta"))
    static float GetArduinoPressureDifference();

    // === 加速度传感器数据获取函数 ===
    
    /** 获取X轴加速度 (g单位) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Accelerometer",
              meta = (Keywords = "arduino accelerometer x axis g"))
    static float GetArduinoAccelX();

    /** 获取Y轴加速度 (g单位) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Accelerometer",
              meta = (Keywords = "arduino accelerometer y axis g"))
    static float GetArduinoAccelY();

    /** 获取Z轴加速度 (g单位) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Accelerometer",
              meta = (Keywords = "arduino accelerometer z axis g"))
    static float GetArduinoAccelZ();

    /** 获取加速度3D向量 (g单位) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Accelerometer",
              meta = (Keywords = "arduino accelerometer vector 3d g"))
    static FVector GetArduinoAccelVector();

    /** 获取加速度大小 (总加速度，g单位) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Accelerometer",
              meta = (Keywords = "arduino accelerometer magnitude total g"))
    static float GetArduinoAccelMagnitude();

    /** 检查设备是否在运动 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Accelerometer",
              meta = (Keywords = "arduino accelerometer moving motion"))
    static bool IsArduinoMoving(float Threshold = 0.1f);

    // === 陀螺仪数据获取函数 ===
    
    /** 获取X轴角速度 (度/秒) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Gyroscope",
              meta = (Keywords = "arduino gyroscope x axis degrees per second"))
    static float GetArduinoGyroX();

    /** 获取Y轴角速度 (度/秒) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Gyroscope",
              meta = (Keywords = "arduino gyroscope y axis degrees per second"))
    static float GetArduinoGyroY();

    /** 获取Z轴角速度 (度/秒) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Gyroscope",
              meta = (Keywords = "arduino gyroscope z axis degrees per second"))
    static float GetArduinoGyroZ();

    /** 获取陀螺仪3D向量 (度/秒) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Gyroscope",
              meta = (Keywords = "arduino gyroscope vector 3d degrees per second"))
    static FVector GetArduinoGyroVector();

    /** 获取角速度大小 (总角速度，度/秒) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Gyroscope",
              meta = (Keywords = "arduino gyroscope magnitude total degrees"))
    static float GetArduinoGyroMagnitude();

    /** 检查设备是否在旋转 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Gyroscope",
              meta = (Keywords = "arduino gyroscope rotating spinning"))
    static bool IsArduinoRotating(float Threshold = 5.0f);

    // === 按钮数据获取函数 ===
    
    /** 获取按钮1状态 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons",
              meta = (Keywords = "arduino button 1 pressed"))
    static bool GetArduinoButton1();

    /** 获取按钮2状态 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons",
              meta = (Keywords = "arduino button 2 pressed"))
    static bool GetArduinoButton2();

    /** 获取按钮3状态 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons",
              meta = (Keywords = "arduino button 3 pressed"))
    static bool GetArduinoButton3();

    /** 获取按钮4状态 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons",
              meta = (Keywords = "arduino button 4 pressed"))
    static bool GetArduinoButton4();

    /** 获取所有按钮状态的位掩码 (每个按钮对应一位) */
    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons",
              meta = (Keywords = "arduino buttons bitmask all"))
    static int32 GetArduinoButtonsBitmask();

    /** 检查是否有任何按钮被按下 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons",
              meta = (Keywords = "arduino buttons any pressed"))
    static bool IsAnyArduinoButtonPressed();

    /** 获取被按下的按钮数量 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons",
              meta = (Keywords = "arduino buttons count pressed"))
    static int32 GetArduinoButtonsPressed();

    // === 高级功能 ===
    
    /** 获取所有传感器数据的结构体 */
    UFUNCTION(BlueprintCallable, Category = "Arduino All Data",
              meta = (Keywords = "arduino all data struct complete"))
    static FJoystickData GetAllArduinoData();

    /** 获取连接状态信息字符串 */
    UFUNCTION(BlueprintCallable, Category = "Arduino All Data",
              meta = (Keywords = "arduino connection status info"))
    static FString GetArduinoConnectionInfo();

    /** 获取网络延迟（如果有时间同步） */
    UFUNCTION(BlueprintCallable, Category = "Arduino All Data",
              meta = (Keywords = "arduino network latency delay"))
    static float GetArduinoNetworkLatency();

    // === 事件检测（类似键盘按键事件） ===
    
    /** 检查摇杆是否刚刚按下（从死区外移动到死区外） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino joystick just pressed started"))
    static bool IsArduinoJoystickJustPressed();

    /** 检查摇杆是否刚刚释放（移动到死区内） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino joystick just released stopped"))
    static bool IsArduinoJoystickJustReleased();

    /** 检查按钮是否刚刚按下（类似键盘按键按下事件） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino button just pressed down event"))
    static bool IsArduinoButtonJustPressed(int32 ButtonNumber);

    /** 检查按钮是否刚刚释放（类似键盘按键释放事件） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino button just released up event"))
    static bool IsArduinoButtonJustReleased(int32 ButtonNumber);

    /** 检查按钮是否持续按下（类似键盘按键持续按下） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino button held down continuous"))
    static bool IsArduinoButtonDown(int32 ButtonNumber);

    // === 压力传感器事件（新增） ===
    
    /** 检查压力传感器1是否被触发（值大于阈值，默认100） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino pressure sensor 1 triggered threshold"))
    static bool IsPressure1Triggered(float Threshold = 100.0f);

    /** 检查压力传感器2是否被触发（值大于阈值，默认100） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino pressure sensor 2 triggered threshold"))
    static bool IsPressure2Triggered(float Threshold = 100.0f);

    /** 检查压力传感器1是否刚刚被触发（类似按键按下事件） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino pressure sensor 1 just triggered pressed event"))
    static bool IsPressure1JustTriggered(float Threshold = 100.0f);

    /** 检查压力传感器2是否刚刚被触发（类似按键按下事件） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino pressure sensor 2 just triggered pressed event"))
    static bool IsPressure2JustTriggered(float Threshold = 100.0f);

    /** 检查压力传感器1是否刚刚释放（类似按键释放事件） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino pressure sensor 1 just released event"))
    static bool IsPressure1JustReleased(float Threshold = 100.0f);

    /** 检查压力传感器2是否刚刚释放（类似按键释放事件） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino pressure sensor 2 just released event"))
    static bool IsPressure2JustReleased(float Threshold = 100.0f);

    /** 检查任意压力传感器是否被触发 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino pressure any sensor triggered"))
    static bool IsAnyPressureTriggered(float Threshold = 100.0f);

    /** 检查两个压力传感器是否都被触发 */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino pressure both sensors triggered"))
    static bool AreBothPressuresTriggered(float Threshold = 100.0f);

    // === 组合事件检测 ===
    
    /** 检查按钮和压力传感器的组合触发（按钮按下且压力传感器触发） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino button pressure combo combination"))
    static bool IsButtonAndPressureTriggered(int32 ButtonNumber, int32 PressureNumber, float Threshold = 100.0f);

    /** 更新所有事件状态（建议在Tick或Event Graph的每帧开始时调用一次） */
    UFUNCTION(BlueprintCallable, Category = "Arduino Events",
              meta = (Keywords = "arduino update events state frame"))
    static void UpdateArduinoEventStates();

private:
    // 用于事件检测的静态变量
    static bool LastFrameInDeadzone;
    static bool CurrentFrameInDeadzone;
    
    // 按钮状态跟踪（用于检测按下/释放事件）
    static bool LastButton1State;
    static bool LastButton2State;
    static bool LastButton3State;
    static bool LastButton4State;
    
    // 压力传感器状态跟踪（新增）
    static bool LastPressure1Triggered;
    static bool LastPressure2Triggered;
    static float LastPressure1Value;
    static float LastPressure2Value;
    
    // 用于事件状态更新的时间戳
    static float LastUpdateTime;
    static bool bEventStatesUpdatedThisFrame;
};
