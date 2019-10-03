/* ************************************************************************
 *
 *   driver functions for ST7036 compatible character displays
 *   - 1 to 3 lines, 16 characters
 *   - 4 bit parallel interface
 *   - 8 bit parallel interface (not supported)
 *   - SPI interface (4-wire)
 *   - I2C interface (ST7036i, not supported yet)
 *   - untested!!!
 *
 *   (c) 2019 by Markus Reschke
 *
 * ************************************************************************ */

/*
 *  hints:
 *  - pin assignment for 4 bit parallel
 *    XRESET  Vcc or LCD_RESET (optional)
 *    CSB     Gnd or LCD_CS (optional)
 *    DB4     LCD_DB4 (default: LCD_PORT Bit #0)
 *    DB5     LCD_DB5 (default: LCD_PORT Bit #1)
 *    DB6     LCD_DB6 (default: LCD_PORT Bit #2)
 *    DB7     LCD_DB7 (default: LCD_PORT Bit #3)
 *    RS      LCD_RS
 *    R/W     Gnd (read only) or LCD_RW (optional)
 *    E       LCD_EN
 *    PSB     pull-up resistor to Vcc (enable parallel mode)
 *  - max. clock for parallel interface: 2.5MHz (3.5MHz @ 5V)
 *  - pin assignment for SPI (4-wire)
 *    XRESET      Vcc or LCD_RESET (optional)
 *    CSB         Gnd or LCD_CS (optional)
 *    SI (DB7)    LCD_SI
 *    SCL (DB6)   LCD_SCL
 *    RS          LCD_RS
 *    PSB         Gnd (enable serial mode)
 *    E           Vcc
 *    For hardware SPI LCD_SCL and LCD_SI have to be the MCU's SCK and
 *    MOSI pins.
 *  - max. SPI clock rate: 5MHz (10MHz @ 5V)
 *  - pin assignment for I2C (ST7036i)
 *    XRESET       Vcc or LCD_RESET (optional)
 *    A0 (DB6)     slave address A0
 *    A1 (DB7)     slave address A1
 *    SCL (DB0)    I2C_SCL
 *    SDA (DB1-5)  I2C_SDA
 *    PSB          Vcc
 *  - max. I2C clock rate: 300kHz (400kHz fast mode @ 5V)
 */


/* local includes */
#include "config.h"           /* global configuration */

#ifdef LCD_ST7036


/*
 *  local constants
 */

/* source management */
#define LCD_DRIVER_C


/*
 *  include header files
 */

/* local includes */
#include "common.h"           /* common header file */
#include "variables.h"        /* global variables */
#include "functions.h"        /* external functions */
#include "ST7036.h"           /* ST7036 specifics */

/* fonts (5x8) */
#include "font_ST7036.h"      /* ST7036's internal font */



/* ************************************************************************
 *   low level functions for 4 bit parallel interface
 * ************************************************************************ */


#ifdef LCD_PAR_4

/*
 *  set up interface bus
 *  - should be called at firmware startup
 */

void LCD_BusSetup(void)
{
  uint8_t           Bits;          /* bitmask */

  /*
   *  set control signals
   */

  Bits = LCD_DDR;                       /* get current directions */

  /* required pins */
  Bits |= (1 << LCD_RS) | (1 << LCD_EN) | 
          (1 << LCD_DB4) | (1 << LCD_DB5) | (1 << LCD_DB6) | (1 << LCD_DB7);

  /* optional output pins */
  #ifdef LCD_RESET
    Bits |= (1 << LCD_RESET);      /* XRESET */
  #endif
  #ifdef LCD_CS
    Bits |= (1 << LCD_CS);         /* CSB */
  #endif
  #ifdef LCD_RW
    Bits |= (1 << LCD_RW);         /* R/W */
  #endif

  LCD_DDR = Bits;                  /* set new directions */

  /* set default levels */

  /* LCD_EN should be low by default */

  #ifdef LCD_RESET
    /* disable reset */
    LCD_PORT |= (1 << LCD_RESET);  /* set XRESET high */
  #endif

  #ifdef LCD_CS
    /* disable chip */
    LCD_PORT |= (1 << LCD_CS);     /* set CSB high */
  #endif

  /* LCD_RW should be low by default (write mode) */
}



/*
 *  create Enable pulse
 *  - LCD needs an Enable pulse to take in data for processing
 */

