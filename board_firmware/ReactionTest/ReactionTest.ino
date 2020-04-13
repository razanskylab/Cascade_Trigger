#ifndef OUTPUT_PIN
	#define OUTPUT_PIN 10
#endif

#ifndef INPUT_PIN
	#define INPUT_PIN 16
#endif

void setup(){
	pinMode(OUTPUT_PIN, OUTPUT);
	pinMode(INPUT_PIN, INPUT);
	pinMode(LED_BUILTIN, OUTPUT);
}

bool status = 0;

void loop(){
	status = digitalReadFast(INPUT_PIN);
	digitalWriteFast(OUTPUT_PIN, status);
	digitalWriteFast(LED_BUILTIN, status);
}