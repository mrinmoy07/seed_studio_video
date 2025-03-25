 
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"


#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  10
#define SIOD_GPIO_NUM  40
#define SIOC_GPIO_NUM  39

#define Y9_GPIO_NUM    48
#define Y8_GPIO_NUM    11
#define Y7_GPIO_NUM    12
#define Y6_GPIO_NUM    14
#define Y5_GPIO_NUM    16
#define Y4_GPIO_NUM    18
#define Y3_GPIO_NUM    17
#define Y2_GPIO_NUM    15
#define VSYNC_GPIO_NUM 38
#define HREF_GPIO_NUM  47
#define PCLK_GPIO_NUM  13

// ====== USER SETTINGS ======
const char* WIFI_SSID     ="CFCASSAM1";
const char* WIFI_PASSWORD = "90909090"; 


// const char* SERVER_HOST   = "4.240.90.1";
// const int   SERVER_PORT   = 445;
 
// String DEVICE_ID = "ESP19475638746552874639746379272472479249238239652369234";

// // How frequently to send frames (ms)
// uint32_t uploadInterval = 2000;

// void setupCamera() {
//   camera_config_t config;
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer   = LEDC_TIMER_0;
//   config.pin_d0       = Y2_GPIO_NUM;
//   config.pin_d1       = Y3_GPIO_NUM;
//   config.pin_d2       = Y4_GPIO_NUM;
//   config.pin_d3       = Y5_GPIO_NUM;
//   config.pin_d4       = Y6_GPIO_NUM;
//   config.pin_d5       = Y7_GPIO_NUM;
//   config.pin_d6       = Y8_GPIO_NUM;
//   config.pin_d7       = Y9_GPIO_NUM;
//   config.pin_xclk     = XCLK_GPIO_NUM;
//   config.pin_pclk     = PCLK_GPIO_NUM;
//   config.pin_vsync    = VSYNC_GPIO_NUM;
//   config.pin_href     = HREF_GPIO_NUM;
//   config.pin_sscb_sda = SIOD_GPIO_NUM;
//   config.pin_sscb_scl = SIOC_GPIO_NUM;
//   config.pin_pwdn     = PWDN_GPIO_NUM;
//   config.pin_reset    = RESET_GPIO_NUM;
//   config.xclk_freq_hz = 20000000;
//   config.pixel_format = PIXFORMAT_JPEG;
  
//   // Select lower framesize if you run out of memory
//   config.frame_size   = FRAMESIZE_QVGA; 
//   config.jpeg_quality = 12; 
//   config.fb_count     = 1;

//   // Initialize the camera
//   esp_err_t err = esp_camera_init(&config);
//   if (err != ESP_OK) {
//     Serial.println("Camera init failed with error 0x" + String(err, HEX));
//     while(true) { delay(100); }
//   }
// }
const char* SERVER_HOST   = "4.240.90.1";
const int   SERVER_PORT   = 80;

String DEVICE_ID = "testDevice";

// How frequently to send frames (ms)
uint32_t uploadInterval = 100;

void setupCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Select lower framesize if you run out of memory
  config.frame_size   = FRAMESIZE_XGA; 
  config.jpeg_quality = 10; 
  config.fb_count     = 5;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.println("Camera init failed with error 0x" + String(err, HEX));
    while(true) { delay(100); }
  }
}

// void setup() {
//   Serial.begin(115200);
  
//   // Connect to Wi-Fi
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nConnected to WiFi!");

//   // Initialize camera
//   setupCamera();
// }
void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Initialize camera
  setupCamera();
}

void loop() {
  // Capture a frame
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    delay(uploadInterval);
    return;
  }

  // Upload it
  bool success = uploadFrame(fb->buf, fb->len);

  // Return the frame buffer back to the driver for reuse
  esp_camera_fb_return(fb);

  // Wait a bit before sending next frame
  delay(uploadInterval);
}

/**
 * Uploads a single JPEG frame buffer to the HTTPS server as multipart/form-data.
 */
// bool uploadFrame(uint8_t *jpg_data, size_t jpg_len) {
//   // WiFiClientSecure client;
//   // // Accept self-signed certificate
//   // client.setInsecure();  
//   WiFiClient client;


//   Serial.print("Connecting to server: ");
//   Serial.print(SERVER_HOST);
//   Serial.print(":");
//   Serial.println(SERVER_PORT);

//   if (!client.connect(SERVER_HOST, SERVER_PORT)) {
//     Serial.println("Connection to server failed.");
//     return false;
//   }

//   // Construct the POST request (multipart/form-data)
//   String boundary = "----ESP32CamUploaderBoundary";
//   String start_request = 
//     "POST /upload/" + DEVICE_ID + " HTTP/1.1\r\n" +
//     "Host: " + String(SERVER_HOST) + "\r\n" +
//     "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n" +
//     "Connection: close\r\n";

//   // Create multipart headers
//   String part1 = "--" + boundary + "\r\n" 
//                  "Content-Disposition: form-data; name=\"frame\"; filename=\"frame.jpg\"\r\n"
//                  "Content-Type: image/jpeg\r\n\r\n";

//   String part2 = "\r\n--" + boundary + "--\r\n";

//   // Calculate the total content length
//   size_t contentLength = part1.length() + jpg_len + part2.length();

//   // Add Content-Length
//   start_request += "Content-Length: " + String(contentLength) + "\r\n\r\n";

//   // Send the initial part of the request
//   client.print(start_request);

//   // Send the multipart headers
//   client.print(part1);

//   // Actual JPEG data
//   client.write(jpg_data, jpg_len);

//   // Final boundary
//   client.print(part2);

//   // Wait for server response
//   unsigned long timeout = millis();
//   while (client.connected() && millis() - timeout < 5000) {
//     if (client.available()) {
//       String line = client.readStringUntil('\n');
//       Serial.println(line);
//       // Simple check for success
//       if (line.indexOf("200") != -1) {
//         // We got a 200 OK response
//         return true;
//       }
//     }
//   }

//   return false;
// }
bool uploadFrame(uint8_t *jpg_data, size_t jpg_len) {
  WiFiClient client;

  Serial.print("Connecting to server: ");
  Serial.print(SERVER_HOST);
  Serial.print(":");
  Serial.println(SERVER_PORT);

  if (!client.connect(SERVER_HOST, SERVER_PORT)) {
    Serial.println("Connection to server failed.");
    return false;
  }

  String boundary = "----ESP32CamUploaderBoundary";
  String start_request = 
    "POST /upload/" + DEVICE_ID + " HTTP/1.1\r\n" +
    "Host: " + String(SERVER_HOST) + "\r\n" +
    "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n" +
    "Connection: close\r\n";

  String part1 = "--" + boundary + "\r\n" 
                 "Content-Disposition: form-data; name=\"frame\"; filename=\"frame.jpg\"\r\n"
                 "Content-Type: image/jpeg\r\n\r\n";

  String part2 = "\r\n--" + boundary + "--\r\n";

  size_t contentLength = part1.length() + jpg_len + part2.length();
  start_request += "Content-Length: " + String(contentLength) + "\r\n\r\n";

  client.print(start_request);
  client.print(part1);
  client.write(jpg_data, jpg_len);
  client.print(part2);

  // 🔍 **NEW: Read and Print Entire Server Response**
  Serial.println("Waiting for server response...");
  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 5000) {
    while (client.available()) {
      String responseLine = client.readStringUntil('\n');
      Serial.println(responseLine); // Print each line of the response
    }
  }

  Serial.println("Upload finished.");
  return true;
}

