// EPOS Application Initializer

#include <architecture.h>
#include <utility/heap.h>
#include <machine.h>
#include <system.h>

extern "C" char _end; // defined by GCC

__BEGIN_SYS

class Init_Flash
{
private:

    static const unsigned int FLASH_BASE = Traits<Machine>::FLASH_BASE;
    static const unsigned int FLASH_TOP = Traits<Machine>::FLASH_TOP;
    static const unsigned int HEAP_SIZE = FLASH_TOP - FLASH_BASE;

public:
    Init_Flash() {
        Flash::_heap = new (&Flash::_preheap[0]) Heap((void *)FLASH_BASE, HEAP_SIZE);
    }
};

// Global object "init_Flash"  must be linked to the Flash (not to the system) and there constructed at first.
Init_Flash init_flash;

__END_SYS
