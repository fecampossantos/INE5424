// Hardware Performance Monitor (HPM)

#ifndef __rv64_pmu_h
#define __rv64_pmu_h

#include <architecture/cpu.h>
#define __common_only__
#include <architecture/pmu.h>
#undef __common_only__

__BEGIN_SYS

class RV64_PMU: public PMU_Common
{

private:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg64 Reg64;

protected:
    static const unsigned int CHANNELS = 29;
    static const unsigned int FIXED = 0;
    static const unsigned int EVENTS = 34;

public:
        
    RV64_PMU() {};

    // Machine Hardware Performance Monitor Event Register

    // Instruction Commit Events, mhpmeventX[7:0] = 0
    enum {
        EXCEPTION_TAKEN                                 = 8,
        INTEGER_LOAD_INSTRUCTION_RETIRED                = 9,
        INTEGER_STORE_INSTRUCTION_RETIRED               = 10,
        ATOMIC_MEMORY_OPERATION_RETIRED                 = 11,
        SYSTEM_INSTRUCTION_RETIRED                      = 12,
        INTEGER_ARITHMETIC_INSTRUCTION_RETIRED          = 13,
        CONDITIONAL_BRANCH_RETIRED                      = 14,
        JAL_INSTRUCTION_RETIRED                         = 15,
        JALR_INSTRUCTION_RETIRED                        = 16,
        INTEGER_MULTIPLICATION_INSTRUCTION_RETIRED      = 17,
        INTEGER_DIVISION_INSTRUCTION_RETIRED            = 18,
        FLOATING_POINT_LOAD_INSTRUCTION_RETIRED         = 19,
        FLOATING_POINT_STORE_INSTRUCTION_RETIRED        = 20,
        FLOATING_POINT_ADDITION_RETIRED                 = 21,
        FLOATING_POINT_MULTIPLICATION_RETIRED           = 22,
        FLOATING_POINT_FUSED_MULTIPLY_ADD_RETIRED       = 23,
        FLOATING_POINT_DIVISION_OR_SQUARE_ROOT_RETIRED  = 24,
        OTHER_FLOATING_POINT_INSTRUCTION_RETIRED        = 25
    };

    // Microarchitectural Events , mhpmeventX[7:0] = 1
    enum {
        LOAD_USE_INTERLOCK                  = 8,
        LONG_LATENCY_INTERLOCK              = 9,
        CSR_READ_INTERLOCK                  = 10,
        INSTRUCTION_CACHE_ITIM_BUSY         = 11,
        DATA_CACHE_DTIM_BUSY                = 12,
        BRANCH_DIRECTION_MISPREDICTION      = 13,
        BRANCH_JUMP_TARGET_MISPREDICTION    = 14,
        PIPELINE_FLUSH_FROM_CSR_WRITE       = 15,
        PIPELINE_FLUSH_FROM_OTHER_EVENT     = 16,
        INTEGER_MULTIPLICATION_INTERLOCK    = 17,
        FLOATING_POINT_INTERLOCK            = 18
    };

    // Memory System Events, mhpmeventX[7:0] = 2
    enum {
        INSTRUCTION_CACHE_MISS                      = 8,
        DATA_CACHE_MISS_OR_MEMORY_MAPPED_IO_ACCESS  = 9,
        DATA_CACHE_WRITEBACK                        = 10,
        INSTRUCTION_TLB_MISS                        = 11,
        DATA_TLB_MISS                               = 12
    };

    // mhpmevent registers
    enum {
        MHPMEVENT3   = 3,
        MHPMEVENT4   = 4,
        MHPMEVENT5   = 5,
        MHPMEVENT6   = 6,
        MHPMEVENT7   = 7,
        MHPMEVENT8   = 8,
        MHPMEVENT9   = 9,
        MHPMEVENT10  = 10,
        MHPMEVENT11  = 11,
        MHPMEVENT12  = 12,
        MHPMEVENT13  = 13,
        MHPMEVENT14  = 14,
        MHPMEVENT15  = 15,
        MHPMEVENT16  = 16,
        MHPMEVENT17  = 17,
        MHPMEVENT18  = 18,
        MHPMEVENT19  = 19,
        MHPMEVENT20  = 20,
        MHPMEVENT21  = 21,
        MHPMEVENT22  = 22,
        MHPMEVENT23  = 23,
        MHPMEVENT24  = 24,
        MHPMEVENT25  = 25,
        MHPMEVENT26  = 26,
        MHPMEVENT27  = 27,
        MHPMEVENT28  = 28,
        MHPMEVENT29  = 29,
        MHPMEVENT30  = 30,
        MHPMEVENT31  = 31
    };

