#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OSCReceiver.h"
#include "ArduinoInputComponent.generated.h"

// === 事件委托声明（类似键盘事件）===

/** 按钮按下事件 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArduinoButtonPressed, int32, ButtonNumber);

/** 按钮释放事件 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArduinoButtonReleased, int32, ButtonNumber);

/** 压力传感器触发事件 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArduinoPressureTriggered, int32, SensorNumber, float, PressureValue);

/** 压力传感器释放事件 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArduinoPressureReleased, int32, SensorNumber, float, PressureValue);

/** 摇杆移动事件 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArduinoJoystickMoved, float, X, float, Y);

/** 摇杆按下事件（离开死区）*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArduinoJoystickPressed, float, X, float, Y);

/** 摇杆释放事件（进入死区）*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnArduinoJoystickReleased);

/** 连接状态变化事件 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArduinoConnectionChanged, bool, bIsConnected);

/**
 * Arduino 输入组件 - 提供事件驱动的输入处理
 * 将此组件添加到 PlayerController、Character 或 GameMode 中
 * 即可在蓝图中直接使用 Arduino 输入事件（无需 Event Tick）
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WORKVOILENCEGAME_API UArduinoInputComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UArduinoInputComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    // === 事件分发器（在蓝图中显示为事件节点）===
    
    /** 当按钮1被按下时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Buttons")
    FOnArduinoButtonPressed OnButton1Pressed;
    
    /** 当按钮1被释放时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Buttons")
    FOnArduinoButtonReleased OnButton1Released;
    
    /** 当按钮2被按下时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Buttons")
    FOnArduinoButtonPressed OnButton2Pressed;
    
    /** 当按钮2被释放时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Buttons")
    FOnArduinoButtonReleased OnButton2Released;
    
    /** 当按钮3被按下时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Buttons")
    FOnArduinoButtonPressed OnButton3Pressed;
    
    /** 当按钮3被释放时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Buttons")
    FOnArduinoButtonReleased OnButton3Released;
    
    /** 当按钮4被按下时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Buttons")
    FOnArduinoButtonPressed OnButton4Pressed;
    
    /** 当按钮4被释放时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Buttons")
    FOnArduinoButtonReleased OnButton4Released;
    
    /** 当任意按钮被按下时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Buttons")
    FOnArduinoButtonPressed OnAnyButtonPressed;
    
    /** 当任意按钮被释放时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Buttons")
    FOnArduinoButtonReleased OnAnyButtonReleased;
    
    // === 压力传感器事件 ===
    
    /** 当压力传感器1被触发时 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Pressure")
    FOnArduinoPressureTriggered OnPressure1Triggered;
    
    /** 当压力传感器1被释放时 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Pressure")
    FOnArduinoPressureReleased OnPressure1Released;
    
    /** 当压力传感器2被触发时 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Pressure")
    FOnArduinoPressureTriggered OnPressure2Triggered;
    
    /** 当压力传感器2被释放时 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Pressure")
    FOnArduinoPressureReleased OnPressure2Released;
    
    /** 当任意压力传感器被触发时 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Pressure")
    FOnArduinoPressureTriggered OnAnyPressureTriggered;
    
    // === 摇杆事件 ===
    
    /** 当摇杆移动时持续触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Joystick")
    FOnArduinoJoystickMoved OnJoystickMoved;
    
    /** 当摇杆被按下时触发（离开死区）*/
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Joystick")
    FOnArduinoJoystickPressed OnJoystickPressed;
    
    /** 当摇杆被释放时触发（进入死区）*/
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Joystick")
    FOnArduinoJoystickReleased OnJoystickReleased;
    
    // === 连接事件 ===
    
    /** 当 Arduino 连接状态改变时触发 */
    UPROPERTY(BlueprintAssignable, Category = "Arduino Events|Connection")
    FOnArduinoConnectionChanged OnConnectionChanged;
    
    // === 可配置参数 ===
    
    /** 压力传感器触发阈值（默认 100）*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arduino Settings")
    float PressureTriggerThreshold = 100.0f;
    
    /** 摇杆死区半径（默认 0.1）*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arduino Settings")
    float JoystickDeadzone = 0.1f;
    
    /** 是否启用调试日志 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arduino Settings")
    bool bEnableDebugLog = false;

private:
    // 上一帧的状态（用于检测变化）
    bool LastButton1State = false;
    bool LastButton2State = false;
    bool LastButton3State = false;
    bool LastButton4State = false;
    
    bool LastPressure1Triggered = false;
    bool LastPressure2Triggered = false;
    
    bool LastJoystickInDeadzone = true;
    
    bool LastConnectionState = false;
    
    // 检测并分发按钮事件
    void CheckButtonEvents();
    
    // 检测并分发压力传感器事件
    void CheckPressureEvents();
    
    // 检测并分发摇杆事件
    void CheckJoystickEvents();
    
    // 检测连接状态
    void CheckConnectionStatus();
};
