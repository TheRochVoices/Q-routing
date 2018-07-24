#include<SPI.h>
#include<RF24.h>

RF24 radio(7, 8);

const byte address[6] = "00002";
char text[] = "00004,12344";

void setup() {

  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

}

void loop() {
  writeData(address, text);
  delay(250)
}


int writeData(byte address[], char text[])
{
  radio.openWritingPipe(address);
  radio.write(&text, sizeof(text));  
}
