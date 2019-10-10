/*
 * Will listen to messages on canbus and print the message data.
 * 
 * Has a scan mode where it will store all unique message IDs.
 * 
 * Can then be told to focus on a single one of those message ids and only print its messages.
 * 
 * Has a message block scan mode where, once scanned, it will only print messages where the first data block is unique.
 */

#include <Canbus.h>  // don't forget to include these
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>
#include "ButtonHandler.h"
#include "Utils.h"

/*
 * A1 - 15 - Up
 * A2 - 16 - Left
 * A3 - 17 - Down
 * A4 - 18 - Click
 * A5 - 19 - Right
 */

ButtonHandler upBTN(15, INPUT_PULLUP, LOW);
ButtonHandler leftBTN(16, INPUT_PULLUP, LOW);
ButtonHandler downBTN(17, INPUT_PULLUP, LOW);
ButtonHandler clickBTN(18, INPUT_PULLUP, LOW);
ButtonHandler rightBTN(19, INPUT_PULLUP, LOW);

uint16_t  ids[100] = {};
int currentIdIndex = 0;


boolean checkIDs = false;
boolean fixedId = false;

void emptyIDs() {
  memset(ids, 0, sizeof(ids));
}

void onUp() {
  checkIDs = !checkIDs;

  if (checkIDs)
    emptyIDs();
  else
    currentIdIndex = 0;
  
  Serial.print("Checking ID Mode: ");
  Serial.print(checkIDs);
  Serial.println("");
}

void onDown() {
  // Send message to lock car
tCAN message;

        message.id = 0xD2; // 210
        message.header.rtr = 0;
        message.header.length = 2; //formatted in DEC
        message.data[0] = 0x20;
  message.data[1] = 0x8;

mcp2515_bit_modify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), 0);
mcp2515_send_message(&message);
}

void onLeft() {
  if (checkIDs)
    return;
    
  currentIdIndex++;

  if (currentIdIndex >= 100)
    currentIdIndex = 0;
    
  Serial.print("Current Id index: ");
  Serial.print(ids[currentIdIndex]);
  Serial.println("");
}

void onRight() {
  if (checkIDs)
    return;

  currentIdIndex--;

  if (currentIdIndex < 0)
    currentIdIndex = 0;

  Serial.print("Current Id index: ");
  Serial.print(ids[currentIdIndex]);
  Serial.println("");
}

void onClick() {
  if (checkIDs) {
    Serial.println("Not setting fixed id when checking for ids is active");
    return;
  }
  
  fixedId = !fixedId;

  if (!fixedId)
    currentIdIndex = 0;

  Serial.print("Fixed ID Mode: ");
  Serial.print(fixedId);
  Serial.println("");
}

void captureId(uint16_t id) {
  if (!checkIDs)
    return;

  boolean within = Utils::findInArray(ids, id);
  
  if (!within) {
    ids[currentIdIndex] = id;
    currentIdIndex++;

    Serial.print("Captured id: ");
    Serial.print(id);
    Serial.println("");
  }
}


void printMessage(tCAN message) {
  Serial.print("ID: ");
  Serial.print(message.id,HEX);
  Serial.print(", ");
  Serial.print("Data: ");
        
  for(int i=0;i<message.header.length;i++)
  {
    Serial.print(message.data[i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void setup()
{
  Serial.begin(9600);

  //Initialise MCP2515 CAN controller at the specified speed
  if(Canbus.init(CANSPEED_125))
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't Init CAN");

  upBTN.initialise(onUp);
  leftBTN.initialise(onLeft);
  downBTN.initialise(onDown);
  clickBTN.initialise(onClick);
  rightBTN.initialise(onRight);
  
  delay(1000);
}

void loop()
{ 
  upBTN.onLoop();
  leftBTN.onLoop();
  downBTN.onLoop();
  clickBTN.onLoop();
  rightBTN.onLoop();
  
  tCAN message;

  if (mcp2515_check_message()) 
  {
    if (mcp2515_get_message(&message)) 
    {
        printMessage(message);
          return;
        //Don't care about this stuff but im lazy and can;t be bothered removing it properly.
      if (checkIDs) {
        captureId(message.id);
        return;
      }
      
      if(!fixedId) {
        return;
      }      
      
      if (ids[currentIdIndex] == message.id)
        printMessage(message);
    }
  }
}