void LCD_EnablePulse(void)
{
  LCD_PORT |= (1 << LCD_EN);       /* set E high */

  /* enable high pulse time >= 200ns */
  wait1us();                       /* wait 1µs */

  LCD_PORT &= ~(1 << LCD_EN);      /* set E low */
}



void LCD_SendNibble(uint8_t Nibble)
{
  uint8_t           Data;

  Data = LCD_PORT;            /* get current bits */
  /* clear all 4 data lines */
  Data &= ~((1 << LCD_DB4) | (1 << LCD_DB5) | (1 << LCD_DB6) | (1 << LCD_DB7));

  #ifdef LCD_DB_STD
    /* standard pins: simply take nibble */
    Data |= Nibble;
  #else
    /* non-standard pins: set bits individually */
    if (Nibble & 0b00000001) Data |= (1 << LCD_DB4);
    if (Nibble & 0b00000010) Data |= (1 << LCD_DB5);
    if (Nibble & 0b00000100) Data |= (1 << LCD_DB6);
    if (Nibble & 0b00001000) Data |= (1 << LCD_DB7);
  #endif

  LCD_PORT = Data;            /* set nibble */

  /* data setup time >= 100ns */

  LCD_EnablePulse();          /* enable pulse */

  /* data hold time >= 40ns */

  /* enable low pulse time >= 150ns */
}



/*
 *  send a byte (data or command) to the LCD
 *  - send byte as two nibbles (MSB first, LSB last)
 *
 *  requires:
 *  - byte value to send
 */

void LCD_Send(uint8_t Byte)
{
  uint8_t           Nibble;

  #ifdef LCD_RW
  /* indicate write mode */
  LCD_PORT &= ~(1 << LCD_RW);      /* set R/W low */
  #endif

  /* send upper nibble (bits 4-7) */
  Nibble = (Byte >> 4) & 0x0F;          /* get upper nibble */
  LCD_SendNibble(Nibble);

  /* send lower nibble (bits 0-3) */
  Nibble = Byte & 0x0F;                 /* get lower nibble */
  LCD_SendNibble(Nibble);

  /* we don't read the LCD for checking the busy flag */
  /* most commands need 26.3µs for processing */
  wait30us();            /* LCD needs some time for processing */

  /* clear data lines on port */  
  Nibble = ~((1 << LCD_DB4) | (1 << LCD_DB5) | (1 << LCD_DB6) | (1 << LCD_DB7));
  LCD_PORT &= Nibble;         /* clear port */
}



/*
 *  send a command to the LCD
 *
 *  requires:
 *  - byte value to send
 */
 
void LCD_Cmd(uint8_t Cmd)
{
  #ifdef LCD_CS
  /* enable chip */
  LCD_PORT &= ~(1 << LCD_CS);      /* set CSB low */
  #endif

  /* indicate command mode */
  LCD_PORT &= ~(1 << LCD_RS);      /* set RS low */

  LCD_Send(Cmd);                   /* send command */

  #ifdef LCD_CS
  /* disable chip */
  LCD_PORT |= (1 << LCD_CS);       /* set CSB high */
  #endif
}



/*
 *  display a single character
 *
 *  requires:
 *  - Char: character to display
 */

void LCD_Char(unsigned char Char)
{
  uint8_t           *Table;        /* pointer to table */
  uint8_t           ID;            /* char ID */  

  /* prevent x overflow */
  if (UI.CharPos_X > LCD_CHAR_X) return;

  /* get character ID from lookup table */
  Table = (uint8_t *)&FontTable;        /* start address */
  Table += Char;                        /* add offset for character */
  ID = pgm_read_byte(Table);            /* get ID number */
  if (ID == 0xff) return;               /* no character available */

  #ifdef LCD_CS
  /* enable chip */
  LCD_PORT &= ~(1 << LCD_CS);      /* set CSB low */
  #endif

  /* indicate data mode */
  LCD_PORT |= (1 << LCD_RS);       /* set RS high */

  LCD_Send(ID);                    /* send character ID */

  #ifdef LCD_CS
  /* disable chip */
  LCD_PORT |= (1 << LCD_CS);       /* set CSB high */
  #endif

  /* update character position */
  UI.CharPos_X++;                  /* next character in current line */
  /* LCD's RAM address already points to next character */
}



/*
 *  load a custom character into LCD module
 *
 *  requires:
 *  - pointer of fixed character data
 *  - ID for custom character (0-7)
 */

