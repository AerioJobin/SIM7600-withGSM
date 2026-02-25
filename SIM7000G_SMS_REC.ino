#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024
#define SerialAT Serial1

// ================= USER SETTINGS =================
#define SMS_TARGET  "//target number"
#define GSM_PIN     ""
// =================================================

#include <TinyGsmClient.h>
#include <SPI.h>
#include <Ticker.h>

// ================= HARDWARE SETTINGS =================
#define UART_BAUD 115200
#define PIN_TX   27
#define PIN_RX   26
#define PWR_PIN  4
// =====================================================

TinyGsm modem(SerialAT);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nStarting SIM7000...");

  pinMode(PWR_PIN, OUTPUT);

  // ===== Proper SIM7000 power-on sequence =====
  digitalWrite(PWR_PIN, LOW);
  delay(2000);              // Hold PWRKEY LOW
  digitalWrite(PWR_PIN, HIGH);
  delay(10000);             // Allow modem to fully boot
  // ============================================

  SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
  delay(3000);

  Serial.println("Initializing modem...");
  if (!modem.init()) {
    Serial.println("❌ Modem init failed");
  } else {
    Serial.println("✅ Modem init OK");
  }

  Serial.println("Modem Name: " + modem.getModemName());

  String info = modem.getModemInfo();
  if (info.length() > 0) {
    Serial.println("Modem Info: " + info);
  } else {
    Serial.println("⚠️ Modem info empty (still OK)");
  }

  // SIM status
  if (modem.getSimStatus() != 3) {
    Serial.println("❌ SIM not ready");
  } else {
    Serial.println("✅ SIM ready");
  }

  // ===== CRITICAL SMS CONFIGURATION =====
  modem.sendAT("+CMGF=1");            // SMS text mode
  modem.waitResponse();

  modem.sendAT("+CSCS=\"GSM\"");      // GSM 7-bit charset
  modem.waitResponse();

  modem.sendAT("+CSMP=17,167,0,0");   // Allow long (concatenated) SMS
  modem.waitResponse();
  // =====================================
}

void loop() {
  Serial.println("Waiting for LTE network (up to 3 minutes)...");
  if (!modem.waitForNetwork(180000L)) {
    Serial.println("❌ Network not registered");
    delay(10000);
    return;
  }

  Serial.println("✅ Network registered");

  int16_t rssi = modem.getSignalQuality();
  Serial.print("Signal quality (RSSI): ");
  Serial.println(rssi);

  bool ok = modem.sendSMS(
    SMS_TARGET,
    "ESP32 + SIM7000 STATUS REPORT: The LTE modem has powered on correctly, the SIM card "
    "is detected and ready, and the device has successfully registered on the LTE "
    "network. This long SMS confirms reliable message transmission using TinyGSM."
  );

  if (ok) {
    Serial.println("✅ SMS sent successfully");
  } else {
    Serial.println("❌ SMS failed to send");
  }

  // Keep modem alive forever
  while (true) {
    modem.maintain();
    delay(1000);
  }
}
