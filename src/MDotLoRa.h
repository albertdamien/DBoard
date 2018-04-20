/*
  Copyright (c) 2018 Damien ALBERT <damien@rouge-orange.fr>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef _MDOT_LORA_H_
#define _MDOT_LORA_H_

#include <Arduino.h>
#include <Component.h>
#include <AltSoftSerial.h>
#include <math.h>

#define EVENT_MDOT_INIT              0
#define EVENT_MDOT_NOT_FOUND         1
#define EVENT_MDOT_FOUND             2
#define EVENT_MDOT_IDENTIFIED        3
#define EVENT_MDOT_CONFIGURED        4
#define EVENT_MDOT_CONNECT_TRYING    5
#define EVENT_MDOT_CONNECT_SUCCESS   6
#define EVENT_MDOT_CONNECT_FAILURE   7
#define EVENT_MDOT_CONNECT_WAITING   8
#define EVENT_MDOT_SEND_READY        9
#define EVENT_MDOT_SEND_ATTEMPT     10
#define EVENT_MDOT_SEND_DELAYED     11
#define EVENT_MDOT_SEND_DISCONNECT  12
#define EVENT_MDOT_SEND_TOO_BIG     13
#define EVENT_MDOT_SEND_ON_AIR      14
#define EVENT_MDOT_LORA_SNR_RECV    15
#define EVENT_MDOT_LORA_RSSI_RECV   16

class MDotLoRa : public Component {
    public:
        MDotLoRa();
        ~MDotLoRa();
        void setup();
        void run(serviceType); 
        bool isReady();         // is the module ready to send message ?
        bool send(char*);       // send a string message
        bool sendb(byte*, int); // send a binary message
        void reset();           // reset the module
        void adr(bool);         // change the Adaptative Data Rate
        String id();            // Device ID
        float rssi();           // Signal Strength
        float snr();            // Signal to Noise Ratio
        
    private:
        String read();                  // read a String from the mDot device
        AltSoftSerial   m_serial;       // the serial comm. line
        int             m_state;        // store the state of the LoRa mDot machine
        int             m_retry;        // store the occurence of retry (AT module detection)
        bool            m_listen;       // is the state machine listening ?
        bool            m_echo;         // is the command echoed back ?
        bool            m_ready;        // is the module ready to send a message ?
        bool            m_joined;       // is the module joined to the gateway ?
        bool            m_adr;          // is adaptative data rate activated ?
        String          m_deviceEui;    // the device EUI-64 address
        String          m_rssi;         // the signal strength indicator
        String          m_snr;          // the signal to noise ratio value
};

#endif // _MDOT_LORA_H_