#include <WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ArduinoOTA.h>

// ==============================
// CONFIGURACIÓN GENERAL
// ==============================
#define LED_PWM 2
#define LED_EXT 23
#define BUTTON_PIN 19

Adafruit_SH1106G display(128, 64, &Wire, -1);

// ==============================
// CONFIGURACIÓN WIFI Y THINGSPEAK
// ==============================
const char* ssid = "ACNET2";
const char* password = "";
unsigned long channelID = 3143600;
// const char* readAPIKey = "TU_READ_API_KEY"; // si es privado

// ==============================
// CONFIGURACIÓN OTA
// ==============================
void setupOTA() {
  ArduinoOTA.setHostname("ESP32_Grupo03");
  ArduinoOTA.setPassword("G03_pass");
  ArduinoOTA.begin();
}

WiFiClient  client;
unsigned long lastRead = 0;
int pantalla = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_EXT, OUTPUT);

  display.begin(0x3C, true);
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);

  conectarWiFi();

  ThingSpeak.begin(client);

  setupOTA();

  displayMessage("Lectura lista", "Esperando datos...");
}

void loop() {
  ArduinoOTA.handle();

  if (millis() - lastRead > 10000) {  // cada 10 segundos
    lastRead = millis();
    leerDatos();
  }
}

void leerDatos() {
  if(WiFi.status() != WL_CONNECTED){
    conectarWiFi();
  }

  int num = ThingSpeak.readIntField(channelID, 1);
  float temp = ThingSpeak.readFloatField(channelID, 2);
  float hum = ThingSpeak.readFloatField(channelID, 3);
  float tiempo = ThingSpeak.readFloatField(channelID, 4);
  String estado = ThingSpeak.readStatus(channelID);

  // Checkea lectura
  int statusCode = ThingSpeak.getLastReadStatus();
  if(statusCode == 200){
    mostrarEnDisplay(num, temp, hum, tiempo, estado);
  }
  else{
    Serial.println("Problemas leyendo el canal. HTTP error code " + String(statusCode)); 
  }

  // Simulación si no hay conexión
  //mostrarEnDisplay("350", "24.3", "55.8", "300", "Habilitado");
}

void mostrarEnDisplay(int num, float temp, float hum, float tiempo, String estado) {
  String line1, line2;
  switch (pantalla) {
    case 0:
      line1 = "Numero Aleatoreo: " + String(num);
      line2 = "Temp: " + String(temp) + "C";
      break;
    case 1:
      line1 = "Humedad: " + String(hum) + "%";
      line2 = "Tiempo: " + String(tiempo) + "s";
      break;
    case 2:
      line1 = "Estado:";
      line2 = estado;
      break;
  }
  pantalla = (pantalla + 1) % 3;
  displayMessage(line1, line2);
}

void conectarWiFi(){
  Serial.print("\nConectando a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);     
  } 
  Serial.println("\nConectado!");
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