void LCD_CustomChar(uint8_t ID)
{
  uint8_t      i;                  /* counter */
  uint8_t      Byte;               /* data byte */
  uint8_t      *Table;             /* pointer to char data */

  /* set data start address */
  Table = (uint8_t *)&FontData;         /* start address */
  Table += (ID * 8);                    /* add offset for character */


  /*
   *  set CGRAM start address (for a 5x8 character)
   *  - lower 3 bits determine the row in a character
   *  - higher 3 bits determine the ID of the character
   *  - so we have to shift the ID to the higher part
   *  - LCD module supports up to 8 custom characters (5x8 font)
   */

  LCD_Cmd(CMD_SET_CGRAM | (ID << 3));

  #ifdef LCD_CS
  /* enable chip */
  LCD_PORT &= ~(1 << LCD_CS);      /* set CSB low */
  #endif

  /* indicate data mode */
  LCD_PORT |= (1 << LCD_RS);            /* set RS high */

  /* write custom character */
  for (i = 0; i < 8; i++)               /* 8 bytes */
  {
    Byte = pgm_read_byte(Table);        /* read byte */
    LCD_Send(Byte);                     /* send byte */

    Table++;                            /* next byte */
  }

  #ifdef LCD_CS
  /* disable chip */
  LCD_PORT |= (1 << LCD_CS);       /* set CSB high */
  #endif
}



/*
 *  interface specific display setup
 */

void LCD_DisplaySetup(void)
{
  uint8_t           Bits;            /* port bits */

  /*
   *  first we have to send the "function set" command three times
   *  - CMD_FUNCTION_SET | FLAG_INTERFACE_8BIT -> 0b0011xxxx
   *  - RS and R/W unset
   *  - DB4 and DB5 set
   */

  #ifdef LCD_CS
  /* enable chip */
  LCD_PORT &= ~(1 << LCD_CS);      /* set CSB low */
  #endif

  /* round #1 */
  MilliSleep(40);                       /* wait 40ms */
  Bits = LCD_PORT;                      /* get current bits */
  /* clear LCD_RS, LCD_DB6 & LCD_DB7 */
  Bits &= ~(1 << LCD_RS) & ~(1 << LCD_DB6) & ~(1 << LCD_DB7);
  /* set LCD_DB4 & LCD_DB5 */
  Bits |= (1 << LCD_DB5) | (1 << LCD_DB4);
  LCD_PORT = Bits;                      /* set new bits */
  LCD_EnablePulse();                    /* enable pulse */

  /* round #2 */
  MilliSleep(2);                        /* wait 2ms */
  LCD_EnablePulse();                    /* enable pulse */

  /* round #3 */
  wait30us();                           /* wait 30µs */
  LCD_EnablePulse();                    /* enable pulse */


  /*
   *  init 4 bit mode
   *  - CMD_FUNCTION_SET | FLAG_INTERFACE_4BIT -> 0b0010xxxx
   *  - same as above besides DB4 -> clear LCD_DB4
   */

  wait30us();                           /* wait 30µs */
  Bits = LCD_PORT;                      /* get current bits */
  Bits &= ~(1 << LCD_DB4);              /* clear LCD_DB4 */
  LCD_PORT = Bits;                      /* set new bits */
  LCD_EnablePulse();                    /* enable pulse */
  wait30us();                           /* wait 30µs */

  #ifdef LCD_CS
  /* disable chip */
  LCD_PORT |= (1 << LCD_CS);       /* set CSB high */
  #endif


  /*
   *  start setting modes
   */

  #ifdef LCD_EXTENDED_CMD
    /* extension instruction set enabled (pin EXT high) */

    /* function set: 4 bit interface / 2 or 3 lines / instruction table 1 */
    LCD_Cmd(CMD_FUNCTION_SET | FLAG_LINES_2 | FLAG_INST_TABLE_1);
  #else
    /* extension instruction set disabled (pin EXT low) */

    /* function set: 4 bit interface / 2 or 3 lines */
    LCD_Cmd(CMD_FUNCTION_SET | FLAG_LINES_2);
  #endif
}

#endif



/* ************************************************************************
 *   low level functions for SPI (4-wire) interface
 * ************************************************************************ */


#ifdef LCD_SPI


/*
 *  set up interface bus
 *  - should be called at firmware startup
 */

