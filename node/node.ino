#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
//char text[] = "Hello there";
int text = 1234;
void writeData(String tex)
{
   radio.stopListening();
  radio.openWritingPipe(address);
  char text[32] = ""; 
  tex.toCharArray(text, 32);
  Serial.println(text);
  radio.write(&text, sizeof(text));  
}

void setup() {
  radio.begin();
  
  radio.setPALevel(RF24_PA_MIN);
  
  Serial.begin(9600);
}
void loop() {
  String text = "hello ther";
  writeData(text);
   // radio.write(&text, sizeof(text));  

 // delay(250);
}
