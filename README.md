# Projeto de Monitoramento de Temperatura e Umidade

Sistema de Monitoramento Ambiental para a Fórmula E: 
Este projeto implementa um sistema de monitoramento de temperatura e umidade usando um sensor TMP36, um potenciômetro, LEDs, um buzzer, e dois servos motores. O sistema exibe as leituras de temperatura e umidade em um display LCD e usa LEDs e um buzzer para sinalizar diferentes condições.

## Componentes

- Arduino
- Sensor de temperatura TMP36
- Potenciômetro
- Display LCD (16x2)
- LEDs (verde, amarelo, vermelho)
- Buzzer
- Servos motores (2)
- Jumpers e resistores

## Circuito

### Pinos Utilizados

- **LCD**: 
  - RS: 12
  - EN: 11
  - D4: 5
  - D5: 4
  - D6: 3
  - D7: 2
- **LEDs**:
  - Verde: 13
  - Amarelo: 8
  - Vermelho: 7
- **Buzzer**: 10
- **Servos**:
  - Umidade: 9
  - Temperatura: 6
- **Sensores**:
  - TMP36: A0
  - Potenciômetro: A1

1. **Leitura dos Sensores**:
    - Leitura do sensor TMP36 para obter a temperatura. A leitura analógica é convertida para tensão e depois para a temperatura em graus Celsius.
    - Leitura do potenciômetro para obter a umidade. A leitura analógica é mapeada para uma escala de 0 a 100%.

2. **Exibição das Leituras**:
    - As leituras de temperatura e umidade são exibidas no monitor serial e no display LCD.

3. **Controle dos Servos**:
    - A cada 2 segundos (definido pelo `interval`), a posição dos servos motores é atualizada com base nas leituras de temperatura e umidade. As leituras são mapeadas para ângulos entre 0 e 180 graus.

4. **Controle dos LEDs e do Buzzer**:
    - Dependendo das leituras de temperatura e umidade, os LEDs e o buzzer são acionados para sinalizar diferentes condições:
        - **LED Verde** acende e o buzzer é desligado quando a temperatura está abaixo de 25°C e a umidade está abaixo ou igual a 50%.
        - **LED Amarelo** acende e o buzzer emite bipes curtos quando a temperatura está entre 25°C e 30°C ou a umidade está entre 50% e 75%.
        - **LED Vermelho** acende e o buzzer emite um tom contínuo quando a temperatura está acima de 30°C ou a umidade está acima de 75%.

### Código

```cpp
#include <LiquidCrystal.h>
#include <Servo.h>

#define TMP36_PIN A0
#define POT_PIN A1

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int ledGreen = 13;
const int ledYellow = 8;
const int ledRed = 7;
const int buzzer = 10;

Servo servoHumidity;
Servo servoTemperature;

unsigned long previousMillis = 0;
const long interval = 2000;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  servoHumidity.attach(9);
  servoTemperature.attach(6);
  
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
}

void loop() {
  unsigned long currentMillis = millis();

  int tempReading = analogRead(TMP36_PIN);
  float voltage = tempReading * (5.0 / 1023.0);
  float temperatureC = (voltage - 0.5) * 100.0;

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

    int servoPosTemp = map(temperatureC, 0, 50, 0, 180);
    servoTemperature.write(servoPosTemp);

    int servoPosHum = map(humidity, 0, 100, 0, 180);
    servoHumidity.write(servoPosHum);
  }

  if (temperatureC < 25 && humidity <= 50) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    noTone(buzzer);
  } else if ((temperatureC >= 25 && temperatureC < 30) || (humidity > 50 && humidity <= 75)) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
    tone(buzzer, 1000, 500);
    delay(500);
    noTone(buzzer);
  } else {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);
    tone(buzzer, 2000);
  }

  delay(2000);
}