void LCD_BusSetup(void)
{
  uint8_t           Bits;          /* bitmask */


  /*
   *  set control signals
   */

  Bits = LCD_DDR;                  /* get current directions */

  /* basic output pins */
  Bits |= (1 << LCD_RS);           /* RS */

  /* optional output pins */
  #ifdef LCD_RESET
    Bits |= (1 << LCD_RESET);      /* XRESET */
  #endif
  #ifdef LCD_CS
    Bits |= (1 << LCD_CS);         /* CSB */
  #endif

  LCD_DDR = Bits;                  /* set new directions */

  #ifdef LCD_RESET
    /* disable reset */
    LCD_PORT |= (1 << LCD_RESET);  /* set XRESET high */
  #endif

  /* set default levels */
  #ifdef LCD_CS
    /* disable chip */
    LCD_PORT |= (1 << LCD_CS);     /* set CSB high */
  #endif


  /*
   *  init SPI bus
   */

  #ifdef SPI_HARDWARE
  /*
   *  set SPI clock rate (max. 2.5MHz)
   */

  /* 1MHz -> f_osc/2 (SPR1 = 0, SPR0 = 0, SPI2X = 1) */
  #if CPU_FREQ / 1000000 == 1
    SPI.ClockRate = SPI_CLOCK_2X;
  #endif

  /* 8MHz -> f_osc/4 (SPR1 = 0, SPR0 = 0, SPI2X = 0) */
  #if CPU_FREQ / 1000000 == 8
    SPI.ClockRate = 0
  #endif

  /* 16MHz -> f_osc/8 (SPR1 = 0, SPR0 = 1, SPI2X = 1) */
  #if CPU_FREQ / 1000000 == 16
    SPI.ClockRate = SPI_CLOCK_R0 | SPI_CLOCK_2X;
  #endif

  /* 20MHz -> f_osc/8 (SPR1 = 0, SPR0 = 1, SPI2X = 1) */
  #if CPU_FREQ / 1000000 == 20
    SPI.ClockRate = SPI_CLOCK_R0 | SPI_CLOCK_2X;
  #endif

  #endif

  SPI_Setup();                     /* set up SPI bus */
}



/*
 *  send a command to the LCD
 *
 *  requires:
 *  - byte value to send
 */
 
void LCD_Cmd(uint8_t Cmd)
{
  /* select chip, if pin available */
  #ifdef LCD_CS
    LCD_PORT &= ~(1 << LCD_CS);    /* set CSB low */
  #endif

  /* indicate command mode */
  LCD_PORT &= ~(1 << LCD_RS);      /* set RS low */

  SPI_Write_Byte(Cmd);             /* write command byte */

  /* deselect chip, if pin available */
  #ifdef LCD_CS
    LCD_PORT |= (1 << LCD_CS);     /* set CSB high */
  #endif
}



/*
 *  display a single character
 *
 *  requires:
 *  - Char: character to display
 */

void LCD_Char(unsigned char Char)
{
  uint8_t           *Table;        /* pointer to table */
  uint8_t           ID;            /* char ID */  

  /* prevent x overflow */
  if (UI.CharPos_X > LCD_CHAR_X) return;

  /* get character ID from lookup table */
  Table = (uint8_t *)&FontTable;        /* start address */
  Table += Char;                        /* add offset for character */
  ID = pgm_read_byte(Table);            /* get ID number */
  if (ID == 0xff) return;               /* no character available */

  #ifdef LCD_CS
  /* enable chip */
  LCD_PORT &= ~(1 << LCD_CS);      /* set CSB low */
  #endif

  /* indicate data mode */
  LCD_PORT |= (1 << LCD_RS);       /* set RS high */

  SPI_Write_Byte(ID);              /* send character ID */

  #ifdef LCD_CS
  /* disable chip */
  LCD_PORT |= (1 << LCD_CS);       /* set CSB high */
  #endif

  /* update character position */
  UI.CharPos_X++;                  /* next character in current line */
  /* LCD's RAM address already points to next character */
}



/*
 *  load a custom character into LCD module
 *
 *  requires:
 *  - pointer of fixed character data
 *  - ID for custom character (0-7)
 */

