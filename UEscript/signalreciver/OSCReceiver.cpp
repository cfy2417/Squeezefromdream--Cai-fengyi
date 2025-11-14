#include "OSCReceiver.h"
#include "OSCManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "OSCAddress.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"

// 静态变量定义
int32 AOSCReceiver::MessageID = 0;
float AOSCReceiver::Timestamp = 0.0f;
FString AOSCReceiver::DeviceName = TEXT("");
bool AOSCReceiver::DataReceived = false;
int32 AOSCReceiver::IsActive = 0;

// 摇杆数据
float AOSCReceiver::JoystickX = 0.0f;
float AOSCReceiver::JoystickY = 0.0f;

// 压力传感器数据
float AOSCReceiver::Pressure1 = 0.0f;
float AOSCReceiver::Pressure2 = 0.0f;

// 加速度数据
float AOSCReceiver::AccelX = 0.0f;
float AOSCReceiver::AccelY = 0.0f;
float AOSCReceiver::AccelZ = 0.0f;

// 陀螺仪数据
float AOSCReceiver::GyroX = 0.0f;
float AOSCReceiver::GyroY = 0.0f;
float AOSCReceiver::GyroZ = 0.0f;

// 按钮状态
bool AOSCReceiver::Button1 = false;
bool AOSCReceiver::Button2 = false;
bool AOSCReceiver::Button3 = false;
bool AOSCReceiver::Button4 = false;

AOSCReceiver::AOSCReceiver()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(true);
}

void AOSCReceiver::BeginPlay()
{
    Super::BeginPlay();
    
    // 创建OSC服务器 - 使用正确的UE5 API
    OSCServer = UOSCManager::CreateOSCServer(
        TEXT("0.0.0.0"), // 监听所有IP
        OSCServerPort,
        false, // 不是多播
        true,  // 开始监听
        TEXT("ArduinoOSCServer"), // 服务器名称
        this   // Outer对象
    );

    if (OSCServer)
    {
        // 绑定OSC消息接收事件
        OSCServer->OnOscMessageReceived.AddDynamic(this, &AOSCReceiver::OnOSCMessageReceived);
        
        UE_LOG(LogTemp, Warning, TEXT("OSC服务器已启动，监听端口: %d"), OSCServerPort);
        UE_LOG(LogTemp, Warning, TEXT("等待来自Arduino的OSC数据..."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("无法创建OSC服务器！"));
    }

    // 重置所有数据
    MessageID = 0;
    Timestamp = 0.0f;
    DeviceName = TEXT("Arduino ESP32");
    DataReceived = false;
    IsActive = 0;
    LocalMessageCounter = 0;

    // 重置传感器数据
    JoystickX = 0.0f;
    JoystickY = 0.0f;
    Pressure1 = 0.0f;
    Pressure2 = 0.0f;
    AccelX = 0.0f;
    AccelY = 0.0f;
    AccelZ = 0.0f;
    GyroX = 0.0f;
    GyroY = 0.0f;
    GyroZ = 0.0f;
    Button1 = false;
    Button2 = false;
    Button3 = false;
    Button4 = false;
}

void AOSCReceiver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 清理OSC服务器
    if (OSCServer)
    {
        OSCServer->Stop();
        OSCServer = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("OSC服务器已停止"));
    }

    Super::EndPlay(EndPlayReason);
}

void AOSCReceiver::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 检查数据超时（如果超过2秒没有收到数据，标记为断开连接）
    if (DataReceived && GetWorld())
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();
        if (CurrentTime - LastUpdateTime > DataTimeout)
        {
            DataReceived = false;
            IsActive = 0;
            DeviceName = TEXT("连接超时");
            UE_LOG(LogTemp, Warning, TEXT("Arduino连接超时"));
        }
    }
}

