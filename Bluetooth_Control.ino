/*
* Control LED's with Smartphone via bluetooth

*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(D2,D3); // RX, TX 

int led1 = D4;
int led2 = D5 ;

String state;

void setup() {
// sets the pins as outputs:
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);

// initialize serial communication at 9600 bits per second:
Serial.begin(9600);
mySerial.begin(38400);
}

void loop() {
 while (mySerial.available()){  //Check if there is an available byte to read
 delay(10); //Delay added to make thing stable 
 char c = mySerial.read(); //Conduct a serial read
 //if (c == '#') {break;} //Exit the loop when the # is detected after the word
 state += c; //Shorthand for voice = voice + c
 Serial.println(state);
  } 

// if the state is 'LED1ON' the Led1 will turn on
if (state.equals("1")) {
digitalWrite(led1, HIGH);
Serial.println("Light On");
}
// if the state is 'LED1OFF' the led1 will turn off
else  if (state.equals("2")){
digitalWrite(led1, LOW); 
Serial.println("Light Off");
}
// if the state is 'LED2ON' the Led2 will turn on
else  if (state.equals("3")){
digitalWrite(led2, HIGH);
Serial.println("AC On");
} 

// if the state is 'LED2OFF' the led2 will turn off
else  if (state.equals("4")){
digitalWrite(led2, LOW);
Serial.println("AC Off");
}
// if the state is 'ALLON' the Led's will turn on
else if (state.equals("5")) {
digitalWrite(led1, HIGH);
digitalWrite(led2, HIGH);     
Serial.println("All On");
}
// if the state is 'ALLOFF' the Led's will turn off
else if (state.equals("6")) {
digitalWrite(led1, LOW);
digitalWrite(led2, LOW);
Serial.println("All Off");
}
state="";
}
