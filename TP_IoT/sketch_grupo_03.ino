const int sensorHTPin = GPIO_NUM_33;    // Sensor H y T     
const int potenPin = GPIO_NUM_32;       // Potenciometro
const int pulsaPin = GPIO_NUM_19;       // Pulsador 
const int ledPinR = GPIO_NUM_23;        // Led Rojo      
const int ledPinV = GPIO_NUM_1;         // Led Verde

int modo = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  pinMode(pulsaPin, INPUT_PULLUP);
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinV, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}
