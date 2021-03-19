#include <WiFi.h>
#include <Espalexa.h>
#include <PubSubClient.h>
Espalexa alexa;

const char* ssid = "Totalplay-60A5";
const char* password = "60A538FCm3yWgW58"; 

const char* mqtt_server = "192.168.100.10";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//const char* ssid = "SANTEL-KING";
//const char* password = "0240Alfsanchivis"; 

#define ledWifi 15
#define focoSala 2
#define lamparaSala 4

void device1(uint8_t brightness);
void device2(uint8_t brightness);

void devFunc1(uint8_t brightness);

String nameDevice1 = "Foco sala";
String nameDevice2 = "Lampara sala";

String nameDevFunc1 = "Iluminacion sala" ;

boolean estaConectado(){
  boolean estado = false;
  if (WiFi.status() == WL_CONNECTED){
   estado = true;
   digitalWrite(ledWifi, 1);
  }else{
    estado = false;
    digitalWrite(ledWifi, 0);
  }
}

void conectarWifi(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Conectando al wifi");
    while (WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);
      Serial.print(".");
      digitalWrite(ledWifi, 0);
      delay(250);
      digitalWrite(ledWifi, 1);
      delay(250);
    }
    for (int i = 0; i < 6; i++){
      digitalWrite(ledWifi, 0);
      delay(100);
      digitalWrite(ledWifi, 1);
      delay(100);
    }
    Serial.println();
    Serial.print("Conectado a ");
    Serial.println(ssid);
    Serial.print("Con la IP");
    Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  pinMode(ledWifi, OUTPUT);
  pinMode(focoSala, OUTPUT);
  pinMode(lamparaSala, OUTPUT);
  conectarWifi();
  alexa.addDevice(nameDevice1, device1);
  alexa.addDevice(nameDevice2, device2);
  
  alexa.addDevice(nameDevFunc1 ,devFunc1);
  
  alexa.begin();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  reconnect();
}

void loop() {
 if (!estaConectado() || !client.connected()){
 Serial.println("Reconectando ...");
 conectarWifi();
 reconnect();
 }else{
  alexa.loop();
  delay(1);

  client.loop();
  //long now = millis();
  //if (now - lastMsg > 2000) {
    //lastMsg = now;
    //++value;
    //snprintf (msg, 75, "hello world #%ld", value);
    //Serial.print("Publish message: ");
    //Serial.println(msg);
    //client.publish("casa/sala", msg);
  //}
  
 }
}

void callback(char* topic, byte* payload, unsigned int length){
  String rama = String(topic);
  
  Serial.print("Mensaje nuevo: ");
  Serial.println(rama);
  String accion;
  for (int i = 0; i < length; i++){
    accion += (char) payload[i];
    //Serial.print((char)payload[i]);
  }
  Serial.println(accion);
  Serial.println(length);

  if (rama == "casa/sala/"){
    //device1(((accion == "on") ? 255 : 0));
    if (accion == "on"){
      digitalWrite(focoSala, 1);
      digitalWrite(lamparaSala, 1);
      Serial.println("Encender iluminacion sala");
    }else{
      digitalWrite(focoSala, 0);
      digitalWrite(lamparaSala, 0);
      Serial.println("Apagar iluminacion sala");
    }
  }
  if (rama == "casa/sala/focoSala"){
    //device1(((accion == "on") ? 255 : 0));
    if (accion == "on"){
      digitalWrite(focoSala, HIGH);
      Serial.println("Foco sala ON");
    }else{
      digitalWrite(focoSala, LOW);
      Serial.println("Foco sala off");
    }
  }
  if (rama == "casa/sala/lamparaSala"){
    if (accion == "on"){
      digitalWrite(lamparaSala, HIGH);
      Serial.println("Foco sala ON");
    }else{
      digitalWrite(lamparaSala, LOW);
      Serial.println("Foco sala off");
    }
  }
 
}

void reconnect(){
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("esp32")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("casa/comedor", "Enviadno desde ESP");
      // ... and resubscribe
      client.subscribe("casa/sala/#");
      client.subscribe("casa/sala/focoSala");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void device1(uint8_t brightness){
  if (brightness == 255){
      digitalWrite(focoSala, HIGH);
      Serial.println("Foco sala ON");
      client.publish("casa/sala/focoSala", "on");
    }else{
    digitalWrite(focoSala, LOW);
    Serial.println("foco sala OFF");
    client.publish("casa/sala/focoSala", "off");
  }
}
void device2(uint8_t brightness){
   if (brightness == 255){
      digitalWrite(lamparaSala, HIGH);
      Serial.println("Lampara sala ON");
      client.publish("casa/sala/lamparaSala", "on");
    }else{
    digitalWrite(lamparaSala, LOW);
    Serial.println("Lampara sala OFF");
    client.publish("casa/sala/lamparaSala", "off");
  }
}

void devFunc1(uint8_t brightness){
  if (brightness == 255){
    digitalWrite(focoSala, 1);
    digitalWrite(lamparaSala, 1);
    Serial.println("Encender iluminacion sala");
    client.publish("casa/sala/", "on");
  }else{
    digitalWrite(focoSala, 0);
    digitalWrite(lamparaSala, 0);
    Serial.println("Apaga iluminacion sala");
    client.publish("casa/sala/", "off");
  }
}
