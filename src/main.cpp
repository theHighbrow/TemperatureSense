#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#define dht_pin 12 //D2 or 4 try other if current fails
#define dht_type DHT11

DHT dht(dht_pin,dht_type);

const char* ssid = "Oppo F7";
const char* password = "12345678";

const char* mqttserver = "soldier.cloudmqtt.com";
const char* mqttuser = "ckbdtizs";
const char* mqttpassword = "hASIpo8xbF36";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//CONNECTING OT WIFI
void setup_wifi()
{
  delay(100);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting . . .");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("Connected to WiFi");
  Serial.println("IP address :");
  Serial.println(WiFi.localIP());
}

//CONNECT TO MQTT
void reconnect()
{
  while(! client.connected())
  {
    Serial.println("Establishing connection with MQTT");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if(client.connect(clientId.c_str(),mqttuser,mqttpassword))
    {
      Serial.println("Connected");
      client.publish("outTopic","VS");
      client.subscribe("inTopic");
    }
    else
      {  
       Serial.println("Connection Failed");
       Serial.println(client.state());
       Serial.println("trying again in 5 seconds");
       delay(5000); 
      }
  }
}

//CALLBACK FUNCTION
void callback(char* topic,byte* payload, unsigned int length)
{
  Serial.println("Message arrived [");
  Serial.println(topic);
  Serial.println("]");
  for(int i=0; i<length; i++)
  {
    Serial.println(char(payload[i]));
  }
  Serial.println();
}
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqttserver,17702);
  client.setCallback(callback);
  reconnect();
}

void loop() {
  // put your main code here, to run repeatedly:

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.println("humidity = ");
  Serial.println(h);
  Serial.println("temperature = ");
  Serial.println(t);

  delay(1000);
  String hh = String(h);
  String msg = String(t);

  Serial.println("Publish msg : ");
  Serial.println(msg);

  int numt = t;
  char cstr[16];
  itoa(numt,cstr,10);

  int numh = h;
  char cshr[16];
  itoa(numh,cshr,10);
  delay(1000);
  client.publish("dht",cstr);
  client.publish("bmp",cshr);
  }