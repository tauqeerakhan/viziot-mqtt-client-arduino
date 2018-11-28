

#include <ESP8266WiFi.h>

#include <VizIoTMqttClient.h>

//Адрес ноги со светодиодом
#define LED_ESP 2

//ssid и пароль доступа для подключения к WI-FI
const char* ssid = "__________";
const char* password = "_____________";

//Зарегистрируйтесь в VizIoT.com и создайте устройство
//Ключ и пароль доступа VizIoT Устройства (можно узнать в настройках устройства)
String VizIoT_Device_key = "______________";
String VizIoT_Device_pass = "_________________";

WiFiClient espClient;
PubSubClient clientMQTT(espClient);
VizIoTMqttClient clientVizIoT(clientMQTT);
long lastMsg = 0;
char msg[50];
byte statusLed = 0;

void setup()
{
  //разрешаем управлять светодиодом
  pinMode(LED_ESP, OUTPUT);
  digitalWrite(LED_ESP, HIGH);
  //Включаем вывод информации в Serial Monitor
  Serial.begin(9600);
  //Подключаемся к WI-FI
  setup_wifi();

  clientVizIoT.config(VizIoT_Device_key, VizIoT_Device_pass, "192.168.0.112", 48651);
  clientVizIoT.listenCommand(callback);

}

//Обработка события получения данных
void callback(String parameter, byte value) {
   Serial.print("Публикация сообщения: parameter");
   Serial.print(parameter);
   Serial.print("value ");
    Serial.println(value);
  if (parameter.compareTo("led") == 0) {
    if (value == 1) {
      statusLed = 1;
      digitalWrite(LED_ESP, LOW);
    } else {
      statusLed = 0;
      digitalWrite(LED_ESP, HIGH);
    }

    snprintf(msg, sizeof(msg), "{\"led\":\"%c\"}", (statusLed) ? '1' : '0');
    Serial.print("Публикация сообщения: ");
    Serial.println(msg);

    clientVizIoT.sendJsonString(String(msg));
  }
}

//Функция подключения к WI-FI
void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    //Ожидает подключения к WI-FI
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi подключен");
}



void loop()
{
  //необходим для обработки входящих сообщения и поддержания подключения к Брокеру
  clientVizIoT.loop();

  //Отправка уровня WI-FI сигнала на сервер каждую минуту
  long now = millis();
  if (now - lastMsg > 60000) {
    lastMsg = now;
    snprintf (msg, sizeof(msg), "{\"rssi\":\"%i\",\"led\":\"%c\"}", WiFi.RSSI(), (statusLed) ? '1' : '0');
    Serial.print("Публикация сообщения: ");
    Serial.println(msg);
    clientVizIoT.sendJsonString(String(msg));
  }
}
