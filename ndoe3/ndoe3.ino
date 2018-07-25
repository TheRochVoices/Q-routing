#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
int i=0;
int flag = 0;
const byte address[2][6] = {"00002", "00004"};
char text[32] = "";

void sendShit()
{
    radio.stopListening();
    radio.openWritingPipe(address[1]);
    radio.write(&text, sizeof(text));
      radio.openReadingPipe(1, address[0]);
        radio.startListening();

  return;
}
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, address[0]);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) 
  {
    radio.read(&text, sizeof(text));
    Serial.println(text);
    sendShit();

  }
}
