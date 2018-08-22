#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
String add = "00001";
String neighbors[4] = {"00002", "00003", "00004", "00005"};
byte address[6];

int rec[1] = {2};
float qTable[4] = {1,3,2, 6};
const int nNodes = 4;
long int beg, done = 0;

int bestNode()
{
  int mini = 9999, index;
  for(int i=0;i<nNodes;i++)
  {
    if(qTable[i] < mini)
    {
      mini = qTable[i];
      index = i;
    }
  }
  return index;
}
void sendData(int  index)
{
  
  byte bestNode[6];
  neighbors[index].getBytes(bestNode, 6);
  
  radio.stopListening();
  radio.openWritingPipe(bestNode);

  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  beg = micros();
  if(radio.isAckPayloadAvailable())
      {
       radio.read(rec,sizeof(int));
        Serial.print("received ack");
        done = micros();
        //Serial.println(rec[0]);
        qTable[index] = (done-beg) + float(.7*rec[0]);
        Serial.println("Contents of qTable: ");
        Serial.println(qTable[index]);
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
      int i = bestNode();
      done = micros();
      rec[0] = qTable[i];
      radio.writeAckPayload( 1, rec, sizeof(int) );
      char text[32] = "";
      radio.read(&text, sizeof(text));
      Serial.println(text);
      //sendData(i);
  }
}
void setup() {
  /*******************************/ 
  add.getBytes(address, 6);

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
  beg = micros();
  recieveData();
}
