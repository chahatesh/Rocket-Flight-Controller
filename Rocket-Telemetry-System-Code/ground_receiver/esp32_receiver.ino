#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define CHANNEL 1

struct __attribute__((packed)) Telemetry {
  uint32_t packetID;

  float ax, ay, az;
  float gx, gy, gz;

  float gforce;
};

Telemetry data;

uint32_t lastID = 0;
uint32_t loss = 0;
uint32_t lastTime = 0;

// ---------------- RECEIVE ----------------
void onReceive(const uint8_t *mac, const uint8_t *incoming, int len) {

  memcpy(&data, incoming, sizeof(data));

  if (lastID != 0 && data.packetID > lastID + 1) {
    loss += (data.packetID - lastID - 1);
  }

  lastID = data.packetID;
  lastTime = millis();

  Serial.print("AX:");
  Serial.print(data.ax, 3);

  Serial.print(" AY:");
  Serial.print(data.ay, 3);

  Serial.print(" AZ:");
  Serial.print(data.az, 3);

  Serial.print(" | GX:");
  Serial.print(data.gx, 2);

  Serial.print(" GY:");
  Serial.print(data.gy, 2);

  Serial.print(" GZ:");
  Serial.print(data.gz, 2);

  Serial.print(" | G:");
  Serial.print(data.gforce, 3);

  Serial.print(" | ID:");
  Serial.print(data.packetID);

  Serial.print(" | LOSS:");
  Serial.print(loss);

  Serial.print(" | LINK:");

  if (millis() - lastTime > 500) {
    Serial.println("LOST");
  } else {
    Serial.println("OK");
  }
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);

  esp_now_init();
  esp_now_register_recv_cb(onReceive);

  Serial.println("Receiver Ready");
}

void loop() {
  delay(10);
}
