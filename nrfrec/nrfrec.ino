#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
const byte address2[6] = "00002";
int rec[1] = {2};
int qTable[2] = {0};
int a, b = 0;


void sendData(byte address[])
{
  radio.stopListening();
  radio.openWritingPipe(address);

  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  if(radio.isAckPayloadAvailable())
      {
       radio.read(rec,sizeof(int));
        Serial.print("received ack");
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
  radio.startListening();
}

void recieveData()
{
  if (radio.available()) {
    b = micros();
    rec[0] = b-a;
    radio.writeAckPayload( 1, rec, sizeof(int) );
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    //sendData(address2);
  }
}
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
  a = micros();
  recieveData();
}
