//Aqui esta o codigo mas tambem é possivel encontra-lo via o link no Read-Me ((simulador no wokwi))

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#include <WiFi.h>
#include <PubSubClient.h>

#define DHTPIN 12 // Pino DATA do Sensor DHT ligado na porta Digital 12
#define DHTTYPE DHT22 // Tipo de sensor, DHT11 neste caso

#define LDRPIN 34 // Pino digital onde o LDR está conectado

//Baguri MQTT
#define TOPICO_SUBSCRIBE    "/TEF/hxh/cmd"
#define TOPICO_PUBLISH      "/TEF/hxh/attrs"
#define TOPICO_PUBLISH      "/TEF/hxh/attrs"
#define TOPICO_PUBLISH_HUMIDITY "/TEF/hxh/attrs/humidity"
#define TOPICO_PUBLISH_TEMPERATURE "/TEF/hxh/attrs/temperature"
#define TOPICO_PUBLISH_LIGHT "/TEF/hxh/attrs/light"

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd (0x27, 16, 2);

// Tempo
unsigned long startTime;
unsigned long lastHumDisplayTime = 0;
unsigned long lastLightDisplayTime = 0;
unsigned long lastTempDisplayTime = 0;

// Display
String ML = "Ambiente a meia luz";
String MC = "Ambiente muito claro";

// Temperatura
int t = 1;
float temp_READ;

// Umidade
int moist = 2;
int mr = 0;

// Led
int D4 = 2;       // Led builtin do ESP32 (led na placa)

// MQTT
WiFiClient espClient;
PubSubClient MQTT(espClient);

char EstadoSaida = '0';

const char* SSID = "FIAP-IBM";
const char* PASSWORD = "Challenge@23!";
const char* BROKER_MQTT = "46.17.108.113";//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

int BROKER_PORT = 1883; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const char* ID_MQTT = "fiware_108"; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void initWiFi() {
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
    Serial.println("Conectado com sucesso na rede " + String(SSID));
    Serial.println("IP obtido: " + WiFi.localIP().toString());
}

void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for(int i = 0; i < length; i++) {
        msg += (char)payload[i];
    }

    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);
    
    if (msg.equals("hxh@on|")) {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }
 
    if (msg.equals("hxh@off|")) {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
}

void reconnectMQTT() {
    while (!MQTT.connected()) {
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        } else {
            Serial.println("Falha ao reconectar no broker.");
            delay(2000);
        }
    }
}

void setup() {
  // Inicialização do sensor DHT22
  dht.begin();

  // Inicialização do LCD
  lcd.init();
  lcd.backlight();

  // Configuração dos pinos de saída
  pinMode(LDRPIN, INPUT);

  // Inicialização da comunicação serial
  Serial.begin(9600);
  while (!Serial);
  pinMode(D4, OUTPUT);

  // Captura do tempo inicial
  startTime = millis();

  //Inicialização MQTT
  initWiFi();
  initMQTT();
}

void display1(int x) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (x == 0) {
    lcd.print("Light: Media");
  }
  else if (x == 1) {
    lcd.print("Light: Forte");
  }
  else if (x == 2){
    lcd.print("Light: Fraca");
  }

  delay(1500);
}

void display2(int x, float tt) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (x == 0) {
    lcd.print("Temp: Baixa");
  }
  else if (x == 1) {
    lcd.print("Temp: OK");
  }
  else if (x == 2) {
    lcd.print("Temp: Alta");
  }

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(tt);
  lcd.print("C");

  delay(1500);
}

void display3(int x, int tt) {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (x == 0) {
    lcd.print("Umidade ALTA");
  }
  else if (x == 1) {
    lcd.print("Umidade OK");
  }
  else if (x == 2) {
    lcd.print("Umidade BAIXA");
  }

  lcd.setCursor(0, 1);
  lcd.print("Umidade: ");
  lcd.print(tt); // converte na porcentagem correta
  lcd.print("%");

  delay(1500);
}

void loop() {
  // Tenta conectar
  if (!MQTT.connected()) {
      reconnectMQTT();
    }

  // Tempo
  unsigned long milisec = millis() - startTime;
  unsigned long sec = milisec / 1000;

  // Leituras + Operações
  int ldrReading = analogRead(LDRPIN);
  int ldrValue = map(ldrReading, 0, 4095, 0, 100); // Mapeie para o brilho (0-100)
  ldrValue = (100 - ldrValue); // Porcentagem correta

  temp_READ = dht.readTemperature(); // Leitura da temperatura
  mr = dht.readHumidity(); // Leitura da umidade

  // Umidade
  if (sec - lastHumDisplayTime >= 2) {
    lastHumDisplayTime = sec;
    
    //MQTT
    String msgHumity = "Umidade: " + String(mr, 2);
    MQTT.publish(TOPICO_PUBLISH_LIGHT, msgHumity.c_str());
    Serial.println(msgHumity);

    if (70 < mr) {
      display3(0, mr);
    } else if (50 > mr) {
      display3(2, mr);
    } else {
      display3(1, mr);
    }
  }

  // LDR (Sensor de Luz)
  if (sec - lastLightDisplayTime >= 3) {
    lastLightDisplayTime = sec;

    //MQTT
    String msgLight = "Light: " + String(ldrValue, 2);
    MQTT.publish(TOPICO_PUBLISH_LIGHT, msgLight.c_str());
    Serial.println(msgLight);

    if (ldrValue < 33) {
      delay(100);
      display1(2);
    } else if (ldrValue > 66) {
      delay(100);
      display1(1);
    } else {
      delay(100);
      display1(0);
    }
  }

  // Temperatura
  if (sec - lastTempDisplayTime >= 5) {
    lastTempDisplayTime = sec;

    //MQTT
    String msgTemp = "Temp: " + String(temp_READ, 2) + "C°";
    MQTT.publish(TOPICO_PUBLISH_TEMPERATURE, msgTemp.c_str());
    Serial.println(msgTemp);

    if (temp_READ < 10) {
      display2(0, temp_READ);
    } else if (temp_READ > 15) {
      display2(2, temp_READ);
    } else {
      display2(1, temp_READ);
    }
  }
  MQTT.loop();
}
