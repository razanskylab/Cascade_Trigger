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

#ifndef OUTPUT_PIN
	#define OUTPUT_PIN 19
#endif

#ifndef INPUT_PIN
	#define INPUT_PIN 16
#endif

void setup(){
	pinMode(OUTPUT_PIN, OUTPUT);
	// pinMode(INPUT_PIN, INPUT);
	// pinMode(LED_BUILTIN, OUTPUT);
}

bool status = 0;

void loop(){
	// status = digitalReadFast(INPUT_PIN);
	digitalWriteFast(OUTPUT_PIN, HIGH);
	WAIT_100_NOPS;
	digitalWriteFast(OUTPUT_PIN, LOW);
	delayMicroseconds(1);
	
	// digitalWriteFast(LED_BUILTIN, status);
}