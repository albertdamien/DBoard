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

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <Arduino.h>

typedef byte serviceType;
typedef byte eventType;

/** Example of event or service type declaration :

  #define EVENT_CHANGE  0
  #define EVENT_STATE_A 1
  ...

**/

typedef void (*onEventHandler)(eventType);

/** Example of event handler function :

  void onEventXYZ(eventType e) {
    switch (e) {
        case EVENT_CHANGE:
            Serial.println(F("Change occured !"));
            break;
        case EVENT_STATE_A:
            Serial.println(F("State A !"));
            break;
        default:
            // other event
            break;
    }
} // onEventXYZ

**/

class Component {
    public:
                        Component();                        // default constructor
        virtual void    setup() = 0;                        // launch during the call of plug() method
        virtual void    run(serviceType) = 0;               // launch the service by the scheduler of DBoard
        static void     loop();                             // to execute into the main Arduino loop() function     
        void            plug(onEventHandler = NULL);        // attach an handler and setup the Component
        void            event(eventType);                   // helper for throwing an event from the Component 
    
    protected:  
        onEventHandler  m_handler;                          // the global event handler
        void            wait(unsigned long, serviceType);   // install a new waiting Task w/ service number
        void            wait(unsigned long);                // install a new waiting Task w/o service
        void            leave(serviceType);                 // give up the waiting Task w/ service
        void            leave();                            // give up the waiting Task w/o service
};

#endif // _COMPONENT_H_