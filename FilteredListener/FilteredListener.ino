/*
 * Will listen to messages on canbus and print the message data.
 * Can be put into 'checking' mode where instead of printing it will store all unique message ids in memory
 * When put out of checking mode only message ID's not within the in memory array will be printed.
 * 
 * This can be helpful for removing background message noise.
 * 
 * PS: Run this on both HS and MS CAN and whilst ive proven the logic works through printing messages
 *  There doesnt appear to be any unique ID's when using lights or electric windows after disabling
 *  the checking.
 *  
 *  Next test is to check for changes in the message data, not for changing IDs. This will be tedious. 
 *  Ill have to check for changes on every single ID found.
 */

#include <Canbus.h>  // don't forget to include these
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>

uint16_t  ids[100] = {};
int currentIdIndex = 0;
boolean checkIDs = false;
int lastButtonValue;

boolean findInArray(uint16_t arr[], uint16_t toFind) 
{
  for(int i = 0; i <= 100; i++) {
      if (arr[i] == toFind)
        return true;
  }

  return false;
}

void setup()
{
  Serial.begin(9600);

  //Initialise MCP2515 CAN controller at the specified speed
  if(Canbus.init(CANSPEED_500))
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't Init CAN");
  
  pinMode(15, INPUT_PULLUP);
  
  delay(1000);
}

boolean isIDUnique(uint16_t id) {
  
  boolean within = findInArray(ids, id);

  if (checkIDs == false) {
    return !within;
  }

  if (!within) {
    ids[currentIdIndex] = id;
    currentIdIndex++;
  }

  return false;
}

void loop()
{ 
  int state = digitalRead(15);

  if (state != lastButtonValue) {
    if (state == LOW) {
      checkIDs = !checkIDs;

      if (checkIDs)        
        Serial.println("Checking");
      else
        Serial.println("Not Checking");
    }
  }

  lastButtonValue = state;

  
  tCAN message;

  if (mcp2515_check_message()) 
  {
    if (mcp2515_get_message(&message)) 
    {
    
      if (!isIDUnique(message.id))
        return;
      
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
  }
}
