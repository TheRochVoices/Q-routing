#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
int rec[1] = {5};
long int beg, done;
int qTable[1] = {0};

void sendData(byte address[])
{
  radio.stopListening();
  radio.openWritingPipe(address);

  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  beg = micros();
  if(radio.isAckPayloadAvailable())
      {
       radio.read(rec,sizeof(int));
        Serial.print("received ack");
        done = micros();
        //Serial.println(rec[0]);
        qTable[0] = rec;
        Serial.println("Contents of qTable: ");
        Serial.println(rec[0]);
      }
      else
      {
        Serial.println("status has become false so stop here....");
      }
  delay(250);
}

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
 sendData(address);
}