    // mhpmcounter registers
    enum {
        MHPMCOUNTER3   = 3,
        MHPMCOUNTER4   = 4,
        MHPMCOUNTER5   = 5,
        MHPMCOUNTER6   = 6,
        MHPMCOUNTER7   = 7,
        MHPMCOUNTER8   = 8,
        MHPMCOUNTER9   = 9,
        MHPMCOUNTER10   = 10,
        MHPMCOUNTER11   = 11,
        MHPMCOUNTER12   = 12,
        MHPMCOUNTER13   = 13,
        MHPMCOUNTER14   = 14,
        MHPMCOUNTER15   = 15,
        MHPMCOUNTER16   = 16,
        MHPMCOUNTER17   = 17,
        MHPMCOUNTER18   = 18,
        MHPMCOUNTER19   = 19,
        MHPMCOUNTER20   = 20,
        MHPMCOUNTER21   = 21,
        MHPMCOUNTER22   = 22,
        MHPMCOUNTER23   = 23,
        MHPMCOUNTER24   = 24,
        MHPMCOUNTER25   = 25,
        MHPMCOUNTER26   = 26,
        MHPMCOUNTER27   = 27,
        MHPMCOUNTER28   = 28,
        MHPMCOUNTER29   = 29,
        MHPMCOUNTER30   = 30,
        MHPMCOUNTER31   = 31
    };

    // Event classes
    enum {
        INSTRUCTION_COMMIT  = 0,
        MICROARCHITECTURAL  = 1,
        MEMORY_SYSTEM       = 2
    };
   
    // Useful bits 
    enum {
        EVENT_CLASS = 0xFF,
        MIN_CHANNEL     = 3,
        MAX_CHANNEL     = 31
    };
    
    static void config(Channel channel, Event event, Flags flags = NONE) {
        
        db<PMU>(TRC) << "PMU::config(c=" << channel << ",e=" << event << ",f=" << flags << ")" << endl;

        // Event = offset [31-8] | class [7-0]

        Reg8 event_class = (event & EVENT_CLASS);

        assert( event_class == INSTRUCTION_COMMIT || 
                event_class == MICROARCHITECTURAL ||
                event_class == MEMORY_SYSTEM);
        
        assert(channel >= MIN_CHANNEL && channel <= MAX_CHANNEL);

        _monitored_events[channel - MIN_CHANNEL] = event;
        start(channel);
    }
    
    static void start(Channel channel) {
        db<PMU>(TRC) << "PMU::start(c=" << channel << ")" << endl;
        
        assert(channel >= MIN_CHANNEL && channel <= MAX_CHANNEL);
        mhpmevent(channel, _monitored_events[channel - MIN_CHANNEL]);
    }

    static void stop(Channel channel) {
        db<PMU>(TRC) << "PMU::stop(c=" << channel << ")" << endl;

        assert(channel >= MIN_CHANNEL && channel <= MAX_CHANNEL);
        mhpmevent(channel, 0);
    }

    static Count read(Channel channel) { 
        db<PMU>(TRC) << "PMU::read(c=" << channel << ")" << endl;

        return mhpmcounter(channel);
    }
    
    static void write(Channel channel, Count count) {
        db<PMU>(TRC) << "PMU::write(ch=" << channel << ",ct=" << count << ")" << endl;

        mhpmcounter(channel, count);
    }

    static void reset(Channel channel) {
        db<PMU>(TRC) << "PMU::reset(c=" << channel << ")" << endl;

        write(channel, 0);
    }

    static Reg64 minstret() {
        Reg64 reg = 0;
        
        ASM(R"(
            csrr    %0, minstret
        )": "=r"(reg));

        return reg;
    }

    static Reg64 mcycle() {
        Reg64 reg = 0;
        
        ASM(R"(
            csrr    %0, mcycle
        )": "=r"(reg));

        return reg;
    }
    
