#ifndef INPUT_PIN
	#define INPUT_PIN 16
#endif

// input pin is number 16

void setup(){
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(INPUT_PIN, INPUT);
};

bool status = 0;

void loop(){
	status = digitalReadFast(INPUT_PIN);
	digitalWriteFast(LED_BUILTIN, status);
};