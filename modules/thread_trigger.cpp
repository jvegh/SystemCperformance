//BEGIN next_trigger.cpp
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <systemc>
#include "thread_trigger.h"
using namespace sc_core;
using namespace std;
//#define ToPrint
void thread_trigger_example::my_waiting_method(void) {
    while(true)
    {
        count++;
#ifdef ToPrint
        cout << "INFO: " << name() << " Entering my_wait_method with STATE" << my_state
             << " with count = " << count
             << " at time " << sc_time_stamp() << "."
             << std::endl;
#endif
        switch (my_state) {
          case STATE1:
            // Wait 1 ns similar to Verilog #1
            if (count == 2) { // visit this state twice
              count = 0;
              my_state = STATE2;
            }//endif
            wait(1,SC_NS);
            break;
          case STATE2:
            // Wait 0 ns similar to Verilog #0 wait for next delta cycle
            if (count == 5) { // visit this state three times
              count = 0;
              my_state = STATE3;
            }//endif
            wait(SC_ZERO_TIME);
            break;
          case STATE3:
            // Wait 20 milliseconds
            if (count == 2) { // visit this state twice
              count = 0;
              my_state = DONE;
//              sc_stop();
            }//endif
            wait(20,SC_MS);
        default:
            break;
        }//endswitch
        if(my_state == DONE)
        {
            #ifdef ToPrint
                    cout << "INFO:  Iter# "  << mRepeat << " iters @" <<  sc_time_stamp() << "."
                         << std::endl;
            #endif
            my_state = STATE1;
            if(--mRepeat)
                TriggerEvent.notify(SC_ZERO_TIME);
            if(mRepeat)
                wait(TriggerEvent);
            else
            {
//#ifdef ToPrint
                if(mRepeat)
            cout <<  "INFO:  Finished WAIT method with " << mRepeat << " iters @"
                 << sc_time_stamp() << "."
                  << std::endl;
//#endif
                if (not sc_end_of_simulation_invoked()) sc_stop(); //< invoke end_of_simulation
            }

         }
    }
}//end my_waiting_method();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//END $Id$