    static void init();

private:

    static void mhpmevent(Channel channel, Reg64 value) {
        // HPM includes 29 channels mhpmevent3–mhpmevent31

        switch (channel) {
            case MHPMEVENT3:
                ASM(R"(csrw    mhpmevent3, %0)": : "r"(value));
                break;
            case MHPMEVENT4:
                ASM(R"(csrw    mhpmevent4, %0)": : "r"(value));
                break;
            case MHPMEVENT5:
                ASM(R"(csrw    mhpmevent5, %0)": : "r"(value));
                break;
            case MHPMEVENT6:
                ASM(R"(csrw    mhpmevent6, %0)": : "r"(value));
                break;
            case MHPMEVENT7:
                ASM(R"(csrw    mhpmevent7, %0)": : "r"(value));
                break;
            case MHPMEVENT8:
                ASM(R"(csrw    mhpmevent8, %0)": : "r"(value));
                break;
            case MHPMEVENT9:
                ASM(R"(csrw    mhpmevent9, %0)": : "r"(value));
                break;
            case MHPMEVENT10:
                ASM(R"(csrw    mhpmevent10, %0)": : "r"(value));
                break;
            case MHPMEVENT11:
                ASM(R"(csrw    mhpmevent11, %0)": : "r"(value));
                break;
            case MHPMEVENT12:
                ASM(R"(csrw    mhpmevent12, %0)": : "r"(value));
                break;
            case MHPMEVENT13:
                ASM(R"(csrw    mhpmevent13, %0)": : "r"(value));
                break;
            case MHPMEVENT14:
                ASM(R"(csrw    mhpmevent14, %0)": : "r"(value));
                break;
            case MHPMEVENT15:
                ASM(R"(csrw    mhpmevent15, %0)": : "r"(value));
                break;
            case MHPMEVENT16:
                ASM(R"(csrw    mhpmevent16, %0)": : "r"(value));
                break;
            case MHPMEVENT17:
                ASM(R"(csrw    mhpmevent17, %0)": : "r"(value));
                break;
            case MHPMEVENT18:
                ASM(R"(csrw    mhpmevent18, %0)": : "r"(value));
                break;
            case MHPMEVENT19:
                ASM(R"(csrw    mhpmevent19, %0)": : "r"(value));
                break;
            case MHPMEVENT20:
                ASM(R"(csrw    mhpmevent20, %0)": : "r"(value));
                break;
            case MHPMEVENT21:
                ASM(R"(csrw    mhpmevent21, %0)": : "r"(value));
                break;
            case MHPMEVENT22:
                ASM(R"(csrw    mhpmevent22, %0)": : "r"(value));
                break;
            case MHPMEVENT23:
                ASM(R"(csrw    mhpmevent23, %0)": : "r"(value));
                break;
            case MHPMEVENT24:
                ASM(R"(csrw    mhpmevent24, %0)": : "r"(value));
                break;
            case MHPMEVENT25:
                ASM(R"(csrw    mhpmevent25, %0)": : "r"(value));
                break;
            case MHPMEVENT26:
                ASM(R"(csrw    mhpmevent26, %0)": : "r"(value));
                break;
            case MHPMEVENT27:
                ASM(R"(csrw    mhpmevent27, %0)": : "r"(value));
                break;
            case MHPMEVENT28:
                ASM(R"(csrw    mhpmevent28, %0)": : "r"(value));
                break;
            case MHPMEVENT29:
                ASM(R"(csrw    mhpmevent29, %0)": : "r"(value));
                break;
            case MHPMEVENT30:
                ASM(R"(csrw    mhpmevent30, %0)": : "r"(value));
                break;
            case MHPMEVENT31:
                ASM(R"(csrw    mhpmevent31, %0)": : "r"(value));
                break;
        }
    }

