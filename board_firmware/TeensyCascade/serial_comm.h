/*
	a little wrapper function to read and send data through serial communication



*/




// uint8 transportation

uint8_t read_uint8()
{
	union{
		uint8_t number;
		byte array[1];
	}	u;
	Serial.readBytes(u.array, 1);
	return u.number;
}

void send_uint8(const uint8_t num2send)
{
	union{
		uint8_t number;
		byte array[1];
	}	u;
	u.number = num2send;
	Serial.write(u.array, 1);
	return;
}

// uint16 transportation

uint16_t read_uint16()
{
	union{
		uint16_t number;
		byte array[2];
	} u;
	Serial.readBytes(u.array, 2);
	return u.number;
}

void send_uint16(const uint16_t num2send)
{
	union{
		uint16_t number;
		byte array[2];
	}	u;
	u.number = num2send;
	Serial.write(u.array, 2);
	return;
}

// uint32 transportations

uint32_t read_uint32()
{
	union{
		uint32_t number;
		byte array[4];
	} u;
	Serial.readBytes(u.array, 4);
	return u.number;
}

void send_uint32(const uint32_t num2send)
{
	union{
		uint32_t number;
		byte array[4];
	} u;
	u.number = num2send;
	Serial.write(u.array, 4);
	return;
}