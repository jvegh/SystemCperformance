#include <systemc>
#include "thread_trigger.h"
#include "next_trigger.h"

using namespace sc_core; 
using namespace std;

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <chrono>
#include <cstring>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
int ContextSwitches;
//https://stackoverflow.com/questions/5120861/how-to-measure-memory-usage-from-inside-a-c-program
int RAM0 = 0;
int REPEAT;
bool ActionWait;

auto Measend = chrono::steady_clock::now();
auto Measbegin = chrono::steady_clock::now();

class Performance
{
public:
   Performance(void);
   ~Performance(void);
   void Measure(void);
   void Print(bool Final);
   int mRAM;
protected:
   int mContextSwitches,mPreviousCS;
   uint64_t mFreemem, mMeasduration, mSduration, mUduration;
   rusage mUsage;
   struct sysinfo mSysInfo;
   struct timespec mts1, mtw1; // both C11 and POSIX
   struct timeval mutime,mstime;
   double mSetupTime,mSimulationTime;
   double mWallTime;
};

Performance::Performance(void)
{
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &mts1); // POSIX
    Measend = chrono::steady_clock::now();
    getrusage(RUSAGE_SELF, &mUsage);
    sysinfo(&mSysInfo);
    mPreviousCS = mUsage.ru_nivcsw;
}
Performance::~Performance(void)
{

}
    void Performance::
Measure(void)
{
        // Meas timing with chrono
    Measend = chrono::steady_clock::now();
    mMeasduration = chrono::duration_cast<chrono::nanoseconds>(Measend - Measbegin).count();
    Measbegin  = Measend;
       // Wall-clock timing with gettime
    struct timespec ts2;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2); // POSIX
    mWallTime = 1000.0*ts2.tv_sec + 1e-6*ts2.tv_nsec
                         - (1000.0*mts1.tv_sec + 1e-6*mts1.tv_nsec);
    mts1 = ts2;
    // Meas Linux system time and user time
    getrusage(RUSAGE_SELF, &mUsage);
    sysinfo(&mSysInfo);
    mUduration = 1e6*mUsage.ru_utime.tv_sec+mUsage.ru_utime.tv_usec
            - 1e6*mutime.tv_sec-mutime.tv_usec;
    mSduration = 1e6*mUsage.ru_stime.tv_sec+mUsage.ru_stime.tv_usec
            - 1e6*mstime.tv_sec-mstime.tv_usec;
    mutime = mUsage.ru_utime;
    mstime = mUsage.ru_stime;
    // Meas context switches
    mContextSwitches = mUsage.ru_nivcsw-mPreviousCS;
    mPreviousCS = mUsage.ru_nivcsw;
    mRAM = ((uint64_t) mSysInfo.freeram*mSysInfo.mem_unit)/1024;
}

    void Performance::
Print(bool Final)
{
    if(!Final)
    { // It is just the end of elaboration, no printing takes place
        mSetupTime = mMeasduration/1000./1000;
        return;
    }
    // We are expected to print at the end of simulation
    mSimulationTime = mMeasduration/1000./1000;
    float iActionTime = mSetupTime;
    float ActionTime = mSimulationTime;
    float TotalAction = mSetupTime + mSimulationTime;
    if(REPEAT)
    {
        iActionTime /= REPEAT;
        ActionTime /= REPEAT;
        TotalAction /= REPEAT;
    }
    else
    {
     printf("This is a base time, with zero repeat\n");
    }

    // Identify which method is used
    if(ActionWait)
        printf("Method 'wait'");
    else
        printf("Method 'next_trigger'");

     printf(", times in msec\n");
    // Print one-line head
    printf("REPEAT,  Tsetup,  Tsimul,   Twall,   Tuser, Tsystem,  iMAPS,  MAPS,tMAPS, RAM(kb),  CSw,\n");
    int RAM = RAM0 - mRAM;
                 printf(
         " %5d,%8.3f,%8.3f,%8.3f,%8.3f"
                             ",%8.3f,%6.3f,%6.3f,%6.3f"
                             ",%8d,%5d,\n",
         REPEAT, // Number of "modules"
         mSetupTime, // Time of elaboration, binding, etc.
         mSimulationTime, // by chrono
         mWallTime,      //posix_wall,
         mUduration/1e3,    // user time by Linux
         mSduration/1e3,    // System time by Linux
         1/iActionTime, // Time of one initial/elaboration action
         1/ActionTime, // Time of one action
         1/TotalAction,   // Elaboration+simulation
         RAM,   // free RAM
         mContextSwitches // Number of non-voluntary CS
         ) ;
                 cout << "INFO: Stopped simulating at " << sc_time_stamp() << "." << std::endl;
}

void
CheckArgumentList(int argc, char** argv)
{
if(argc < 3)
{
  cerr << " Correct usage:\n" << argv[0] << " {NEXT/WAIT}  Repeat#" << endl; // Print out a kind of help, no logging yet
  exit (EXIT_FAILURE);
}
// OK, we have all 3 args
// Check if the repetition# is correct
 char* pEnd;
 REPEAT = strtol(argv[2], &pEnd, 10);
 if(REPEAT<0 || REPEAT>50000)
 {
   std::cerr << "No of repetitions is "<< REPEAT <<", it must be between 0 and " << 40000;
   exit (EXIT_FAILURE);
 }
 if(string(argv[1])==string("NEXT"))
    {
    ActionWait = false;
    }
    else if(string(argv[1])==string("WAIT"))
    {
     ActionWait = true;
    }
    else
    {
     std::cerr << "The action can be  {NEXT/WAIT} only " << endl;
     exit (EXIT_FAILURE);
    }
}

int sc_main(int argc,char *argv[]) 
{
    char MyBuffer[30];
    Performance Perf;
    CheckArgumentList(argc, argv);	//?? There must be 3 args
    vector <next_trigger_example*> MyNextVector;
    vector <thread_trigger_example*> MyWaitVector;
    Perf.Measure();
    RAM0 = Perf.mRAM;
    for(int i = 0; i<REPEAT; i++)
    {
        sprintf(MyBuffer,"%d",i);
        if(ActionWait)
        {
            MyWaitVector.push_back(new thread_trigger_example((string("thread_example")+string(MyBuffer)).c_str()));
        }
        else
        {
            MyNextVector.push_back(new next_trigger_example((string("method_example")+string(MyBuffer)).c_str()));
        }
    }
    Perf.Measure();
    Perf.Print(false);    // Just remember elaboration time
    Perf.Measure();
    sc_start(36000,SC_SEC); // Simulate for 10 hours
    Perf.Measure();
    Perf.Print(true);
    return 0;
}//end sc_main()
