#ifndef NEXT_TRIGGER_H
#define NEXT_TRIGGER_H
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// DESCRIPTION
//   This example resulted out of a request on http://www.systemc.org's
//   help forum.
//
//   Q: I want to know how wait a delay of one delta cycle for an
//      SC_METHOD in SystemC, compatible to #1 of verilog.
//
//   A: You asked for two different things (as expressed in an
//      SC_THREAD):
//
//   1. wait(0,SC_NS); // equivalent to Verilog #0 wait of one delta
//      cycle
//   2. wait(1,SC_NS); // equivalent to Verilog #1
//
//   Of course wait() is not allowed in SC_METHOD.
//   
//   First a few caveats:
//   
//   CAVEAT 1:
//   I am concerned on reading your original statement that you may be
//   attempting to translate an often misunderstood Verilog construct into
//   SystemC. #1 is sometimes used in Verilog always block with
//   non-blocking assignments in manners dangerous to the health of the
//   project.
//   
//   CAVEAT 2:
//   SystemC is not a great language to use for modeling RTL. The RTL
//   capabilities of SystemC are good for co-simulating with automatically
//   generated RTL (either output of a synthesis tool or translator from
//   Verilog). SystemC is for higher levels of abstraction. You should
//   never expect to get faster simulation from RTL expressed in SystemC.
//   
//   CAVEAT 3:
//   If you really need to do this you should probably use an SC_THREAD,
//   but perhaps the following will help.
//   
//   HOW TO DO IT:
//   The following will be appearing slightly revised in our new book on
//   SystemC from Kluwer-Academic next Spring 2004...
//
// EXPLANATION
//
//   SC_METHOD's are not allowed to call wait or suspend mid-way in the
//   manner an SC_THREAD can. An SC_METHOD process is atomic from a
//   time/simulation sense, and must complete (i.e. exit via a return
//   statement) before time is allowed to proceed. 
//  
//   Note that because an SC_METHOD starts fresh on each invocation, it
//   has no state, so you must use class data members in your sc_module
//   class to maintain state information. Local variables are lost
//   between invocations.
//
//   The next_trigger() method provides a mechanism for an SC_METHOD
//   to dynamically change it's sensitivity list. next_trigger() uses
//   all the syntax of wait(), and behaves as if the actual process of
//   "waiting" occurs after the method exits. By using a state variable
//   member in the module class, a method may gain the effect of a wait
//   as illustrated in the following example.
//
//   One potentially confusing and/or useful aspect of next_trigger() is
//   that it may be called more than once before exiting an SC_METHOD. In
//   this situation (NOT illustrated below), the LAST call to next_trigger()
//   replaces all the others.
//  
//   See the animated presentation on the web at the following location:
//   <http://www.eklectically.com/Library/Tutorials/SimulationEngine-v1.0.htm>
//   for an example of how SystemC simulation engine works. Must be
//   registered to view this <<http://www.eklectically.com>.
//   Register for free!
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <systemc>
using namespace sc_core;

SC_MODULE(next_trigger_example)
{
  enum possible_states {STATE1=1, STATE2, STATE3, DONE};
  possible_states my_state;
  int count,mRepeat;       // Used to make things more interesting
  sc_core::sc_event  TriggerEvent;
  
  SC_CTOR(next_trigger_example)
  : my_state(STATE1)
  , count(0) 
  {
      mRepeat = 100;
    SC_METHOD(my_waiting_method);
        sensitive << TriggerEvent;
//        dont_initialize();
  }//end SC_CTOR
  void Initialize(void)
  {
      TriggerEvent.notify(SC_ZERO_TIME);
  }

  void my_waiting_method(void);
};//end SC_MODULE(next_trigger_example)

#endif
