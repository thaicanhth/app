#define DHTPIN 12      // Pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11 or DHT22
#define MCP23017_ADDR 0x26
#define flameThreshold 200
#define gasThreshold 850

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

char keys[4][4] = {
        { '1', '2', '3', 'A' },
        { '4', '5', '6', 'B' },
        { '7', '8', '9', 'C' },
        { '*', '0', '#', 'D' }
};

Adafruit_MCP23X17 mcp;

String correctPassword = "1234";
String inputPassword;
unsigned long lastKeyTime = 0;
char lastKey = '\0';
int trangThaiCambien = 0;  // Khởi tạo giá trị ban đầu

LiquidCrystal_I2C lcd(0x27, 16, 2);

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
#define pinCambien 15  // Chân cảm biến PIR kết nối tới chân 32
#define pinDen 27      // Chân LED kết nối tới chân 27

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
    if (ldrValue > 500) {  // Kiểm tra nếu giá trị nhỏ hơn 500
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
    delay(200);
}

void checkSensors() {
    int flameValue = analogRead(sensorPin);
    int gasValue = analogRead(gasPin);
    Serial.print("Flame Sensor Value: ");
    Serial.println(flameValue);
    Serial.print("Gas Value: ");
    Serial.println(gasValue);

    bool flameDetected = flameValue > flameThreshold;
    bool gasDetected = gasValue > gasThreshold;

    if (flameDetected || gasDetected) {
        digitalWrite(BUZZER_PIN, HIGH);  // Bật buzzer
        lcd.clear();
        if (flameDetected) {
            lcd.print("Flame Detected!");
            Serial.println("Buzzer ON - Flame detected");
        }
        if (gasDetected) {
            lcd.setCursor(0, 1);
            lcd.print("Gas Detected!");
            Serial.println("Buzzer ON - Gas detected");
        }
    } else {
        digitalWrite(BUZZER_PIN, LOW);  // Tắt buzzer
        showSmartHome();
        Serial.println("Buzzer OFF - No flame or gas detected");
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
        lcd.setCursor(0, 1);
        lcd.print("Dang mua");
        Serial.print("đang mưa");
        digitalWrite(BUZZER_PIN, HIGH);  // Bật buzzer
    } else {
        showSmartHome();
        digitalWrite(BUZZER_PIN, LOW);  // Tắt buzzer
        Serial.print("ko mưa");
    }
}

void checkFan() {
    int gasValue = analogRead(gasPin);
    bool gasDetected = gasValue > gasThreshold;
    if (gasDetected) {             // Nếu giá trị khí gas vượt ngưỡng
        digitalWrite(fanPin, HIGH);  // Bật quạt
    } else {
        digitalWrite(fanPin, LOW);  // Tắt quạt
    }
}


void loop() {
    checkLdr();
    checkCamBien();
    if (!client.connected()) {
        connectToMQTT();
    }
    client.loop();
    checkSensors();
    checkRain();
    checkFan();
    char key = readKeypad();
    if (key != 0 && millis() - lastKeyTime > 200) {
        lastKeyTime = millis();
        if (key == '#') {
            if (inputPassword == correctPassword) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Correct Password");
                myServo1.write(90);
                myServo2.write(90);
                delay(3000);
                myServo1.write(0);
                myServo2.write(180);
                delay(2000);  // Display "Correct Password" for 2 seconds
                showSmartHome();
                // Additional actions when the password is correct
            } else {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Wrong Password");
                delay(2000);  // Display "Wrong Password" for 2 seconds
                showSmartHome();
            }
            inputPassword = "";  // Reset input password
        } else if (key == '*') {
            inputPassword = "";  // Clear the current input
        } else {
            inputPassword += key;
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print(inputPassword);  // Display the current input password on the LCD
        }
    }

    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }
    Serial.print("UID tag :");
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    content.toUpperCase();
    if (content.substring(1) == "B1 0E 12 1D") {
        Serial.println("Access Granted");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Granted");
        myServo3.write(90);
        delay(3000);
        myServo3.write(0);
        showSmartHome();  // Hiển thị "SmartHome" sau khi sử dụng RFID
        delay(2000);      // Đợi 2 giây
    } else {
        Serial.println("Access Denied");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Denied");
        delay(2000);
        showSmartHome();  // Hiển thị "SmartHome" sau khi sử dụng RFID
        delay(2000);      // Đợi 2 giây
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
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
        client.publish("SmartHome/temperature", String(t).c_str());
        client.publish("SmartHome/humidity", String(h).c_str());
        client.loop();
    }
    delay(100);
}