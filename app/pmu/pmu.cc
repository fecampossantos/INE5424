#include <utility/ostream.h>
#include <architecture.h>
#include <time.h>

using namespace EPOS;

OStream cout;

void statistics()
{
  cout << "Unhalted CPU cycles: " << PMU::read(0)
       << "\nCPU cycles: " << PMU::read(1)
       << "\nInstructions: " << PMU::read(2)
       << "\nExceptions: " << PMU::read(3)
       << "\nInterrupts: " << PMU::read(4) << endl;
}

void twos_mult_table()
{
  int two = 2;
  int result = 0;

  for (int i = 0; i < 100; i++)
  {
    result = result + (two * i);
  }
}

void configure_PMU()
{
  PMU::config(0, PMU::UNHALTED_CYCLES);
  PMU::config(1, PMU::CPU_CYCLES);
  PMU::config(2, PMU::INSTRUCTIONS_RETIRED);
  PMU::config(3, PMU::EXCEPTIONS);
  PMU::config(4, PMU::INTERRUPTS);
}

void stop_pmu()
{
  PMU::stop(0);
  PMU::stop(1);
  PMU::stop(2);
  PMU::stop(3);
  PMU::stop(4);
}

void start_pmu()
{
  PMU::start(0);
  PMU::start(1);
  PMU::start(2);
  PMU::start(3);
  PMU::start(4);
}

void reset_pmu()
{
  PMU::reset(0);
  PMU::reset(1);
  PMU::reset(2);
  PMU::reset(3);
  PMU::reset(4);
}

int main()
{
  cout << "Testing PMU... " << endl;
  cout << "\nStarting stats: " << endl;
  configure_PMU();
  statistics();

  cout << "\nfilling CPU with instructions...";
  twos_mult_table();
  cout << "\nNew stats: " << endl;
  statistics();

  cout << "\nStopping PMU counters and filling CPU again...";
  stop_pmu();
  twos_mult_table();
  cout << "\nNew stats: " << endl;
  statistics();

  cout << "\nStarting counters again...";
  start_pmu();
  twos_mult_table();
  cout << "\nNew stats: " << endl;
  statistics();

  cout << "\nResetting...;"
  reset_pmu();
  cout << "\nFinal stats: " << endl;
  statistics();

  return 0;
}