void LCD_CustomChar(uint8_t ID)
{
  uint8_t      i;                  /* counter */
  uint8_t      Byte;               /* data byte */
  uint8_t      *Table;             /* pointer to char data */

  /* set data start address */
  Table = (uint8_t *)&FontData;         /* start address */
  Table += (ID * 8);                    /* add offset for character */


  /*
   *  set CGRAM start address (for a 5x8 character)
   *  - lower 3 bits determine the row in a character
   *  - higher 3 bits determine the ID of the character
   *  - so we have to shift the ID to the higher part
   *  - LCD module supports up to 8 custom characters (5x8 font)
   */

  LCD_Cmd(CMD_SET_CGRAM | (ID << 3));

  #ifdef LCD_CS
  /* enable chip */
  LCD_PORT &= ~(1 << LCD_CS);      /* set CSB low */
  #endif

  /* indicate data mode */
  LCD_PORT |= (1 << LCD_RS);            /* set RS high */

  /* write custom character */
  for (i = 0; i < 8; i++)               /* 8 bytes */
  {
    Byte = pgm_read_byte(Table);        /* read byte */
    SPI_Write_Byte(Byte);               /* send byte */

    Table++;                            /* next byte */
  }

  #ifdef LCD_CS
  /* disable chip */
  LCD_PORT |= (1 << LCD_CS);       /* set CSB high */
  #endif
}



/*
 *  interface specific display setup
 */

void LCD_DisplaySetup(void)
{
  MilliSleep(40);                       /* wait 40ms */

  /* function set: 8 bit interface / 2 or 3 lines */
  LCD_Cmd(CMD_FUNCTION_SET | FLAG_INTERFACE_8BIT | FLAG_LINES_2);

  #ifdef LCD_EXTENDED_CMD
    /* extension instruction set enabled (pin EXT high) */

    /* function set: 8 bit interface / 2 or 3 lines / instruction table 1 */
    LCD_Cmd(CMD_FUNCTION_SET | FLAG_INTERFACE_8BIT | FLAG_LINES_2 | FLAG_INST_TABLE_1);
  #else
    /* extension instruction set disabled (pin EXT low) */

    /* function set: 8 bit interface / 2 or 3 lines */
    LCD_Cmd(CMD_FUNCTION_SET | FLAG_INTERFACE_8BIT | FLAG_LINES_2);
  #endif
}

#endif



/* ************************************************************************
 *   high level functions
 * ************************************************************************ */


/*
 *  clear the display 
 */ 

void LCD_Clear(void)
{
  LCD_Cmd(CMD_CLEAR);              /* send clear command */
  MilliSleep(1);                   /* LCD needs some time for processing */

  /* reset character position */
  UI.CharPos_X = 1;
  UI.CharPos_Y = 1;
}



#ifdef LCD_EXTENDED_CMD

/*
 *  set contrast
 *
 *  required:
 *  - Contrast: 0-63
 */

void LCD_Contrast(uint8_t Contrast)
{
  uint8_t           Bits;          /* temp. value */

  /* set contrast */

  /* contrast set: lower 4 bits of contrast */
  Bits = Contrast & 0b00001111;         /* filter lower 4 bits */
  LCD_Cmd(CMD_CONTRAST | Bits);

  /* power/ICON/contrast control: booster on / upper 2 bits of constrast */
  Bits = Contrast & 0b00110000;         /* filter upper 2 bits */
  Bits >>= 4;                           /* move bits to the right */
  LCD_Cmd(CMD_MISC_CONTROL | FLAG_BOOSTER_ON | Bits);

  NV.Contrast = Contrast;          /* update value */
}

#endif



/*
 *  initialize LCD
 */
 