    static void mhpmcounter(Channel channel, Count value) {
        // HPM includes 29 counters mhpmcounter3–mhpmcounter31 (one for each channel).

        assert(channel >= MIN_CHANNEL && channel <= MAX_CHANNEL);
        
        switch (channel) {
            case MHPMCOUNTER3:
                ASM(R"(csrw    mhpmcounter3, %0)": : "r"(value));
                break;
            case MHPMCOUNTER4:
                ASM(R"(csrw    mhpmcounter4, %0)": : "r"(value));
                break;
            case MHPMCOUNTER5:
                ASM(R"(csrw    mhpmcounter5, %0)": : "r"(value));
                break;
            case MHPMCOUNTER6:
                ASM(R"(csrw    mhpmcounter6, %0)": : "r"(value));
                break;
            case MHPMCOUNTER7:
                ASM(R"(csrw    mhpmcounter7, %0)": : "r"(value));
                break;
            case MHPMCOUNTER8:
                ASM(R"(csrw    mhpmcounter8, %0)": : "r"(value));
                break;
            case MHPMCOUNTER9:
                ASM(R"(csrw    mhpmcounter9, %0)": : "r"(value));
                break;
            case MHPMCOUNTER10:
                ASM(R"(csrw    mhpmcounter10, %0)": : "r"(value));
                break;
            case MHPMCOUNTER11:
                ASM(R"(csrw    mhpmcounter11, %0)": : "r"(value));
                break;
            case MHPMCOUNTER12:
                ASM(R"(csrw    mhpmcounter12, %0)": : "r"(value));
                break;
            case MHPMCOUNTER13:
                ASM(R"(csrw    mhpmcounter13, %0)": : "r"(value));
                break;
            case MHPMCOUNTER14:
                ASM(R"(csrw    mhpmcounter14, %0)": : "r"(value));
                break;
            case MHPMCOUNTER15:
                ASM(R"(csrw    mhpmcounter15, %0)": : "r"(value));
                break;
            case MHPMCOUNTER16:
                ASM(R"(csrw    mhpmcounter16, %0)": : "r"(value));
                break;
            case MHPMCOUNTER17:
                ASM(R"(csrw    mhpmcounter17, %0)": : "r"(value));
                break;
            case MHPMCOUNTER18:
                ASM(R"(csrw    mhpmcounter18, %0)": : "r"(value));
                break;
            case MHPMCOUNTER19:
                ASM(R"(csrw    mhpmcounter19, %0)": : "r"(value));
                break;
            case MHPMCOUNTER20:
                ASM(R"(csrw    mhpmcounter20, %0)": : "r"(value));
                break;
            case MHPMCOUNTER21:
                ASM(R"(csrw    mhpmcounter21, %0)": : "r"(value));
                break;
            case MHPMCOUNTER22:
                ASM(R"(csrw    mhpmcounter22, %0)": : "r"(value));
                break;
            case MHPMCOUNTER23:
                ASM(R"(csrw    mhpmcounter23, %0)": : "r"(value));
                break;
            case MHPMCOUNTER24:
                ASM(R"(csrw    mhpmcounter24, %0)": : "r"(value));
                break;
            case MHPMCOUNTER25:
                ASM(R"(csrw    mhpmcounter25, %0)": : "r"(value));
                break;
            case MHPMCOUNTER26:
                ASM(R"(csrw    mhpmcounter26, %0)": : "r"(value));
                break;
            case MHPMCOUNTER27:
                ASM(R"(csrw    mhpmcounter27, %0)": : "r"(value));
                break;
            case MHPMCOUNTER28:
                ASM(R"(csrw    mhpmcounter28, %0)": : "r"(value));
                break;
            case MHPMCOUNTER29:
                ASM(R"(csrw    mhpmcounter29, %0)": : "r"(value));
                break;
            case MHPMCOUNTER30:
                ASM(R"(csrw    mhpmcounter30, %0)": : "r"(value));
                break;
            case MHPMCOUNTER31:
                ASM(R"(csrw    mhpmcounter31, %0)": : "r"(value));
                break;
        }
    }
    
