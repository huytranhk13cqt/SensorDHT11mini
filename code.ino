
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <Stepper.h>

const int stepsPerRevolution = 2048; // độ phân giải 
Stepper myStepper(stepsPerRevolution, 27, 12, 14, 13);


char auth[] = "t6Ybrt5il-Ib93nccl8-hmt0WKBJCmYT";
char ssid[] = "Tien Dat 3";
char pass[] = "06082000";

int led = 25;
int ac = 32;
int ac1 = 33;
int val = 0;
int cungchieu = 0;
#define DHTTYPE DHT11
#define DHTPIN 26
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("không đọc được dữ liệu từ DHT11");
    return;
  }
  
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}


void setup()
{

  pinMode(led,OUTPUT);
  pinMode(ac1,INPUT);
  pinMode(ac ,INPUT);

  Serial.begin(9600);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  Blynk.begin(auth, ssid, pass);

  myStepper.setSpeed(10);
 
}


void loop(){
  Blynk.run();
  timer.run();
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  val = digitalRead(ac);
  cungchieu = digitalRead(ac1);
  Serial.print("val = ");
  Serial.println(val);
  Serial.print("cungchieu = ");
  Serial.println(cungchieu);
  Serial.print(F("Độ ẩm là: "));
  Serial.print(h);
  Serial.print(F("% || Nhiệt độ là: "));
  Serial.print(t);
  Serial.println(F("^C"));

  if (cungchieu == 1 && val == 0)
  {
    Serial.println("CUNG CHIEU KIM DONG HO");
    myStepper.step(-stepsPerRevolution);
  }

  if (val == 1){
    if (t >= 30 && t <= 35 )
    {
      digitalWrite(led,HIGH);
      Serial.println("NGUOC CHIEU KIM DONG HO");
      myStepper.step(stepsPerRevolution);}
    else
    {
      digitalWrite(led,LOW);
    }}  
}
