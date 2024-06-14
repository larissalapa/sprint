"""
#include <LiquidCrystal.h>
#include <Servo.h>

#define TMP36_PIN A0
#define POT_PIN A1
"""
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int ledGreen = 13;
const int ledYellow = 8;
const int ledRed = 7;
const int buzzer = 10;

Servo servoHumidity; // Servo motor para umidade
Servo servoTemperature; // Servo motor para temperatura

unsigned long previousMillis = 0; // Variável para armazenar o último tempo
const long interval = 2000; // Intervalo de tempo para a atualização do servo (2 segundos)

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  servoHumidity.attach(9); // Conecta o servo motor ao pino 9
  servoTemperature.attach(6); // Conecta o servo motor ao pino 6
  
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
}

void loop() {
  unsigned long currentMillis = millis();

  // Leitura do sensor de temperatura TMP36
  int tempReading = analogRead(TMP36_PIN);
  float voltage = tempReading * (5.0 / 1023.0);
  float temperatureC = (voltage - 0.5) * 100.0;

  // Leitura do potenciômetro para umidade
  int potReading = analogRead(POT_PIN);
  float humidity = map(potReading, 0, 1023, 0, 100);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" *C");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperatureC);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print(" %");

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Calculo para ajustar os servos
    int servoPosTemp = map(temperatureC, 0, 50, 0, 180); // Mapeia a temperatura para o ângulo do servo
    servoTemperature.write(servoPosTemp);

    int servoPosHum = map(humidity, 0, 100, 0, 180); // Mapeia a umidade para o ângulo do servo
    servoHumidity.write(servoPosHum);
  }

  // Controle dos LEDs e do buzzer baseado em temperatura e umidade
  if (temperatureC < 25 && humidity <= 50) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    noTone(buzzer);
  } else if ((temperatureC >= 25 && temperatureC < 30) || (humidity > 50 && humidity <= 75)) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
    tone(buzzer, 1000, 500); // Beep contínuo (500 ms)
    delay(500);         // Aguarda 500 ms
    noTone(buzzer);     // Desliga o buzzer
  } else {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
    tone(buzzer, 2000); // Tom contínuo
  }

  delay(2000);
}
