// EPOS System Initialization

#include <system.h>
#include <time.h>
#include <process.h>

__BEGIN_SYS

void System::init()
{
    // if(Traits<Alarm>::enabled)
    if(Traits<Alarm>::enabled && CPU::id() == 0) // only enables alarm on main cpu
        Alarm::init();

    if(Traits<Thread>::enabled)
        Thread::init();
}

__END_SYS
