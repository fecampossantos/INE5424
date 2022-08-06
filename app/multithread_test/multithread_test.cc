#include <utility/ostream.h>
#include <architecture/rv64/rv64_cpu.h>
#include <machine/riscv/riscv_timer.h>
#include <machine/riscv/riscv_ic.h>

#include <machine/display.h>
#include <time.h>
#include <synchronizer.h>
#include <process.h>

using namespace EPOS;

#define N_THREADS 4

Thread *thread_list[N_THREADS];

OStream cout;

int calculate(int i);

int main()
{
  cout << "Running multithread test with " << CPU::cores() << " cores" << endl;
  cout << endl;

  for (auto i = 0; i < N_THREADS; i++)
  {
    cout << "Creating thread #" << i << endl;
    // creates thread with function that runs enough to be preempted
    thread_list[i] = new Thread(&calculate, i);
  }

  for (auto i = 0; i < N_THREADS; i++)
  {
    int core = thread_list[i]->join();
    cout << "Thread " << i << " ran on core " << core << "." << endl;
  }

  return 0;
}

int calculate(int i)
{
    int j = 0;

    for (auto i = 0; i < 10000000; i++) {
        j = j +1;
    }

    int core = CPU::id();
    return core;
}
