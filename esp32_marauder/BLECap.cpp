#include "BLECap.h"
#include "SPIFFS.h"
#ifdef HAS_BT
  #include <NimBLEDevice.h>
#endif

const char* hexTable[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};

void bleAdvCapture(BLEAdvertisedDevice *advertisedDevice)
{
  size_t dataLength = advertisedDevice->getPayloadLength();
  uint8_t* data = advertisedDevice->getPayload();
  String display_string = "";
  for (int i = 0; i < dataLength; i++)
  {
    display_string.concat(hexTable[data[i] >> 4]); // encode 4 msb in hex
    display_string.concat(hexTable[data[i] & 15]); // encode 4 lsb in hex
  }
  return display_string;
}
