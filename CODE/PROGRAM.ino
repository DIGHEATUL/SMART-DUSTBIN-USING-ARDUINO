
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);
#include <TinyGPS.h>
String incomingData;
char phone_no[] = "xxxxxxxxxx"; //replace with phone no. to get sms
TinyGPS gps;  //Creates a new instance of the TinyGPS object
int LED_G1 = A0;
int LED_G2 = A1;
int LED_Y = A2;
int LED_R = A3;

int trigPin = 4;    
int echoPin = 8;   
int trigPin_l = 9;    
int echoPin_l = 10;   
long duration, dist, average;   
long aver[3];   //array for average

long duration_d, dist_d, average_d;   
long aver_d[3];   //array for average

const int speedStep =200;
const int speedDelay = 1000;// delay between speed increment

#define P1A 5 // define pin 10as for P1A
#define P2A 6 // define pin 11 as for P2A
#define EN12 7 // define pin 9 as for 1,2EN enable


 
void setup() {

mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
Serial.begin(9600);

pinMode(P1A, OUTPUT);// define pin as OUTPUT for P1A
  pinMode(P2A, OUTPUT);// define pin as OUTPUT for P2A
  pinMode(EN12, OUTPUT);// define pin as OUTPUT for 1,2EN
   
  pinMode(trigPin, OUTPUT);  
    pinMode(echoPin, INPUT);  
       //close cap on power on
    delay(100);

pinMode(trigPin_l, OUTPUT);  
    pinMode(echoPin_l, INPUT);  
    pinMode(LED_G1, OUTPUT);
    pinMode(LED_G2, OUTPUT);
    pinMode(LED_Y, OUTPUT);
    pinMode(LED_R, OUTPUT);
}


void measure_d() {  
 digitalWrite(10,HIGH);
digitalWrite(trigPin, LOW);
delayMicroseconds(5);
digitalWrite(trigPin, HIGH);
delayMicroseconds(15);
digitalWrite(trigPin, LOW);
pinMode(echoPin, INPUT);
duration_d = pulseIn(echoPin, HIGH);
dist_d = (duration_d/2) / 29.1;    //obtain distance
}

void measure() {  
 digitalWrite(11,HIGH);
digitalWrite(trigPin_l, LOW);
delayMicroseconds(5);
digitalWrite(trigPin_l, HIGH);
delayMicroseconds(15);
digitalWrite(trigPin_l, LOW);
pinMode(echoPin_l, INPUT);
duration = pulseIn(echoPin_l, HIGH);
dist = (duration/2) / 29.1;    //obtain distance
}
void loop() {
  loc();

for (int j=0;j<=2;j++) {   //average distance
    measure_d();               
   aver_d[j]=dist_d;            
    delay(10);              //delay between measurements
  }
 dist_d=(aver_d[0]+aver_d[1]+aver_d[2])/3; 
 
if(dist_d<=13)      //Check the sensor output

{

 L293D('L',speedStep, 1); 
delay(600);
  L293D('L',speedStep, 0);
delay(7000);
   L293D('r',speedStep, 1);
   delay(240);
   L293D('r',speedStep, 0);
}
else{
  for (int i=0;i<=2;i++) {   //average distance
    measure();               
   aver[i]=dist;            
    delay(10);              //delay between measurements
  }
 dist=(aver[0]+aver[1]+aver[2])/3;    
if(dist<=3) //as your choice
{
   Serial.print("level is:");
   Serial.println(dist);
   location();
   digitalWrite(LED_R, HIGH);
   digitalWrite(LED_Y, LOW);
   digitalWrite(LED_G1, LOW);
   digitalWrite(LED_G2, LOW);
}

else if ( dist<=20)  //as your choice
{
   Serial.print("level is:");
   Serial.println(dist);
   digitalWrite(LED_R, LOW);
   digitalWrite(LED_Y, HIGH);
   digitalWrite(LED_G1, LOW);
   digitalWrite(LED_G2, LOW);
}
else if ( dist<=30)  //as your choice
{
   Serial.print("level is:");
   Serial.println(dist);
   digitalWrite(LED_R, LOW);
   digitalWrite(LED_Y, LOW);
   digitalWrite(LED_G1, LOW);
   digitalWrite(LED_G2, HIGH);
}

}
}
void location()
 {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
 
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }
 
  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    mySerial.print("AT+CMGF=1\r"); 
    delay(400);
    mySerial.print("AT+CMGS=\"");
    mySerial.print(phone_no);
    mySerial.println("\"");
    
    delay(300);

  mySerial.println("DUSTBIN IS FULLED");// The SMS text you want to send
     delay(100);   
   mySerial.print("http://www.google.com/maps/?q=");
    mySerial.print(flat, 6);
  mySerial.print(",");
    mySerial.print(flon, 6);
    delay(200);
    mySerial.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    
 }
 }



void loc()

  {



 if (mySerial.available()>0)
   Serial.write(mySerial.read());
 String c = mySerial.readString();
c.trim();
 if (c.indexOf("SEND LOCATION") >= 0)


 
  {
 bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
 
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }
 
  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
     delay(1000);  // Delay of 1 second
     mySerial.println("AT+CMGS=\"+918080590778\"\r"); // enter mobile number
     delay(1000);


  mySerial.println("LOCATION LINK -");// The SMS text you want to send
     delay(100);   
  mySerial.print("http://www.google.com/maps/?q=");
  mySerial.print(flat, 6);
  mySerial.print(",");
  mySerial.print(flon, 6);
    delay(200);
  mySerial.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
 
  }
  }
  }

  void L293D(char dir,int spd, int en)
{
if(dir =='L')
  {
if(en ==0){
       Serial.println(" CW Motor Stopped");
    }else{
       Serial.print(" Rotating CW: "); 
       Serial.println(spd);//print actual speed value        
    }
    digitalWrite(EN12 ,en);// Enable 1A and 2A 
    analogWrite(P1A,spd);// send PWM with spd value to P1A
    digitalWrite(P2A,LOW);// LOW singal to P2A       

  }else{
if(en ==0){
       Serial.println(" CCW Motor Stopped");
    }else{
       Serial.print(" Rotating CCW: "); 
       Serial.println(spd);//print actual speed value     
    }    
    digitalWrite(EN12 ,en);// Disable 1A and 2A    
    digitalWrite(P1A,LOW);// Keep thos LOW P1A
    analogWrite(P2A,spd);// send PWM with spd value to  P2A  
  }
}
