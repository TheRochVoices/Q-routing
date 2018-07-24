/* This is a code for coordinator nodes in the wireless sensor network */

#include <SPI.h>
#include "RF24.h"

#define MAX_CON 3               //Max no of devices a node can connect to, default value is 3
#define CON_CHANNEL 0x100       //Channel for listening and broadcasting request for connecting a new node to the network or getting connected to the network, default value 100 

// ce, csn pins
RF24 radio(7, 8);
const byte address[6] = "00001";


int nodes[3]={0,0,0};                   //Array that stores node id of nodes in connection
int channels[3]={-1,-1,-1};             //Channels available for this node to scan
int no_connected_nodes=0;               //No of nodes currently connected to this node
int my_id=1;                            //ID of gateway


void listen_request() {
  /* This function is to add a new node to the network */
  char id[4];
  id[0]=char(no_connected_nodes);
  id[1]=char(my_id);
  radio.setChannel(CON_CHANNEL);
  radio.openReadingPipe(0,address);
  radio.startListening();

  if(radio.available())
        {
         /* Send ID to connecting device */
         Serial.println("radio is available"); 
         radio.stopListening();
         radio.openWritingPipe(address);
         radio.write(&id, sizeof(id));
         Serial.println("ID Sent");
         Serial.println(id);

         /* Listen for acknowledgement */
         radio.openReadingPipe(0,address);
         radio.startListening();
         unsigned long started_waiting_at = micros();
         boolean timeout = false;
         while(!radio.available())                                  // While nothing is received
          {if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
                  Serial.println("Timeout");
                  timeout=true;
                  break;
                }      
          }
         if(!timeout) 
         {int temp;
         radio.read(&temp, sizeof(temp));
         Serial.println("Connected to ID= ");
         Serial.print(temp);
         nodes[no_connected_nodes]=temp;
         channels[no_connected_nodes]=MAX_CON*my_id-no_connected_nodes;
         Serial.println("Channel= ");
         Serial.print(channels[no_connected_nodes]);
         no_connected_nodes++;} 
        }
        
}

void setup() {
  Serial.begin(9600);  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues 
  radio.setPALevel(RF24_PA_LOW);

}

void loop() {
  if(no_connected_nodes<=3)
  {listen_request();}


}
