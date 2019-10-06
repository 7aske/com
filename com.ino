#define CLCK 3
#define INP 4
#define OUT 5

int in_val = 0;
volatile byte in_byte = 0;
volatile uint8_t bit_pos = 0;

uint8_t last_bit_timeout = 100;

void setup()
{
	pinMode(INP, INPUT);
	pinMode(CLCK, INPUT);
	pinMode(OUT, OUTPUT);
	attachInterrupt(digitalPinToInterrupt(CLCK), onClock, RISING);
	Serial.begin(9600);
}

void loop()
{
	if (last_bit_timeout > 0)
	{
		last_bit_timeout--;
	}
	else
	{
		if (bit_pos > 0)
		{
			Serial.write((char)10);
		}
		in_byte = 0;
		bit_pos = 0;
	}
	delay(1);
}

void onClock()
{
	digitalWrite(OUT, 0);
	in_val = digitalRead(INP);
	Serial.print(in_val);
	digitalWrite(OUT, in_val);
	if (bit_pos == 8)
	{
		in_byte = 0;
		bit_pos = 0;
	}

	if (in_val)
	{
		in_byte |= (0x80 >> bit_pos);
	}

	bit_pos++;
	if (bit_pos == 8)
	{
		Serial.print((char)32);
		Serial.print((char)in_byte);
		if (in_byte != 10)
			Serial.print((char)10);
	}
	last_bit_timeout = 100;
}
