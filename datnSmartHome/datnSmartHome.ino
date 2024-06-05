#include <Wire.h>
#include <DHT.h>
#include <Adafruit_MCP23X17.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

char keys[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

Adafruit_MCP23X17 mcp;


String inputPassword = "";
const String primaryPassword = "1234";  // Define the primary password
bool newStatus = false;                 // Declare status variable
bool doorStatus = false;
bool updateDoorStatus();
bool updateDoorStatuskeypad();
bool fanStatus = false; 
bool ledStatus = false;
bool doorStatusKeypad = false;

unsigned long lastKeyTime = 0;
char lastKey = '\0';
int trangThaiCambien = 0;  // Khởi tạo giá trị ban đầu

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 0      // Pin connected to the DHT sensor
#define DHTTYPE DHT22  // DHT22
#define MCP23017_ADDR 0x26
#define flameThreshold 500
#define gasThreshold 700


#define RST_PIN 2
#define SS_PIN 5
#define BUZZER_PIN 17
#define SERVO1_PIN 4
#define SERVO2_PIN 13
#define SERVO3_PIN 33
#define gasPin 35
#define sensorPin 34
#define rainSensor 26
#define fanPin 14
#define ldrPin 32  // Pin đọc tín hiệu từ LDR
#define ledPin 25
#define pinCambien 15  // Chân cảm biến PIR kết nối tới chân 15
#define pinDen 27      // Chân LED kết nối tới chân 27
#define ledDen 12

const char *mqtt_server = "e9d8971ecb334d3d9e0c799cc633385b.s1.eu.hivemq.cloud";
const char *mqtt_username = "Huylong";
const char *mqtt_password = "Longk2002";

static const char *root_ca = R"EOF(
  -----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
  -----END CERTIFICATE-----
)EOF";

String password = "";
unsigned long lastKeypadTime = 0;
unsigned long reservationInterval = 1 * 60 * 1000;  // 1 minute in milliseconds                       // Track buzzer state
unsigned long lastLowTime = 0;
const unsigned long waitTime = 30 * 60 * 1000;  // 30 minutes in milliseconds
char ssid[] = "Hideonbush";
char pass[] = "111111110";
const char *serverName = "http://172.20.10.5:3000/security";
const char *serverNameFan = "http://172.20.10.5:3000/device";

Servo myServo1;
Servo myServo2;
Servo myServo3;

MFRC522 mfrc522(SS_PIN, RST_PIN);
char mqtt_client_id[20];
char keypadString[17] = "";
DHT dht(DHTPIN, DHTTYPE);
WiFiClientSecure espClient;
PubSubClient client(espClient);

void showSmartHome() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SmartHome");
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  dht.begin();
  lcd.backlight();
  showSmartHome();  // Hiển thị "SmartHome" khi khởi động
  connectToWiFi();
  myServo1.attach(SERVO1_PIN);
  myServo2.attach(SERVO2_PIN);
  myServo3.attach(SERVO3_PIN);
  myServo1.write(0);
  myServo2.write(180);
  myServo3.write(0);
  pinMode(rainSensor, INPUT);
  pinMode(sensorPin, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(gasPin, INPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(pinCambien, INPUT);  // Chân cảm biến là INPUT
  pinMode(pinDen, OUTPUT);     // Chân đèn là OUTPUT
  pinMode(ledDen, OUTPUT);
  mcp.begin_I2C(MCP23017_ADDR);
  for (int i = 8; i < 12; i++) {
    mcp.pinMode(i, OUTPUT);
    mcp.digitalWrite(i, HIGH);
  }
  for (int i = 12; i < 16; i++) {
    mcp.pinMode(i, INPUT_PULLUP);
  }
  SPI.begin();
  mfrc522.PCD_Init();
  connectToMQTT();
}

void connectToWiFi() {
  WiFi.begin(ssid, pass);
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 20000) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("Failed to connect to WiFi. Check credentials or try again.");
  }
}