void LCD_Init(void)
{
  #ifdef LCD_RESET
  /* reset display */
  LCD_PORT &= ~(1 << LCD_RESET);        /* set XRESET low */
  wait100us();                          /* wait 100µs */
  LCD_PORT |= (1 << LCD_RESET);         /* set XRESET high */
  wait10us();                           /* wait 10µs */
  #endif

  LCD_DisplaySetup();         /* interface specific setup */


  /*
   *  set modes
   */

  #ifdef LCD_EXTENDED_CMD
    /* extension instruction set enabled (pin EXT high) */
  
    /* bias set: bias 1/5 / 3-line */
    LCD_Cmd(CMD_BIAS | FLAG_BIAS_5 | FLAG_3LINE);

    /* set contrast and enable booster (default contrast: 32) */
    LCD_Contrast(LCD_CONTRAST);

    /* follower control: follower on / factor 2.00 */
    LCD_Cmd(CMD_FOLLOWER | FLAG_FOLLOWER_ON | FLAG_FOLLOWER_200);
  #endif

  /* display: display on / cursor off / no blinking */
  LCD_Cmd(CMD_DISPLAY_CONTROL | FLAG_DISPLAY_ON);

  /* entry mode: increment cursor position / no scrolling */    
  LCD_Cmd(CMD_ENTRY_MODE | FLAG_CURSOR_INCREASE);


  /*
   *  load custom characters
   */

  /* todo: why not a simple loop? */

  /* custom symbols for components */
  LCD_CustomChar(LCD_CHAR_DIODE_AC);    /* diode symbol '|>|' */
  LCD_CustomChar(LCD_CHAR_DIODE_CA);    /* diode symbol '|<|' */
  LCD_CustomChar(LCD_CHAR_CAP);         /* capacitor symbol '||' */
  LCD_CustomChar(LCD_CHAR_RESISTOR_L);  /* resistor symbol '[' */
  LCD_CustomChar(LCD_CHAR_RESISTOR_R);  /* resistor symbol ']' */

  /* omega and µ */
  LCD_CustomChar(LCD_CHAR_OMEGA);       /* Omega */
  LCD_CustomChar(LCD_CHAR_MICRO);       /* µ / micro */


  /* and clear display */
  LCD_Clear();

  /* update character maximums */
  UI.CharMax_X = LCD_CHAR_X;       /* characters per line */
  UI.CharMax_Y = LCD_CHAR_Y;       /* lines */
  #ifdef LCD_EXTENDED_CMD
  UI.MaxContrast = 63;             /* maximum LCD contrast */
  #endif
}



/*
 *  set LCD character position
 *  - since we can't read the LCD and don't use a RAM buffer
 *    we have to move page-wise in y direction
 *  - top left: 1/1
 *
 *  requires:
 *  - x:  horizontal position (1-)
 *  - y:  vertical position (1-)
 */

void LCD_CharPos(uint8_t x, uint8_t y)
{
  uint8_t           Address = 0;        /* RAM address */

  /* update character position */
  UI.CharPos_X = x;
  UI.CharPos_Y = y;

  /*
   *  vertical start address
   *  Line 1: 0x00
   *  Line 2: 0x10
   *  Line 3: 0x20
   */

  y--;                        /* address starts at 0 */

  if (y == 1)                 /* line 2 */
  {
    Address = 0x10;           /* starts at 0x10 */
  }
  else if (y == 2)            /* line 3 */
  {
    Address = 0x20;           /* starts at 0x20 */
  }

  /*
   *  horizontal position
   *  line address + horizontal position
   */

  x--;                             /* address starts at 0 */
  Address += x;                    /* add x position */

  /* set RAM address */
  LCD_Cmd(CMD_SET_DDRAM | Address);     /* send command */
}



/*
 *  clear one single character line
 *
 *  requires:
 *  - Line: line number (1-3)
 *    special case line 0: clear remaining space in current line
 */ 

void LCD_ClearLine(uint8_t Line)
{
  uint8_t           n = 0;         /* counter */

  if (Line == 0)         /* special case: rest of current line */
  {
    n = UI.CharPos_X;         /* current character position */
    n--;                      /* starts at 0 */
  }
  else                   /* standard case: specified line */
  {
    LCD_CharPos(1, Line);     /* go to beginning of line */
  }

  while (n < 16)              /* up to 16 bytes */
  {
    LCD_Char(' ');            /* send space */
    n++;                      /* next one */
  }
}



/*
 *  set cursor
 *
 *  required:
 *  - Mode: cursor mode
 *    0: cursor on
 *    1: cursor off
 */

void LCD_Cursor(uint8_t Mode)
{
  uint8_t           Command;

  LCD_CharPos(LCD_CHAR_X, LCD_CHAR_Y);       /* move to bottom right */

  /* default: cursor off */
  Command = CMD_DISPLAY_CONTROL | FLAG_DISPLAY_ON | FLAG_CURSOR_OFF;

  /* enable cursor if requested */
  if (Mode) Command |= FLAG_CURSOR_ON;

  LCD_Cmd(Command);           /* send command */
}



/* ************************************************************************
 *   clean-up of local constants
 * ************************************************************************ */

/* source management */
#undef LCD_DRIVER_C

#endif

/* ************************************************************************
 *   EOF
 * ************************************************************************ */




