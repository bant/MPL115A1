//=============================================================================
// File Name    : MPL115A1.c
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

/* Includes ------------------------------------------------------------------ */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "MPL115A1.h"

/* local typedef ------------------------------------------------------------- */
/* local define -------------------------------------------------------------- */
#define VERSION     "0.1"

/* local macro --------------------------------------------------------------- */
/* local variables ----------------------------------------------------------- */
char *usage =   "Usage: MPL115A1 [option] arg1 arg2\n" \
                "        -c # : SPI Channel No. (default 0)\n" \
                "        -s # : SPI SPEED (default 1000000)\n" \
                "        -t # : Average times (default 100)\n" \
                "        -h   : display help\n"
                "        -v   : display version\n" ;

char *copyright = "MPL115A1 version: %s\n" \
                  "Copyright (c) 2013 BANT\n" ;

int spi_ch;
int spi_speed;

/* local functions prototype ------------------------------------------------- */


/****************************************
 * MPL115A1 register write function     *
 ****************************************/
void MPL115A1_write_register(unsigned char address, unsigned char data)
{
    //write any data byte to any single address
    //adds a 0 to the MSB of the address byte (WRITE mode)
    unsigned char spiData[2];

    spiData[0] = address & 0x7F;
    spiData[1] = data;

    if(wiringPiSPIDataRW(spi_ch, spiData, sizeof(spiData)) < 0)
    {
        fprintf (stderr, "Unable to write SPI device %d: %s\n", spi_ch, strerror (errno)) ;
        exit(EXIT_FAILURE);
    }
}

/****************************************
 * MPL115A1 register read function      *
 ****************************************/
unsigned char MPL115A1_read_register(unsigned char address)
{
    // returns the contents of any 1 byte register from any address
    // sets the MSB for every address byte (READ mode)
    unsigned char spiData[2];

    spiData[0] = address | 0x80;
    spiData[1] = 0x00;          // dummy

    if(wiringPiSPIDataRW(spi_ch, spiData, sizeof(spiData)) < 0)
    {
        fprintf (stderr, "Unable to read SPI device %d: %s\n", spi_ch, strerror (errno)) ;
        exit(EXIT_FAILURE);
    }

    return spiData[1];
}

/****************************************
 * pressure read function               *
 ****************************************/
