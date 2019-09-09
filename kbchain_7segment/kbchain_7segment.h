#ifndef __KBChain_7segment_H__
#define __KBChain_7segment_H__

#include "driver.h"
#include "device.h"
#include "i2c-dev.h"
#include "kidbright32.h"

#define KBChain_7segment_BLINK_CMD 0x80
#define KBChain_7segment_BLINK_DISPLAYON 0x01
#define KBChain_7segment_BLINK_OFF 0
#define KBChain_7segment_BLINK_2HZ  1
#define KBChain_7segment_BLINK_1HZ  2
#define KBChain_7segment_BLINK_HALFHZ  3
#define KBChain_7segment_CMD_BRIGHTNESS 0xE0

#define SEVENSEG_DIGITS 5

#define BYTE 0
#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

#ifndef _BV
  #define _BV(bit) (1<<(bit))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

static const uint8_t numbertable[] = {
	0x3F,   /* 0 */
	0x06,   /* 1 */
	0x5B,   /* 2 */
	0x4F,   /* 3 */
	0x66,   /* 4 */
	0x6D,   /* 5 */
	0x7D,   /* 6 */
	0x07,   /* 7 */
	0x7F,   /* 8 */
	0x6F,   /* 9 */
	0x77,   /* a */
	0x7C,   /* b */
	0x39,   /* C */
	0x5E,   /* d */
	0x79,   /* E */
	0x71,   /* F */
};

class KBChain_7segment : public Device {
	private:
		enum {
			s_detect,
			s_runing,
			s_wait,
			s_error
		} state;
		TickType_t tickcnt, polling_tickcnt;

		I2CDev *i2c;

		uint8_t position;

	public:
		// constructor
		KBChain_7segment(int bus_ch, int dev_addr) ;
		
		// override
		void init(void);
		void process(Driver *drv);
		int prop_count(void);
		bool prop_name(int index, char *name);
		bool prop_unit(int index, char *unit);
		bool prop_attr(int index, char *attr);
		bool prop_read(int index, char *value);
		bool prop_write(int index, char *value);
		
		// method
		bool i2cWriteByte(uint8_t c) ;

		void setBrightness(uint8_t b);
		void blinkRate(uint8_t b);
		void writeDisplay(void);
		void clear(void);

		uint16_t displaybuffer[8]; 

		void init(uint8_t a);
		
		size_t write(uint8_t c);

		void print(char, int = BYTE);
		void print(unsigned char, int = BYTE);
		void print(int, int = DEC);
		void print(unsigned int, int = DEC);
		void print(long, int = DEC);
		void print(unsigned long, int = DEC);
		void print(double, int = 2);
		void println(char, int = BYTE);
		void println(unsigned char, int = BYTE);
		void println(int, int = DEC);
		void println(unsigned int, int = DEC);
		void println(long, int = DEC);
		void println(unsigned long, int = DEC);
		void println(double, int = 2);
		void println(void);
		
		void writeDigitRaw(uint8_t x, uint8_t bitmask);
		void writeDigitNum(uint8_t x, uint8_t num, bool dot = false);
		void drawColon(bool state);
		void drawL3(bool state);
		void printNumber(long, uint8_t = 2);
		void printFloat(double, uint8_t = 2, uint8_t = DEC);
		void printError(void);

		void writeColon(void);
		
};

#endif