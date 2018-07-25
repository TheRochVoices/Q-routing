#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
int i=0;

const byte address[2][6] = {"00002", "00004"};
void setup() {
  radio.begin();
  radio.openWritingPipe(address[0]);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  if(i%2==0)
  {
    radio.openWritingPipe(address[1]);
    const char text[] = "00003,1234";
    radio.write(&text, sizeof(text));
    delay(1000);
  }
  else
  {
    radio.openWritingPipe(address[0]);
    const char text[] = "00003,1234";
    radio.write(&text, sizeof(text));
    delay(1000);    
  }
  i++;
}
