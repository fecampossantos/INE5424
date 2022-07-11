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

Thread * thread_list[8];

OStream cout;

int calculate(int i);

int main()
{
    cout << "Running multithread test with " << CPU::cores() << " cores" << endl;
    cout << endl;

    for (auto i = 0; i < N_THREADS; i++) {
        cout << "Creating thread #" << i << endl;
        thread_list[i] = new Thread(&calculate);
    }

    // Each time one of the words is swapped from a queue to another
    // a WRN is raised, we kept warnings ON for test purposes
    for (auto i = 0; i < N_THREADS; i++) {
        int core = thread_list[i]->join();
        cout << "Thread " << i << " returned from core " << core << " with priority " << thread_list[i]->criterion() << endl;
    }

    return 0;
}

// Worker with dummy operations that take long enough to be preempted
int calculate() {
    int j = 0;

    int core = CPU::id();

    for (auto i = 0; i < 10000000; i++) {
        j++;
    }

    return core;
}