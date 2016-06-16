#include "Arduino.h"

// nrf24_reliable_datagram_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging client
// with the RHReliableDatagram class, using the RH_NRF24 driver to control a NRF24 radio.
// It is designed to work with the other example nrf24_reliable_datagram_server
// Tested on Uno with Sparkfun WRL-00691 NRF24L01 module
// Tested on Teensy with Sparkfun WRL-00691 NRF24L01 module
// Tested on Anarduino Mini (http://www.anarduino.com/mini/) with RFM73 module
// Tested on Arduino Mega with Sparkfun WRL-00691 NRF25L01 module

#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#include "keys.h"

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Singleton instance of the radio driver
//RH_NRF24 driver;
RH_NRF24 nrf24(7, 8);   // For RFM73 on Anarduino Mini

// uint8_t data[RH_NRF24_MAX_MESSAGE_LEN];

// Dont put this on the stack:
// uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

#define BUTTON_COUNT 8

uint8_t buttonPin[] = { 4, A4, A5, 5, A1, A0, A3, A2 };
int buttonState[]   = { 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long lastUpdate[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

uint8_t buttonMap[] = {
  KEY_UP_ARROW,
  KEY_DOWN_ARROW,
  KEY_LEFT_ARROW,
  KEY_RIGHT_ARROW,
  'd',
  'f',
  KEY_BACKSPACE,
  KEY_RETURN
};

uint8_t buttonUpdate[2];

void setup()
{
  // data[0] = '\0';
  Serial.begin(115200);

  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }

  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}

void loop()
{
  for (int i = 0; i < BUTTON_COUNT; i++) {
    int btn = (digitalRead(buttonPin[i]) == 1) ? 0 : 1;
    if(btn != buttonState[i] || (millis() - lastUpdate[i]) > 10) {
      buttonState[i] = btn;

      buttonUpdate[0] = buttonMap[i];
      buttonUpdate[1] = '0' + buttonState[i];

      nrf24.send(buttonUpdate, sizeof(buttonUpdate));
      nrf24.waitPacketSent();
      lastUpdate[i] = millis();
    }
  }
}
