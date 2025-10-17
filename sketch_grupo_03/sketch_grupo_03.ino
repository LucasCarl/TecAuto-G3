#include <Adafruit_SH110X.h>
#include <Adafruit_GFX.h>
#include <DHT.h>

// ==============================
// CONFIGURACIÓN GENERAL
// ==============================
#define DHTPIN 33           // Pin del sensor DHT
#define DHTTYPE DHT22       // O DHT11 según la placa
#define LED_PWM 2          // LED Integrado con PWM (LEDC)
#define LED_EXT 23          // LED externo (alarma)
#define POT_PIN 32          // Potenciómetro (entrada analógica)
#define BUTTON_PIN 19       // Pulsador
#define TOUCH_PLUS 13       // Pin táctil +
#define TOUCH_MINUS 4       // Pin táctil -

// Configuración LEDC
//#define LED_CHANNEL 0       // Canal LEDC
#define LED_FREQ 5000       // Frecuencia 5kHz
#define LED_RESOLUTION 10    // Resolución 10 bits (0-1023)

// Pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Variables globales
int modo = 1;
float temp, hum;
int potValue = 0;
int brillo = 0;

float HmD = 40;   // Humedad mínima deseada
float TMD = 30;   // Temperatura máxima deseada

DHT dht(DHTPIN, DHTTYPE);

// ==============================
// SETUP
// ==============================
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_EXT, OUTPUT);

  // Configurar LEDC para LED PWM
  ledcAttach(LED_PWM, LED_FREQ, LED_RESOLUTION);

  dht.begin();
  display.begin(0x3C, true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("TP1 IoT - ESP32");
  display.display();
  delay(2000);
}

// ==============================
// LOOP PRINCIPAL
// ==============================
void loop() {
  // Cambio de modo con pulsador
  if (digitalRead(BUTTON_PIN) == LOW) {
    modo++;
    if (modo > 4) modo = 1;
    delay(400); // antirrebote
  }

  // Leer sensores
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  
  // Validar lecturas del DHT
  if (isnan(hum) || isnan(temp)) {
    Serial.println("Error: Fallo en lectura DHT");
    return; // Salir del loop si hay error
  }
  
  potValue = analogRead(POT_PIN);

  // Evaluar cada modo
  switch (modo) {
    case 1:
      mostrarAmbientales();
      break;
    case 2:
      controlLED();
      break;
    case 3:
      configurarHumedad();
      break;
    case 4:
      configurarTemperatura();
      break;
  }

  verificarAlarma();
  delay(200);
}

// ==============================
// FUNCIONES DE MODO
// ==============================

// MODO 1: Visualizar temperatura y humedad
void mostrarAmbientales() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Modo 1: Ambiente");
  display.printf("Temp: %.1f C\n", temp);
  display.printf("Hum:  %.1f %%", hum);
  display.display();
}

// MODO 2: Control de LED con potenciómetro
void controlLED() {
  brillo = map(potValue, 0, 4095, 0, 1023);
  ledcWrite(LED_PWM, brillo);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Modo 2: LED PWM");
  display.printf("Pot: %d\nBrillo: %d", potValue, brillo);
  display.display();
}

// MODO 3: Configurar Humedad mínima (touch)
void configurarHumedad() {
  if (touchRead(TOUCH_PLUS) < 40) HmD += 1;
  if (touchRead(TOUCH_MINUS) < 40) HmD -= 1;
  if (HmD < 20) HmD = 20;
  if (HmD > 80) HmD = 80;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Modo 3: HmD");
  display.printf("Humedad Min: %.1f%%", HmD);
  display.display();
}

// MODO 4: Configurar Temp. Máxima (touch)
void configurarTemperatura() {
  if (touchRead(TOUCH_PLUS) < 40) TMD += 0.5;
  if (touchRead(TOUCH_MINUS) < 40) TMD -= 0.5;
  if (TMD < 20) TMD = 20;
  if (TMD > 40) TMD = 40;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Modo 4: TMD");
  display.printf("Temp Max: %.1f C", TMD);
  display.display();
}

// ==============================
// FUNCIONES AUXILIARES
// ==============================
void verificarAlarma() {
  if (temp > TMD || hum < HmD) {
    digitalWrite(LED_EXT, HIGH);
  } else {
    digitalWrite(LED_EXT, LOW);
  }
}