void AOSCReceiver::OnOSCMessageReceived(const FOSCMessage& Message, const FString& IPAddress, int32 Port)
{
    // 获取OSC地址
    FOSCAddress Address = Message.GetAddress();
    FString AddressString = Address.GetFullPath();
    
    // 更新基础信息
    LocalMessageCounter++;
    MessageID = LocalMessageCounter;
    DataReceived = true;
    IsActive = 1;
    
    if (GetWorld())
    {
        LastUpdateTime = GetWorld()->GetTimeSeconds();
        Timestamp = LastUpdateTime;
    }

    // 根据地址解析不同类型的数据
    bool bProcessed = false;
    float FloatValue = 0.0f;
    int32 IntValue = 0;

    // 摇杆数据
    if (AddressString == TEXT("/avatar/input/joystick/x"))
    {
        if (UOSCManager::GetFloat(Message, 0, FloatValue))
        {
            JoystickX = FloatValue;
            bProcessed = true;
        }
    }
    else if (AddressString == TEXT("/avatar/input/joystick/y"))
    {
        if (UOSCManager::GetFloat(Message, 0, FloatValue))
        {
            JoystickY = FloatValue;
            bProcessed = true;
        }
    }
    // 压力传感器数据
    else if (AddressString == TEXT("/avatar/input/pressure/1"))
    {
        if (UOSCManager::GetFloat(Message, 0, FloatValue))
        {
            Pressure1 = FloatValue;
            bProcessed = true;
        }
    }
    else if (AddressString == TEXT("/avatar/input/pressure/2"))
    {
        if (UOSCManager::GetFloat(Message, 0, FloatValue))
        {
            Pressure2 = FloatValue;
            bProcessed = true;
        }
    }
    // 加速度数据
    else if (AddressString == TEXT("/avatar/input/accel/x"))
    {
        if (UOSCManager::GetFloat(Message, 0, FloatValue))
        {
            AccelX = FloatValue;
            bProcessed = true;
        }
    }
    else if (AddressString == TEXT("/avatar/input/accel/y"))
    {
        if (UOSCManager::GetFloat(Message, 0, FloatValue))
        {
            AccelY = FloatValue;
            bProcessed = true;
        }
    }
    else if (AddressString == TEXT("/avatar/input/accel/z"))
    {
        if (UOSCManager::GetFloat(Message, 0, FloatValue))
        {
            AccelZ = FloatValue;
            bProcessed = true;
        }
    }
    // 陀螺仪数据
    else if (AddressString == TEXT("/avatar/input/gyro/x"))
    {
        if (UOSCManager::GetFloat(Message, 0, FloatValue))
        {
            GyroX = FloatValue;
            bProcessed = true;
        }
    }
    else if (AddressString == TEXT("/avatar/input/gyro/y"))
    {
        if (UOSCManager::GetFloat(Message, 0, FloatValue))
        {
            GyroY = FloatValue;
            bProcessed = true;
        }
    }
    else if (AddressString == TEXT("/avatar/input/gyro/z"))
    {
        if (UOSCManager::GetFloat(Message, 0, FloatValue))
        {
            GyroZ = FloatValue;
            bProcessed = true;
        }
    }
    // 按钮数据
    else if (AddressString == TEXT("/avatar/input/button/1"))
    {
        if (UOSCManager::GetInt32(Message, 0, IntValue))
        {
            Button1 = IntValue != 0;
            bProcessed = true;
        }
    }
    else if (AddressString == TEXT("/avatar/input/button/2"))
    {
        if (UOSCManager::GetInt32(Message, 0, IntValue))
        {
            Button2 = IntValue != 0;
            bProcessed = true;
        }
    }
    else if (AddressString == TEXT("/avatar/input/button/3"))
    {
        if (UOSCManager::GetInt32(Message, 0, IntValue))
        {
            Button3 = IntValue != 0;
            bProcessed = true;
        }
    }
    else if (AddressString == TEXT("/avatar/input/button/4"))
    {
        if (UOSCManager::GetInt32(Message, 0, IntValue))
        {
            Button4 = IntValue != 0;
            bProcessed = true;
        }
    }

    // 调试输出（每50个消息打印一次，避免刷屏）
    if (bProcessed && MessageID % 50 == 0)
    {
        UE_LOG(LogTemp, Log, TEXT("OSC数据已接收 ID=%d | 摇杆(%.2f,%.2f) | 压力(%.2f,%.2f) | 加速度(%.2f,%.2f,%.2f) | 陀螺仪(%.1f,%.1f,%.1f) | 按钮(%d%d%d%d)"),
               MessageID, JoystickX, JoystickY, Pressure1, Pressure2,
               AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ,
               Button1?1:0, Button2?1:0, Button3?1:0, Button4?1:0);
    }

    if (!bProcessed)
    {
        UE_LOG(LogTemp, Warning, TEXT("未识别的OSC地址: %s"), *AddressString);
    }
}
