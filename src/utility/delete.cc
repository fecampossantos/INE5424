// EPOS Dynamic Memory Allocators Implementation

#include <system.h>

using namespace EPOS;

// C++ dynamic memory deallocators
void operator delete(void * object) {
    if (object < (void*) Traits<Machine>::FLASH_BASE)
        return free(object);
    else
        return free(object, FLASH);
}

void operator delete[](void * object) {
    if (object < (void*) Traits<Machine>::FLASH_BASE)
        return free(object);
    else
        return free(object, FLASH);
}

void operator delete(void * object, size_t bytes) {
    if (object < (void*) Traits<Machine>::FLASH_BASE)
        return free(object);
    else
        return free(object, FLASH);
}

void operator delete[](void * object, size_t bytes) {
    if (object < (void*) Traits<Machine>::FLASH_BASE)
        return free(object);
    else
        return free(object, FLASH);
}
