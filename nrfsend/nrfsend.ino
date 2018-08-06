#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

int rec[1] = {5};
long int beg, done;
void setup() {
  Serial.begin(57600);
  radio.begin();
  delay(100);

  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  radio.setRetries(15,15);
 
}
void loop() {
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  beg = micros();
  if(radio.isAckPayloadAvailable())
      {
       radio.read(rec,sizeof(int));
        Serial.print("received ack payload is : ");
        done = micros();
        Serial.println(rec[0]);
        Serial.println("Propogation Delay ");
        Serial.println(done-beg);
      }
      else
      {
        Serial.println("status has become false so stop here....");
      }
  delay(250);
}
