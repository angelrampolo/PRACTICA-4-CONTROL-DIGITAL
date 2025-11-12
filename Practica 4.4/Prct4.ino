#include <Controllino.h>
#include "Stone_HMI_Define.h"
#include "Procesar_HMI.h"

// ==== PARÁMETROS PID ====
float salidaPID = 0;
float Kp = 1, Ki = 5, Kd = 1;
const float Ts = 0.05;      // 50 ms
float errorPID[3] = {0, 0, 0};

// ==== MOTOR Y HMI ====
const int pinPWM = CONTROLLINO_D0;
int referencia = 0;
char txt_duty[10];
char txt_rpm[10];

// ==== ENCODER ====
const int pinEncoder = CONTROLLINO_IN1;
volatile unsigned long pulsos = 0;
float velocidadRPM = 0;
const uint16_t PPR = 36;
const float fs = 1 / Ts;

// ==== TIEMPOS ====
unsigned long t_lecturaHMI = 0;
unsigned long t_envioHMI = 0;

// ==== PROTOTIPO ====
void ISR_pulso();

// ==================================================
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  pinMode(pinPWM, OUTPUT);
  pinMode(pinEncoder, INPUT);

  // Inicialización HMI
  Stone_HMI_Set_Value("slider", "slider1", NULL, 0);
  Stone_HMI_Set_Value("spin_box", "spin_box1", NULL, 1);
  Stone_HMI_Set_Value("spin_box", "spin_box2", NULL, 5);
  Stone_HMI_Set_Value("spin_box", "spin_box3", NULL, 0);

  STONE_push_series("line_series", "line_series1", 0);
  STONE_push_series("line_series", "line_series2", 0);
  STONE_push_series("line_series", "line_series3", 0);


  attachInterrupt(digitalPinToInterrupt(pinEncoder), ISR_pulso, FALLING);

  // Timer1 modo CTC
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= B00000100;     // prescaler 256
  TIMSK1 |= B00000010;     // habilita comparación
  OCR1A = 3125;            // 50 ms
  interrupts();

  HMI_init();
}

// ==================================================
void loop() {
  // Lectura HMI cada 5 ms

  if (millis() - t_lecturaHMI >= 10) {
    referencia = HMI_get_value("slider", "slider1");
    Kp = HMI_get_value("spin_box", "spin_box1")/100;
    Ki = HMI_get_value("spin_box", "spin_box2")/100;
    Kd = HMI_get_value("spin_box", "spin_box3")/100;
    t_lecturaHMI = millis();
  }

  // Envío HMI cada 100 ms
  if (millis() - t_envioHMI >= 100) {
    t_envioHMI = millis();
    dtostrf(referencia, 7, 2, txt_duty);
    dtostrf(velocidadRPM, 7, 2, txt_rpm);

    Stone_HMI_Set_Text("label", "label2", txt_duty);
    Stone_HMI_Set_Text("label", "label4", txt_rpm);

    STONE_push_series("line_series", "line_series1", referencia);
    STONE_push_series("line_series", "line_series2", velocidadRPM);
    STONE_push_series("line_series", "line_series3", (int)salidaPID);
  }
}

// ==================================================
ISR(TIMER1_COMPA_vect) {
  TCNT1 = 0;
  velocidadRPM = (float(pulsos) * 60 * fs) / PPR;

  // Actualización de errores
  errorPID[2] = errorPID[1];
  errorPID[1] = errorPID[0];
  errorPID[0] = referencia - velocidadRPM;

  // PID incremental
  float deltaU = Kp * (errorPID[0] - errorPID[1])
               + (Ki * Ts) * errorPID[0]
               + (Kd / Ts) * (errorPID[0] - 2 * errorPID[1] + errorPID[2]);
  salidaPID += deltaU;

  // limite para que no se vaa al infinito
  if (salidaPID > 255) salidaPID = 255;
  if (salidaPID < 0)   salidaPID = 0;

  analogWrite(pinPWM, (int)salidaPID);


  pulsos = 0;
}

// ==================================================
void ISR_pulso() {
  pulsos++;
}
