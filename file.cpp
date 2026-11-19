


// ===== DETECTOR DE BARULHO — VERSÃO DE TESTE (POTENCIÔMETRO) + ADAFRUIT IO =====
// ESP32 + potenciômetro (simula o som) + 3 LEDs + buzzer

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// --- Adafruit IO ---
#define AIO_USER "isaac1224"
#define AIO_KEY  "aio_KEYa36BaoI9QZNzSla6j1zcZ2cPX"



WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, "io.adafruit.com", 1883, AIO_USER, AIO_KEY);
Adafruit_MQTT_Publish feedVolume(&mqtt, AIO_USER "/feeds/volume");
Adafruit_MQTT_Publish feedNivel (&mqtt, AIO_USER "/feeds/nivel");

// --- Pinos ---
const int SENSOR       = 34;
const int LED_VERDE    = 25;
const int LED_AMARELO  = 26;
const int LED_VERMELHO = 27;
const int BUZZER       = 14;

// --- Ajustes ---
const int LIMITE_MODERADO = 1600;  // acima disso = moderado
const int LIMITE_ALTO     = 2800;  // acima disso = alto
const int CONFIRMACOES    = 3;     // leituras seguidas para trocar o LED
const unsigned long INTERVALO_ENVIO = 10000;  // 10 s (limite grátis: 30 envios/min)

// --- Variáveis ---
int nivelAtual = 0;  // 0 = baixo, 1 = moderado, 2 = alto
int contador   = 0;
unsigned long ultimoEnvio     = 0;
unsigned long ultimaTentativa = 0;

// Tenta conectar ao MQTT sem travar o resto do programa
void manterMQTT() {
  if (mqtt.connected()) return;
  if (millis() - ultimaTentativa < 5000) return;  // tenta a cada 5 s
  ultimaTentativa = millis();

  Serial.println("Conectando ao Adafruit IO...");
  if (mqtt.connect() == 0) {
    Serial.println("MQTT conectado!");
  } else {
    Serial.println("Falhou, tentando de novo...");
    mqtt.disconnect();
  }
}

void setup() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(115200);

  WiFi.begin("Wokwi-GUEST", "", 6);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println(" OK");
}

void loop() {
  manterMQTT();

  // 1. MEDIR: a posição do potenciômetro faz o papel do volume
  int volume = analogRead(SENSOR);
  delay(50);

  // 2. CLASSIFICAR
  int nivel;
  if (volume < LIMITE_MODERADO) {
    nivel = 0;  // baixo
  } else if (volume < LIMITE_ALTO) {
    nivel = 1;  // moderado
  } else {
    nivel = 2;  // alto
  }

  // 3. IGNORAR RUÍDOS CURTOS
  if (nivel == nivelAtual) {
    contador = 0;
  } else {
    contador++;
    if (contador >= CONFIRMACOES) {
      nivelAtual = nivel;
      contador = 0;
    }
  }

  // 4. LEDS E BUZZER
  digitalWrite(LED_VERDE,    nivelAtual == 0);
  digitalWrite(LED_AMARELO,  nivelAtual == 1);
  digitalWrite(LED_VERMELHO, nivelAtual == 2);
  digitalWrite(BUZZER,       nivelAtual == 2);

  // 5. MONITOR SERIAL
  Serial.print("Volume: ");
  Serial.print(volume);
  Serial.print("   Nivel: ");
  Serial.println(nivelAtual);

  // 6. ENVIAR PARA O ADAFRUIT IO (a cada 10 s, sem travar)
  if (mqtt.connected() && millis() - ultimoEnvio >= INTERVALO_ENVIO) {
    ultimoEnvio = millis();
    feedVolume.publish((int32_t)volume);
    feedNivel.publish((int32_t)nivelAtual);
    Serial.println(">> Enviado ao Adafruit IO");
  }
}




