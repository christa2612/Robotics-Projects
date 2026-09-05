#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>

const char* AP_SSID = "Nano32-Servo-Arm";
const char* AP_PASSWORD = "12345678";

// Nano32 / ESP32 common I2C pins.
const uint8_t I2C_SDA_PIN = 21;
const uint8_t I2C_SCL_PIN = 22;

const uint8_t SERVO_COUNT = 4;
const uint8_t SERVO_CHANNELS[SERVO_COUNT] = {0, 1, 2, 3};
const uint8_t MIN_ANGLE = 0;
const uint8_t MAX_ANGLE = 90;
const uint16_t SERVO_FREQ = 50;
const uint16_t SERVO_MIN_US = 500;
const uint16_t SERVO_MAX_US = 2400;

const uint8_t MODE1 = 0x00;
const uint8_t MODE2 = 0x01;
const uint8_t PRESCALE = 0xFE;
const uint8_t LED0_ON_L = 0x06;

WebServer server(80);

uint8_t servoAngles[SERVO_COUNT] = {45, 45, 45, 45};
uint8_t pcaAddress = 0;

bool i2cWrite8(uint8_t address, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

uint8_t i2cRead8(uint8_t address, uint8_t reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) {
    return 0;
  }
  Wire.requestFrom((int)address, 1);
  return Wire.available() ? Wire.read() : 0;
}

