#include <WiFi.h>
#include <PubSubClient.h>

#define DO_FLAME_PIN 15  // ESP32's pin GPIO13 connected to DO pin of the flame sensor
#define AO_MQ2_PIN 0 // ESP32's pin connected to the AOUT pin of the MQ2 sensor
#define DO_MQ2_PIN 4
#define RELAY_PIN 19
#define BUZZER_PIN 21

const char *ssid = "Lich Son";
const char *password = "27062002";
const char *mqtt_server = "mqtt-dashboard.com";
const int mqtt_port = 1883;
const char *mqtt_user = "nonameboy";
const char *mqtt_password = "1111";
const char *mqtt_fire_topic = "canhBaoChay/fire";
const char *mqtt_gas_topic = "canhBaoChay/gas";
const char *mqtt_pump_topic = "canhBaoChay/pump";
const char *mqtt_buzzer_topic = "canhBaoChay/buzzer";

WiFiClient wiFiClient;
PubSubClient client(wiFiClient);

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to wifi network with IP address: ");
  Serial.println(WiFi.localIP());
}

void connect_to_broker() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    String clientId = "clientId";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(mqtt_fire_topic);
      client.subscribe(mqtt_gas_topic);
      client.subscribe(mqtt_pump_topic);
      client.subscribe(mqtt_buzzer_topic);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 2 seconds...");
      delay(2000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  // Handle incoming MQTT messages (if needed)
}

void sendStatusToMQTT(const char *topic, const char *status) {
  client.publish(topic, status);
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  connect_to_broker();
  pinMode(DO_FLAME_PIN, INPUT);
  pinMode(AO_MQ2_PIN, INPUT);
  pinMode(DO_MQ2_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  client.loop();
  if (!client.connected()) {
    connect_to_broker();
  }
  int flame_state = digitalRead(DO_FLAME_PIN);
  int mq2_state = digitalRead(DO_MQ2_PIN);
  int mq2_analog_value = analogRead(AO_MQ2_PIN);
  int relayState = digitalRead(RELAY_PIN);
  int buzzerState = digitalRead(BUZZER_PIN);
  if (flame_state == LOW) {
    Serial.println("Cháy và khói được phát hiện!");
    
    // coi keu
    digitalWrite(BUZZER_PIN, HIGH);
    
    // may bom hoat dong
    relayState = LOW;
    digitalWrite(RELAY_PIN, relayState);

    Serial.println("Đang kích hoạt bơm nước để dập đám cháy!");
  } else {

    Serial.println("Không có cháy!");
    digitalWrite(BUZZER_PIN, LOW);
    relayState = HIGH;
    digitalWrite(RELAY_PIN, relayState);
  }
  String data = String(random(0xffff), HEX);
  client.publish(mqtt_fire_topic,String(flame_state).c_str());
  client.publish(mqtt_gas_topic,data.c_str());
  client.publish(mqtt_pump_topic,String(relayState).c_str());
  client.publish(mqtt_buzzer_topic,String(buzzerState).c_str());
  Serial.print("Nồng độ khói: ");
  Serial.println(mq2_analog_value);
  Serial.print("Trạng thái cảnh báo lửa: ");
  Serial.println(flame_state);
  Serial.print("Trạng thái cảnh báo khói: ");
  Serial.println(mq2_state);
  Serial.print("Trạng thái bơm: ");
  Serial.println(relayState);
  Serial.print("Trạng thái còi ");
  Serial.println(buzzerState);
  delay(1000);
}