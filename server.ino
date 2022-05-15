#include<ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include "index.h"
ESP8266WebServer server;
#define statusPin 2
int k = 0;

char* ssid = "xxxxxxxxxxxx";
char* password = "xxxxxxxxxxxxxx";


SoftwareSerial mcuSerial(14, 12); // RX, TX
char data[100];
char tempdata[100];
char Customerdata[100];
char Purchasedata[100];
String out = "";
String customer = "";
String catrStatus = "";

void setup() {

  pinMode(statusPin, OUTPUT);
  Serial.begin(9600);
  mcuSerial.begin(9600);
  while (!Serial) {
    ;
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(statusPin, LOW);
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
  digitalWrite(statusPin, HIGH);

  
  server.on("/", []() {
    server.send_P(200, "text/html", webpage);
  });
  server.on("/rcvd", Customer_handle);
  delay(10);
  server.on("/pdt", Product_handle);
  delay(10);
  server.on("/cart", Cart_handle);
delay(10);
  
  
  server.begin();

}

void loop()
{
  server.handleClient();
  readserial();
  Customer_handle();
  Product_handle();
  Cart_handle();
 
  //Serial.println(out);
  mcuSerial.print(WiFi.localIP());
  delay(100);
}


void readserial()
{
  short pos = -1;
  while (mcuSerial.available() && pos < 100) data[++pos] = mcuSerial.read();
  delay(100);
 
if(pos != -1 && String(data[0]) == "@"){
  memcpy (tempdata, data, 100);
}

if(pos != -1 && String(data[0]) == "$"){
  memcpy (Customerdata, data, 100);
}

if(pos != -1 && String(data[0]) == "%"){
  memcpy (Purchasedata, data, 100);
}

if(pos != -1 && String(data[0]) == "!"){
 memset(data, (char)0, sizeof(data));
 memset(tempdata, (char)0, sizeof(tempdata));
 memset(Customerdata, (char)0, sizeof(Customerdata));
 memset(Purchasedata, (char)0, sizeof(Purchasedata));
}


memset(data, (char)0, sizeof(data));
  
  out = tempdata;
  customer = Customerdata;
  catrStatus = Purchasedata;
  
 // memcpy (data, tempdata, 100);
  
 // out = data;
  
  out.trim();
  customer.trim();
  catrStatus.trim();
  
   Serial.println("Cus" + customer);
    Serial.println("pdt" + out);
    Serial.println("status" + catrStatus);
   
   /*
  if(String(data[0])!= "@" && k ==0){
    //data = out;
  memset(data, (char)0, sizeof(data));
  k=1;
  
  }
  if(String(data[0])== "@"){
    k=0;
  }
  */
}



void Product_handle() {
  server.send(200, "text/plain", String(out));
  
}

void Customer_handle() {
  server.send(200, "text/plain", String(customer));
  
}
void Cart_handle() {
  server.send(200, "text/plain", String(catrStatus));
  
}








/*
  void readserial()
  {
  short pos=-1;
  while(mcuSerial.available()&&pos<100) data[++pos]=mcuSerial.read();
  String rcvd = data;
  memset(data, (char)0, sizeof(data));
  if(rcvd != ""){
  rcvd.trim();
  out = rcvd;
  }
  else{
  out = out;
  }
  }
*/
