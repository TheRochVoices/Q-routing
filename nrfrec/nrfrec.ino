#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
int rec[1] = {2};

void setup() {
  Serial.begin(9600);
  radio.begin();
  delay(100);
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  radio.setRetries(15,15);
}
void loop() {
  if (radio.available()) {
    radio.writeAckPayload( 1, rec, sizeof(int) );
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}
