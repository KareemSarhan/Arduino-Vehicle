  #include <Wire.h>

#include <SPFD5408_Adafruit_GFX.h> // Core graphics library

#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library

#include <SPFD5408_TouchScreen.h>


#define YP A3 // must be an analog pin, use "An" notation!

#define XM A2 // must be an analog pin, use "An" notation!

#define YM 9 // can be a digital pin

#define XP 8 // can be a digital pin

#define LCD_CS A3

#define LCD_CD A2

#define LCD_WR A1

#define LCD_RD A0

#define LCD_RESET A4


#define WHITE 0x0000 //Black->White

#define YELLOW 0x001F //Blue->Yellow

#define CYAN 0xF800 //Red->Cyan

#define PINK 0x07E0 //Green-> Pink

#define RED 0x07FF //Cyan -> Red

#define GREEN 0xF81F //Pink -> Green

#define BLUE 0xFFE0 //Yellow->Blue

#define BLACK 0xFFFF //White-> Black

#define MINPRESSURE 10

#define MAXPRESSURE 1000


/*____Calibrate TFT LCD_____*/

#define TS_MINX 850

#define TS_MINY 100

#define TS_MAXX 180

#define TS_MAXY 780

/*______End of Calibration______*/ 

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 10); //300 is the sensitivity

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET); //Start communication with LCD 

int X,Y;

long Num1,Num2,Number;

int action=1;

boolean result = false; 

long songNumber = 1;

int pause=-1;



//byte SlaveReceived = 0;

void setup() {
pinMode(13,OUTPUT);
Serial.begin(9600); //Use serial monitor for debugging

tft.reset(); //Always reset at start

tft.begin(0x9341); // My LCD uses LIL9341 Interface driver IC

tft.setRotation(0); // I just roated so that the power jack faces up - optional

tft.fillScreen(WHITE); 

draw_BoxNButtons();

Wire.begin(8);                          //Begins I2C communication with Slave Address as 8 at pin (A4,A5)
  
Wire.onRequest(requestEvent);           //Function call when Master request value from Slave


} 

void loop() {
digitalWrite(13,HIGH);
TSPoint p = waitTouch();

X = p.y; Y = p.x;

// Serial.print(X); Serial.print(','); Serial.println(Y);// + " " + Y); 
action=0;
DetectButtons(); 
display();




} 

TSPoint waitTouch() {

TSPoint p;

do {

p = ts.getPoint();

pinMode(XM, OUTPUT);

pinMode(YP, OUTPUT);

} while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));

p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());

p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());;


return p;

} 

void DetectButtons()
{ 
      action = 0;
  if(X>150 && X<165 && Y>140 && Y<200 ) {
    Serial.println("here");//Pause
    action=1;
    //songNumber=-1;
    pause = -1;
    result = false;
  }
  if(X>220 && X<250 && Y>140 && Y<200){
    pause = 0;
    Serial.println("here1");//Play
    result = true;
      action = 2;  
      Serial.println(action);   
  }
  if(X>330 && X<360 && Y>140 && Y<200 && result ){
    Serial.println("here2");//previous
    
    if(songNumber == 1){
      songNumber = 6;
    }
    else{
     songNumber = songNumber - 1;
    }
    //Serial.println(songNumber);
    action = 3;
  
  }

 if(X>50 && X<120 && Y>140 && Y<200 && result){
    Serial.println("here3");//next
    if(songNumber == 6){
     songNumber = 1;
    }
    else{
      songNumber = songNumber + 1;
    }
     //Serial.println(songNumber);
    action = 4;
   
  }
  
 // tft.println(songNumber);
} 


void draw_BoxNButtons()

{
tft.fillTriangle(150,195,190,195,170,175,BLACK); //play
tft.fillRect(155,100,35,5,BLACK);// pause
tft.fillRect(155,120,35,5,BLACK);
tft.fillTriangle(150,260,190,260,170,280,BLACK);//previous
tft.fillTriangle(150,280,190,280,170,300,BLACK);
tft.fillTriangle(150,40,190,40,170,20,BLACK);//next
tft.fillTriangle(150,60,190,60,170,40,BLACK);

}

void display(){
  
//tft.fillRect(0, 0, 240, 10, WHITE); //clear result box
//
tft.setCursor(50, 70);
tft.setTextSize(4);
tft.setTextColor(BLACK, WHITE);
tft.setRotation(3);
if(songNumber!=-1 && songNumber !=0){
tft.println(songNumber); //update new value
}
tft.setRotation(0);
}


void requestEvent()                                //This Function is called when Master wants value from slave
{
//  //int potvalue = analogRead(A0);                   // Reads analog value from POT (0-5V)
//  //byte SlaveSend = map(potvalue,0,1023,0,127);    // Convert potvalue digital value (0 to 1023) to (0 to 127)
  //Wire.write(songNumber);    // sends one byte converted POT value to master
  if(pause == -1)
  Wire.write(-1);
  else
  Wire.write(songNumber);
}
