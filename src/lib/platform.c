#include <avr/io.h>
#include <platform.h>

#ifndef attiny4
    #include <avr/boot.h>
#endif

#ifdef atmega8
    #define SIGRD 5
#endif

#define DDR_SPI DDRB
#ifdef atmega8
    #define DD_MOSI PB3
    #define DD_SCK PB5
#else
    #define DD_MOSI PB2
    #define DD_SCK PB1
#endif

#ifdef atmega32u4
    #define UBRRH UBRR1H
    #define UBRRL UBRR1L
    #define UCSRB UCSR1B
    #define RXEN RXEN1
    #define TXEN TXEN1
    #define UCSRC UCSR1C
    #define USBS USBS1
    #define UCSZ0 UCSZ10
    #define UCSRA UCSR1A
    #define UDRE UDRE1
    #define UDR UDR1
#endif
#ifdef attiny4
    #define SPDR _SFR_MEM8(0x41FE)
    #define UDR _SFR_MEM8(0x41FF)
#endif

#define SPI_SELECT_PORT_DISABLED 0x0

#ifdef atmega32u4
    #define SPI_SELECT_CMD PORTE
    #define SPI_SELECT_DATA PORTD
    #define SPI_SELECT_CMD_ACTIVE 0x40
    #define SPI_SELECT_DATA_ACTIVE 0x4
#endif

#ifdef atmega8
    #define SPI_SELECT_CMD PORTD
    #define SPI_SELECT_DATA PORTD
    #define SPI_SELECT_CMD_ACTIVE 0x80
    #define SPI_SELECT_DATA_ACTIVE 0x1
#endif

#ifdef attiny4
    #define SPI_SELECT_CMD PORTB
    #define SPI_SELECT_DATA PORTB
    #define SPI_SELECT_CMD_ACTIVE 0x8
    #define SPI_SELECT_DATA_ACTIVE 0x1
#endif

#define BAUD 9600
#define CPU_CLK 8000000
#define CYCLES_PER_MS CPU_CLK/1000
#define SIMULATED_PLATFORM_SIGNATURE 0xBF

void delay(unsigned long milliseconds){
    long i,j = 0;
    for(i; i < milliseconds; i++){
        for(j; j < CYCLES_PER_MS; j++){
            asm volatile ("nop");
        }
    }
}

char getPlatformType(){
#ifndef attiny4
  return boot_signature_byte_get(0);
#else
  return *((char*)0x3FC0);
#endif
}

void platformBasedDelay(unsigned long milliseconds) {
  if(getPlatformType() == SIMULATED_PLATFORM_SIGNATURE)
    delay(milliseconds >> 8);
  else
    delay(milliseconds);
}

void platformBasedSPIBegin(void)
{
#ifndef attiny4
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
#endif
}

void platformBasedSPITransmit(char cData)
{
    /* Start transmission */
    SPDR = cData;
#ifndef attiny4
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)))
    ;
#endif
}

void platformBasedSerialBegin()
{
#ifndef attiny4
    /* Set baud rate */
    UBRRH = (unsigned char)(BAUD>>8);
    UBRRL = (unsigned char)BAUD;
    /* Enable receiver and transmitter */
    UCSRB = (1<<RXEN)|(1<<TXEN);
    /* Set frame format: 8data, 2stop bit */
    UCSRC = (1<<USBS)|(3<<UCSZ0);
#endif
}

void platformBasedSerialWrite(unsigned char data)
{
#ifndef attiny4
    /* Wait for empty transmit buffer */
    while ( !( UCSRA & (1<<UDRE)) )
        ;
#endif
    /* Put data into buffer, sends the data */
    UDR = data;
}

void writeDisplayCommand(uint8_t c) {

  SPI_SELECT_CMD = SPI_SELECT_PORT_DISABLED;
  SPI_SELECT_DATA = SPI_SELECT_PORT_DISABLED;
  SPDR = c;
  SPI_SELECT_CMD = SPI_SELECT_CMD_ACTIVE;
}

