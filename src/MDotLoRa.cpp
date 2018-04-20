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

#include <MDotLoRa.h>

// mDot State-Machine 
#define MDOT_ATTENTION          0
#define MDOT_INITIALIZATION     1
#define MDOT_IDENTIFICATION     2
#define MDOT_CONFIGURATION      3
#define MDOT_CONNECTION         4
#define MDOT_EXPECTATION        5
#define MDOT_RECEPTION          6
#define MDOT_RSSI_QUESTION      7
#define MDOT_SNR_QUESTION       8
#define MDOT_ADR_MODIFICATION   9

MDotLoRa::MDotLoRa() : // always pins 8 and 9
    m_serial(),
    m_listen(false),
    m_echo(false),
    m_ready(false),
    m_joined(false),
    m_adr(false),
    m_deviceEui(),
    m_rssi(),
    m_snr(),
    m_state(MDOT_ATTENTION) {
    // nothing left to write here
} // constructor

MDotLoRa::~MDotLoRa() {
    ;
} // destructor

void MDotLoRa::setup() {
    m_serial.begin(9600);
    m_serial.listen();
    wait(1000);  
} // setup

void MDotLoRa::run(serviceType _serv) {
    String str = "";
    switch (m_state) {
        case MDOT_ATTENTION: 
            event(EVENT_MDOT_INIT);
            if (m_listen) {
                str = read();
                if (str.length() == 0) {
                    if (m_retry++ < 100) wait(50);
                    else {
                        Serial.println(F("[ LoRa ] MultiTech mDot module not found"));
                        event(EVENT_MDOT_NOT_FOUND);
                        while (true); // watchdog reset
                    }
                } else if (str.startsWith(F("AT"))) {
                    m_echo = true;
                    wait(10);
                } else if (m_echo && str == "OK") {
                    //Serial.println(F("[ LoRa ] Found AT device on pins 8|9"));
                    m_state = MDOT_INITIALIZATION;
                    m_listen = false;
                    wait(10);
                } else {
                    wait(1000);
                }
            } else {
                m_serial.println(F("AT")); // Attention 
                m_listen = true;
                m_echo = false;
                m_retry = 0;
                wait(10);
            }
            break;
        case MDOT_INITIALIZATION: 
            if (m_listen) {
                str = read();
                if (str.length() == 0) {
                    wait(10);
                } else if (str.startsWith(F("ATI"))) {
                    m_echo = true;
                    wait(10);
                } else if (m_echo && str == "MultiTech mDot") {
                    event(EVENT_MDOT_FOUND); 
                    Serial.println(F("[ LoRa ] Found MultiTech mDot module"));
                    wait(10);
                } else if (m_echo && str == "OK") {
                    m_state = MDOT_IDENTIFICATION;
                    m_listen = false;
                    wait(10);
                } else {
                    wait(100);
                }
            } else {
                m_serial.println(F("ATI")); // Request ID
                m_listen = true;
                m_echo = false;
                wait(10);
            }
            break;
        case MDOT_IDENTIFICATION: 
            if (m_listen) {
                str = read();
                  
                if (str.length() == 0) {
                    wait(10);
                } else if (str.startsWith(F("AT+DI"))) {
                    m_echo = true;
                    wait(10);
                } else if (m_echo && str == "OK") {
                    Serial.print(F("[ LoRa ] Device EUI-64: "));
                    Serial.println(m_deviceEui);
                    m_state = MDOT_CONFIGURATION;
                    m_listen = false;
                    wait(10);
                } else {
                    m_deviceEui += str;
                    m_deviceEui.replace('-', ':');
                    wait(100);
                }
            } else {
                m_serial.println(F("AT+DI")); // Device ID
                m_deviceEui = "";
                m_listen = true;
                m_echo = false;
                wait(10);
            }
            break;
        case MDOT_CONFIGURATION:
            if (m_listen) {
                str = read();
                if (str.length() == 0) {
                    wait(10);
                } else if (str.startsWith(F("AT+NK"))) {
                    Serial.println(F("[ LoRa ] Application Key: a2a1cc7afbd0ce739ac7b7274a77ab40"));
                    m_state = MDOT_CONNECTION;
                    m_listen = false;
                    wait(2000);
                } else {
                    wait(100);
                }
            } else {
                m_serial.println(F("AT+NK=0,a2a1cc7afbd0ce739ac7b7274a77ab40"));
                m_listen = true;
                wait(200);
            }
            break;
        case MDOT_CONNECTION:
            if (m_listen) {
                str = read();
                if (str.length() == 0) {
                    event(EVENT_MDOT_CONNECT_TRYING); 
                    Serial.println(F("[ LoRa ] Connecting ..."));
                    wait(5000);
                } else if (str.startsWith(F("Join backoff")) || str.startsWith(F("No free channel"))) {
                    event(EVENT_MDOT_CONNECT_WAITING); 
                    m_state = MDOT_EXPECTATION;
                    m_listen = false;
                    wait(10);
                } else if (str.startsWith(F("Failed to join network"))) {
                    event(EVENT_MDOT_CONNECT_FAILURE); 
                    Serial.println(F("[ LoRa ] Failed to join network"));
                    m_listen = false;
                    wait(10000);
                } else if (str.startsWith(F("Successfully joined network"))) {
                    event(EVENT_MDOT_CONNECT_SUCCESS);
                    Serial.println(F("[ LoRa ] Connected to LoRa Network !"));
                    event(EVENT_MDOT_SEND_DELAYED);
                    m_state = MDOT_EXPECTATION;
                    m_joined = true;
                    m_listen = false;
                    wait(10);
                } else {
                    wait(100);
                }
            } else {
                m_serial.println(F("AT+JOIN"));
                m_listen = true;
                wait(200);
            }
            break;
        case MDOT_EXPECTATION: 
            if (m_listen) {
                str = read();
                //Serial.print(F("[ LoRa ] Asking time to wait: "));
                //Serial.println(str);
                if (str.length() == 0) {
                    if (m_retry++ > 1000) {
                        while (true); // watchdog reset
                    } else wait(10);
                } else if (str.startsWith(F("AT+TXN"))) {
                    str = read();
                    if (str.toInt() > 0) {
                        Serial.print(F("[ LoRa ] Waiting "));
                        Serial.print(str);
                        Serial.println(F(" ms the next free channel"));
                    }
                    m_echo = true;
                    wait(str.toInt() + 10);
                } else if (m_echo) {
                    m_state = (m_joined) ? MDOT_RECEPTION : MDOT_CONNECTION;
                    m_listen = false;
                    wait(100);
                } else {
                    if (m_retry++ > 1000) {
                        while (true); // watchdog reset
                    } else wait(100);
                }
            } else {
                m_serial.println(F("AT+TXN")); // get time in ms until next free channel
                m_listen = true;
                m_echo = false;
                m_retry = 0;
                wait(200);
            }
            break;
        case MDOT_RECEPTION:
            if (m_listen) {
                event(EVENT_MDOT_SEND_ATTEMPT);
                str = read();
                //Serial.print(F("[ LoRa ] Sending bytes: "));
                //Serial.println(str);
                if (str.length() == 0) {
                    if (m_retry++ > 100) { 
                        while (true); // watchdog reset
                    } else wait(1000);
                } else if (str.startsWith(F("No free channel"))) {
                    event(EVENT_MDOT_SEND_DELAYED);
                    m_state = MDOT_EXPECTATION;
                    m_listen = false;
                    wait(10);
                } else if (str.startsWith(F("Network Not Joined"))) {
                    event(EVENT_MDOT_SEND_DISCONNECT);
                    m_state = MDOT_CONNECTION;
                    m_listen = false;
                    wait(10);
                } else if (str.startsWith(F("Invalid hex string"))) {
                    Serial.println(F("[ LoRa ] Invalid hex string, (hex:242)"));
                    m_state = MDOT_EXPECTATION;
                    m_listen = false;
                    wait(10);
                } else if (str.startsWith(F("Data exceeds datarate max payload"))) {
                    event(EVENT_MDOT_SEND_TOO_BIG);
                    Serial.println(F("[ LoRa ] Data exceeds datarate max payload"));
                    m_state = MDOT_EXPECTATION;
                    m_listen = false;
                    wait(10);
                } else if (str == "OK") {
                    event(EVENT_MDOT_SEND_ON_AIR);
                    Serial.println(F("[ LoRa ] Bytes on air"));
                    m_state = MDOT_RSSI_QUESTION;
                    m_listen = false;
                    wait(10);
                } else {
                    event(EVENT_MDOT_SEND_READY);
                    m_ready = true;
                    m_retry = 0;
                    wait(100);
                }
            } else {
                //Serial.println(F("Listening ..."));
                event(EVENT_MDOT_SEND_READY);
                m_ready = true;
                m_retry = 0;
                wait(1000);
            }
            break;
        case MDOT_RSSI_QUESTION: 
            if (m_listen) {
                str = read();
                //Serial.print(F("[ LoRa ] Asking RSSI: "));
                //Serial.println(str);
                if (str.length() == 0) {
                    if (m_retry++ > 100) { 
                        event(EVENT_MDOT_LORA_RSSI_RECV);
                        m_state = MDOT_SNR_QUESTION;
                        m_listen = false;
                    } 
                    wait(10);
                } else if (str.startsWith(F("AT+RSSI"))) {
                    m_echo = true;
                    wait(10);
                } else if (m_echo && str == "OK") {
                    event(EVENT_MDOT_LORA_RSSI_RECV);
                    m_state = MDOT_SNR_QUESTION;
                    m_listen = false;
                    wait(10);
                } else {
                    m_rssi = str;
                    if (m_retry++ > 100) { 
                        while (true); // watchdog reset
                    } else wait(500);
                }
            } else {
                m_serial.println(F("AT+RSSI")); // Signal Strength 
                m_listen = true;
                m_retry = 0;
                m_echo = false;
                wait(10);
            }
            break;
        case MDOT_SNR_QUESTION: 
            if (m_listen) {
                str = read();
                //Serial.print(F("[ LoRa ] Asking SNR: "));
                //Serial.println(str);
                if (str.length() == 0) {
                    if (m_retry++ > 100) { 
                        event(EVENT_MDOT_LORA_SNR_RECV);
                        event(EVENT_MDOT_SEND_DELAYED);
                        m_state = MDOT_EXPECTATION;
                        m_listen = false;
                    }
                    wait(10);
                } else if (str.startsWith(F("AT+SNR"))) {
                    m_echo = true;
                    wait(10);
                } else if (m_echo && str == "OK") {
                    event(EVENT_MDOT_LORA_SNR_RECV);
                    event(EVENT_MDOT_SEND_DELAYED);
                    m_state = MDOT_EXPECTATION;
                    m_listen = false;
                    wait(10);
                } else {
                    m_snr = str;
                    if (m_retry++ > 100) { 
                        while (true); // watchdog reset
                    } else wait(500);
                }
            } else {
                m_serial.println(F("AT+SNR")); // Signal to Noise Ratio
                m_listen = true;
                m_retry = 0;
                m_echo = false;
                wait(10);
            }
            break;
        case MDOT_ADR_MODIFICATION: 
            if (m_listen) {
                str = read();
                if (str.length() == 0) {
                    wait(10);
                } else if (str.startsWith(F("AT+ADR"))) {
                    m_echo = true;
                    wait(10);
                } else if (m_echo && str == "OK") {
                    event(EVENT_MDOT_SEND_DELAYED);
                    m_state = MDOT_EXPECTATION;
                    m_listen = false;
                    wait(10);
                } else {
                    wait(500);
                }
            } else {
                wait(10);
            } 
            break;
        default:
            wait(1000);
    }
} // run

