#include <WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DHT.h>
#include <ArduinoOTA.h>

// ==============================
// CONFIGURACIÓN GENERAL
// ==============================
#define DHTPIN 33
#define DHTTYPE DHT22
#define LED_PWM 2
#define LED_EXT 23
#define POT_PIN 32
#define BUTTON_PIN 19

// ==============================
// CONFIGURACIÓN DE WIFI Y THINGSPEAK
// ==============================
const char* ssid = "ACNET2";
const char* password = "";
unsigned long channelID = 3143600;
const char* writeAPIKey = "CERUUAI3S4HZJW8Z";

// ==============================
// CONFIGURACIÓN OTA
// ==============================
void setupOTA() {
  ArduinoOTA.setHostname("ESP32_Grupo03");
  ArduinoOTA.setPassword("G03_pass");
  ArduinoOTA.begin();
}

WiFiClient cliente;
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SH1106G display(128, 64, &Wire, -1);

const char* estados[] = {"No disponible", "En reparación", "Habilitado", "Fuera de servicio"};
int estadoIndex = 0;
unsigned long lastSendTime = 0;
unsigned long startTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_EXT, OUTPUT);
  analogWrite(LED_PWM, 0);

  dht.begin();

  display.begin(0x3C, true);
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");

  startTime = millis();
  setupOTA();

  ThingSpeak.begin(cliente);

  displayMessage("Inicio OK", "Esperando envio...");
}

void loop() {
  ArduinoOTA.handle();

  if (digitalRead(BUTTON_PIN) == LOW) {
    estadoIndex = (estadoIndex + 1) % 4;
    delay(300);
    displayMessage("Estado cambiado", estados[estadoIndex]);
  }

  unsigned long now = millis();
  if (now - lastSendTime > 16000) {
    lastSendTime = now;
    enviarDatos();
  }
}

void enviarDatos() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int num = random(100, 501);
  long tiempo = (millis() - startTime) / 1000;
  const char* estadoActual = estados[estadoIndex];

  Serial.printf("Enviando datos -> Valor:%d Temp:%.2f Hum:%.2f Tiempo:%lus Estado:%s\n",
                num, temp, hum, tiempo, estadoActual);

  if (WiFi.status() == WL_CONNECTED) {
    ThingSpeak.setField (1,num);
    ThingSpeak.setField (2,temp);
    ThingSpeak.setField (3,hum);
    ThingSpeak.setField (4,tiempo);
    ThingSpeak.setStatus(estadoActual);
    int statusCode = ThingSpeak.writeFields(channelID,writeAPIKey);
    Serial.print("Datos enviados a ThingSpeak!   ");
    if(statusCode == 200){
      Serial.println("Canal escrito correctamente.");
    }
    else{
      Serial.println("Problemas al actualizar el canal. HTTP error code " + String(statusCode));
    }
  }

  displayMessage("Datos enviados", estadoActual);
}

void displayMessage(String l1, String l2) {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.println(l1);
  display.setCursor(0, 30);
  display.setTextSize(1);
  display.println(l2);
  display.display();
}