void connectToMQTT() {
  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, 8883);
  int attempt = 0;
  while (!client.connected() && attempt < 3) {
    if (client.connect(mqtt_client_id, mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      attempt++;
    }
  }
  if (!client.connected()) {
    // Xử lý lỗi kết nối MQTT ở đây
  }
}

void checkLdr() {
  int ldrValue = analogRead(ldrPin);  // Đọc giá trị từ LDR (sử dụng analogRead cho tín hiệu analog)
  Serial.println(ldrValue);
  Serial.print("ldr Value: ");  // In giá trị đọc được ra Serial Monitor

  // Ngưỡng ánh sáng: bạn có thể điều chỉnh giá trị này
  if (ldrValue > 500) {          // Kiểm tra nếu giá trị nhỏ hơn 500
    digitalWrite(ledPin, HIGH);  // Bật đèn LED nếu ánh sáng yếu
  } else {
    digitalWrite(ledPin, LOW);  // Tắt đèn LED nếu ánh sáng đủ
  }
}

void checkCamBien() {
  // Đọc trạng thái của cảm biến
  trangThaiCambien = digitalRead(pinCambien);
  // Kiểm tra nếu có chuyển động
  if (trangThaiCambien == HIGH) {
    digitalWrite(pinDen, HIGH);  // Bật đèn
    Serial.println("Chuyển động phát hiện!");
  } else {
    digitalWrite(pinDen, LOW);  // Tắt đèn
    Serial.println("Không phát hiện chuyển động");
  }
}

void checkSensors() {
  int flameValue = analogRead(sensorPin);
  int gasValue = analogRead(gasPin);
  Serial.print("Flame: ");
  Serial.println(flameValue);
  Serial.print("Gas: ");
  Serial.println(gasValue);
  bool flameDetected = flameValue > flameThreshold;
  bool gasDetected = gasValue > gasThreshold;
  if (flameDetected || gasDetected) {
    digitalWrite(BUZZER_PIN, HIGH);  // Bật buzzer
    lcd.clear();
    if (flameDetected) {
      lcd.print("Flame Detected!");
    }
    if (gasDetected) {
      lcd.setCursor(0, 1);
      lcd.print("Gas Detected!");
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Tắt buzzer
    showSmartHome();
  }
}


char readKeypad() {
  for (int row = 8; row < 12; row++) {
    mcp.digitalWrite(row, LOW);

    for (int col = 12; col < 16; col++) {
      if (mcp.digitalRead(col) == LOW) {
        mcp.digitalWrite(row, HIGH);
        return keys[row - 8][col - 12];
      }
    }
    mcp.digitalWrite(row, HIGH);
  }

  return 0;
}

void checkRain() {
  int rainStatus = digitalRead(rainSensor);
  if (rainStatus == LOW) {  // LOW nghĩa là có mưa
    // Hiển thị trên LCD
    lcd.setCursor(0, 0);
    lcd.print("Dang mua");
    digitalWrite(BUZZER_PIN, HIGH);  // Bật buzzer
  } else {
    showSmartHome();
    digitalWrite(BUZZER_PIN, LOW);  // Tắt buzzer
  }
}

void checkKeypad() {
  checkDoorStatus();
  char key = readKeypad();
  if (key) {
    if (key == '*') {
      inputPassword = "";
      Serial.println("Password cleared");
    } else if (key == '#') {
      Serial.print("Password entered: ");
      Serial.println(inputPassword);
      // Compare with primary password
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://172.20.10.5:3000/security/664e3d5ab81a9f60deb5e083");
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
          String payload = http.getString();
          Serial.println(httpResponseCode);
          Serial.println(payload);

          StaticJsonDocument<256> doc;
          DeserializationError error = deserializeJson(doc, payload);

          if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
          } else {
            const char *primaryPassword = doc["primaryPassword"];  // Get the primaryPassword from JSON
            if (inputPassword == primaryPassword) {
              Serial.println("KEYPAD password is correct! Opening servo...");
              // Open the door
              myServo1.write(70);
              myServo2.write(70);
              Serial.println("Door is OPEN");

              // Update door status on server to true
              bool statusUpdated = updateDoorStatuskeypad(true);
              if (statusUpdated) {
                checkDoorStatusrfid();  // Check and update door status
              } else {
                Serial.println("Failed to update door status on server");
              }
            } else {
              // Close the door
              myServo1.write(0);
              myServo2.write(180);
              Serial.println("Door is CLOSED");

              // Update door status on server to false
              bool statusUpdated = updateDoorStatuskeypad(false);
              if (statusUpdated) {
                checkDoorStatus();  // Check and update door status
              } else {
                Serial.println("Failed to update door status on server");
              }
            }
          }
        } else {
          Serial.println("Error on HTTP request");
        }
        http.end();
      }
    } else {
      inputPassword += key;
      lcd.setCursor(0, 1);
      lcd.print(inputPassword);
    }
  }
}


