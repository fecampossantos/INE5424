#include <machine.h>
#include <time.h>
#include <process.h>
#include <synchronizer.h>

using namespace EPOS;

const int ITERATIONS = 50;
const int CPU_WORK_ITERATIONS = 100000000;

OStream cout;

int one() {
    for (int i = 0; i < ITERATIONS; i++) {
        cout << CPU::id() << " 1" << endl;

        for (int j = 0; j < CPU_WORK_ITERATIONS; j++) {
        }
    }

    cout << "UM!" << endl;
    return 0;
}

int two() {
    for (int i = 0; i < ITERATIONS; i++) {
        cout << CPU::id() << " 2" << endl;

        for (int j = 0; j < CPU_WORK_ITERATIONS; j++) {
        }
    }
    cout << "DOIS!" << endl;
    return 0;
}

int three() {
    for (int i = 0; i < ITERATIONS; i++) {
        cout << CPU::id() << " 3" << endl;

        for (int j = 0; j < CPU_WORK_ITERATIONS; j++) {
        }
    }

    cout << "TRES!" << endl;
    return 0;
}

int main() {
    new Thread(&one);
    new Thread(&two);
    new Thread(&three);

    Thread::self()->pass();


    cout << "fim da main" << endl;

    return 0;
}
