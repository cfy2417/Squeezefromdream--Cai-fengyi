void setup() {
    Serial.begin(115200);
    unsigned long serialTimeout = millis();
    while (!Serial && (millis() - serialTimeout < 2000)) {
        delay(10);
    }
    
    // ✅ 修改1：使用INPUT模式（不需要上拉）
    pinMode(button1Pin, INPUT);
    pinMode(button2Pin, INPUT);
    pinMode(button3Pin, INPUT);
    
    // ✅ 修改2：延迟让模块稳定
    delay(100);
    
    // ✅ 修改3：初始化按钮状态（直接读取，不反转）
    button1LastState = digitalRead(button1Pin);
    button2LastState = digitalRead(button2Pin);
    button3LastState = digitalRead(button3Pin);
    
    button1State = button1LastState;
    button2State = button2LastState;
    button3State = button3LastState;
    
    Serial.println("Button states initialized:");
    Serial.print("B1="); Serial.print(button1State);
    Serial.print(" B2="); Serial.print(button2State);
    Serial.print(" B3="); Serial.println(button3State);
    
    // 初始化I2C和陀螺仪
    Wire.begin(sdaPin, sclPin);
    mpu.initialize();
    
    // ... 其余代码保持不变
}

// ✅ 修改4：去掉反转逻辑
bool readButtonDebounced(int pin, bool currentState, bool &lastState, unsigned long &lastDebounceTime) {
    bool reading = digitalRead(pin); // 直接读取，不反转
    
    if (reading != lastState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != currentState) {
            currentState = reading;
        }
    }
    
    lastState = reading;
    return currentState;
}

ESP32主机端 - 同样修复
cppvoid setup() {
    // ... 其他初始化
    
    // ✅ Button4也改为INPUT
    pinMode(button4Pin, INPUT);
    delay(100);
    
    button4LastState = digitalRead(button4Pin);
    button4State = button4LastState;
    
    Serial.print("Button4 initial state: ");
    Serial.println(button4State);
    
    // ... 其余代码
}

// ✅ 修改readAndSendButton函数
void readAndSendButton(int pin, const char* address, bool &currentState, bool &lastState) {
    bool reading = digitalRead(pin); // 直接读取，不反转
    
    if (reading != lastState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != currentState) {
            currentState = reading;
            sendOSCBool(address, currentState);
        }
    }
    
    lastState = reading;
}