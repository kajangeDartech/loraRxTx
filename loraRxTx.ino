#include <SPI.h>
#include <LoRa.h>

char payload[3] = "92";         //number of flies read by the camera, this will be read by serial port from the rpi (for now just keep it as a set value)
char addrTx[3] = "12";          //address of this transmitter
char packet[5];                 //packet to transmit after combining address and the payload
char addSet[2] = "12";          //set address
char addrRx[2] ;                //received address, will be extracted from the first two bytes of the received packet
char rxPacket[5] ;              //received packet from another lora
char txPacket[5] ;              //packet to be sent by this lora
int i, j, m;                    //declare integers, they will be assigned size of address and payload frames

void setup() {

  Serial.begin(9600);
  while (!Serial);
  Serial.println("This is Node1");
  if (!LoRa.begin(433E6)) {
  Serial.println("Starting LoRa failed!");
  while (1);
  }
}

void loop(){
/*
 * read packets, store the value on the variable rxPacket
 */
int packetSize = LoRa.parsePacket();            //parse packet
  if (packetSize){
//    Serial.print("Received packet '");

    if(LoRa.available()){    
      for(int k = 0; k < 4; k++){                 
      rxPacket[k] = ((char) LoRa.read());                     //read packet
//      Serial.print(addrRx);
//      Serial.print("is:");
      }
      Serial.print("I receive: ");
      Serial.println(rxPacket);                     //print the received packet on the serial port
  }
  }


/*
 * after receiving packet, forward it if it has the address as the one we expect
 */
  for(i = 0; i < 2 ; i++){                          //copy the first two characters from received packet to variable of the received address(addrRx)
    addrRx[i] = rxPacket[i];
  }
  Serial.print("The address of the received packet is: ");
  Serial.println(addrRx);                           //this is the received address
  Serial.println("=====================================");

//  printf("%s\n",addrRx );             //print the exrtracted address from the received packet

  if(addrRx[0] == addSet[0]){                //compare the first character of the loraRead and the set value
  if(addrRx[1] == addSet[1]){                //if the first characters are same, check the second characters

/*
if the 1st character of the received address is the same as the first character of the set address, if the 2nd character of the received address
is equal to the second character of the set address -> forward the packet
*/
    LoRa.beginPacket();
    LoRa.print(rxPacket);
    LoRa.endPacket();
    delay(500);

    Serial.println("Packet has been forwarded");
  }
  else{                 
    Serial.println("wrong second char_notSend, packet not forwarded");      //if the first characters are not the same, print 1st char's arent the same(dont forward the received packet)
  }
}
  else{
    printf("wrong first char_notSend, packet not forwarded");     //if the first characters are not the same, print 1st char's arent the same(dont forward the received packet)
  }

  
 /*
  as long as the addres of the transmitter is not equal to zero i.e the end of the address string != 0,
  assign each element from that address to the packet, staring from the first element (element 0 of the array)
  */
  for(i = 0; addrTx[i] != 0 ; i++){
    txPacket[i] = addrTx[i];
      }

  /*
  starting from the last element of the packet, continue to write elements to it
   from the addr's element zero with constraint less than size of size of addr
  */
      for(i, j=0; i < (sizeof(payload) + sizeof(addrTx)), j < sizeof(payload); i++, j++){
        txPacket[i] = payload[j];
      } 
      
  Serial.print("I transmitt: ");
  Serial.println(txPacket); 
/*
 * know transimitt the packet after combining the address and the payload
 */
    LoRa.beginPacket();
    LoRa.print(txPacket);
    LoRa.endPacket();
    delay(1000);
}