void checkrfid() {
  checkDoorStatusrfid();
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      String rfidData = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        rfidData += String(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.print("RFID Data: ");
      Serial.println(rfidData);

      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://172.20.10.5:3000/security/664e2067299448f755ddac86");
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
          String payload = http.getString();
          Serial.println(httpResponseCode);
          Serial.println(payload);

          StaticJsonDocument<256> doc;
          DeserializationError error = deserializeJson(doc, payload);

          if (error) {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
          } else {
            const char *primaryPassword = doc["primaryPassword"];  // Get the primaryPassword from JSON
            if (rfidData == primaryPassword) {
              Serial.println("RFID password is correct! Opening servo...");
              // Open the door
              myServo3.write(90);
              Serial.println("Door is OPEN");

              // Update door status on server to true
              bool statusUpdated = updateDoorStatus(true);
              if (statusUpdated) {
                checkDoorStatusrfid();  // Check and update door status
              } else {
                Serial.println("Failed to update door status on server");
              }
            } else {
              // Close the door
              myServo3.write(0);
              Serial.println("Door is CLOSED");

              // Update door status on server to false
              bool statusUpdated = updateDoorStatus(false);
              if (statusUpdated) {
                checkDoorStatusrfid();  // Check and update door status
              } else {
                Serial.println("Failed to update door status on server");
              }
            }
          }
        } else {
          Serial.println("Error on HTTP request");
        }
        http.end();
      }
      mfrc522.PICC_HaltA();
    } else {
      Serial.println("Failed to read RFID card");
    }
  }
}
void loop() {
  checkKeypad();
  mfrc522.PCD_Init();  // Khởi tạo module RFID trong mỗi vòng lặp
  checkrfid();
  checkLdr();
  checkCamBien();
  checkSensors();
  checkLEDFromServer();
  checkRain();
  checkFanFromServer(); 
  checkrfid();
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  if (currentMillis - previousMillis >= 2000) {
    previousMillis = currentMillis;
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int gasValue = analogRead(gasPin);
    bool flameValue = digitalRead(sensorPin);
    client.publish("SmartHome/MQ8", String(gasValue).c_str());
    client.publish("SmartHome/flame", String(flameValue).c_str());
    client.publish("SmartHome/temperature", String(dht.readTemperature()).c_str());
    client.publish("SmartHome/humidity", String(h).c_str());
    client.loop();
  }
}

bool updateDoorStatuskeypad(bool Status) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://172.20.10.5:3000/device/6650e07457dacb0993aab456");
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<512> doc;
    doc["category"]["name"] = "Door";
    doc["category"]["topic"] = "Door";
    doc["category"]["status"] = Status;
    doc["category"]["color"] = "Color(0xffff1900)";
    doc["category"]["voice"] = "đóng-mở cửa";
    doc["category"]["notification"] = true;
    doc["category"]["time"] = "1";
    doc["category"]["icon"] = "0xf52a";
    doc["_id"] = "6650e07457dacb0993aab456";
    doc["namecategory"] = "Door";
    doc["colorcategory"] = "green";

    String json;
    serializeJson(doc, json);

    int httpResponseCode = http.PUT(json);
    http.end();

    if (httpResponseCode == 200) {
      Serial.println("Door server");
      return true;  // Cập nhật thành công
    } else {
      Serial.print("Error server: ");
      Serial.println(httpResponseCode);
      return false;  // Cập nhật không thành công
    }
  }
  Serial.println("WiFi not connected");
  return false;
}

void checkDoorStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://172.20.10.5:3000/device/6650e07457dacb0993aab456");

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(payload);

      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
      } else {
        bool doorStatusKeypad = doc["category"]["status"];  // Lấy trạng thái cửa từ JSON
        if (doorStatusKeypad) {
          myServo1.write(70);  // Mở cửa nếu trạng thái là true
          myServo2.write(70);  // Mở cửa nếu trạng thái là true
          Serial.println("Door is OPEN");
        } else {
          myServo1.write(0);    // Đóng cửa nếu trạng thái là false
          myServo2.write(180);  // Đóng cửa nếu trạng thái là false
          Serial.println("Door is CLOSED");
        }
      }
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
}
bool updateDoorStatus(bool newStatus) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://172.20.10.5:3000/device/664dfd4603ae96243f42b891");
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<512> doc;
    doc["category"]["name"] = "Gate";
    doc["category"]["topic"] = "Gate";
    doc["category"]["status"] = newStatus;
    doc["category"]["color"] = "Color(0xffff1900)";
    doc["category"]["voice"] = "đóng-mở cửa";
    doc["category"]["notification"] = true;
    doc["category"]["time"] = "1";
    doc["category"]["icon"] = "63595";
    doc["_id"] = "664dfd4603ae96243f42b891";
    doc["namecategory"] = "Gate";
    doc["colorcategory"] = "green";

    String json;
    serializeJson(doc, json);

    int httpResponseCode = http.PUT(json);
    http.end();

    if (httpResponseCode == 200) {
      Serial.println("Gate");
      return true;  // Cập nhật thành công
    } else {
      Serial.print("Errorserver: ");
      Serial.println(httpResponseCode);
      return false;  // Cập nhật không thành công
    }
  }
  Serial.println("WiFi not connected");
  return false;
}

void checkDoorStatusrfid() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://172.20.10.5:3000/device/664dfd4603ae96243f42b891");

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(payload);

      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
      } else {
        bool doorStatus = doc["category"]["status"];  // Lấy trạng thái cửa từ JSON
        if (doorStatus) {
          myServo3.write(90);  // Mở cửa nếu trạng thái là true
          Serial.println("Door is OPEN");
        } else {
          myServo3.write(0);  // Đóng cửa nếu trạng thái là false
          Serial.println("Door is CLOSED");
        }
      }
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
}

void checkFanFromServer() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverNameFan);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(payload);
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      fanStatus = false;
      JsonArray arr = doc["data"];
      for (JsonObject obj : arr) {
        const char* categoryName = obj["category"]["name"];
        bool categoryStatus = obj["category"]["status"];
        if (strcmp(categoryName, "fan") == 0) {
          fanStatus = categoryStatus;
          break;
        }
      }
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }

  if (fanStatus) {
    digitalWrite(fanPin, HIGH);
    Serial.println("Fan is ON");
  } else {
    digitalWrite(fanPin, LOW);
    Serial.println("Fan is OFF");
  }
}

void checkLEDFromServer() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverNameFan);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(payload);

      StaticJsonDocument<500> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      // Duyệt qua mảng "data" và kiểm tra trạng thái của từng "category"
      ledStatus = false; // Khởi tạo ledStatus là false trước khi duyệt qua mảng
      JsonArray arr = doc["data"];
      for (JsonObject obj : arr) {
        const char* categoryName = obj["category"]["name"];
        bool categoryStatus = obj["category"]["status"];
        if (strcmp(categoryName, "ledPin") == 0) {
          ledStatus = categoryStatus;
          break;
        }
      }
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }

  // Kiểm tra và điều khiển LED dựa trên giá trị của biến ledStatus
  if (ledStatus) {
    digitalWrite(ledDen, HIGH);
    Serial.println("LED is ON");
  } else {
    digitalWrite(ledDen, LOW);
    Serial.println("LED is OFF");
  }
}