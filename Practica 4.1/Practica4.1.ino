// practica: encender los leds del tablero uno por uno usando punteros
#include <Controllino.h> // libreria principal del controllino

// arreglo con los pines de los leds
int pinesLeds[] = {
  CONTROLLINO_D7, CONTROLLINO_D0, CONTROLLINO_D6,
  CONTROLLINO_D12, CONTROLLINO_D13, CONTROLLINO_D14,
  CONTROLLINO_D8, CONTROLLINO_D2, CONTROLLINO_D1, CONTROLLINO_D7
};

// puntero que apunta al primer elemento del arreglo
int *ptrLeds = pinesLeds;

// variables para controlar el tiempo y el recorrido
unsigned long tiempoPrevio = 0;
int intervalo = 500;
int indiceLed = 0;

// configuracion inicial
void setup() {
  // recorrer el arreglo con puntero
  for (int *p = ptrLeds; p < ptrLeds + 10; p++) {
    pinMode(*p, OUTPUT);   // *p da el valor (numero de pin)
  }

  // encender el primer led
  digitalWrite(*ptrLeds, HIGH);
}

// bucle principal
void loop() {
  unsigned long tiempoActual = millis();

  // verificar si ya paso el intervalo
  if (tiempoActual - tiempoPrevio >= intervalo) {

    // apagar el led actual usando puntero
    digitalWrite(*(ptrLeds + indiceLed), LOW);

    // avanzar al siguiente
    indiceLed++;

    // si llega al final del arreglo, reinicia
    if (indiceLed >= 10) {
      indiceLed = 0;
    }

    // encender el siguiente led
    digitalWrite(*(ptrLeds + indiceLed), HIGH);

    // guardar el tiempo actual
    tiempoPrevio = tiempoActual;
  }
}
