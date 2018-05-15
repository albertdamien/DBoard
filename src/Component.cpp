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

#include <Component.h>
#include <DBoard.h>

Component::Component() : 
    m_handler(NULL) {
    // nothing left to write here
} // constructor

void Component::plug(onEventHandler _handler) {
    if (_handler) m_handler = _handler;
    setup();
} // plug

void Component::loop() {
    DBoard::Uno().run();
} // loop

void Component::wait(unsigned long _delay) {
    DBoard::Uno().wait(this, _delay);
} // wait

void Component::wait(unsigned long _delay, byte _serv) {
    DBoard::Uno().wait(this, _serv, _delay);
} // wait

void Component::leave() {
    DBoard::Uno().leave(this);
} // leave

void Component::leave(byte _serv) {
    DBoard::Uno().leave(this, _serv);
} // leave

void Component::event(eventType _event) {
  if (m_handler) m_handler(_event);
} // event