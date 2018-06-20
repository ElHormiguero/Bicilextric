/*Control de un escalextric con una señal PWM sobre un transistor, velocidad obtenida con dos imanes en la rueda y un sensor hall, se muestra de forma analógica con un servomotor y con una matriz de 7 segmentos.
  Autor: Javier Vargas. El Hormiguero.
  https://creativecommons.org/licenses/by/4.0/
*/

//PINES
#define PinServo 10
#define PinOut 11
#define PinSensor 12
#define PinBoton 13

//CONFIGURACION
//generales
#define TiempoAntirrebotes 45 //(ms)
#define PerimetroRueda 1.07f //(m) (Perimetro / 2 --> 2 imanes)
#define VelMax 35 //(km/h)
#define TimeOut 500 //Tiempo máximo de espera entre el paso de dos imanes
//servo
#define AngVel0 150
#define AngVelMax 20
//salida
#define pwmMin 0

//LIBRERIAS
//7 segmentos
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
Adafruit_7segment matrix = Adafruit_7segment();

//servo
#include <Servo.h>
Servo velocimetro;

float v = 0;
float d = 0;
unsigned long t = 0;
boolean Andando = 0;

void setup () {
  //Serial.begin(115200);

  pinMode(PinOut, OUTPUT);
  pinMode(PinSensor, INPUT_PULLUP);
  pinMode(PinBoton, INPUT_PULLUP);

  velocimetro.attach(PinServo);

  matrix.begin(0x70);
  Reset();
}

///////////////////////////////////////
//////////////////LOOP/////////////////
///////////////////////////////////////

void loop () {

  if (SensorHall()) {
    Andando = 1;
    MedirVelocidadDistancia();
    MoverServo();
    Display();
    ControlCoche();
    t = millis();
  }

  if (millis() > t + TimeOut && Andando) {
    Reset();
    Andando = 0;
  }

  if (!digitalRead(PinBoton)) {
    d = 0;
    Reset();
  }

}

///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////

boolean SensorHall() {
  static boolean estado = 0;
  static unsigned long m = 0;

  boolean s = digitalRead(PinSensor);
  boolean out = 0;

  if (m < millis()) {
    if (s) { //Activado
      m = millis() + TiempoAntirrebotes;
      if (estado == 0) { //Flanco de subida
        estado = 1;
        out = 1;
      }
    }
    if (!s && estado) estado = 0;
  }

  return out;
}

void  MedirVelocidadDistancia() {
  static float v0 = 0;
  unsigned long incrT = millis() - t;
  static boolean acMax = 0;

  //Velocidad obtenida
  v = 3600 * (float)PerimetroRueda / incrT;
  //Limite de velocidad
  v = constrain(v, 0, VelMax);
  //Filtro de la velocidad
  float K = (float)map(constrain(incrT, 50, 500), 50, 500, 85, 98) / 100;
  v = K * v + (1 - K) * v0;
  //Velocidad minima aceptable
  if (v < 5) v = 0;
  //Limte de aceleracion
  if (v - v0 > 10 && !acMax) v = v0, acMax = 1;
  else {
    acMax = 0;
    v0 = v; //Velocidad anterior
  }

  //Distancia recorrida
  d += PerimetroRueda;
  d = constrain(d, 0, 9999);
}

void MoverServo() {
  int angulo = map(10 * v, 0, 10 * VelMax,  AngVel0,  AngVelMax); //Velocidad en km/h
  velocimetro.write(angulo);
}

void ControlCoche() {
  int pwmOut;
  if (v < 0.1) pwmOut = 0;
  else pwmOut = map(10 * v , 0, 10 * VelMax,  pwmMin,  255);
  analogWrite(PinOut, pwmOut);
}

void Display() {
  matrix.println((int)v);
  matrix.writeDisplay();
}

void Reset() {
  v = 0;
  MoverServo();
  ControlCoche();
  Display();
}

