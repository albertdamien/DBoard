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

#include <Encoder.h>

// state value of quadratic-encoder state machine
#define STATE_00    0
#define STATE_10    1
#define STATE_11    2
#define STATE_12    3
#define STATE_21    4
#define STATE_22    5
#define STATE_31    6
#define STATE_32    7

Encoder::Encoder(int _pinA, int _pinB) :
  m_pinA(_pinA),
  m_pinB(_pinB),
  m_state(STATE_00) {
  // si _pin est digital alors pinMode sinon rien
} // constructor

void Encoder::setup() {
    pinMode(m_pinA, INPUT_PULLUP);
    pinMode(m_pinB, INPUT_PULLUP);
    wait(0);
} // setup

void Encoder::run(serviceType _serv) {
    byte prevValue = m_value;
    m_value = (digitalRead(m_pinA) << 1) + digitalRead(m_pinB);

    if (prevValue != m_value) {
        switch(m_state) {
            case STATE_00:
                if (m_value == 0) {
                    m_state = STATE_10;
                } else if (m_value == 1) {
                    m_state = STATE_12;
                } else {
                    m_state = STATE_11;
                }
                break;
            case STATE_10:
                if (m_value == 1) {
                    m_state = STATE_31;
                } else if (m_value == 2) {
                    m_state = STATE_32;
                } else {
                    m_state = STATE_00;
                }
                break;
            case STATE_11:
                if (m_value == 0) {
                    m_state = STATE_21;
                } else if (m_value == 1) {
                    m_state = STATE_31;
                } else {
                    m_state = STATE_00;
                }
                break;
            case STATE_12:
                if (m_value == 0) {
                    m_state = STATE_22;
                } else if (m_value == 2) {
                    m_state = STATE_32;
                } else {
                    m_state = STATE_00;
                }
                break;
            case STATE_21:
                if (m_value == 1) {
                    m_state = STATE_31;
                } else {
                    m_state = STATE_00;
                }
                break;
            case STATE_22:
                if (m_value  == 2)
                    m_state = STATE_32;
                else {
                    m_state = STATE_00;  
                }
                break;
            case STATE_31:
                if (m_value  == 3) event(EVENT_ENC_CW);
                m_state = STATE_00;
                break;
            case STATE_32:
                if (m_value  == 3) event(EVENT_ENC_CCW);
                m_state = STATE_00;
                break;
        }
    }

    // waiting next run loop
    if (m_state == STATE_00 && prevValue == 3) 
        wait(10);   // wait an action on knob
    else 
        wait(0);    // fast reading the next A & B pins value
} // run