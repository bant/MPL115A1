//=============================================================================
// File Name    : MPL115A1.h
//
// Title        : MPL115A1 Barometric Pressure Sensor Read command
// Revision     : 0.1
// Notes        :
// Target MCU   :
// Tool Chain   :
//
// LICENSE      : GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// Revision History:
// When         Who         Description of change
// -----------  ----------- -----------------------
// 2013/10/30   BANT        1st version
//=============================================================================

#ifndef MPL115A1_H
#define MPL115A1_H

/* typedef ------------------------------------------------------------------- */
/* define -------------------------------------------------------------------- */
//--- SPI define ----//
#define SPI_CH_0            0
#define SPI_CH_1            1
#define SPI_SPEED_1000000   1000000

//--- MPL115A1 register define --//
#define MPL115A1_PRESH    0x00    // 80
#define MPL115A1_PRESL    0x02    // 82
#define MPL115A1_TEMPH    0x04    // 84
#define MPL115A1_TEMPL    0x06    // 86

#define MPL115A1_A0MSB    0x08    // 88
#define MPL115A1_A0LSB    0x0A    // 8A
#define MPL115A1_B1MSB    0x0C    // 8C
#define MPL115A1_B1LSB    0x0E    // 8E
#define MPL115A1_B2MSB    0x10    // 90
#define MPL115A1_B2LSB    0x12    // 92
#define MPL115A1_C12MSB   0x14    // 94
#define MPL115A1_C12LSB   0x16    // 96
#define MPL115A1_C11MSB   0x18    // 98
#define MPL115A1_C11LSB   0x1A    // 9A
#define MPL115A1_C22MSB   0x1C    // 9C
#define MPL115A1_C22LSB   0x1E    // 9E

#define MPL115A1_STARTPRES 0x20 // start pressure measurement
#define MPL115A1_STARTTEMP 0x22 // start temperature measurement
#define MPL115A1_STARTBOTH 0x24 // start both simultaneously

/* macro -------------------------------------------------------------------- */
/* variables ---------------------------------------------------------------- */
/* functions prototype ------------------------------------------------------ */
void MPL115A1_write_register(unsigned char address, unsigned char data);
unsigned char MPL115A1_read_register(unsigned char address);
float MPL115A1_read_pressure(void);
float MPL115A1_read_temperature(void);
void print_usage(int retcode);

#endif
