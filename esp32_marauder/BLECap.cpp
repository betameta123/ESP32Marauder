#include "BLECap.h"
#include "SPIFFS.h"
#include "CommandLine.h"

#ifdef HAS_BT
  #include <NimBLEDevice.h>
#endif

const char* hexTable = "0123456789ABCDEF";

String toHex(uint8_t* data, size_t length) {
  String s = "";
  for(int i = 0; i < length; i++){
    s += hexTable[data[i] >> 4];
    s += hexTable[data[i] & 15];
  }
  return s;
}

String bleAdvCapture(BLEAdvertisedDevice *advertisedDevice)
{
  // time, addr, target_addr, rssi, txpower, name, manufacture, URI, payload
  size_t dataLength = advertisedDevice->getPayloadLength();
  uint8_t* data = advertisedDevice->getPayload();
  String display_string = "";

  display_string.concat(advertisedDevice->getTimestamp());
  display_string.concat(",");
  display_string.concat(advertisedDevice->getAddress().toString().c_str());
  display_string.concat(",");
  if(advertisedDevice->haveTargetAddress()) {
    display_string.concat("\"");
    for(int i = 0; i < advertisedDevice->getTargetAddressCount(); i++) {
      display_string.concat((advertisedDevice->getTargetAddress(i)).toString().c_str());
      display_string.concat(",");
    }
    display_string.concat("\"");
  }
  display_string.concat(",");
  if(advertisedDevice->haveRSSI())
    display_string.concat(advertisedDevice->getRSSI());
  display_string.concat(",");
  if(advertisedDevice->haveTXPower())
    display_string.concat(advertisedDevice->getTXPower());
  display_string.concat(",");
  if(advertisedDevice->haveName())
    display_string.concat(advertisedDevice->getName().c_str());
  display_string.concat(",");
  if(advertisedDevice->haveManufacturerData())
    display_string.concat(toHex((uint8_t*)advertisedDevice->getManufacturerData().c_str(), advertisedDevice->getManufacturerData().size()));
  display_string.concat(",");
  if(advertisedDevice->haveURI())
    display_string.concat(advertisedDevice->getURI().c_str());
  display_string.concat(",");
  display_string.concat(toHex(data, dataLength));

  return display_string;
}
