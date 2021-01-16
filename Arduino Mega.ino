//Speakers and LCD on Mega:
//============================
#include <Wire.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
char MasterReceive;
int master ;
int songID = 1;
//Line Follower and Car Motors
//=============================
// -------defining Inputs------/
#define LS 22       // left sensor
#define RS 26     // right sensor

// -------defining Outputs------/
#define LM1 5       // left motor
#define LM2 6       // left motor
#define RM1 7       // right motor
#define RM2 12       // right motor
#define EnableA 2   //left
#define EnableB 4   //right

#define Lamp1  28
#define Lamp2  30    

//=============================
// LDR:
//=====

int ledpin = 9;
int ldrpin = A0;
int value = 0;
int ledpin2 = 13;
//=============================
//Seven Segment and Joy Stick
//============================
int x_key = A1;                                              
int y_key = A2;                                           
int x_pos;
int y_pos;
int A=40;
int B=42;
int C=44;
int D=46;
int E=48;
int F=50;
int G=52;
//============================

















void TaskMovement(void *pvParameters);
void TaskFeatures(void *pvParameters);

void TaskFeatures(void *pbParameters)
{while(1)
{
  Serial.println("here");
     //=============================
    // LDR:
    //=====
   value = analogRead(ldrpin);
  value = constrain(value,350,500);
  value = map(value,400,500,255,0);

  Serial.println(value);
  value = analogRead(ldrpin);
  value = constrain(value,350,500);
  value = map(value,400,500,255,0);

  Serial.println(value);
  analogWrite(ledpin, 382-value);
analogWrite(ledpin2, 382 - value);
  delay(200);

    //=============================
    //Seven Segment and Joy Stick
    //============================
    x_pos = analogRead (x_key) ;  
    y_pos = analogRead (y_key) ; 
    if (x_pos < 300) //D
    {
      Serial.println(x_pos);
      digitalWrite(A,HIGH);
      digitalWrite(B,LOW);
      digitalWrite(C,LOW);
      digitalWrite(D,LOW); 
      digitalWrite(E,LOW);
      digitalWrite(F,HIGH);
      digitalWrite(G,LOW);   
    }
    if (x_pos > 700) //R
    {
      digitalWrite(A,LOW);
      digitalWrite(B,HIGH);
      digitalWrite(C,HIGH);
      digitalWrite(D,HIGH); 
      digitalWrite(E,LOW);
      digitalWrite(F,LOW);
      digitalWrite(G,HIGH); 
    }      
    if (y_pos < 300) //N
    {
      digitalWrite(A,LOW);
      digitalWrite(B,LOW);
      digitalWrite(C,LOW);
      digitalWrite(D,HIGH); 
      digitalWrite(E,LOW);
      digitalWrite(F,LOW);
      digitalWrite(G,HIGH); 
    }
    if (y_pos > 700) //P as it is 
    {
      digitalWrite(A,LOW);
      digitalWrite(B,LOW);
      digitalWrite(C,HIGH);
      digitalWrite(D,HIGH); 
      digitalWrite(E,LOW);
      digitalWrite(F,LOW);
      digitalWrite(G,LOW); 
  }  
  //Speakers and LCD on MEGA
  static unsigned long timer = millis();
  Wire.requestFrom(8,1);                           // request 1 byte from slave arduino (8)
  while(Wire.available()){
  MasterReceive = Wire.read();                // receive a byte from the slave arduino and store in MasterReceive                   
  master = (int)MasterReceive;
//  Serial.println((int)MasterReceive);
  if(master ==-1){
//    Serial.println("kk");
     myDFPlayer.pause(); 
  }else if(master == songID){
    myDFPlayer.start();
  }else{
    myDFPlayer.play(master);
    songID = master;
  }
  //myDFPlayer.play(master); 
//delay(10000);
    
  }
  vTaskDelay( 2000 / portTICK_PERIOD_MS );
}

  }
void TaskMovement(void *pbParameters)
{
  while(1)
  {
    Serial.println("here2");
//Line Follower and Car Motors
    //=============================
    if(digitalRead(RS) == 1 && digitalRead(LS) == 1)     // Move Forward3
  { 
    analogWrite(EnableA,70);
    analogWrite(EnableB,80);

    digitalWrite(LM1, LOW );
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
      //lamps
    digitalWrite(Lamp1, LOW);
    digitalWrite(Lamp2, LOW); 
    
 }

 if(digitalRead(RS) == 0 && digitalRead(LS) ==0)     // o2af
  { 
    analogWrite(EnableA,0);
    analogWrite(EnableB,0);

    digitalWrite(LM1, LOW );
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
      //lamps
    digitalWrite(Lamp1, LOW);
    digitalWrite(Lamp2, LOW); 
    
 }
 else{
     if( digitalRead(LS) == 0)   // Turn right
  {

     analogWrite(EnableA,0);
     analogWrite(EnableB,150);
    
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
     //lamps
    digitalWrite(Lamp1, LOW);
    digitalWrite(Lamp2, HIGH);         

  }
  
   if(digitalRead(RS) == 0 )    // turn left
  { 

    analogWrite(EnableA,150);
    analogWrite(EnableB,0);
    
   digitalWrite(LM1, LOW );
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    
    digitalWrite(Lamp1, HIGH);
    digitalWrite(Lamp2, LOW);

  }
 }
  vTaskDelay( 100 / portTICK_PERIOD_MS );  
  }
  
  }
void setup()
{

 //Speakers and LCD on Mega
  pinMode(8,INPUT);
  Wire.begin();   
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  //Serial.println(F("DFRobot DFPlayer Mini Demo"));
  //Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
//    Serial.println(F("Unable to begin:"));
//    Serial.println(F("1.Please recheck the connection!"));
//    Serial.println(F("2.Please insert the SD card!"));
    while(true);
    }
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  //Line Follower and Car Motors
  //=============================
  Serial.begin(9600);
  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
  pinMode(EnableA, OUTPUT);  //left
  pinMode(EnableB, OUTPUT);  //right

  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);

  pinMode(Lamp1, OUTPUT);
  pinMode(Lamp2, OUTPUT);
//    Serial.println("here2");

  //=============================
  //Seven Segment and Joy Stick
  //============================
  pinMode (x_key, INPUT) ;                     
  pinMode (y_key, INPUT) ; 
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  pinMode(E,OUTPUT);
  pinMode(F,OUTPUT);
  pinMode(G,OUTPUT); 
  
  xTaskCreate(TaskMovement,"TaskMovement",128,NULL,2,NULL);
  xTaskCreate(TaskFeatures,"TaskFeatures",128,NULL,1,NULL);
}

void loop()
{

    
}
