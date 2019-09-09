#ifndef __KBChain_7segment_CPP__
#define __KBChain_7segment_CPP__

#include "kbchain_7segment.h"

KBChain_7segment::KBChain_7segment(int bus_ch, int dev_addr) {
	channel = bus_ch;
	address = dev_addr;
	polling_ms = 100;
}

void KBChain_7segment::init(void) {
	error = false;
	initialized = true;
}

int KBChain_7segment::prop_count(void) {
	// not supported
	return 0;
}

bool KBChain_7segment::prop_name(int index, char *name) {
	// not supported
	return false;
}

bool KBChain_7segment::prop_unit(int index, char *unit) {
	// not supported
	return false;
}

bool KBChain_7segment::prop_attr(int index, char *attr) {
	// not supported
	return false;
}

bool KBChain_7segment::prop_read(int index, char *value) {
	// not supported
	return false;
}

bool KBChain_7segment::prop_write(int index, char *value) {
	// not supported
	return false;
}
// --------------------------------------

void KBChain_7segment::process(Driver *drv) {
	i2c = (I2CDev *)drv;

	switch (state) {
		case s_detect:
			// detect i2c device
			if (i2c->detect(channel, address) == ESP_OK) {
				// clear error flag
				error = false;
				// set initialized flag
				initialized = true;

				i2cWriteByte(0x21);  // turn on OSC
				blinkRate(KBChain_7segment_BLINK_OFF);
			
				setBrightness(15); // Set max brightness

				state = s_runing;
			} else {
				state = s_error;
			}
			break;
		
		case s_runing: {
			if (is_tickcnt_elapsed(this->tickcnt, this->polling_ms)) {
				tickcnt = get_tickcnt();

				if (i2c->detect(channel, address) != ESP_OK) {
					state = s_error;
				}
			}
		}
		
		case s_wait:
			if (error) {
				// wait polling_ms timeout
				if (is_tickcnt_elapsed(this->tickcnt, this->polling_ms)) {
					state = s_detect;
				}
			}
			break;

		case s_error:
			// set error flag
			error = true;
			// clear initialized flag
			initialized = false;
			// get current tickcnt
			tickcnt = get_tickcnt();
			// goto wait and retry with detect state
			state = s_wait;
			break;

	}
}

// Method
bool KBChain_7segment::i2cWriteByte(uint8_t c) {
	uint8_t buff = c;
	return i2c->write(channel, address, &buff, 1) == ESP_OK;
}

void KBChain_7segment::setBrightness(uint8_t b) {
	if (b > 15) b = 15; //
	i2cWriteByte(KBChain_7segment_CMD_BRIGHTNESS | b);
}

void KBChain_7segment::blinkRate(uint8_t b) {
	if (b > 3) b = 0; // turn off if not sure
  
	i2cWriteByte(KBChain_7segment_BLINK_CMD | KBChain_7segment_BLINK_DISPLAYON | (b << 1)); 
}

void KBChain_7segment::writeDisplay(void) {
	uint8_t buffSize = (8 * 2) + 1;
	uint8_t buff[buffSize];
	buff[0] = 0x00;

	uint8_t inx = 1;
	for (uint8_t i=0; i<8; i++) {
		buff[inx++] = displaybuffer[i] & 0xFF;    
		buff[inx++] = displaybuffer[i] >> 8;    
	}
 	i2c->write(channel, address, buff, buffSize);
}

void KBChain_7segment::clear(void) {
  for (uint8_t i=0; i<8; i++) {
    displaybuffer[i] = 0;
  }
}

void KBChain_7segment::print(unsigned long n, int base)
{
  if (base == 0) write(n);
  else printNumber(n, base);
}

void KBChain_7segment::print(char c, int base)
{
  print((long) c, base);
}

void KBChain_7segment::print(unsigned char b, int base)
{
  print((unsigned long) b, base);
}

void KBChain_7segment::print(int n, int base)
{
  print((long) n, base);
}

void KBChain_7segment::print(unsigned int n, int base)
{
  print((unsigned long) n, base);
}

void  KBChain_7segment::println(void) {
  position = 0;
}

void  KBChain_7segment::println(char c, int base)
{
  print(c, base);
  println();
}

void KBChain_7segment::println(unsigned char b, int base)
{
  print(b, base);
  println();
}

void KBChain_7segment::println(int n, int base)
{
  print(n, base);
  println();
}

