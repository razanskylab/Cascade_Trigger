#ifndef WAITNS_H
#define WAITNS_H

// wait a few clock cycles
#define NOP __asm__ __volatile__ ("nop\n\t") // ~6 ns = one clock cycle
#define WAIT_10_NS NOP; NOP;
#define WAIT_20_NS NOP; NOP; NOP; NOP;
#define WAIT_40_NS WAIT_20_NS; WAIT_20_NS;
#define WAIT_60_NS WAIT_20_NS; WAIT_20_NS; WAIT_20_NS;
#define WAIT_80_NS WAIT_40_NS; WAIT_40_NS;
#define WAIT_100_NS WAIT_80_NS; WAIT_20_NS;

#endif