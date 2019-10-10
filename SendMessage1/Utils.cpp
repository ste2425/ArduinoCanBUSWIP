#include "Utils.h"

bool Utils::findInArray(uint16_t arr[], uint16_t toFind) 
{
  for(int i = 0; i <= 100; i++) {
      if (arr[i] == toFind)
        return true;
  }

  return false;
}
