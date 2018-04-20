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

#ifndef _DBOARD_H_
#define _DBOARD_H_

#include <Arduino.h>
#include <Component.h>
#include <HeartbeatLed.h>

// Time type definition
typedef unsigned long millisType;

// Task definition
struct Task {
    Task*         m_next; // next Task address (16 bits)
    millisType    m_term; // launch term in ms (32 bits)
    Component*    m_comp; // Component address (16 bits)
    serviceType   m_serv; // Service number     (8 bits)
};

class DBoard {
    public:
                            DBoard();                                  // default constructor
                            ~DBoard();                                 // default destructor
        static DBoard&      Uno();                                     // singleton member method
        void                run();                                     // main scheduling loop 
        void                wait(Component*, serviceType, millisType); // start a new Task and append it to queue (w/ service)
        void                wait(Component*, millisType);              // start a new Task and append it to queue (w/o service)
        void                leave(Component*, serviceType);            // pull out the Task from the queue (w/ service)
        void                leave(Component*);                         // pull out the Task from the queue (w/o service)
        int                 waitings();                                // return the number of Tasks
        void                print();                                   // print the entire List of Task
        void                health();                                  // print health informations (RAM/Tasks)
                  
    private:                  
        int                 freeMemory();                              // retrieve the free SRAM
        bool                isOlder(Task*, Task*);                     // is the first Task older than the second one   
        Task*               m_currentTask;                             // the current Task in run() method
        Task*               m_tasks;                                   // scheduled Tasks of Components
        HeartbeatLed*       m_heartbeat;                               // heartbeat led indicator
        millisType          m_monitorMillis;                           // date of the last monitor report
        millisType          m_monitorDelay;                            // delay between each report
        const millisType    m_halfwayMillis = 2147483647;              // this is halfway to the max millis value 
};

#endif // _DBOARD_H_