bool MDotLoRa::send(char* _str) {
    if (m_state == MDOT_RECEPTION && m_ready && !m_listen) {
        m_serial.print(F("AT+SEND=")); // Send
        m_serial.println(_str);
        m_ready = false;
        m_listen = true;
        m_retry = 0;
        return true;
    } else return false;
} // send

bool MDotLoRa::sendb(byte* _tab, int _tabSize) {
    int i;
    char buf[3];
  /*
    Serial.print(F("[ LoRa ] Want to send bytes (state:"));
    Serial.print(m_state);
    Serial.print(F(" ready:"));
    Serial.print(m_ready);
    Serial.print(F(" listen:"));
    Serial.print(m_listen);
    Serial.println(")");
  */
    if (m_state == MDOT_RECEPTION && m_ready && !m_listen) {
        m_serial.print(F("AT+SENDB=")); // Send
        for (i = 0; i < _tabSize; i++) {
            sprintf(buf, "%02x", _tab[i]);
            m_serial.print(String(buf)); 
        }
        m_serial.println();
        m_ready = false;
        m_listen = true;
        m_retry = 0;
        return true;
    } else return false;
} // send

void MDotLoRa::reset() {
    Serial.println(F("[ LoRa ] Reset mDot module"));
    m_serial.println(F("ATZ")); // Reset CPU
    m_state = MDOT_IDENTIFICATION;
    m_joined = false;
    m_ready = false;
    m_listen = false;
} // reset

void MDotLoRa::adr(bool _adr) {
    if (m_ready && !m_listen && (m_adr != _adr)) {
        m_serial.print(F("AT+ADR=")); // Adaptative Data Rate
        m_serial.println(_adr);
        m_state = MDOT_ADR_MODIFICATION;
        m_listen = true;
        m_echo = false;
        m_adr = _adr;
    }
} // adr

String MDotLoRa::id() {
    return m_deviceEui;
} // id

float MDotLoRa::rssi() {
    return (m_rssi) ? m_rssi.substring(0, m_rssi.indexOf(',')).toFloat() : 0.0;
} // rssi

float MDotLoRa::snr() {
    return (m_snr) ? m_snr.substring(0, m_snr.indexOf(',')).toFloat() : 0.0;
} // snr

bool MDotLoRa::isReady() {
    return m_ready;
} // isReady

String MDotLoRa::read() {
    char car;
    String str = "";
    if (m_serial.isListening()) {
        while (m_serial.available() && car != '\n') {
            car = m_serial.read();
            if (isPrintable(car)) str.concat(car);
        } 
        str.trim();
    }
    return str;
} // read