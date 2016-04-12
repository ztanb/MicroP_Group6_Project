The variables that the board will transmit to the phone are:
int16_t AccPitch;	-- Filtered Pitch angle in 100ths of a degree
int16_t AccRoll;	-- Filtered Roll angle in 100ths of a degree
int16_t Temperature;	-- Filtered Temperture sensor data in 10ths of a degree
uint8_t LEDControl;	-- Its value will determine the LED pattern
uint8_t DoubleTap;	-- Not sure how to implement it yet

If you are manipulating them in a new file, simply declare them as extern at the beginning of the file (might work in header too not sure)