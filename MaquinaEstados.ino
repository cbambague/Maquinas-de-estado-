#include "AsyncTaskLib.h"
#include <LiquidCrystal.h>

const int rs = 11, en = 12, d4 = 31, d5 = 32, d6 = 33, d7 = 34;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494

#define BUZZER_PASIVO 7    // buzzer pasivo en pin 

#define pinTemp A1
float valTemp = 0.0;
float tempC = 0.0;
void leer_temperatura(void);

AsyncTask asyncTaskTemp(200, true, leer_temperatura);

int pinR=10; 
int pinG=9;   
int pinB=13;

enum estado
{
  estadoA,
  estadoB,
  estadoC,
  estadoD
};

estado currentState;

int melodia[] = {    // array con las notas de la melodia
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};
int duraciones[] = {    // array con la duracion de cada nota
  8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 4, 4, 8, 8, 4, 4, 4, 2 
};

void leer_temperatura()
{
  valTemp = analogRead(pinTemp);
  tempC = ((valTemp*5000)/1023)/10;
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC);
  lcd.print(" C");
  delay(200);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Iniciando...");
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(pinR,OUTPUT);
  pinMode(pinG,OUTPUT);
  pinMode(pinB,OUTPUT);

  pinMode(BUZZER_PASIVO, OUTPUT);
  currentState = estadoA;  
  asyncTaskTemp.Start();
}

void loop()
{
  asyncTaskTemp.Update();
  actualizarEstado();
  
  }
    
void actualizarEstado()
{
  switch (currentState)
  {
    case estado::estadoA: funcionEstadoA();   break;
    case estado::estadoB: funcionEstadoB();   break;
    case estado::estadoC: funcionEstadoC();   break;
    case estado::estadoD: funcionEstadoD();   break;
    default: break;
  }
}

void funcionEstadoA(){
  color(0,255,0);
  if(tempC > 18){
     currentState = estado::estadoB;
    }
  }
  
void funcionEstadoB(){
  color(0,0,255);
  if(tempC > 25){
     currentState = estado::estadoC;
     }
  if(tempC <= 18){
     currentState = estado::estadoA;
     }
  }

  
void funcionEstadoC(){
  color(255,0,0);
  if(tempC > 40){
     currentState = estado::estadoD;
  }
  if(tempC <= 25){
     currentState = estado::estadoB;
  }
}
  
  
void funcionEstadoD(){
  color(255,255,0); 
  buzzer(); 
  delay(500);
  currentState = estado::estadoB;
  }
  // Rojo 255,0,0 Amarillo 255,255,0 verde 0,255,0
  void color(int R, int G, int B){
  analogWrite(pinR, R);
  analogWrite(pinG, G);
  analogWrite(pinB, B);
}
void buzzer(){
  for (int i = 0; i < 25; i++) {      // bucle repite 25 veces
      int duracion = 1000 / duraciones[i];    // duracion de la nota en milisegundos
      tone(BUZZER_PASIVO, melodia[i], duracion);  // ejecuta el tono con la duracion
      int pausa = duracion * 1.30;      // calcula pausa
      delay(pausa);         // demora con valor de pausa
      noTone(BUZZER_PASIVO);        // detiene reproduccion de tono
  }
}
