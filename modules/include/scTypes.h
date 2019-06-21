/*!  @file scTypes.h
 * @brief Contains type definitions in SystemC terms
 *
 *  @bug No known bugs.
 */
#ifndef SC_TYPES_H
#define SC_TYPES_H

#include <iostream>
#include <systemc>
#include "Config.h"
using namespace std;
using namespace sc_core;
using namespace sc_dt;


#define SCMODE       // Comment to suppress messages of form DEBUG_PRINT(x)
#for debugging, one can use 'normal' types
#ifndef SCMODE
// The word type used by the SystemC model
typedef int SC_WORD_TYPE;
#else
// The word type used by the SystemC model
typedef sc_dt::sc_uint<WORD_SIZE> SC_WORD_TYPE;

#endif

#endif // SC_TYPES_H