    static Count mhpmcounter(Channel channel) {
        // HPM includes 29 counters mhpmcounter3–mhpmcounter31 (one for each channel).

        assert(channel >= MIN_CHANNEL && channel <= MAX_CHANNEL);

        Count value = 0;
        
        switch (channel) {
            case MHPMCOUNTER3:
                ASM(R"(csrr   %0, mhpmcounter3)": "=r"(value));
                break;
            case MHPMCOUNTER4:
                ASM(R"(csrr   %0, mhpmcounter4)": "=r"(value));
                break;
            case MHPMCOUNTER5:
                ASM(R"(csrr   %0, mhpmcounter5)": "=r"(value));
                break;
            case MHPMCOUNTER6:
                ASM(R"(csrr   %0, mhpmcounter6)": "=r"(value));
                break;
            case MHPMCOUNTER7:
                ASM(R"(csrr   %0, mhpmcounter7)": "=r"(value));
                break;
            case MHPMCOUNTER8:
                ASM(R"(csrr   %0, mhpmcounter8)": "=r"(value));
                break;
            case MHPMCOUNTER9:
                ASM(R"(csrr   %0, mhpmcounter9)": "=r"(value));
                break;
            case MHPMCOUNTER10:
                ASM(R"(csrr   %0, mhpmcounter10)": "=r"(value));
                break;
            case MHPMCOUNTER11:
                ASM(R"(csrr   %0, mhpmcounter11)": "=r"(value));
                break;
            case MHPMCOUNTER12:
                ASM(R"(csrr   %0, mhpmcounter12)": "=r"(value));
                break;
            case MHPMCOUNTER13:
                ASM(R"(csrr   %0, mhpmcounter13)": "=r"(value));
                break;
            case MHPMCOUNTER14:
                ASM(R"(csrr   %0, mhpmcounter14)": "=r"(value));
                break;
            case MHPMCOUNTER15:
                ASM(R"(csrr   %0, mhpmcounter15)": "=r"(value));
                break;
            case MHPMCOUNTER16:
                ASM(R"(csrr   %0, mhpmcounter16)": "=r"(value));
                break;
            case MHPMCOUNTER17:
                ASM(R"(csrr   %0, mhpmcounter17)": "=r"(value));
                break;
            case MHPMCOUNTER18:
                ASM(R"(csrr   %0, mhpmcounter18)": "=r"(value));
                break;
            case MHPMCOUNTER19:
                ASM(R"(csrr   %0, mhpmcounter19)": "=r"(value));
                break;
            case MHPMCOUNTER20:
                ASM(R"(csrr   %0, mhpmcounter20)": "=r"(value));
                break;
            case MHPMCOUNTER21:
                ASM(R"(csrr   %0, mhpmcounter21)": "=r"(value));
                break;
            case MHPMCOUNTER22:
                ASM(R"(csrr   %0, mhpmcounter22)": "=r"(value));
                break;
            case MHPMCOUNTER23:
                ASM(R"(csrr   %0, mhpmcounter23)": "=r"(value));
                break;
            case MHPMCOUNTER24:
                ASM(R"(csrr   %0, mhpmcounter24)": "=r"(value));
                break;
            case MHPMCOUNTER25:
                ASM(R"(csrr   %0, mhpmcounter25)": "=r"(value));
                break;
            case MHPMCOUNTER26:
                ASM(R"(csrr   %0, mhpmcounter26)": "=r"(value));
                break;
            case MHPMCOUNTER27:
                ASM(R"(csrr   %0, mhpmcounter27)": "=r"(value));
                break;
            case MHPMCOUNTER28:
                ASM(R"(csrr   %0, mhpmcounter28)": "=r"(value));
                break;
            case MHPMCOUNTER29:
                ASM(R"(csrr   %0, mhpmcounter29)": "=r"(value));
                break;
            case MHPMCOUNTER30:
                ASM(R"(csrr   %0, mhpmcounter30)": "=r"(value));
                break;
            case MHPMCOUNTER31:
                ASM(R"(csrr   %0, mhpmcounter31)": "=r"(value));
                break;
        }

        return value;
    }

private:
    static Event _monitored_events[CHANNELS]; 
    static const Event _events[EVENTS]; 

};

class PMU: private RV64_PMU
{
    friend class CPU;

private:
    typedef RV64_PMU Engine;

public:
    using Engine::CHANNELS;
    using Engine::FIXED;
    using Engine::EVENTS;

    using Engine::Event;
    using Engine::Count;
    using Engine::Channel;

public:
    PMU() {}

    using Engine::config;
    using Engine::read;
    using Engine::write;
    using Engine::start;
    using Engine::stop;
    using Engine::reset;
    
    using Engine::mcycle;
    using Engine::minstret;

private:
    static void init() { Engine::init(); }
};


__END_SYS

#endif
