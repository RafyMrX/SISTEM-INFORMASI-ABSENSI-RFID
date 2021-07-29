#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "xxx"; //ssid
const char* password = "xxx"; //password wifi
const char* host = ""; //ip address

#define LED 15
#define BZ 5
#define BTN 4

#define SDA 2
#define RST 0

MFRC522 mfrc522(SDA,RST);

void setup(){
  Serial.begin(115200);

  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("Wifi Terhubung");
  Serial.println("Ip Address");
  Serial.println(WiFi.localIP());

  pinMode(LED, OUTPUT);
  pinMode(BZ, OUTPUT);
  pinMode(BTN, INPUT);
  noTone(BZ);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Dekatkan Kartu RFID Anda ke Reader");
  Serial.println();
  
}

void loop(){

if(digitalRead(BTN)== 1){
  tone(BZ, 2000);
  digitalWrite(LED,HIGH);
  delay(400);
  noTone(BZ);
  tone(BZ, 2000);
  delay(400);
  noTone(BZ);
  
  delay(2000);
  digitalWrite(LED,LOW);
  
  String getData, Link;
  HTTPClient http;
  WiFiClient client;
//  get data

  Link = "http://192.168.1.68:8000/mode";
  http.begin(client,Link);
  
  int httpCode = http.GET();
  String payload = http.getString();
  Serial.println(payload);
  http.end();
  
 }
  digitalWrite(LED,LOW);

if(! mfrc522.PICC_IsNewCardPresent())
     return ;

if(! mfrc522.PICC_ReadCardSerial())
     return ;

 String IDTAG = "";
  for(byte i=0; i<mfrc522.uid.size; i++)
  {
      IDTAG += mfrc522.uid.uidByte[i];
  }

  //nyalakan lampu LED
  digitalWrite(LED, HIGH);
  tone(BZ, 2000);
  delay(400);
  noTone(BZ);
  digitalWrite(LED, LOW);
  WiFiClient client;
  const int httpPort = 80;
  if(!client.connect(host, httpPort))
  {
     Serial.println("Connection Failed");
     return;
  }

  String Link;
  HTTPClient http;
  Link = "http://192.168.1.68:8000/postkartu/" + IDTAG;
  http.begin(client, Link);

  int httpCode = http.GET();
  String payload = http.getString();
  Serial.println(payload);
  http.end();

  Serial.println(Link);
  delay(2000);
  
  
}
