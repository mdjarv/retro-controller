#include "Arduino.h"

// nrf24_reliable_datagram_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging server
// with the RHReliableDatagram class, using the RH_NRF24 driver to control a NRF24 radio.
// It is designed to work with the other example nrf24_reliable_datagram_client
// Tested on Uno with Sparkfun WRL-00691 NRF24L01 module
// Tested on Teensy with Sparkfun WRL-00691 NRF24L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <RH_NRF24.h>
#include <SPI.h>

#include "Keyboard.h"

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Singleton instance of the radio driver
//RH_NRF24 driver;
RH_NRF24 nrf24(9, 10);   // For RFM73 on Anarduino Mini

void setup()
{
  Serial.begin(115200);
  Keyboard.begin();

  if (!nrf24.init())
    Serial.println("init failed");
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}

uint8_t buttonUpdate[2];

void loop()
{
  if (nrf24.available())
  {
    uint8_t len = sizeof(buttonUpdate);
    if (nrf24.recv(buttonUpdate, &len))
    {
      if(buttonUpdate[1] == '1') {
        Keyboard.press(buttonUpdate[0]);
      } else {
        Keyboard.release(buttonUpdate[0]);
      }
    }
  }
}
