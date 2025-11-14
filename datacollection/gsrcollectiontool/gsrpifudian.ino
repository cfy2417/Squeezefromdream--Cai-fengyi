/*
 * Grove GSR传感器数据采集 - 用于Ledalab分析
 * 
 * 连接方式：
 * - Grove GSR传感器连接到Arduino的A0引脚
 * 
 * 采样率：10Hz（每100ms采集一次）
 * 这个采样率适合皮肤电反应分析
 */

const int GSR_PIN = A0;
const int SAMPLE_RATE_MS = 100;  // 100ms = 10Hz采样率
const float VCC = 5.0;            // Arduino供电电压
const float R_KNOWN = 10000.0;    // Grove GSR上的已知电阻(10kΩ)

unsigned long lastSampleTime = 0;
unsigned long startTime = 0;
bool isRecording = false;

void setup() {
  Serial.begin(115200);
  pinMode(GSR_PIN, INPUT);
  
  // 等待串口连接
  while (!Serial) {
    delay(10);
  }
  
  Serial.println("Grove GSR Sensor - Ledalab Data Collector");
  Serial.println("Commands:");
  Serial.println("  START - 开始记录数据");
  Serial.println("  STOP  - 停止记录数据");
  Serial.println("----------------------------------------");
}

void loop() {
  // 检查串口命令
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();
    
    if (command == "START") {
      isRecording = true;
      startTime = millis();
      lastSampleTime = startTime;
      Serial.println("RECORDING_STARTED");
      Serial.println("HEADER:Time(ms),GSR_Raw,GSR_Resistance(kohm),GSR_Conductance(uS)");
    } else if (command == "STOP") {
      isRecording = false;
      Serial.println("RECORDING_STOPPED");
    }
  }
  
  // 如果正在记录，按固定采样率采集数据
  if (isRecording) {
    unsigned long currentTime = millis();
    
    if (currentTime - lastSampleTime >= SAMPLE_RATE_MS) {
      lastSampleTime = currentTime;
      
      // 读取GSR传感器原始值
      int gsrRaw = analogRead(GSR_PIN);
      
      // 计算电压
      float voltage = (gsrRaw / 1023.0) * VCC;
      
      // 计算皮肤电阻 (kΩ)
      // 根据分压公式: Vout = Vin * R_skin / (R_known + R_skin)
      float resistance = 0;
      if (voltage > 0) {
        resistance = ((VCC - voltage) * R_KNOWN) / voltage / 1000.0;  // 转换为kΩ
      }
      
      // 计算皮肤电导 (μS = 微西门子)
      // 电导 = 1/电阻，单位转换：1/(kΩ) = mS, 再转为μS
      float conductance = 0;
      if (resistance > 0) {
        conductance = 1000.0 / resistance;  // μS
      }
      
      // 输出数据：时间戳,原始值,电阻(kΩ),电导(μS)
      unsigned long timestamp = currentTime - startTime;
      
      Serial.print("DATA:");
      Serial.print(timestamp);
      Serial.print(",");
      Serial.print(gsrRaw);
      Serial.print(",");
      Serial.print(resistance, 4);
      Serial.print(",");
      Serial.println(conductance, 4);
    }
  }
}