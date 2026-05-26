//Temperature sensor ports declaration
#include <DHT.h>
#include<SoftwareSerial.h>
//Variables
int h;  //Stores humidity value
int t; //Stores temperature value
int coolerCircuitOnOff=50;
int testProbe=44;
int complementSignal=46;

bool coolerCircuitSt;

//Constants
#define DHTPIN1 52
#define DHTPIN2 48     // what pin we're connected to
#define DHTTYPE DHT11  

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
//DHT dht(DHTPIN2, DHTTYPE2);  // Initialize DHT sensor
float voltage;


//RainDrop sensor Ports declaration
/*int rainAnalogPin = A1;
int rainDigitalPin = 6;
int temp=A0;*/
float intsensorValue;
bool raindetected;

//Ultra sonic sensor Ports declaration
#define echopin1 4 // echo pin
#define trigpin1 2 // Trigger pin
#define echopin2 6 // echo pin
#define trigpin2 5// Trigger pin


float distance1;
float duration1;
float distance2;
float duration2;


bool obstacledetected;
//bluetooth trnasferdata port declaration
SoftwareSerial B(10,11);    //10-RX,  11-TX

void setup() {
  pinMode(echopin1,INPUT);
  pinMode(trigpin1,OUTPUT);
  pinMode(echopin2,INPUT);
  pinMode(trigpin2,OUTPUT);      

  dht1.begin();
  dht2.begin();

  B.begin(9600);

  Serial.begin(9600);
}
void loop(){
  tecControl();
  measureTemp();
  rainDrop();
  if(raindetected){
    imergencyLanding();
  }
  obstacleDetection();
  if(obstacledetected){
    Serial.println("obstacle detected"); 
  }
  transferData();
  transferData();
  
  rainDrop();
  if(raindetected){
    imergencyLanding();
  }

}

void rainDrop(){
  // read the input on analog pin 0:
  intsensorValue = analogRead(A0);
  // print out the value you read:
  if(intsensorValue>350){
    Serial.println("Rain not detected!");
    raindetected=false;
  }
  else{
    Serial.println("Rain detected!");
    raindetected=true;
  }
  Serial.println(intsensorValue);
  delay(1500);        // delay in between reads for stability
}

void imergencyLanding(){
  UDSreading1();
}

void UDSreading1(){
  
  digitalWrite(trigpin1,LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigpin1, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(trigpin1,LOW);
  
  duration1=pulseIn(echopin1,HIGH);
  distance1=(duration1*0.034)/2;
  
  Serial.print("\nDistance : ");
  Serial.print(distance1);
  Serial.println(" cm");
  delay(1000);
}

void UDSreading2(){
  
  digitalWrite(trigpin2,LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigpin2, HIGH);
  delayMicroseconds(5);
  
  digitalWrite(trigpin2,LOW);
  
  duration2=pulseIn(echopin2,HIGH);
  distance2=(duration2*0.034)/2;
  
  delay(1000);
}

void obstacleDetection(){
  UDSreading2();
  if(distance2<300){
      delay(300);
      UDSreading2();
      if(distance2<300){
        obstacledetected=true;
      }else{
        obstacledetected=false;
      }
    }else{
      obstacledetected=false;  
  }
}


float measureHumid() {
  h = dht1.readHumidity()+dht2.readHumidity();
  h=h/2;
  //Print temp and humidity values to serial monitor
  Serial.println("Humidity: ");
  Serial.print(h,"%");
  return h;
}

float measureTemp() {
  t = dht1.readTemperature()+ dht2.readTemperature();
  t=t/2;
  if(coolerCircuitSt){
    t=t-4;
  }else{
    delay(3500);
    t=t;
  }
  //Print temp and humidity values to serial monitor
  Serial.print(" \nTemp: ");
  Serial.print(t);
  Serial.println(" °C");
  return t;
}

float setMaxTemp(){
  int test=digitalRead(testProbe);
  if (test==HIGH){
    return 320;}
  if (test==LOW){
    return 20;}
}

void tecControl(){
  delay(1000);
  float tMax=setMaxTemp();

  float temperature=measureTemp();
  if(tMax<=temperature){
    digitalWrite(coolerCircuitOnOff,HIGH);
    digitalWrite(complementSignal,LOW);
    coolerCircuitSt=1;
    coolerConditionUpdate("Cooler Circuit on");
  }else{
    digitalWrite(coolerCircuitOnOff,LOW);
    digitalWrite(complementSignal,HIGH);
    coolerConditionUpdate("Cooler Circuit off");
    coolerCircuitSt=0;
  }  
}

void coolerConditionUpdate(String status){
  Serial.println(status);
}


void transferData(){
  if(coolerCircuitSt){
    int temp=measureTemp()-8;
  }
  int temp=measureTemp();
  int humid=measureHumid()+3;
  B.print(temp);
  B.print(",");

  if(coolerCircuitSt){
    B.print("Cooling");
    B.print(",");
  }else{
    B.print("Ideal Temp");
    B.print(",");
  }

  B.print(humid);
  B.print(",");

  if(raindetected){
    B.print("Rainy");
    B.print(",");
  }else{
    B.print("Sunny");
    B.print(",");
  }

  if(obstacledetected){
    B.print("obstacle detected");
    B.print(",");
  }else{
    B.print("Clear");
    B.print(",");
  }

   B.print(";");

}