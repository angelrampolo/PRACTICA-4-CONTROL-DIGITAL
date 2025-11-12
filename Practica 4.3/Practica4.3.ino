// practica: control de dos leds desde el controllino con botones y el hmi
#include <Controllino.h>
#include "Stone_HMI_Define.h"
#include "Procesar_HMI.h"

// pines asociados a los leds y botones
const int pinLed1   = CONTROLLINO_D0;
const int pinLed2   = CONTROLLINO_D1;
const int pinBtn1   = CONTROLLINO_I16;
const int pinBtn2   = CONTROLLINO_I17;

// variables de control de pwm y porcentaje
int pwm1 = 0;
int pwm2 = 0;
float porcentaje1 = 0;
float porcentaje2 = 0;

// banderas y estados
int estadoPrevioBtn1 = LOW;
int estadoPrevioBtn2 = LOW;
int flagLed1 = 0;
int flagLed2 = 0;

// punteros para facilitar el manejo
int *ptrLed1 = (int *)&pinLed1;
int *ptrLed2 = (int *)&pinLed2;
int *ptrBtn1 = (int *)&pinBtn1;
int *ptrBtn2 = (int *)&pinBtn2;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  // configuracion de pines
  pinMode(*ptrLed1, OUTPUT);
  pinMode(*ptrLed2, OUTPUT);
  pinMode(*ptrBtn1, INPUT);
  pinMode(*ptrBtn2, INPUT);

  // inicializacion del hmi
  HMI_init();
  Stone_HMI_Set_Value("spin_box", "spin_box1", NULL, 0);
  Stone_HMI_Set_Value("spin_box", "spin_box2", NULL, 0);
}

void loop() {
  // leer valores desde el hmi
  porcentaje1 = HMI_get_value("spin_box", "spin_box1");
  porcentaje2 = HMI_get_value("spin_box", "spin_box2");

  // lectura del boton 1 con deteccion de flanco
  int estadoBtn1 = digitalRead(*ptrBtn1);
  if (estadoBtn1 == HIGH && estadoPrevioBtn1 == LOW) {
    flagLed1++;
    if (flagLed1 > 2) flagLed1 = 1;
  }
  estadoPrevioBtn1 = estadoBtn1;

  // control del primer led segun bandera
  if (flagLed1 == 1) {
    digitalWrite(*ptrLed1, HIGH);
  } else if (flagLed1 == 2) {
    digitalWrite(*ptrLed1, LOW);
    flagLed1 = 0;
  }

  // lectura del boton 2 con deteccion de flanco
  int estadoBtn2 = digitalRead(*ptrBtn2);
  if (estadoBtn2 == HIGH && estadoPrevioBtn2 == LOW) {
    flagLed2++;
    if (flagLed2 > 2) flagLed2 = 1;
  }
  estadoPrevioBtn2 = estadoBtn2;

  // control del segundo led segun bandera
  if (flagLed2 == 1) {
    digitalWrite(*ptrLed2, HIGH);
  } else if (flagLed2 == 2) {
    digitalWrite(*ptrLed2, LOW);
    flagLed2 = 0;
  }

  // si el primer led esta activo, actualizar su pwm
  if (porcentaje1 >= 0 && porcentaje1 <= 100 && flagLed1 == 1) {
    pwm1 = map(porcentaje1, 0, 100, 0, 255);
    analogWrite(*ptrLed1, pwm1);
    Serial.print("led1 duty: ");
    Serial.print(porcentaje1);
    Serial.print("% -> pwm: ");
    Serial.println(pwm1);
  }

  // si el segundo led esta activo, actualizar su pwm
  if (porcentaje2 >= 0 && porcentaje2 <= 100 && flagLed2 == 1) {
    pwm2 = map(porcentaje2, 0, 100, 0, 255);
    analogWrite(*ptrLed2, pwm2);
    Serial.print("led2 duty: ");
    Serial.print(porcentaje2);
    Serial.print("% -> pwm: ");
    Serial.println(pwm2);
  }

  delay(10); // pequena pausa para evitar rebotes
}
