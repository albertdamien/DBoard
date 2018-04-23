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

#include <Relay.h>

Relay::Relay(int _pin) :
    m_relayPin(_pin) {
    // nothing left to write here
} // constructor

Relay::~Relay() {
    digitalWrite(m_relayPin, LOW);
} // destructor

void Relay::setup() {
    pinMode(m_relayPin, OUTPUT);
    digitalWrite(m_relayPin, LOW);
} // setup

void Relay::run(byte _serv) {
    ; // nothing here
} // run

void Relay::open() {
    digitalWrite(m_relayPin, LOW);
} // open

void Relay::close() {
    digitalWrite(m_relayPin, HIGH);
} // close