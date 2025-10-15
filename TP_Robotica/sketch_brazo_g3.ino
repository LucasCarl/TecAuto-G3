// Codigo del profe
// En el monitor serial escribir 
// servo(1,2,3) punto(.) y angulo a rotar (0..180)
#include <Servo.h>
#include <math.h>
// pines de los servos
# define pinS1 8
# define pinS2 9
# define pinS3 10
//parametros
float dp=1.5;
float dz=5;
float dh=0;
float l1=8;
float l2=8;
 // coordenadas
float x;
float y;
float z;
float q1;
float q2;
float q3;
float a;
float p ;

int pos ;
int posIni;
// posiciones de Home Servos OJO.... que sea cercana a la real inicial
// el servo ira bien rapido hasta ahi 
int posIni1 = 90;
int posIni2 = 90;
int posIni3 = 90;
int dt = 20; // velocidad giro = grado/dt (milisegundos) OJO que no sea muy rapido

int espera = 2000; // tiempo de espera para el puerto serial
Servo servo_1;
Servo servo_2;
Servo servo_3;

void setup() {
  servo_1.attach(pinS1);
  servo_1.write(posIni1);
  servo_2.attach(pinS2);
  servo_2.write(posIni2);
  servo_3.attach(pinS3);
  servo_3.write(posIni3);
  Serial.begin(9600);
    // mostrarPos(); 

}


void loop()

{
  
  if (Serial.available() >=3) {
    int servo = Serial.readStringUntil('.').toInt();
    int posFin = Serial.readStringUntil('\n').toInt();
	//Serial.print("Moviendo Servo ");
    	//Serial.print(servo);
    	//Serial.print(" a Posicion: ");
  		//Serial.println(posFin);
    
    switch (servo) {
    case 0:
     Serial.println("Puerto disponible");
    break;

    case 1:
    posIni=posIni1;
    MoverServo1(posIni,posFin,dt);
      posIni1=posFin;
      mostrarPos ();
                      break;
    case 2:
      posIni=posIni2;
    MoverServo2(posIni,posFin,dt);
      posIni2=posFin;  
      mostrarPos ();
           break;
           
    case 3:
   posIni=posIni3;
    MoverServo3(posIni,posFin,dt);
      posIni3=posFin;
      mostrarPos ();
           break;   
  default:
      Serial.println("Servo no valido (1..3)");
      
} 
	
  }
  
  	
  delay(espera);
} 
// fin loop
// funciones  
 void MoverServo1(int posIni, int posFin,int dt) 
 {
 if(posFin>posIni) {
      for (pos = posIni; pos <= posFin; pos++) {
        // tell servo to go to position in variable 'pos'
        servo_1.write(pos);
        // wait 15 ms for servo to reach the position
        delay(dt);  // Wait for dt millisecond(s)
      }
    }
      if(posFin<posIni) {
      for (pos = posIni; pos >= posFin; pos--) {
        // tell servo to go to position in variable 'pos'
        servo_1.write(pos);
        // wait 15 ms for servo to reach the position
        delay(dt);  // Wait for dt millisecond(s)
      }
           }  
   }
void MoverServo2(int posIni, int posFin,int dt) 
 {
 if(posFin>posIni) {
      for (pos = posIni; pos <= posFin; pos++) {
        // tell servo to go to position in variable 'pos'
        servo_2.write(pos);
        // wait 15 ms for servo to reach the position
        delay(dt);  // Wait for dt millisecond(s)
      }
    }
      if(posFin<posIni) {
      for (pos = posIni; pos >= posFin; pos--) {
        // tell servo to go to position in variable 'pos'
        servo_2.write(pos);
        // wait 15 ms for servo to reach the position
        delay(dt);  // Wait for dt millisecond(s)
      }
           }  
   }
void MoverServo3(int posIni, int posFin,int dt) 
 {
 if(posFin>posIni) {
      for (pos = posIni; pos <= posFin; pos++) {
        // tell servo to go to position in variable 'pos'
        servo_3.write(pos);
        // wait 15 ms for servo to reach the position
        delay(dt);  // Wait for 15 millisecond(s)
      }
    }
      if(posFin<posIni) {
      for (pos = posIni; pos >= posFin; pos--) {
        // tell servo to go to position in variable 'pos'
        servo_3.write(pos);
        // wait 15 ms for servo to reach the position
        delay(dt);  // Wait for 15 millisecond(s)
      }
           }  
   }
void mostrarPos() {
  calc_coord (posIni1, posIni2, posIni3);
    Serial.print("S1: ");
    Serial.print(posIni1);
    Serial.print(", S2: ");
    Serial.print(posIni2);
	Serial.print(" ,S3: ");
    Serial.println(posIni3);
    Serial.print("X: ");
    Serial.print(x);
    Serial.print(", Y: ");
    Serial.print(y);
	Serial.print(" ,Z: ");
    Serial.println(z);

}
void calc_coord ( int posIni1, int posIni2, int posIni3) 
{
  q1=posIni1*PI/180;
  q2=posIni2*PI/180;
  q3=posIni3*PI/180;
  a=l1*cos(q2)+l2*cos(PI/2-q3);
  z=dz+l1*sin(q2)+l2*sin(PI/2-q3);
  p=dp+a+dh;
  x=p*cos(q1);
  y=p*sin(q1);
}

