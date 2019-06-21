//BEGIN next_trigger.cpp
// essentially the next_trigger example, made repeatable
// Altogether makes 10 iterations and repeat 100 times,
// i.e. altogether 1000 SystemC action are done on the object

#include <systemc>
#include "next_trigger.h"
using namespace sc_core;
using namespace std;
//#define ToPrint
void next_trigger_example::my_waiting_method(void) {
   count++;
#ifdef ToPrint
  cout << "INFO: " << name() << " Entering my_trigger_method with STATE" << my_state
       << " with count = " << count
       << " at time " << sc_time_stamp() << "." 
       << std::endl;
#endif
  switch (my_state) {
    case STATE1:
      // Wait 1 ns similar to Verilog #1
      next_trigger(1,SC_NS);
      if (count == 2) { // visit this state twice
        count = 0;
        my_state = STATE2;
      }//endif
      break;
    case STATE2:
      // Wait 0 ns similar to Verilog #0 wait for next delta cycle
      next_trigger(SC_ZERO_TIME);
      if (count == 5) { // visit this state five times
        count = 0;
        my_state = STATE3;
      }//endif
      break;
    case STATE3:
      // Wait 20 milliseconds
      next_trigger(20,SC_MS);
      if (count == 2) { // visit this state twice
        count = 0;
        my_state = DONE;
      };//endif
  default:
      break;
  }//endswitch
#ifdef ToPrint
  cout << "INFO:  Leaving TRIGGER_method with STATE" << my_state
       << " with count = " << count
       << " at time " << sc_time_stamp() << "."
       << std::endl;
#endif
  if(my_state==DONE)
  {
      my_state = STATE1;
      if(--mRepeat)
      {
          next_trigger(TriggerEvent); // Be ready again to repeat
      }
      if(mRepeat)
      {
          TriggerEvent.notify(SC_ZERO_TIME);
#ifdef ToPrint
          cout <<  "INFO: iter#" << mRepeat << std::endl;
#endif
      }
      else
      {
//#ifdef ToPrint
          if(mRepeat)
      cout <<  "INFO:  Finished NEXT_TRIGGER method with " << mRepeat << " iters @"
       << sc_time_stamp() << "."
            << std::endl;
//#endif
          if (not sc_end_of_simulation_invoked()) sc_stop(); //< invoke end_of_simulation
      }
  }
  return;
}//end my_waiting_method();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//END $Id$
