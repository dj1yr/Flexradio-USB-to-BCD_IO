/* this software is alpha!!
only CAT Data read from Flex 6x00
*/


#include "Arduino.h"
#include "PCF8575.h"          // https://github.com/xreef/PCF8575_library
#include <SoftwareSerial.h>

SoftwareSerial cat_ser(5, 6); // RX, TX

PCF8575 pcf8575(0x20);


char inputByte;  
String inputbuffer = "";  
bool readComplete = false; 

byte band = 9;
int A = 7;
int B = 8;
int C = 9;
int D = 10; 

bool TX_in = 2;
bool TX_out = 3;
int TX_ok = false;

bool pcf_a0;
bool pcf_a1;
bool pcf_a2;
bool pcf_a3;
bool pcf_a4;
bool pcf_a5;
bool pcf_a6;
bool pcf_a7;

bool pcf_b0;
bool pcf_b1;
bool pcf_b2;
bool pcf_b3;
bool pcf_b4;
bool pcf_b5;
bool pcf_b6;
bool pcf_b7;
byte pcf_a;
byte pcf_b;

byte bcdoutputPins[] {7,8,9,10};

/*byte yaesu_bcd [16][4]={   //BCD table -- yaesu_bcd [band][A,B,C;D]
{0,0,0,0},
{0,0,0,1},    //160m
{0,0,1,0},    //80m
{0,0,1,1},    //60/40m
{0,1,0,0},    //30m
{0,1,0,1},    //20m
{0,1,1,0},    //17m
{0,1,1,1},    //15m
{1,0,0,0},    //12m
{1,0,0,1},    //10m
{1,0,1,0},    //6m
{1,0,1,1},    //4m
{1,1,0,0},    //2m
{1,1,0,1},    //70cm
{1,1,1,0},    //23cm
{1,1,1,1},
};*/



float yaesu_bcd [16][4]={   //BCD table -- yaesu_bcd [band][A,B,C;D]
{A=0,B=0,C=0,D=0},
{A=0,B=0,C=0,D=1},    //160m
{A=0,B=0,C=1,D=0},    //80m
{A=0,B=0,C=1,D=1},    //60/40m
{A=0,B=1,C=0,D=0},    //30m
{A=0,B=1,C=0,D=1},    //20m
{A=0,B=1,C=1,D=0},    //17m
{A=0,B=1,C=1,D=1},    //15m
{A=1,B=0,C=0,D=0},    //12m
{A=1,B=0,C=0,D=1},    //10m
{A=1,B=0,C=1,D=0},    //6m
{A=1,B=0,C=1,D=1},    //4m
{A=1,B=1,C=0,D=0},    //2m
{A=1,B=1,C=0,D=1},    //70cm
{A=1,B=1,C=1,D=0},    //23cm
{A=1,B=1,C=1,D=1},
};

void setup(){
  Serial.begin(9600);       //Debug Serial
  Serial1.begin(9600);      //CAT Serial Flex
  cat_ser.begin(9600);      //CAT Serial optional

// pinMode BCD
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
//  pinMode TX
  pinMode(TX_in, INPUT_PULLUP);
  pinMode(TX_out, OUTPUT); 
// PCF8575
  // Set pinMode to OUTPUT
  for(int i=0;i<16;i++) {
    pcf8575.pinMode(i, OUTPUT);
  }
  pcf8575.begin();

}   //end setup

void loop() {


      if (Serial1.available() > 0) {
        inputByte = (char) Serial1.read(); 
        inputbuffer += inputByte; 
        if (inputByte == ';') {
        readComplete = true; 
        }
      }

    
    if (readComplete) {
        String input_str = inputbuffer;
        String freq ;
        freq = input_str.substring(4,13);


    if (freq>= "470000000"){          // 23cm Band selcted
      band = 14;
      bcdoutputPins[4] = band;}
          else if (freq >= "150000000"){  // 70cm Band selcted
        band = 13;
        bcdoutputPins[4] = band;}  
      else if (freq >= "074000000"){  // 2m Band selcted
        band = 12;
        bcdoutputPins[4] = band;}      
      else if (freq >= "054000000"){  // 4m Band selcted
        band = 11;
        bcdoutputPins[4] = band;}      
      else if (freq >= "030000000"){  // 6m Band selcted
        band = 10;
        bcdoutputPins[4] = band;}
      else if (freq >= "028000000"){  // 10m Band selcted
        band = 9;
        bcdoutputPins[4] = band;}        
      else if (freq >= "024000000"){  // 12 Band selcted
        band = 8;
        bcdoutputPins[4] = band;}
      else if (freq >= "020000000"){  // 15m Band selcted
        band = 7;
        bcdoutputPins[4] = band;}
      else if (freq >= "015000000"){  // 17m Band selcted  
        band = 6;
        bcdoutputPins[4] = band;}
      else if (freq >= "012000000"){  // 20m Band selcted
        band = 5;
        bcdoutputPins[4] = band;}        
      else if (freq >= "009000000"){  // 30 Band selcted
        band = 4;
        bcdoutputPins[4] = band;}
      else if (freq >= "005000000"){  // 60/40m Band selcted
        band = 3;
        bcdoutputPins[4] = band;}
      else if (freq >= "002000000"){  // 80m Band selcted
        band = 2;
        bcdoutputPins[4] = band;}
      else if (freq >= "001700000"){  // 160m Band selcted
        band = 1;
        bcdoutputPins[4] = band;}        
      else {
        band = 0;
        bcdoutputPins[4] = band;}                    // no Band selcted  

        Serial.print("freq_Hz=");
        Serial.println(freq);
        Serial.print("Band=");
        Serial.println(band);
        Serial.print("Outputbyte=");
        Serial.println(bcdoutputPins[4], BIN);
        Serial.flush();
        pcf8575_write();
        
        inputbuffer = "";
        readComplete = false; 
      }

    if (band = 0){
        TX_ok = false;}
      else {
        TX_ok = true;}  

    if((TX_ok)&&digitalRead(TX_in==LOW)){
        digitalWrite(TX_out,HIGH);}
      else{
        digitalWrite(TX_out,LOW);}    

  } //end loop

//Test############################
  void pcf8575_write() {
  static int pin = 0;
  /*pcf8575.digitalWrite(pin, HIGH);
  delay(100);
  pcf8575.digitalWrite(pin, LOW);
  delay(100);
  pin++;
  if (pin > 15) pin = 0;*/

      digitalWrite(A,A);
      digitalWrite(B,B);
      digitalWrite(C,C);
      digitalWrite(D,D);

  } //end subloop