bool i2cDevicePresent(uint8_t address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

uint8_t findPca9685Address() {
  for (uint8_t address = 0x40; address <= 0x7F; address++) {
    if (i2cDevicePresent(address)) {
      return address;
    }
  }
  return 0;
}

void setPwmFrequency(uint8_t address, uint16_t frequency) {
  // OUTDRV=1 gives totem-pole outputs, which is the normal servo signal mode.
  i2cWrite8(address, MODE2, 0x04);

  float prescaleValue = 25000000.0;
  prescaleValue /= 4096.0;
  prescaleValue /= frequency;
  prescaleValue -= 1.0;
  uint8_t prescale = (uint8_t)(prescaleValue + 0.5);

  uint8_t oldMode = i2cRead8(address, MODE1);
  uint8_t sleepMode = (oldMode & 0x7F) | 0x10;
  i2cWrite8(address, MODE1, sleepMode);
  i2cWrite8(address, PRESCALE, prescale);
  i2cWrite8(address, MODE1, oldMode);
  delay(5);
  i2cWrite8(address, MODE1, oldMode | 0xA1);
}

void setPwm(uint8_t address, uint8_t channel, uint16_t on, uint16_t off) {
  uint8_t reg = LED0_ON_L + 4 * channel;
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(on & 0xFF);
  Wire.write(on >> 8);
  Wire.write(off & 0xFF);
  Wire.write(off >> 8);
  Wire.endTransmission();
}

uint16_t microsecondsToTicks(uint16_t us) {
  const uint32_t periodUs = 1000000UL / SERVO_FREQ;
  return (uint16_t)((uint32_t)us * 4096UL / periodUs);
}

uint8_t clampAngle(int angle) {
  if (angle < MIN_ANGLE) {
    return MIN_ANGLE;
  }
  if (angle > MAX_ANGLE) {
    return MAX_ANGLE;
  }
  return (uint8_t)angle;
}

bool refreshPca() {
  if (pcaAddress != 0 && i2cDevicePresent(pcaAddress)) {
    return true;
  }

  pcaAddress = findPca9685Address();
  if (pcaAddress == 0) {
    return false;
  }

  setPwmFrequency(pcaAddress, SERVO_FREQ);
  return true;
}

bool writeServo(uint8_t index, int angle) {
  if (index >= SERVO_COUNT || !refreshPca()) {
    return false;
  }

  uint8_t safeAngle = clampAngle(angle);
  uint16_t pulseUs = map(safeAngle, MIN_ANGLE, MAX_ANGLE, SERVO_MIN_US, SERVO_MAX_US);
  setPwm(pcaAddress, SERVO_CHANNELS[index], 0, microsecondsToTicks(pulseUs));
  servoAngles[index] = safeAngle;
  return true;
}

void writeAllServos(uint8_t angle) {
  for (uint8_t i = 0; i < SERVO_COUNT; i++) {
    writeServo(i, angle);
    delay(20);
  }
}

String addressText() {
  if (pcaAddress == 0) {
    return "not found";
  }
  char buffer[8];
  snprintf(buffer, sizeof(buffer), "0x%02X", pcaAddress);
  return String(buffer);
}

String statusJson() {
  refreshPca();
  String json = "{\"pcaFound\":";
  json += pcaAddress == 0 ? "false" : "true";
  json += ",\"pcaAddress\":\"";
  json += addressText();
  json += "\",\"ip\":\"";
  json += WiFi.softAPIP().toString();
  json += "\",\"angles\":[";
  for (uint8_t i = 0; i < SERVO_COUNT; i++) {
    if (i > 0) {
      json += ",";
    }
    json += servoAngles[i];
  }
  json += "]}";
  return json;
}

void sendJson(int code, const String& body) {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(code, "application/json", body);
}

void handleRoot() {
  server.send(200, "text/html", R"HTML(
<!doctype html>
<html>
<head>
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>Nano32 Servo Arm</title>
  <style>
    body{font-family:Arial,sans-serif;margin:0;background:#f5f7fb;color:#1e293b}
    main{max-width:760px;margin:auto;padding:24px}
    header{display:flex;justify-content:space-between;gap:12px;align-items:center;flex-wrap:wrap}
    h1{margin:0;font-size:32px}.status{margin:8px 0 20px;color:#475569}
    button{border:0;border-radius:8px;background:#0f766e;color:white;font-weight:700;padding:12px 16px}
    .row{background:white;border:1px solid #dbe3ef;border-radius:8px;padding:16px;margin:12px 0}
    .top{display:flex;justify-content:space-between;font-weight:700;margin-bottom:12px}
    input{width:100%;accent-color:#0f766e}
  </style>
</head>
<body>
<main>
  <header>
    <div>
      <h1>Nano32 Servo Arm</h1>
      <div id="status" class="status">Loading...</div>
    </div>
    <button onclick="demo()">Test Move</button>
  </header>
  <div id="controls"></div>
</main>
<script>
const controls = document.querySelector("#controls");
const statusEl = document.querySelector("#status");
const values = [45,45,45,45];

function draw() {
  controls.innerHTML = "";
  values.forEach((value, channel) => {
    const row = document.createElement("div");
    row.className = "row";
    row.innerHTML = `<div class="top"><span>Channel ${channel}</span><span id="v${channel}">${value}°</span></div><input type="range" min="0" max="90" value="${value}">`;
    const input = row.querySelector("input");
    input.addEventListener("input", () => {
      values[channel] = Number(input.value);
      document.querySelector(`#v${channel}`).textContent = `${values[channel]}°`;
      fetch(`/servo?channel=${channel}&angle=${values[channel]}`);
    });
    controls.appendChild(row);
  });
}

async function loadStatus() {
  const res = await fetch("/status");
  const data = await res.json();
  data.angles.forEach((angle, index) => values[index] = angle);
  statusEl.textContent = `PCA9685: ${data.pcaFound ? data.pcaAddress : "NOT FOUND"} | IP: ${data.ip}`;
  draw();
}

async function demo() {
  statusEl.textContent = "Sending test move...";
  await fetch("/demo");
  await loadStatus();
}

draw();
loadStatus();
</script>
</body>
</html>
)HTML");
}

void handleStatus() {
  sendJson(200, statusJson());
}

void handleServo() {
  int channel = server.arg("channel").toInt();
  int angle = server.arg("angle").toInt();
  bool ok = writeServo((uint8_t)channel, angle);
  sendJson(ok ? 200 : 500, statusJson());
}

void handleDemo() {
  for (uint8_t channel = 0; channel < 16; channel++) {
    if (refreshPca()) {
      uint16_t lowPulse = microsecondsToTicks(SERVO_MIN_US);
      uint16_t midPulse = microsecondsToTicks((SERVO_MIN_US + SERVO_MAX_US) / 2);
      uint16_t highPulse = microsecondsToTicks(SERVO_MAX_US);
      setPwm(pcaAddress, channel, 0, lowPulse);
      delay(120);
      setPwm(pcaAddress, channel, 0, midPulse);
      delay(120);
      setPwm(pcaAddress, channel, 0, highPulse);
      delay(120);
      setPwm(pcaAddress, channel, 0, midPulse);
      delay(120);
    }
  }
  writeAllServos(45);
  sendJson(pcaAddress == 0 ? 500 : 200, statusJson());
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  refreshPca();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/servo", handleServo);
  server.on("/demo", handleDemo);
  server.begin();

  writeAllServos(45);
}

void loop() {
  server.handleClient();
}
