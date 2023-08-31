#include "GpsInterface.h"

#ifdef HAS_GPS

char nmeaBuffer[100];

MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

HardwareSerial Serial2(2);

void GpsInterface::begin() {

  Serial2.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);

  MicroNMEA::sendSentence(Serial2, "$PSTMSETPAR,1201,0x00000042");
  MicroNMEA::sendSentence(Serial2, "$PSTMSAVEPAR");

  MicroNMEA::sendSentence(Serial2, "$PSTMSRR");

  delay(4000);

  if (Serial2.available()) {
    Serial.println("GPS Attached Successfully");
    this->gps_enabled = true;
    while (Serial2.available())
      Serial2.read();
  }
}

// Thanks JosephHewitt
String GpsInterface::dt_string_from_gps(){
  //Return a datetime String using GPS data only.
  String datetime = "";
  if (nmea.isValid() && nmea.getYear() > 0){
    datetime += nmea.getYear();
    datetime += "-";
    datetime += nmea.getMonth();
    datetime += "-";
    datetime += nmea.getDay();
    datetime += " ";
    datetime += nmea.getHour();
    datetime += ":";
    datetime += nmea.getMinute();
    datetime += ":";
    datetime += nmea.getSecond();
  }
  return datetime;
}

void GpsInterface::setGPSInfo() {
  this->good_fix = nmea.isValid();
  this->num_sats = nmea.getNumSatellites();

  this->gps_year = nmea.getYear();
  this->gps_month = nmea.getMonth();
  this->gps_day = nmea.getDay();
  this->gps_hour = nmea.getHour();
  this->gps_minute = nmea.getMinute();
  this->gps_second = nmea.getSecond();
  this->gps_hundredths = nmea.getHundredths();

  this->datetime = this->dt_string_from_gps();

  this->lat = String((float)nmea.getLatitude()/1000000, 7);
  this->lon = String((float)nmea.getLongitude()/1000000, 7);
  long alt = 0;
  if (!nmea.getAltitude(alt)){
    alt = 0;
  }
  this->altf = (float)alt / 1000;

  //nmea.clear();
}

void GpsInterface::showGPSInfo() {
  Serial.print("Valid fix: ");
  Serial.println(nmea.isValid() ? "yes" : "no");

  Serial.print("Nav. system: ");
  if (nmea.getNavSystem())
    Serial.println(nmea.getNavSystem());
  else
    Serial.println("none");

  Serial.print("Num. satellites: ");
  Serial.println(nmea.getNumSatellites());

  Serial.print("HDOP: ");
  Serial.println(nmea.getHDOP() / 10., 1);

  Serial.print("Date/time: ");
  Serial.print(nmea.getYear());
  Serial.print('-');
  Serial.print(int(nmea.getMonth()));
  Serial.print('-');
  Serial.print(int(nmea.getDay()));
  Serial.print('T');
  Serial.print(int(nmea.getHour()));
  Serial.print(':');
  Serial.print(int(nmea.getMinute()));
  Serial.print(':');
  Serial.println(int(nmea.getSecond()));

  long latitude_mdeg = nmea.getLatitude();
  long longitude_mdeg = nmea.getLongitude();
  Serial.print("Latitude (deg): ");
  Serial.println(latitude_mdeg / 1000000., 6);

  Serial.print("Longitude (deg): ");
  Serial.println(longitude_mdeg / 1000000., 6);

  long alt;
  Serial.print("Altitude (m): ");
  if (nmea.getAltitude(alt))
    Serial.println(alt / 1000., 3);
  else
    Serial.println("not available");

  Serial.print("Speed: ");
  Serial.println(nmea.getSpeed() / 1000., 3);
  Serial.print("Course: ");
  Serial.println(nmea.getCourse() / 1000., 3);

  Serial.println("-----------------------");
  nmea.clear();
}

String GpsInterface::getNumSatsString() {
  return (String)num_sats;
}

bool GpsInterface::getFixStatus() {
  return this->good_fix;
}

bool GpsInterface::getGpsModuleStatus() {
  return this->gps_enabled;
}

void GpsInterface::main() {
  while (Serial2.available()) {
    //Fetch the character one by one
    char c = Serial2.read();
    //Serial.print(c);
    //Pass the character to the library
    nmea.process(c);
  }

  uint8_t num_sat = nmea.getNumSatellites();

  if ((nmea.isValid()) && (num_sat > 0)) {
    //this->good_fix = true;
    //this->num_sats = nmea.getNumSatellites();
    this->setGPSInfo();
    //Serial.print("Got fix: ");
    //Serial.println(num_sats);
    //this->showGPSInfo();
  }
  else if ((!nmea.isValid()) && (num_sat <= 0)) {
    //this->good_fix = false;
    //this->num_sats = nmea.getNumSatellites();
    this->setGPSInfo();
    //Serial.println("No fix");
  }
}
#endif