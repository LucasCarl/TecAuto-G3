#include <ESP32Servo.h>
#include <math.h>

// Pines de los servos
#define pinS1 4
#define pinS2 15
#define pinS3 13

// Parámetros del brazo
float dp = 0;
float dz = 6.5;
float dh = 5.5;
float l1 = 8;
float l2 = 8;

// Coordenadas y ángulos
float x, y, z;
float q1, q2, q3;
float a, p;

// Posiciones actuales
int posIni1 = 90;
int posIni2 = 90;
int posIni3 = 90;
int pos;

// Velocidad y tiempos
int dt = 10;         // Velocidad del movimiento (ms por grado)
int espera = 1000;   // Delay entre comandos

// Servo objects
Servo servo_1;
Servo servo_2;
Servo servo_3;

// Posiciones [S1, S2, S3]
int pos_agarre[3] = {90, 90, 90};     // Posición de inicio para agarrar
int puntoA[3]     = {120, 70, 60};    // Punto de entrega A
int puntoB[3]     = {60, 60, 80};     // Punto de entrega B
int puntoC[3]     = {100, 50, 70};    // Punto de entrega C

void setup() {
  Serial.begin(9600);

  // Conectar servos y mover a posición inicial
  servo_1.attach(pinS1);
  servo_1.write(posIni1);

  servo_2.attach(pinS2);
  servo_2.write(posIni2);

  servo_3.attach(pinS3);
  servo_3.write(posIni3);

  Serial.println("Listo. Escribe goA, goB o goC para mover el brazo.");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');

    if (input == "goA") {
      Serial.println("Moviendo a Punto A...");
      moverAB(pos_agarre);
      delay(500); // Simula agarre
      moverAB(puntoA);
      delay(500); // Simula soltar
      moverAB(pos_agarre);
    }

    else if (input == "goB") {
      Serial.println("Moviendo a Punto B...");
      moverAB(pos_agarre);
      delay(500);
      moverAB(puntoB);
      delay(500);
      moverAB(pos_agarre);
    }

    else if (input == "goC") {
      Serial.println("Moviendo a Punto C...");
      moverAB(pos_agarre);
      delay(500);
      moverAB(puntoC);
      delay(500);
      moverAB(pos_agarre);
    }

    else {
      // Comandos tipo "1.90"
      int servo = input.substring(0, input.indexOf('.')).toInt();
      int posFin = input.substring(input.indexOf('.') + 1).toInt();

      switch (servo) {
        case 1:
          moverServo1(posIni1, posFin, dt);
          posIni1 = posFin;
          break;
        case 2:
          moverServo2(posIni2, posFin, dt);
          posIni2 = posFin;
          break;
        case 3:
          moverServo3(posIni3, posFin, dt);
          posIni3 = posFin;
          break;
        default:
          Serial.println("Comando no válido. Usa 1.90 o goA/B/C.");
      }

      mostrarPos();
    }
  }

  delay(espera);
}

// ---- Funciones de movimiento de cada servo ----

void moverServo1(int posIni, int posFin, int dt) {
  if (posFin > posIni) {
    for (pos = posIni; pos <= posFin; pos++) {
      servo_1.write(pos);
      delay(dt);
    }
  } else {
    for (pos = posIni; pos >= posFin; pos--) {
      servo_1.write(pos);
      delay(dt);
    }
  }
}

void moverServo2(int posIni, int posFin, int dt) {
  if (posFin > posIni) {
    for (pos = posIni; pos <= posFin; pos++) {
      servo_2.write(pos);
      delay(dt);
    }
  } else {
    for (pos = posIni; pos >= posFin; pos--) {
      servo_2.write(pos);
      delay(dt);
    }
  }
}

void moverServo3(int posIni, int posFin, int dt) {
  if (posFin > posIni) {
    for (pos = posIni; pos <= posFin; pos++) {
      servo_3.write(pos);
      delay(dt);
    }
  } else {
    for (pos = posIni; pos >= posFin; pos--) {
      servo_3.write(pos);
      delay(dt);
    }
  }
}

// ---- Movimiento conjunto del brazo ----

void moverAB(int destino[3]) {
  moverServo1(posIni1, destino[0], dt);
  posIni1 = destino[0];

  moverServo2(posIni2, destino[1], dt);
  posIni2 = destino[1];

  moverServo3(posIni3, destino[2], dt);
  posIni3 = destino[2];

  mostrarPos();
}

// ---- Mostrar posición actual (opcional) ----

void mostrarPos() {
  calc_coord(posIni1, posIni2, posIni3);

  Serial.print("S1: "); Serial.print(posIni1);
  Serial.print(", S2: "); Serial.print(posIni2);
  Serial.print(", S3: "); Serial.println(posIni3);

  Serial.print("Q1: "); Serial.print(q1 * 180 / PI);
  Serial.print(", Q2: "); Serial.print(q2 * 180 / PI);
  Serial.print(", Q3: "); Serial.println(q3 * 180 / PI);

  Serial.print("X: "); Serial.print(x);
  Serial.print(", Y: "); Serial.print(y);
  Serial.print(", Z: "); Serial.println(z);
  Serial.println("--------------");
}

// ---- Calcular coordenadas cartesianas (opcional) ----

void calc_coord(int posIni1, int posIni2, int posIni3) {
  q1 = posIni1 * PI / 180;
  q2 = PI - (posIni2 * PI / 180);
  q3 = (posIni3 * PI / 180) - PI / 2 - q2;

  a = l1 * cos(q2) + l2 * cos(q2 + q3);
  z = dz + l1 * sin(q2) + l2 * sin(q2 + q3);
  p = dp + a + dh;

  x = p * cos(q1);
  y = p * sin(q1);
}
