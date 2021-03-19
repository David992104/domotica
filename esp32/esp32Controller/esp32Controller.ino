#include <WiFi.h>


const char* ssid = "Totalplay-60A5";
const char* password = "60A538FCm3yWgW58"; 

#define ledWifi 15
#define focoSala 2

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
  if (WiFi.status() != WL_CONNECTED){
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
    Serial.println();
    Serial.println("Conectado a ");
    Serial.print(ssid);
    Serial.println("Con la IP");
    Serial.print(WiFi.localIP());
  }
}

void funcionFocoSala(uint8_t estado){
  if (estado){
    
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledWifi, OUTPUT);
  conectarWifi();
}

void loop() {
 if (!estaConectado()){
  conectarWifi();
 }

}
