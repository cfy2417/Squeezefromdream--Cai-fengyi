#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "OSCServer.h"
#include "OSCMessage.h"
#include "OSCReceiver.generated.h"

USTRUCT(BlueprintType)
struct FJoystickData
{
    GENERATED_BODY()

    // 基础数据
    UPROPERTY(BlueprintReadOnly, Category = "Basic")
    int32 MessageID = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Basic")
    float Timestamp = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Basic")
    FString DeviceName = TEXT("");

    UPROPERTY(BlueprintReadOnly, Category = "Basic")
    bool DataReceived = false;

    UPROPERTY(BlueprintReadOnly, Category = "Basic")
    int32 IsActive = 0;

    // 摇杆数据
    UPROPERTY(BlueprintReadOnly, Category = "Joystick")
    float JoystickX = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Joystick")
    float JoystickY = 0.0f;

    // 压力传感器数据
    UPROPERTY(BlueprintReadOnly, Category = "Pressure")
    float Pressure1 = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Pressure")
    float Pressure2 = 0.0f;

    // 加速度数据 (g单位)
    UPROPERTY(BlueprintReadOnly, Category = "Accelerometer")
    float AccelX = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Accelerometer")
    float AccelY = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Accelerometer")
    float AccelZ = 0.0f;

    // 陀螺仪数据 (度/秒)
    UPROPERTY(BlueprintReadOnly, Category = "Gyroscope")
    float GyroX = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Gyroscope")
    float GyroY = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Gyroscope")
    float GyroZ = 0.0f;

    // 按钮状态
    UPROPERTY(BlueprintReadOnly, Category = "Buttons")
    bool Button1 = false;

    UPROPERTY(BlueprintReadOnly, Category = "Buttons")
    bool Button2 = false;

    UPROPERTY(BlueprintReadOnly, Category = "Buttons")
    bool Button3 = false;

    UPROPERTY(BlueprintReadOnly, Category = "Buttons")
    bool Button4 = false;
};

UCLASS(BlueprintType, Blueprintable)
class WORKVOILENCEGAME_API AOSCReceiver : public AActor
{
    GENERATED_BODY()

public:
    AOSCReceiver();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void Tick(float DeltaTime) override;

    // OSC服务器
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "OSC")
    class UOSCServer* OSCServer;

    // 全局可访问的传感器数据
    static int32 MessageID;
    static float Timestamp;
    static FString DeviceName;
    static bool DataReceived;
    static int32 IsActive;

    // 摇杆数据
    static float JoystickX;
    static float JoystickY;

    // 压力传感器数据
    static float Pressure1;
    static float Pressure2;

    // 加速度数据
    static float AccelX;
    static float AccelY;
    static float AccelZ;

    // 陀螺仪数据
    static float GyroX;
    static float GyroY;
    static float GyroZ;

    // 按钮状态
    static bool Button1;
    static bool Button2;
    static bool Button3;
    static bool Button4;

    // 蓝图可调用的数据获取函数
    // 基础数据
    UFUNCTION(BlueprintCallable, Category = "Arduino Basic")
    static int32 GetMessageID() { return MessageID; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Basic")
    static float GetTimestamp() { return Timestamp; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Basic")
    static FString GetDeviceName() { return DeviceName; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Basic")
    static int32 GetIsActive() { return IsActive; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Basic")
    static bool GetDataReceived() { return DataReceived; }

    // 摇杆数据
    UFUNCTION(BlueprintCallable, Category = "Arduino Joystick")
    static float GetJoystickX() { return JoystickX; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Joystick")
    static float GetJoystickY() { return JoystickY; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Joystick")
    static FVector2D GetJoystickVector() { return FVector2D(JoystickX, JoystickY); }

    // 压力传感器数据
    UFUNCTION(BlueprintCallable, Category = "Arduino Pressure")
    static float GetPressure1() { return Pressure1; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Pressure")
    static float GetPressure2() { return Pressure2; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Pressure")
    static FVector2D GetPressureVector() { return FVector2D(Pressure1, Pressure2); }

    // 加速度数据
    UFUNCTION(BlueprintCallable, Category = "Arduino Accelerometer")
    static float GetAccelX() { return AccelX; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Accelerometer")
    static float GetAccelY() { return AccelY; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Accelerometer")
    static float GetAccelZ() { return AccelZ; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Accelerometer")
    static FVector GetAccelVector() { return FVector(AccelX, AccelY, AccelZ); }

    // 陀螺仪数据
    UFUNCTION(BlueprintCallable, Category = "Arduino Gyroscope")
    static float GetGyroX() { return GyroX; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Gyroscope")
    static float GetGyroY() { return GyroY; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Gyroscope")
    static float GetGyroZ() { return GyroZ; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Gyroscope")
    static FVector GetGyroVector() { return FVector(GyroX, GyroY, GyroZ); }

    // 按钮状态
    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons")
    static bool GetButton1() { return Button1; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons")
    static bool GetButton2() { return Button2; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons")
    static bool GetButton3() { return Button3; }

    UFUNCTION(BlueprintCallable, Category = "Arduino Buttons")
    static bool GetButton4() { return Button4; }

    // 检查设备是否连接且活跃
    UFUNCTION(BlueprintCallable, Category = "Arduino Basic")
    static bool IsJoystickConnected() { return DataReceived && IsActive == 1; }

    // 获取所有数据的结构体
    UFUNCTION(BlueprintCallable, Category = "Arduino All Data")
    static FJoystickData GetAllJoystickData()
    {
        FJoystickData Data;
        Data.MessageID = MessageID;
        Data.Timestamp = Timestamp;
        Data.DeviceName = DeviceName;
        Data.DataReceived = DataReceived;
        Data.IsActive = IsActive;
        Data.JoystickX = JoystickX;
        Data.JoystickY = JoystickY;
        Data.Pressure1 = Pressure1;
        Data.Pressure2 = Pressure2;
        Data.AccelX = AccelX;
        Data.AccelY = AccelY;
        Data.AccelZ = AccelZ;
        Data.GyroX = GyroX;
        Data.GyroY = GyroY;
        Data.GyroZ = GyroZ;
        Data.Button1 = Button1;
        Data.Button2 = Button2;
        Data.Button3 = Button3;
        Data.Button4 = Button4;
        return Data;
    }

private:
    // OSC消息接收回调函数
    UFUNCTION()
    void OnOSCMessageReceived(const FOSCMessage& Message, const FString& IPAddress, int32 Port);

    // OSC服务器设置
    FString OSCServerIP = TEXT("0.0.0.0");
    int32 OSCServerPort = 7654;

    // 数据更新时间跟踪
    float LastUpdateTime = 0.0f;
    float DataTimeout = 2.0f; // 2秒无数据则认为断开连接

    // 消息计数器，用于更新基础信息
    int32 LocalMessageCounter = 0;
};