void writeDisplayData(uint8_t c) {
  SPI_SELECT_CMD = SPI_SELECT_PORT_DISABLED;
  SPI_SELECT_DATA = SPI_SELECT_DATA_ACTIVE;
  SPDR = c;
  SPI_SELECT_CMD = SPI_SELECT_CMD_ACTIVE;
}

void platformBasedDisplayBegin() {
    if(getPlatformType() != SIMULATED_PLATFORM_SIGNATURE){
        writeDisplayCommand(0x1);
        platformBasedDelay(150);
        writeDisplayCommand(0x11);
        platformBasedDelay(255);
        writeDisplayCommand(0xB1);
        writeDisplayData(0x1);
        writeDisplayData(0x2C);
        writeDisplayData(0x2D);
        writeDisplayCommand(0xB2);
        writeDisplayData(0x1);
        writeDisplayData(0x2C);
        writeDisplayData(0x2D);
        writeDisplayCommand(0xB3);
        writeDisplayData(0x1);
        writeDisplayData(0x2C);
        writeDisplayData(0x2D);
        writeDisplayData(0x1);
        writeDisplayData(0x2C);
        writeDisplayData(0x2D);
        writeDisplayCommand(0xB4);
        writeDisplayData(0x7);
        writeDisplayCommand(0xC0);
        writeDisplayData(0xA2);
        writeDisplayData(0x02);
        writeDisplayData(0x84);
        writeDisplayCommand(0xC1);
        writeDisplayData(0xC5);
        writeDisplayCommand(0xC2);
        writeDisplayData(0x0A);
        writeDisplayData(0x00);
        writeDisplayCommand(0xC3);
        writeDisplayData(0x8A);
        writeDisplayData(0x2A);
        writeDisplayCommand(0xC4);
        writeDisplayData(0x8A);
        writeDisplayData(0xEE);
        writeDisplayCommand(0xC5);
        writeDisplayData(0x0E);
        writeDisplayCommand(0x20);
        writeDisplayCommand(0x36);
        writeDisplayData(0xC8);
        writeDisplayCommand(0x3A);
        writeDisplayData(0x05);
        writeDisplayCommand(0x2A);
        writeDisplayData(0x00);
        writeDisplayData(0x00);
        writeDisplayData(0x00);
        writeDisplayData(0x7F);
        writeDisplayCommand(0x2B);
        writeDisplayData(0x00);
        writeDisplayData(0x00);
        writeDisplayData(0x00);
        writeDisplayData(0x9F);
        writeDisplayCommand(0xE0);
        writeDisplayData(0x02);
        writeDisplayData(0x1C);
        writeDisplayData(0x07);
        writeDisplayData(0x12);
        writeDisplayData(0x37);
        writeDisplayData(0x32);
        writeDisplayData(0x29);
        writeDisplayData(0x2D);
        writeDisplayData(0x29);
        writeDisplayData(0x25);
        writeDisplayData(0x2B);
        writeDisplayData(0x39);
        writeDisplayData(0x00);
        writeDisplayData(0x01);
        writeDisplayData(0x03);
        writeDisplayData(0x10);
        writeDisplayCommand(0xE1);
        writeDisplayData(0x03);
        writeDisplayData(0x1D);
        writeDisplayData(0x07);
        writeDisplayData(0x06);
        writeDisplayData(0x2E);
        writeDisplayData(0x2C);
        writeDisplayData(0x29);
        writeDisplayData(0x2D);
        writeDisplayData(0x2E);
        writeDisplayData(0x2E);
        writeDisplayData(0x37);
        writeDisplayData(0x3F);
        writeDisplayData(0x00);
        writeDisplayData(0x00);
        writeDisplayData(0x02);
        writeDisplayData(0x10);
        writeDisplayCommand(0x13);
        platformBasedDelay(10);
        writeDisplayCommand(0x29);
        platformBasedDelay(100);
    }
}
