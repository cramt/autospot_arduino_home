#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;
String ip = "";
long lastIpSend;

void setup() {
  lastIpSend = 0;
  Serial.begin(115200);
  Serial.println("hello there");

  WiFi.begin("testy", "trapsaregay");


  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");

  Serial.println(WiFi.localIP());
 
  ip = WiFi.localIP().toString();

  Udp.begin(69);
}

char packetBuffer[255]; //buffer to hold incoming packet

void loop() {
  if(lastIpSend + 1000 < millis()){
    Udp.beginPacket("192.168.43.1", 6969);
    Udp.write(("__home__=" + ip).c_str());
    Udp.endPacket();
    lastIpSend = millis();
  }

  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    String content = packetBuffer;
  }
}
