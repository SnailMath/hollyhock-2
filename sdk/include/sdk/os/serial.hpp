/**
 * @file
 * @brief Functions used for sending and receiving serial data.
 */

// This works exactly the same as the calls on the cg50. Read more here:
// https://prizm.cemetech.net/index.php?title=Category:Syscalls:Serial
// These implementations are similar to:
// https://github.com/Jonimoose/libfxcg/blob/master/include/fxcg/serial.h

#pragma once
#include <stdint.h>

/**
 * Opens the 3-pin serial port with the specified parameters.
 * see https://prizm.cemetech.net/index.php?title=Serial_Open for more info.
 * mode:
 * 0 always 0
 * 1 bit rate	 0=300, 5=9600, 9=115200
 * 2 parity	 0=none, 1=odd, 2=even
 * 3 data length 0=8bit, 1=7bit
 * 4 stop bits	 0=1bit, 1=2bit
 * 5 always 0
 * Returns: 
 * 0 success
 * 3 already open
 *
 * @param mode The mode of the serial port. 
 */
extern "C"
int Serial_Open(unsigned char *mode);

extern "C"
int Serial_IsOpen(void);

extern "C"
int Serial_Close(int mode);

extern "C"
int Serial_Read(unsigned char *out, int sz, short *count);

extern "C"
int Serial_ReadSingle(unsigned char *out);

extern "C"
int Serial_Peek(int idx, unsigned char *out);

extern "C"
int Serial_PollRX(void);

extern "C"
int Serial_ClearRX(void);

/**
 * Sends a number of bytes through the 3-pin serial port, by putting them in the transmit buffer.
 * see https://prizm.cemetech.net/index.php?title=Serial_Write
 *
 * @param buf pointer to buffer containing the bytes to transmit
 * @count amount of bytes to transmit from buf
 */
extern "C"
int Serial_Write(const unsigned char *buf, int count);

extern "C"
int Serial_WriteSingle(unsigned char x);

extern "C"
int Serial_WriteUnbuffered(unsigned char x);

extern "C"
int Serial_PollTX(void);

extern "C"
int Serial_ClearTX(void);