float MPL115A1_read_pressure(void)
{
    signed char sia0MSB, sia0LSB;
    signed char sib1MSB, sib1LSB;
    signed char sib2MSB, sib2LSB;
    signed char sic12MSB, sic12LSB;
    signed char sic11MSB, sic11LSB;
    signed char sic22MSB, sic22LSB;
    signed int sia0, sib1, sib2, sic12, sic11, sic22, siPcomp;
    float decPcomp;
    signed long lt1, lt2, lt3, si_c11x1, si_a11, si_c12x2;
    signed long si_a1, si_c22x2, si_a2, si_a1x1, si_y1, si_a2x2;
    unsigned int uiPadc;
    unsigned char uiPH, uiPL;
    unsigned int uiTadc;
    unsigned char uiTH, uiTL;

    // read pressure, temperature and coefficients, calculate and return absolute pressure [kPa]

    MPL115A1_write_register(MPL115A1_STARTBOTH, 0x00); // start temperature and pressure conversions
    delay(3); // AN: data is typically ready after 3ms, DS for both: max. 1ms

    // read raw pressure
    uiPH = MPL115A1_read_register(MPL115A1_PRESH);
    uiPL = MPL115A1_read_register(MPL115A1_PRESL);
    uiTH = MPL115A1_read_register(MPL115A1_TEMPH);
    uiTL = MPL115A1_read_register(MPL115A1_TEMPL);

    uiPadc = (unsigned int) uiPH << 8;
    uiPadc += (unsigned int) uiPL & 0x00FF;
    uiTadc = (unsigned int) uiTH << 8;
    uiTadc += (unsigned int) uiTL & 0x00FF;

    // read coefficients, put into 16-bit Variables
    // a0, pressure offset coefficient
    sia0MSB = MPL115A1_read_register(MPL115A1_A0MSB);
    sia0LSB = MPL115A1_read_register(MPL115A1_A0LSB);
    sia0 = (signed int) sia0MSB << 8;
    sia0 += (signed int) sia0LSB & 0x00FF;

    // b1, pressure sensitivity coefficient
    sib1MSB = MPL115A1_read_register(MPL115A1_B1MSB);
    sib1LSB = MPL115A1_read_register(MPL115A1_B1LSB);
    sib1 = (signed int) sib1MSB << 8;
    sib1 += (signed int) sib1LSB & 0x00FF;

    // b2, 1st order temperature offset coefficient (TCO)
    sib2MSB = MPL115A1_read_register(MPL115A1_B2MSB);
    sib2LSB = MPL115A1_read_register(MPL115A1_B2LSB);
    sib2 = (signed int) sib2MSB << 8;
    sib2 += (signed int) sib2LSB & 0x00FF;

    // c12, temperature sensitivity coefficient (TCS)
    sic12MSB = MPL115A1_read_register(MPL115A1_C12MSB);
    sic12LSB = MPL115A1_read_register(MPL115A1_C12LSB);
    sic12 = (signed int) sic12MSB << 8;
    sic12 += (signed int) sic12LSB & 0x00FF;

    // c11, pressure linearity (2nd order) coefficient
    sic11MSB = MPL115A1_read_register(MPL115A1_C11MSB);
    sic11LSB = MPL115A1_read_register(MPL115A1_C11LSB);
    sic11 = (signed int) sic11MSB << 8;
    sic11 += (signed int) sic11LSB & 0x00FF;

    // c22, 2nd order temperature offset coefficient
    sic22MSB = MPL115A1_read_register(MPL115A1_C22MSB);
    sic22LSB = MPL115A1_read_register(MPL115A1_C22LSB);
    sic22 = (signed int) sic22MSB << 8;
    sic22 += (signed int) sic22LSB & 0x00FF;

    // Coefficient 9 equation compensation

    // 10bit stored in 16bit, shift right
    uiPadc = uiPadc >> 6;
    uiTadc = uiTadc >> 6;

    // Step 1: c11x1 = c11 * Padc
    lt1 = (signed long) sic11;
    lt2 = (signed long) uiPadc;
    lt3 = lt1*lt2;
    si_c11x1 = (signed long) lt3;

    // Step 2: a11 = b1 + c11x1
    lt1 = ((signed long)sib1)<<14;
    lt2 = (signed long) si_c11x1;
    lt3 = lt1 + lt2;
    si_a11 = (signed long)(lt3>>14);

    // Step 3: c12x2 = c12 * Tadc
    lt1 = (signed long) sic12;
    lt2 = (signed long) uiTadc;
    lt3 = lt1*lt2;
    si_c12x2 = (signed long)lt3;

    // Step 4: a1 = a11 + c12x2
    lt1 = ((signed long)si_a11<<11);
    lt2 = (signed long)si_c12x2;
    lt3 = lt1 + lt2;
    si_a1 = (signed long) lt3>>11;

    // Step 5: c22x2 = c22*Tadc
    lt1 = (signed long)sic22;
    lt2 = (signed long)uiTadc;
    lt3 = lt1 * lt2;
    si_c22x2 = (signed long)(lt3);

    // Step 6: a2 = b2 + c22x2
    lt1 = ((signed long)sib2<<15);
    lt2 = ((signed long)si_c22x2>1);
    lt3 = lt1+lt2;
    si_a2 = ((signed long)lt3>>16);

    // Step 7: a1x1 = a1 * Padc
    lt1 = (signed long)si_a1;
    lt2 = (signed long)uiPadc;
    lt3 = lt1*lt2;
    si_a1x1 = (signed long)(lt3);

    // Step 8: y1 = a0 + a1x1
    lt1 = ((signed long)sia0<<10);
    lt2 = (signed long)si_a1x1;
    lt3 = lt1+lt2;
    si_y1 = ((signed long)lt3>>10);

    // Step 9: a2x2 = a2 * Tadc
    lt1 = (signed long)si_a2;
    lt2 = (signed long)uiTadc;
    lt3 = lt1*lt2;
    si_a2x2 = (signed long)(lt3);

    // Step 10: pComp = y1 + a2x2
    lt1 = ((signed long)si_y1<<10);
    lt2 = (signed long)si_a2x2;
    lt3 = lt1+lt2;

    // Fixed point result with rounding
    //siPcomp = ((signed int)lt3>>13);
    siPcomp = lt3/8192;

    // decPcomp is defined as a floating point number
    // Conversion to decimal value from 1023 ADC count value
    // ADC counts are 0 to 1023, pressure is 50 to 115kPa respectively
    decPcomp = ((65.0/1023.0)*siPcomp)+50;

    return decPcomp;
}

