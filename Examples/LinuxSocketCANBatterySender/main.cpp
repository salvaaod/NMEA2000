/*
  Linux SocketCAN example:
  Sends static Battery Status PGN 127508 once per second on can0.

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

const unsigned long kTransmitMessages[] PROGMEM = {127508L, 0};

void ConfigureDevice() {
  NMEA2000.SetProductInformation(
      "00000001",      // Model serial code
      1300,             // Product code
      "Pi Battery Tx", // Model ID
      "1.0.0",         // Software version
      "1.0.0"          // Model version
  );

  NMEA2000.SetDeviceInformation(
      1,    // Unique number
      170,  // Device function = Battery
      35,   // Device class = Electrical Generation
      2046  // Manufacturer code
  );

  NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly, 22);
  NMEA2000.ExtendTransmitMessages(kTransmitMessages);
  NMEA2000.Open();
}

void SendStaticBatteryStatus() {
  tN2kMsg msg;
  const unsigned char batteryInstance = 0;
  const double voltage = 12.70;       // Volts
  const double current = -3.20;       // Amps (negative = discharging)
  const double temperatureK = 298.15; // 25 C in Kelvin
  const unsigned char sid = 1;

  SetN2kPGN127508(msg, batteryInstance, voltage, current, temperatureK, sid);
  NMEA2000.SendMsg(msg);
}

} // namespace

int main() {
  ConfigureDevice();

  while (true) {
    NMEA2000.ParseMessages();
    SendStaticBatteryStatus();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}
