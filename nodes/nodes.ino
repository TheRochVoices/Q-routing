/* This is a code for coordinator nodes in the wireless sensor network */

#include <SPI.h>
#include "RF24.h"


#define CON_CHANNEL 0x100       //Channel for listening and broadcasting request for connecting a new node to the network or getting connected to the network, default value 100 

// ce, csn pins
RF24 radio(7,8);
const byte address[6][2] = {"00001","00002"};

int MAX_CON=3;               //Max no of devices a node can connect to, default value is 3
int nodes[3]={0,0,0};                   //Array that stores node id of nodes in connection
String channels[3];             //Channels available for this node to scan
int no_connected_nodes=0;               //No of nodes currently connected to this node
int my_id;                              //ID of node

void request_connection() {
    /* This function is for connecting a new node to the network */
    Serial.println("Incide");
    radio.setChannel(CON_CHANNEL);
    int con_request=0;
    int flag=1;
    
    while (flag){
      
      /* Send Request */
      radio.stopListening();
      radio.openWritingPipe(address[0]);
      radio.write(&con_request, sizeof(con_request));
      delay(50);
      
      /* Listen for reply */
      radio.openReadingPipe(0,address[0]);
      radio.startListening();
      if(radio.available())
        {flag=0;
         char id[4];
         radio.read(&id, sizeof(id));
         my_id = 2*(int(id[1])-48)+(int(id[0])-48);
         delay(200);
         Serial.println("again");
         
         /* Send Acknowledgement */
         Serial.println("Sending ack");
         radio.stopListening();
         radio.openWritingPipe(address[0]);
         radio.write(&my_id, sizeof(my_id));

         /* Update Parameters */
         nodes[no_connected_nodes]=int(id[1])-48;
         channels[no_connected_nodes]="0x"+(MAX_CON*(int(id[1])-48)-no_connected_nodes);
         no_connected_nodes++;
         Serial.println("Connected");
         Serial.println(my_id);
         Serial.println(nodes[no_connected_nodes]);
         delay(2000);
        }
      }
}

void listen_request() {
  /* This function is to add a new node to the network */

  String temp_id;
  char id[4];
  temp_id=temp_id+no_connected_nodes+my_id;
  temp_id.toCharArray(id, 4);
  radio.setChannel(CON_CHANNEL);
  radio.openReadingPipe(0,address[0]);
  radio.startListening();

  if(radio.available())
        { 
         radio.stopListening();
         radio.openWritingPipe(address[0]);
         radio.write(&id, sizeof(id));
         
         radio.openReadingPipe(0,address[0]);
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
