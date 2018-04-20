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

#include <Timer.h>

Timer::Timer() :
    m_interval(0),
    m_repeat(false) {
    // nothing left to write here
} // constructor

void Timer::setup() {
    ;
} // setup

void Timer::run(serviceType _serv) {
    event(EVENT_TMR_ALARM);         // launch the alarm event handler
    if (m_repeat) wait(m_interval); // rearm the alarm if in repeat mode
} // run

void Timer::set(unsigned long _alarm, bool _m_repeat) {
    m_interval = _alarm;
    m_repeat = _m_repeat;
    reset();
} // set

void Timer::reset() {
    leave();            // abandons the previous possible task
    wait(m_interval);   // launch a new Task with the correct time
} // reset

void Timer::set(unsigned long _alarm) {
    set(_alarm, false);
} // set