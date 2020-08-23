#ifndef NOPS_H
#define NOPS_H
	#define NOP __asm__ __volatile__ ("nop\n\t") // ~6 ns = one clock cycle
	#define WAIT_10_NS NOP; NOP;
	#define WAIT_20_NS NOP; NOP; NOP; NOP;
	#define WAIT_40_NS WAIT_20_NS; WAIT_20_NS;
	#define WAIT_60_NS WAIT_20_NS; WAIT_20_NS; WAIT_20_NS;
	#define WAIT_80_NS WAIT_40_NS; WAIT_40_NS;
	#define WAIT_100_NS WAIT_80_NS; WAIT_20_NS;
	#define WAIT_200_NS WAIT_100_NS; WAIT_100_NS;
	#define WAIT_400_NS WAIT_200_NS; WAIT_200_NS;

	#define WAIT_5_NOPS NOP; NOP; NOP; NOP; NOP;
	#define WAIT_10_NOPS WAIT_5_NOPS; WAIT_5_NOPS;
	#define WAIT_50_NOPS WAIT_10_NOPS; WAIT_10_NOPS; WAIT_10_NOPS; WAIT_10_NOPS; WAIT_10_NOPS;
	#define WAIT_100_NOPS WAIT_50_NOPS; WAIT_50_NOPS;
#endif