#ifndef WAITNS_H
#define WAITNS_H

// wait a few clock cycles
#define NOP __asm__ __volatile__ ("nop\n\t") // ~ 4 ns = one clock cycle
#define WAIT_10_NS NOP; NOP; // 2 NOPS
#define WAIT_20_NS NOP; NOP; NOP; NOP; // 4 NOPS
#define WAIT_40_NS WAIT_20_NS; WAIT_20_NS; // 8 NOPS
#define WAIT_50_NS WAIT_20_NS; WAIT_20_NS; WAIT_10_NS; // 10 NOPS
#define WAIT_60_NS WAIT_20_NS; WAIT_20_NS; WAIT_20_NS;
#define WAIT_80_NS WAIT_40_NS; WAIT_40_NS;
#define WAIT_100_NS WAIT_80_NS; WAIT_20_NS; // 20 NOPS
#define WAIT_120_NS WAIT_100_NS; WAIT_20_NS; // 24 NOPS
#define WAIT_150_NS WAIT_100_NS; WAIT_50_NS; // 30 NOPS
#define WAIT_200_NS WAIT_100_NS WAIT_100_NS;

#endif