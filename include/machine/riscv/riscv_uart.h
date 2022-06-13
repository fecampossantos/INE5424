// EPOS RISC-V UART Mediator Declarations

#ifndef __riscv_uart_h
#define __riscv_uart_h

#include <architecture/cpu.h>
#include <machine/uart.h>
#include <system/memory_map.h>

__BEGIN_SYS

class UART: private UART_Common
{
private:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg32 Reg32;

    static const unsigned int UNIT = Traits<UART>::DEF_UNIT;
    static const unsigned int BAUD_RATE = Traits<UART>::DEF_BAUD_RATE;
    static const unsigned int DATA_BITS = Traits<UART>::DEF_DATA_BITS;
    static const unsigned int PARITY = Traits<UART>::DEF_PARITY;
    static const unsigned int STOP_BITS = Traits<UART>::DEF_STOP_BITS;

    // UART registers offsets from UART_BASE
    enum {
        TXDATA  = 0x00,
        RXDATA  = 0x04,
        TXCTRL  = 0x08,
        RXCTRL  = 0x0c,
        IE      = 0x10,
        IP      = 0x14,
        DIV     = 0x18  // f(baud) = f(in) / (DIV + 1)
    };

        TXDATA          = 0x0,
        RXDATA          = 0x04,
        
        TXCTRL          = 0x08,
        RXCTRL          = 0x0C,

        IE              = 0x10,
        IP              = 0x14,

        DIV             = 0x18
    };

public:
    using UART_Common::NONE;
    using UART_Common::EVEN;
    using UART_Common::ODD;

public:
    UART(unsigned int unit = UNIT, unsigned int baud_rate = BAUD_RATE, unsigned int data_bits = DATA_BITS, unsigned int parity = PARITY, unsigned int stop_bits = STOP_BITS) {
        config(baud_rate, data_bits, parity, stop_bits);
    }

    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        
        // Activating channels
        reg(TXCTRL) |= (0b010 << 15) | 0b1;
        reg(RXCTRL) |= 0b1;

        // Enabling txwm
        int_enable();

        Reg32 br = Traits<UART>::CLOCK / (BAUD_RATE);
        if (Traits<UART>::CLOCK / br > BAUD_RATE) {
            br += 1;
        }

        // get ceiling
        Reg8 div_least = Reg8(br & 0xff);
        Reg8 div_most = Reg8(br >> 8);

        reg(DIV) = ((div_most << 8) | div_least);

        // Buffer for dequeuing
        _buffer = 0;
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        *baud_rate = Traits<UART>::CLOCK / (reg(DIV) & 0xFFFF);
    }

    void buffer(unsigned int uart_register) {
        _buffer = reg(uart_register);
    }

    Reg8 rxd() { return (_buffer & 0xFF); }

    void txd(Reg8 c) { reg(TXDATA) = c; }

    bool rxd_empty() { buffer(RXDATA); return (_buffer >> 31); }
    
    bool txd_full() { buffer(TXDATA); return (_buffer >> 31); }

    bool busy() { return false; /* not applicable */ }

    char get() { 
        while(rxd_empty()); 
        return rxd(); 
    }

    void put(char c) { 
        while(txd_full()); 
        txd(c);
    }

    int read(char * data, unsigned int max_size) {
        for (unsigned int i = 0; i < max_size; i++)
            data[i] = get();

        return max_size;
    }

    int write(const char * data, unsigned int size) {
        for (unsigned int i = 0; i < size; i++)
            put(data[i]);
        
        return size;
    }

    void flush() { while(!(reg(IP) & 0b1)); }

    bool ready_to_get() { return !rxd_empty(); }

    bool ready_to_put() { return !txd_full(); }

    void int_enable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        reg(IE) = 0b01;
    }

    void int_disable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        reg(IE) = 0;
    }

    void loopback(bool flag) {}

    void power(const Power_Mode & mode) {}

private:
    static void init() {}

    static volatile CPU::Reg32 & reg(unsigned int uart_register) { 
        return reinterpret_cast<volatile CPU::Reg32 *>(Memory_Map::UART_BASE)[uart_register / sizeof(CPU::Reg32)];
    }

    Reg32 _buffer;
};

__END_SYS

#endif
