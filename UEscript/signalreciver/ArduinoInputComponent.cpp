#include "ArduinoInputComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

UArduinoInputComponent::UArduinoInputComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics; // 在物理之前更新，确保输入及时
}

void UArduinoInputComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化状态
    LastButton1State = AOSCReceiver::GetButton1();
    LastButton2State = AOSCReceiver::GetButton2();
    LastButton3State = AOSCReceiver::GetButton3();
    LastButton4State = AOSCReceiver::GetButton4();
    
    LastPressure1Triggered = AOSCReceiver::GetPressure1() > PressureTriggerThreshold;
    LastPressure2Triggered = AOSCReceiver::GetPressure2() > PressureTriggerThreshold;
    
    FVector2D JoystickVec = AOSCReceiver::GetJoystickVector();
    LastJoystickInDeadzone = JoystickVec.Size() <= JoystickDeadzone;
    
    LastConnectionState = AOSCReceiver::IsJoystickConnected();
    
    if (bEnableDebugLog)
    {
        UE_LOG(LogTemp, Warning, TEXT("Arduino Input Component 已初始化"));
    }
}

void UArduinoInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // 检查是否连接
    //if (!AOSCReceiver::GetDataReceived())
    //{
       // return;
    //}
    
    // 检测所有事件
    CheckConnectionStatus();
    CheckButtonEvents();
    CheckPressureEvents();
    CheckJoystickEvents();
}

void UArduinoInputComponent::CheckButtonEvents()
{
    // 按钮1
    bool CurrentButton1 = AOSCReceiver::GetButton1();
    if (CurrentButton1 && !LastButton1State)
    {
        OnButton1Pressed.Broadcast(1);
        OnAnyButtonPressed.Broadcast(1);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 按钮1按下"));
        }
    }
    else if (!CurrentButton1 && LastButton1State)
    {
        OnButton1Released.Broadcast(1);
        OnAnyButtonReleased.Broadcast(1);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 按钮1释放"));
        }
    }
    LastButton1State = CurrentButton1;
    
    // 按钮2
    bool CurrentButton2 = AOSCReceiver::GetButton2();
    if (CurrentButton2 && !LastButton2State)
    {
        OnButton2Pressed.Broadcast(2);
        OnAnyButtonPressed.Broadcast(2);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 按钮2按下"));
        }
    }
    else if (!CurrentButton2 && LastButton2State)
    {
        OnButton2Released.Broadcast(2);
        OnAnyButtonReleased.Broadcast(2);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 按钮2释放"));
        }
    }
    LastButton2State = CurrentButton2;
    
    // 按钮3
    bool CurrentButton3 = AOSCReceiver::GetButton3();
    if (CurrentButton3 && !LastButton3State)
    {
        OnButton3Pressed.Broadcast(3);
        OnAnyButtonPressed.Broadcast(3);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 按钮3按下"));
        }
    }
    else if (!CurrentButton3 && LastButton3State)
    {
        OnButton3Released.Broadcast(3);
        OnAnyButtonReleased.Broadcast(3);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 按钮3释放"));
        }
    }
    LastButton3State = CurrentButton3;
    
    // 按钮4
    bool CurrentButton4 = AOSCReceiver::GetButton4();
    if (CurrentButton4 && !LastButton4State)
    {
        OnButton4Pressed.Broadcast(4);
        OnAnyButtonPressed.Broadcast(4);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 按钮4按下"));
        }
    }
    else if (!CurrentButton4 && LastButton4State)
    {
        OnButton4Released.Broadcast(4);
        OnAnyButtonReleased.Broadcast(4);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 按钮4释放"));
        }
    }
    LastButton4State = CurrentButton4;
}

void UArduinoInputComponent::CheckPressureEvents()
{
    // 压力传感器1
    float CurrentPressure1 = AOSCReceiver::GetPressure1();
    bool CurrentPressure1Triggered = CurrentPressure1 > PressureTriggerThreshold;
    
    if (CurrentPressure1Triggered && !LastPressure1Triggered)
    {
        OnPressure1Triggered.Broadcast(1, CurrentPressure1);
        OnAnyPressureTriggered.Broadcast(1, CurrentPressure1);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 压力传感器1触发 (%.2f)"), CurrentPressure1);
        }
    }
    else if (!CurrentPressure1Triggered && LastPressure1Triggered)
    {
        OnPressure1Released.Broadcast(1, CurrentPressure1);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 压力传感器1释放 (%.2f)"), CurrentPressure1);
        }
    }
    LastPressure1Triggered = CurrentPressure1Triggered;
    
    // 压力传感器2
    float CurrentPressure2 = AOSCReceiver::GetPressure2();
    bool CurrentPressure2Triggered = CurrentPressure2 > PressureTriggerThreshold;
    
    if (CurrentPressure2Triggered && !LastPressure2Triggered)
    {
        OnPressure2Triggered.Broadcast(2, CurrentPressure2);
        OnAnyPressureTriggered.Broadcast(2, CurrentPressure2);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 压力传感器2触发 (%.2f)"), CurrentPressure2);
        }
    }
    else if (!CurrentPressure2Triggered && LastPressure2Triggered)
    {
        OnPressure2Released.Broadcast(2, CurrentPressure2);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 压力传感器2释放 (%.2f)"), CurrentPressure2);
        }
    }
    LastPressure2Triggered = CurrentPressure2Triggered;
}

void UArduinoInputComponent::CheckJoystickEvents()
{
    FVector2D JoystickVec = AOSCReceiver::GetJoystickVector();
    float Magnitude = JoystickVec.Size();
    bool CurrentInDeadzone = Magnitude <= JoystickDeadzone;
    
    // 摇杆移动事件（持续触发）
    if (!CurrentInDeadzone)
    {
        OnJoystickMoved.Broadcast(JoystickVec.X, JoystickVec.Y);
    }
    
    // 摇杆按下事件（从死区进入活动区）
    if (!CurrentInDeadzone && LastJoystickInDeadzone)
    {
        OnJoystickPressed.Broadcast(JoystickVec.X, JoystickVec.Y);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 摇杆按下 (%.2f, %.2f)"), JoystickVec.X, JoystickVec.Y);
        }
    }
    // 摇杆释放事件（从活动区进入死区）
    else if (CurrentInDeadzone && !LastJoystickInDeadzone)
    {
        OnJoystickReleased.Broadcast();
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Log, TEXT("Arduino: 摇杆释放"));
        }
    }
    
    LastJoystickInDeadzone = CurrentInDeadzone;
}

void UArduinoInputComponent::CheckConnectionStatus()
{
    bool CurrentConnectionState = AOSCReceiver::IsJoystickConnected();
    
    if (CurrentConnectionState != LastConnectionState)
    {
        OnConnectionChanged.Broadcast(CurrentConnectionState);
        
        if (bEnableDebugLog)
        {
            UE_LOG(LogTemp, Warning, TEXT("Arduino: 连接状态变化 - %s"),
                   CurrentConnectionState ? TEXT("已连接") : TEXT("已断开"));
        }
    }
    
    LastConnectionState = CurrentConnectionState;
}
