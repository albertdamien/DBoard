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

#include <DBoard.h>
#include <avr/wdt.h>

void yield(void) {
    DBoard::Uno().run();
} // yield

DBoard::DBoard() :
    m_currentTask(NULL),
    m_tasks(NULL),
    m_monitorMillis(millis()),
    m_monitorDelay(10000) {

    // wait for serial port to connect. Needed for native USB
    Serial.begin(115200);
    while (!Serial);

    // welcome string
    Serial.println(F("[DBoard] Boot up d|board v1.0.0"));

    // attach the incorporated component
    m_heartbeat = new HeartbeatLed(LED_BUILTIN, 30, 100, 2500);
    m_heartbeat->plug();
    // enable the watchdog timer
    Serial.println(F("[DBoard] WatchDog Timer enabled"));
    wdt_enable(WDTO_1S); // 1 sec timeout before reset
} // constructor

DBoard::~DBoard() {
    Task* _current(m_tasks);

    while (_current) {
        m_tasks = _current;
        _current = _current->m_next;
        delete(m_tasks);
    }
    m_tasks = NULL;
} // destructor

DBoard& DBoard::Uno() {
    static DBoard instance; // singleton instance
    return instance;
} // Uno

void DBoard::run() {
    // Reset Watch dog Timer otherwise a watchdog-initiated device reset will occur
    wdt_reset();

    // Launch scheduled elapsed Task
    if (m_tasks && ((m_tasks->m_term <= millis()) \
                || ((m_tasks->m_term >= m_halfwayMillis) && (millis() < m_halfwayMillis)))) {   // handle millis() rollover
        if (m_currentTask) { // reentrance of run() caused by delay() usage
            if (m_tasks != m_currentTask && ((millis() - m_tasks->m_term) > 5)) {
                Serial.print(F("[DBoard] Overrunning task 0x"));
                Serial.println((unsigned int)m_currentTask, HEX);
            } else {
                // in the current Task, nothing to do now
            }
        } else {
            if ((millis() - m_tasks->m_term) > 20) {
                Serial.print(F("[DBoard] Task 0x"));
                Serial.print((unsigned int)m_tasks, HEX);
                Serial.print(F(" running too late by "));
                Serial.print(millis() - m_tasks->m_term);
                Serial.println(F(" ms"));
            }
            m_currentTask = m_tasks;            // store the Task address
            m_tasks = m_tasks->m_next;          // move to the next Task

            m_currentTask->m_comp->run(m_currentTask->m_serv); // run the Task here

            delete(m_currentTask);              // and delete it
            m_currentTask = NULL;               // before clearing the address
        }
    }
    //  Monitor health each 10s
    if (millis() - m_monitorMillis > m_monitorDelay) {
        m_monitorMillis = millis();
        health();
        //print();
    }
} // run

void DBoard::wait(Component* _comp, byte _serv, unsigned long _delay) {
    Task* _current(m_tasks);

    // create a new Task and fills it
    Task *_tmp = new Task();
    if (_delay == 0) _delay++; // 1 ms is the minimum to avoid corruption in the Task list
    _tmp->m_term =  millis() + _delay;  // now + delay = term
    _tmp->m_comp = _comp;
    _tmp->m_serv = _serv;

    if (NULL == m_tasks) {
        m_tasks = _tmp;  // first Task
    } else { // add the Task respectfully to earliest deadline first scheduling
        while ((NULL != _current->m_next) && isOlder(_tmp, _current->m_next)) {
            _current = _current->m_next;
        }
        if (isOlder(_tmp, _current)) {
            _tmp->m_next = _current->m_next;
            _current->m_next = _tmp;
        } else {
            _tmp->m_next = m_tasks;
            m_tasks = _tmp;
        }
    }
} // wait

void DBoard::wait(Component* _comp, unsigned long _delay) {
    wait(_comp, 0, _delay);
} //wait

void DBoard::leave(Component* _comp, byte _serv) {
    Task* _current(m_tasks);
    Task* _target(NULL);

    if (_current) {
        if ((_current->m_comp == _comp) && (_current->m_serv == _serv)) {
                m_tasks = _current->m_next;
                delete(_current);           // delete first Task
                return true;                // the single Task is matching
        } else {
            if (_current->m_next) {
                _target = _current->m_next;

                while ((_target->m_comp != _comp) || (_target->m_serv != _serv)) {
                    _current = _target;
                    _target = _target->m_next;
                    if (NULL == _target) return false; // _comp not found in the Tasks list
                } // while
                _current->m_next = _target->m_next;
                delete(_target);
                return true;        // found _comp in the Tasks list
            } else return false;    // the single Task is not matching
        }
    } else return false; // empty list
} // leave

void DBoard::leave(Component* _comp) {
    leave(_comp, 0);
} //leave

int DBoard::waitings() {
    int _size(0);
    Task* _current(m_tasks);

    while (_current) {
        _size++;
        _current = _current->m_next;
    }
    return _size;
} // waitings

void DBoard::print() {
    Task* _current(m_tasks);
    int nth(0);
    char buffer[20];
    Serial.print(F("#### Tasks list ("));
    Serial.print(waitings());
    Serial.print(F(" tasks @"));
    Serial.print(millis());
    Serial.println(F(" ms) ####"));
    while (_current) {
        sprintf(buffer, "%010lu|%#p:%x",_current->m_term, _current->m_comp, _current->m_serv);
        Serial.println(String(buffer));
        _current = _current->m_next;
    } // while
    Serial.println();
} // print

void DBoard::health() {
    m_heartbeat->beat();
    Serial.print(F("[DBoard] "));
    Serial.print(freeMemory());
    Serial.print(F("B free | "));
    Serial.print(waitings());
    Serial.print(F(" tasks | "));
    Serial.print(millis());
    Serial.println(F(" ms"));
} // health

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

int DBoard::freeMemory() {
    int free_memory;

    if((int)__brkval == 0)
        free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
        free_memory = ((int)&free_memory) - ((int)__brkval);

    return free_memory;
} // freeMemory

bool DBoard::isOlder(Task* _t1, Task* _t2) {
    return ((_t1->m_term < m_halfwayMillis) && (_t2->m_term > m_halfwayMillis)) ? true : (_t1->m_term > _t2->m_term); // handle millis() rollover
} // isOlder
