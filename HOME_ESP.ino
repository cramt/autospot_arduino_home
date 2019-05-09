#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <AccelStepper.h>

WiFiUDP Udp;
String ip = "";
long lastIpSend;

const int _D0 = 16;
const int _D1 = 5;
const int _D2 = 4;
const int _D3 = 0;
const int _D5 = 14;
const int _D6 = 12;
const int _D7 = 13;
const int _D8 = 15;

AccelStepper stepper1(AccelStepper::FULL4WIRE, _D1, _D0, _D3, _D2);
AccelStepper stepper2(AccelStepper::FULL4WIRE, _D6, _D5, _D8, _D7);

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
  stepper1.setMaxSpeed(60);
  stepper1.setAcceleration(60);
  stepper1.moveTo(0);
  stepper2.setMaxSpeed(60);
  stepper2.setAcceleration(60);
  stepper2.moveTo(0);
}

char packetBuffer[255]; //buffer to hold incoming packet

void loop() {
  stepper1.run();
  stepper2.run();
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
    int index = content.indexOf(',');
    int xy = content.substring(0,index).toInt();
    int yz = content.substring(index+1, content.length()).toInt();
    stepper1.moveTo(xy);
    stepper2.moveTo(yz);
  }
}
