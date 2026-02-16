/*
  Linux SocketCAN example:
  Sends static Engine Parameters, Rapid Update PGN 127488 once per second on can0.

  Dependencies:
  - This NMEA2000 library
  - NMEA2000_socketCAN library (https://github.com/ttlappalainen/NMEA2000_socketCAN)
*/

#define USE_N2K_CAN USE_N2K_SOCKET_CAN
#define SOCKET_CAN_PORT "can0"

#include <NMEA2000_CAN.h>
#include <N2kMessages.h>

#include <chrono>
#include <thread>

namespace {

const unsigned long kTransmitMessages[] PROGMEM = {127488L, 0};

void ConfigureDevice() {
  NMEA2000.SetProductInformation(
      "00000001",      // Model serial code
      1300,             // Product code
      "Pi Engine Tx",  // Model ID
      "1.0.0",         // Software version
      "1.0.0"          // Model version
  );

  NMEA2000.SetDeviceInformation(
      1,    // Unique number
      150,  // Device function = Engine
      50,   // Device class = Propulsion
      2046  // Manufacturer code
  );

  NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly, 22);
  NMEA2000.ExtendTransmitMessages(kTransmitMessages);
  NMEA2000.Open();
}

void SendStaticEngineInfo() {
  tN2kMsg msg;
  const unsigned char engineInstance = 1;
  const double rpm = 1800.0;
  const double boostPressure = N2kDoubleNA;
  const int8_t tiltTrim = N2kInt8NA;

  SetN2kEngineParamRapid(msg, engineInstance, rpm, boostPressure, tiltTrim);
  NMEA2000.SendMsg(msg);
}

} // namespace

int main() {
  ConfigureDevice();

  while (true) {
    NMEA2000.ParseMessages();
    SendStaticEngineInfo();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}