void KBChain_7segment::println(unsigned int n, int base)
{
  print(n, base);
  println();
}

void KBChain_7segment::println(long n, int base)
{
  print(n, base);
  println();
}

void KBChain_7segment::println(unsigned long n, int base)
{
  print(n, base);
  println();
}

void KBChain_7segment::println(double n, int digits)
{
  print(n, digits);
  println();
}

void KBChain_7segment::print(double n, int digits)
{
  printFloat(n, digits);
}

size_t KBChain_7segment::write(uint8_t c) {

  uint8_t r = 0;

  if (c == '\n') position = 0;
  if (c == '\r') position = 0;

  if ((c >= '0') && (c <= '9')) {
    writeDigitNum(position, c-'0');
    r = 1;
  }

  position++;
  if (position == 2) position++;

  return r;
}

void KBChain_7segment::writeDigitRaw(uint8_t d, uint8_t bitmask) {
  if (d > 4) return;
  displaybuffer[d] = bitmask;
}

void KBChain_7segment::drawColon(bool state) {
  if (state)
  displaybuffer[2] = 0x3;  // For 7SEG LTC-4727JR
  else
    displaybuffer[2] = 0;
}

void KBChain_7segment::drawL3(bool state) {
  if (state)
  displaybuffer[2] = 0x4;  // For 7SEG LTC-4727JR
  else
    displaybuffer[2] = 0;
}

void KBChain_7segment::writeColon(void) {
	uint8_t buff[3];
	buff[0] = 0x04;
	buff[1] = displaybuffer[2] & 0xFF;
	buff[2] = displaybuffer[2] >> 8;
	i2c->write(channel, address, buff, 3);
}

void KBChain_7segment::writeDigitNum(uint8_t d, uint8_t num, bool dot) {
  if (d > 4) return;

  writeDigitRaw(d, numbertable[num] | (dot << 7));
}

void KBChain_7segment::print(long n, int base)
{
  printNumber(n, base);
}

void KBChain_7segment::printNumber(long n, uint8_t base)
{
    printFloat(n, 0, base);
}

void KBChain_7segment::printFloat(double n, uint8_t fracDigits, uint8_t base) 
{ 
  uint8_t numericDigits = 4;   // available digits on display
  bool isNegative = false;  // true if the number is negative
  
  // is the number negative?
  if(n < 0) {
    isNegative = true;  // need to draw sign later
    --numericDigits;    // the sign will take up one digit
    n *= -1;            // pretend the number is positive
  }
  
  // calculate the factor required to shift all fractional digits
  // into the integer part of the number
  double toIntFactor = 1.0;
  for(int i = 0; i < fracDigits; ++i) toIntFactor *= base;
  
  // create integer containing digits to display by applying
  // shifting factor and rounding adjustment
  uint32_t displayNumber = n * toIntFactor + 0.5;
  
  // calculate upper bound on displayNumber given
  // available digits on display
  uint32_t tooBig = 1;
  for(int i = 0; i < numericDigits; ++i) tooBig *= base;
  
  // if displayNumber is too large, try fewer fractional digits
  while(displayNumber >= tooBig) {
    --fracDigits;
    toIntFactor /= base;
    displayNumber = n * toIntFactor + 0.5;
  }
  
  // did toIntFactor shift the decimal off the display?
  if (toIntFactor < 1) {
    printError();
  } else {
    // otherwise, display the number
    int8_t displayPos = 4;
    
    if (displayNumber)  //if displayNumber is not 0
    {
      for(uint8_t i = 0; displayNumber || i <= fracDigits; ++i) {
        bool displayDecimal = (fracDigits != 0 && i == fracDigits);
        writeDigitNum(displayPos--, displayNumber % base, displayDecimal);
        if(displayPos == 2) writeDigitRaw(displayPos--, 0x00);
        displayNumber /= base;
      }
    }
    else {
      writeDigitNum(displayPos--, 0, false);
    }
  
    // display negative sign if negative
    if(isNegative) writeDigitRaw(displayPos--, 0x40);
  
    // clear remaining display positions
    while(displayPos >= 0) writeDigitRaw(displayPos--, 0x00);
  }
}

void KBChain_7segment::printError(void) {
  for(uint8_t i = 0; i < SEVENSEG_DIGITS; ++i) {
    writeDigitRaw(i, (i == 2 ? 0x00 : 0x40));
  }
}


#endif