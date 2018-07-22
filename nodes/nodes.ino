/* This is a code for coordinator nodes in the wireless sensor network */

#include <SPI.h>
#include "RF24.h"

#define MAX_CON 3               //Max no of devices a node can connect to, default value is 3
#define CON_CHANNEL 0x100       //Channel for listening and broadcasting request for connecting a new node to the network or getting connected to the network, default value 100 

// ce, csn pins
RF24 radio(9, 10);
uint64_t address = 0xF0F0F0F0E1LL;


int nodes[3]={0,0,0};                   //Array that stores node id of nodes in connection
int channels[3]={-1,-1,-1};             //Channels available for this node to scan
int no_connected_nodes=0;               //No of nodes currently connected to this node
int my_id;                              //ID of node

void request_connection() {
    /* This function is for connecting a new node to the network */
    
    radio.setChannel(CON_CHANNEL);
    char con_request[]="NEW";
    int flag=1;
    
    while (flag){
            
      /* Send Request */
      radio.stopListening();
      radio.openWritingPipe(address);
      radio.write(&con_request, sizeof(con_request));

      /* Listen for reply */
      radio.openReadingPipe(0,address);
      radio.startListening();
      delay(50);
      if(radio.available())
        {flag=0;
         radio.read(&my_id, sizeof(my_id));

         /* Send Acknowlegdement */
         char temp[4];
         radio.stopListening();
         radio.openWritingPipe(address);
         radio.write(&temp, sizeof(temp));

         /* Update Parameters */
         int node_id = 2*atoi(temp[1])+atoi(temp[0]);
         nodes[no_connected_nodes]=node_id;
         channels[no_connected_nodes]=MAX_CON*atoi(temp[1])-no_connected_nodes;
         no_connected_nodes++;
        }
      }
}

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
         radio.stopListening();
         radio.openWritingPipe(address);
         radio.write(&id, sizeof(id));
         
         radio.openReadingPipe(0,address);
         radio.startListening();
         unsigned long started_waiting_at = micros();
         boolean timeout = false;
         while(!radio.available())                                  // While nothing is received
          {if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
                  timeout=true;
                  break;
                }      
          }
         if(!timeout) 
         {int temp;
         radio.read(&temp, sizeof(temp));
         nodes[no_connected_nodes]=temp;
         channels[no_connected_nodes]=MAX_CON*my_id-no_connected_nodes;
         no_connected_nodes++;} 
        }
        
}

void setup() {
  Serial.begin(9600);  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues 
  radio.setPALevel(RF24_PA_LOW);
  request_connection();

}

void loop() {



}
