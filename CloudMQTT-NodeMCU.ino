#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "CloudThat_IoT_Lab2_2.4GHz";
const char* password = "ghjklasdf1*&";
const char* mqtt_server = "m10.cloudmqtt.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 13670);
  client.setCallback(callback);

  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  char message[length];

  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    char var = (char)payload[i];
    message[i] = var;
    
  }
  String msgRaw = String(message);
  String msg = msgRaw.substring(0, length);
 

    int index1 = msg.indexOf(',');  //finds location of first ,
    String thing = msg.substring(0, index1);   //captures first data String
    int index2 = msg.indexOf(',', index1+1 );   //finds location of second ,
    String state = msg.substring(index1+1);   //captures second data String

     Serial.print("Thing :");
     Serial.println(String(thing));

     Serial.print("state :");
     Serial.println(String(state));

     if(String(thing)=="bulb1" && String(state)=="true"){
        digitalWrite(D2, HIGH);
        Serial.println("high");
     }
     else if(String(thing)=="bulb1" && String(state)=="false"){
        digitalWrite(D2, LOW);
        Serial.println("low");
     }



}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", "ryflwpvz", "IR6JdpAccm2F")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
     // client.publish("/cloudmqtt", "hello world");
      // ... and resubscribe
      client.subscribe("/cloudmqtt");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    //Serial.print("Publish message: ");
    //Serial.println(msg);
    //client.publish("outTopic", msg);
  }
}
