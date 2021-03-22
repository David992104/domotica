#include <WiFi.h>
#include <Espalexa.h>
#include <PubSubClient.h>
Espalexa alexa;
const char* mqtt_server = "192.168.100.10";

long lastMsg = 0;
char msg[50];
int value = 0;

const char* ssid = "SANTEL-KING";
const char* password = "0240Alfsanchivis"; 

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

  reconnect();
}

void loop() {
 if (!estaConectado() ){
  Serial.println("Reconectando ...");
  conectarWifi();
  reconnect();
 }else{
  alexa.loop();
  delay(1);
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
