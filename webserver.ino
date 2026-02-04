#include <WiFi.h>
#include <WebServer.h>
#include <Arducam_Mega.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>

// Pins for LILYGO T-SIM7600G
const int CAM_CS = 5;
const int SD_CS  = 13;

Arducam_Mega myCAM(CAM_CS);
WebServer server(80);

const char* ssid = "Aerio4048";
const char* password = "aerio12345678";

// --- CAMERA TO SD SAVING LOGIC ---
String saveToSD() {
  uint8_t status = myCAM.takePicture(CAM_IMAGE_MODE_320X320, CAM_IMAGE_PIX_FMT_JPG);
  if (status != 0) return "";

  // Find a unique filename
  int i = 0;
  String fileName;
  while (true) {
    fileName = "/img" + String(i) + ".jpg";
    if (!SD.exists(fileName)) break;
    i++;
  }

  File file = SD.open(fileName, FILE_WRITE);
  if (!file) return "";

  uint32_t len = myCAM.getTotalLength();
  while (len--) {
    file.write(myCAM.readByte());
  }
  file.close();
  return fileName;
}

// --- WEB HANDLERS ---

void handleRoot() {
  String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body>";
  html += "<h1>📸 Aerio Cam System</h1>";
  html += "<p><a href='/capture'><button style='padding:15px;'>Take New Photo & Save</button></a></p>";
  html += "<p><a href='/gallery'><button style='padding:15px;'>View SD Gallery</button></a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleCapture() {
  String savedPath = saveToSD();
  if (savedPath != "") {
    String html = "<html><body><h2>Success!</h2>";
    html += "<p>Saved as: " + savedPath + "</p>";
    html += "<img src='/streamFile?name=" + savedPath + "' style='width:320px;'><br>";
    html += "<a href='/'>Back Home</a></body></html>";
    server.send(200, "text/html", html);
  } else {
    server.send(500, "text/plain", "Hardware Error: Check SD/Camera");
  }
}

void handleGallery() {
  File root = SD.open("/");
  String html = "<html><body><h1>SD Card Gallery</h1><ul>";
  File file = root.openNextFile();
  while (file) {
    String name = String(file.name());
    if (name.endsWith(".jpg")) {
      html += "<li><a href='/streamFile?name=/" + name + "'>" + name + "</a></li>";
    }
    file = root.openNextFile();
  }
  html += "</ul><br><a href='/'>Back Home</a></body></html>";
  server.send(200, "text/html", html);
}

void handleFileStream() {
  if (!server.hasArg("name")) { server.send(404); return; }
  String path = server.arg("name");
  File file = SD.open(path, FILE_READ);
  if (!file) { server.send(404); return; }
  server.streamFile(file, "image/jpeg");
  file.close();
}

// --- SETUP & LOOP ---

void setup() {
  Serial.begin(115200);
  
  // Initialize SPI
  SPI.begin(18, 19, 23);

  // Init SD
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card error!");
  }

  // Init Camera
  myCAM.begin();

  // Init Wi-Fi Hotspot
  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP: "); Serial.println(WiFi.softAPIP());

  // Define Routes
  server.on("/", handleRoot);
  server.on("/capture", handleCapture);
  server.on("/gallery", handleGallery);
  server.on("/streamFile", handleFileStream);

  server.begin();
  Serial.println("Web Server Started.");
}

void loop() {
  server.handleClient();
}