/****************************************
 * temperature read function            *
 ****************************************/
float MPL115A1_read_temperature(void)
{
    unsigned int uiTadc;
    unsigned char uiTH, uiTL;

    // read, calculate and return temperature

    MPL115A1_write_register(MPL115A1_STARTTEMP, 0x00); // start temperature conversion
    delay(3); // AN: data is typically ready after 3ms, DS for temp: max. 0.7ms

    // read raw temperature
    uiTH = MPL115A1_read_register(MPL115A1_TEMPH);
    uiTL = MPL115A1_read_register(MPL115A1_TEMPL);

    uiTadc = (unsigned int) uiTH << 8;
    uiTadc += (unsigned int) uiTL & 0x00FF;

    // 10bit stored in 16bit, shift right
    uiTadc = uiTadc >> 6;

    // Tadc is 472 counts at 25degC, -5.35 counts/degC
    // return ((float)uiTadc - 472.0)/(-5.35) + 25
    return (605.75-uiTadc)*0.186916;
}

/****************************************
 * print usage                          *
 ****************************************/
void print_usage(int retcode)
{
    printf(usage);
    exit(retcode);
}

/****************************************
 * main function                        *
 ****************************************/
int main(int argc, char *argv[])
{
    int i, times;
    int result;
    float pressure, temperature;
    extern char *optarg;
    extern int  optind, opterr;

    spi_ch = SPI_CH_0;
    spi_speed = SPI_SPEED_1000000;
    times = 100;

    while((result=getopt(argc,argv,"c:hs:t:v"))!=-1)
    {
        switch(result)
        {
        case 'c':
            if(strlen(optarg) != 1)
                print_usage(EXIT_FAILURE);
            switch(optarg[0])
            {
            case '0':
                spi_ch = SPI_CH_0;
                break;
            case '1':
                spi_ch = SPI_CH_1;
                break;
            default:
                print_usage(EXIT_FAILURE);
                break;
            }
            break;

        case 'h':
            print_usage(EXIT_SUCCESS);
            break;

        case 's':
            spi_speed = atoi(optarg);
            if (spi_speed == 0)
                print_usage(EXIT_FAILURE);
            break;

        case 't':
            times = atoi(optarg);
            if (times == 0)
                print_usage(EXIT_FAILURE);
            break;

        case 'v':
            printf (copyright, VERSION) ;
            return EXIT_SUCCESS;

        case '?':
            print_usage(EXIT_FAILURE);
            break;
        }
    }

    if (wiringPiSPISetup(spi_ch, spi_speed) < 0)
    {
        fprintf (stderr, "Unable to setup SPI device %d: %s\n", spi_ch, strerror (errno)) ;
        return EXIT_FAILURE;
    }

    printf("***** Start MPL115A1 Barometric Pressure Sensor *****\n");
    printf("[Conditions]------------\n");
    printf("SPI Channel: %d ch\n", spi_ch);
    printf("SPI Speed: %d Hz\n", spi_speed);
    printf("Average times: %d\n\n",times);

    pressure = 0;
    temperature = 0;
    for (i=0; i<times; i++)
    {
        pressure += MPL115A1_read_pressure();
        temperature += MPL115A1_read_temperature();
    }
    printf("[Results]---------------\n");
    printf("Pressure: %.1f hPa\n", pressure*10/times);
    printf("Temperature: %.1f degC\n\n", temperature/times);

    return EXIT_SUCCESS;
}

