/************************************************
Sistema de Monitoreo Ambiental IoT
ESP32 + MQ135 + ML8511 + KY038 + LCD + Blynk
************************************************/

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*************** BLYNK ****************/

char auth[] = "TU_TOKEN_BLYNK";
char ssid[] = "TU_WIFI";
char pass[] = "TU_PASSWORD";

/*************** LCD ****************/

LiquidCrystal_I2C lcd(0x27,16,2);

/*************** PINES ****************/

#define MQ135_PIN 34
#define UV_PIN 35
#define SOUND_PIN 32

#define LED_R 25
#define LED_G 26
#define LED_B 27

/*************** VARIABLES *************/

float co2;
float uv;
int ruido;

/*************** SETUP *****************/

void setup() {

Serial.begin(115200);

lcd.init();
lcd.backlight();

pinMode(LED_R,OUTPUT);
pinMode(LED_G,OUTPUT);
pinMode(LED_B,OUTPUT);

Blynk.begin(auth, ssid, pass);

lcd.setCursor(0,0);
lcd.print("Sistema IoT");
lcd.setCursor(0,1);
lcd.print("Inicializando");

delay(2000);

}

/*************** LOOP *****************/

void loop() {

Blynk.run();

leerSensores();
mostrarLCD();
enviarBlynk();
controlLED();

delay(2000);

}

/*************** FUNCIONES *************/

void leerSensores(){

int valorMQ = analogRead(MQ135_PIN);
co2 = map(valorMQ,0,4095,400,2000);

int valorUV = analogRead(UV_PIN);
uv = (valorUV * 3.3 / 4095) * 10;

ruido = analogRead(SOUND_PIN);

}

void mostrarLCD(){

lcd.clear();

lcd.setCursor(0,0);
lcd.print("CO2:");
lcd.print(co2);
lcd.print("ppm");

lcd.setCursor(0,1);
lcd.print("UV:");
lcd.print(uv);

delay(2000);

lcd.clear();

lcd.setCursor(0,0);
lcd.print("Ruido:");
lcd.print(ruido);

}

void enviarBlynk(){

Blynk.virtualWrite(V0, co2);
Blynk.virtualWrite(V1, uv);
Blynk.virtualWrite(V2, ruido);

}

void controlLED(){

if(co2 < 800){

digitalWrite(LED_R, LOW);
digitalWrite(LED_G, LOW);
digitalWrite(LED_B, HIGH);

}

else if(co2 >= 800 && co2 < 1200){

digitalWrite(LED_R, LOW);
digitalWrite(LED_G, HIGH);
digitalWrite(LED_B, LOW);

}

else{

digitalWrite(LED_R, HIGH);
digitalWrite(LED_G, LOW);
digitalWrite(LED_B, LOW);

}

}
