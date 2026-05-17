#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Wire.h>

#define CHANNEL 1
#define MPU_ADDR 0x68
//Change Mac to ESP32 Mac
uint8_t receiverMac[] = {};

struct __attribute__((packed)) Telemetry {
  uint32_t packetID;

  float ax, ay, az;
  float gx, gy, gz;

  float gforce;
};

Telemetry data;

uint32_t packetID = 0;
bool sentOK = false;

float gx_off = 0, gy_off = 0, gz_off = 0;

// ---------------- SEND CALLBACK ----------------
void onSent(const uint8_t *mac, esp_now_send_status_t status) {
  sentOK = (status == ESP_NOW_SEND_SUCCESS);
}

// ---------------- GYRO CALIBRATION ----------------
void calibrateGyro() {
  long gx = 0, gy = 0, gz = 0;

  Serial.println("Calibrating gyro...");
  delay(2000);

  for (int i = 0; i < 300; i++) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6);

    gx += (Wire.read() << 8) | Wire.read();
    gy += (Wire.read() << 8) | Wire.read();
    gz += (Wire.read() << 8) | Wire.read();

    delay(3);
  }

  gx_off = gx / 300.0;
  gy_off = gy / 300.0;
  gz_off = gz / 300.0;

  Serial.println("Gyro done");
}

// ---------------- READ IMU ----------------
void readIMU() {

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14);

  int16_t ax = (Wire.read() << 8) | Wire.read();
  int16_t ay = (Wire.read() << 8) | Wire.read();
  int16_t az = (Wire.read() << 8) | Wire.read();

  Wire.read(); Wire.read();

  int16_t gx = (Wire.read() << 8) | Wire.read();
  int16_t gy = (Wire.read() << 8) | Wire.read();
  int16_t gz = (Wire.read() << 8) | Wire.read();

  data.ax = ax / 16384.0;
  data.ay = ay / 16384.0;
  data.az = az / 16384.0;

  data.gx = (gx - gx_off) / 131.0;
  data.gy = (gy - gy_off) / 131.0;
  data.gz = (gz - gz_off) / 131.0;

  data.gforce = sqrt(
    data.ax * data.ax +
    data.ay * data.ay +
    data.az * data.az
  );

  data.packetID = packetID++;
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  setCpuFrequencyMhz(80); // reduce power spikes

  Wire.begin(5, 4); // SDA, SCL (ESP32-C3 safe pins)
  Wire.setClock(100000);

  // wake MPU
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  delay(1000);
  calibrateGyro();

  WiFi.mode(WIFI_STA);

  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);
  esp_wifi_set_max_tx_power(60);

  esp_now_init();
  esp_now_register_send_cb(onSent);

  esp_wifi_config_espnow_rate(WIFI_IF_STA, WIFI_PHY_RATE_LORA_250K);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMac, 6);
  peer.channel = CHANNEL;
  peer.encrypt = false;

  esp_now_add_peer(&peer);

  Serial.println("Sender Ready");
}

// ---------------- LOOP ----------------
void loop() {

  readIMU();

  esp_now_send(receiverMac, (uint8_t*)&data, sizeof(data));

  delay(60);
}
