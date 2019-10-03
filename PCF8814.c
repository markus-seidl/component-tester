/* ************************************************************************
 *
 *   driver functions for PCF8814 compatible grafic displays
 *   - aka Nokia 1100 display
 *   - 96 x 65 pixels
 *   - SPI interface (4 line (not supported yet), 3 line)
 *     modules are wired for 3-line SPI usually
 *   - I2C (not supported yet)
 *
 *   (c) 2019 by Markus Reschke
 *
 * ************************************************************************ */

/*
 *  hints:
 *  - pin assignment for SPI (3-line)
 *    /RES        LCD_RESET
 *    /CS         LCD_CS  (optional)
 *    SCLK        LCD_SCLK
 *    SDIN        LCD_SDIN
 *    Bit-bang mode only!
 *  - max. SPI clock rate: 6.5MHz
 *  - max. I2C clock rate: 400kHz (fast mode) or 3.4MHz (high speed mode)
 */


/* local includes */
#include "config.h"           /* global configuration */

#ifdef LCD_PCF8814


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
#include "PCF8814.h"          /* PCF8814 specifics */

/* fonts and symbols */
/* vertically aligned, vertical bit order flipped, page-wise grouping */
#include "font_6x8_vf.h"
#include "symbols_24x24_vfp.h"



/*
 *  derived constants
 */

/* banks/bytes required for character's height */
#define CHAR_BANKS       ((FONT_SIZE_Y + 7) / 8)

/* number of banks */
#define LCD_BANKS        9

/* number of lines and characters per line */
#define LCD_CHAR_X       (LCD_DOTS_X / FONT_SIZE_X)
#define LCD_CHAR_Y       ((LCD_DOTS_Y / 8) / CHAR_BANKS)

/* component symbols */
#ifdef SW_SYMBOLS
  /* pages/bytes required for symbol's height */
  #define SYMBOL_BANKS        ((SYMBOL_SIZE_Y + 7) / 8)

  /* size in relation to a character */
  #define LCD_SYMBOL_CHAR_X   ((SYMBOL_SIZE_X + FONT_SIZE_X - 1) / FONT_SIZE_X)
  #define LCD_SYMBOL_CHAR_Y   ((SYMBOL_SIZE_Y + CHAR_BANKS * 8 - 1) / (CHAR_BANKS * 8))

  /* check y size: we need at least 2 lines */
  #if LCD_SYMBOL_CHAR_Y < 2
    #error <<< Symbols too small! >>>
  #endif
#endif



/*
 *  local variables
 */

/* position management */
uint8_t             X_Start;       /* start position X (column) */
uint8_t             Y_Start;       /* start position Y (bank) */



/* ************************************************************************
 *   low level functions for SPI interface (3 wire)
 * ************************************************************************ */


/*
 *  The 3 wire SPI interface ignores the D/C line and adds a D/C control
 *  bit to the SPI data resulting in a 9 bit frame:  
 *  - first bit for D/C, followed by D7 to D0
 *  - bitbang SPI only since the MCU's hardware SPI supports just bytes
 */

#if defined (LCD_SPI) && defined (SPI_BITBANG) && defined (SPI_9)

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

  /* optional output pins */
  #ifdef LCD_RESET
    Bits |= (1 << LCD_RESET);      /* /RES */
  #endif
  #ifdef LCD_CS
    Bits |= (1 << LCD_CS);         /* /CS */
  #endif

  LCD_DDR = Bits;                  /* set new directions */

  /* set default levels */
  #ifdef LCD_CS
    /* disable chip */
    LCD_PORT |= (1 << LCD_CS);     /* set /CS high */
  #endif

  #ifdef LCD_RESET
    /* disable reset */
    LCD_PORT |= (1 << LCD_RESET);  /* set /RES high */
  #endif


  /*
   *  init SPI bus
   */

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
    LCD_PORT &= ~(1 << LCD_CS);    /* set /CS low */
  #endif

  SPI_Write_Bit(0);                /* indicate command (D/C=0) */
  SPI_Write_Byte(Cmd);             /* write command byte */

  /* deselect chip, if pin available */
  #ifdef LCD_CS
    LCD_PORT |= (1 << LCD_CS);     /* set /CS high */
  #endif
}



/*
 *  send data to the LCD
 *
 *  requires:
 *  - byte value to send
 */

void LCD_Data(uint8_t Data)
{
  /* select chip, if pin available */
  #ifdef LCD_CS
    LCD_PORT &= ~(1 << LCD_CS);    /* set /CS low */
  #endif

  SPI_Write_Bit(1);                /* indicate data (D/C=1) */
  SPI_Write_Byte(Data);            /* write data byte */

  /* deselect chip, if pin available */
  #ifdef LCD_CS
    LCD_PORT |= (1 << LCD_CS);     /* set /CS high */
  #endif
}

#endif



/* ************************************************************************
 *   high level functions
 * ************************************************************************ */


/*
 *  set LCD dot position
 *  - since we can't read the LCD and don't use a RAM buffer
 *    we have to move bank-wise in y direction
 *  - top left: 0/0
 *
 *  requires:
 *  - x:  horizontal position (0-)
 *  - y:  vertical position (0-)
 */

void LCD_DotPos(uint8_t x, uint8_t y)
{
  uint8_t           Temp;     /* temp. value */

  /* vertical position (bank) */
  LCD_Cmd(CMD_BANK | y);           /* set bank */

  /* horizontal position (column) */
  Temp = x;
  Temp &= 0b00001111;              /* filter lower nibble */
  LCD_Cmd(CMD_COLUMN_L | Temp);    /* set lower nibble */

  Temp = x;
  Temp >>= 4;                      /* shift upper nibble to lower */
  Temp &= 0b00000111;              /* filter nibble (just 3 bits) */
  LCD_Cmd(CMD_COLUMN_H | Temp);    /* set upper nibble */
}



/*
 *  set LCD character position
 *  - since we can't read the LCD and don't use a RAM buffer
 *    we have to move bank-wise in y direction
 *  - top left: 1/1
 *
 *  requires:
 *  - x:  horizontal position (1-)
 *  - y:  vertical position (1-)
 */

void LCD_CharPos(uint8_t x, uint8_t y)
{
  /* update UI */
  UI.CharPos_X = x;
  UI.CharPos_Y = y;

  /*
   *  set start dot position
   *  - start is left top of character
   */

  /* horizontal position (column) */
  x--;                             /* columns start at 0 */
  x *= FONT_SIZE_X;                /* offset for character */
  X_Start = x;                     /* update start position */

  /* vertical position (bank) */
  y--;                             /* banks start at 0 */
  y *= CHAR_BANKS;                 /* offset for character */
  Y_Start = y;                     /* update start position */

  LCD_DotPos(x, y);                /* set dot position */
}



/*
 *  clear one single character line
 *
 *  requires:
 *  - Line: line number (1-)
 *    special case line 0: clear remaining space in current line
 */

void LCD_ClearLine(uint8_t Line)
{
  uint8_t           MaxBank;            /* bank limit */
  uint8_t           n = 1;              /* counter */  

  if (Line == 0)         /* special case: rest of current line */
  {
    Line = UI.CharPos_Y;      /* current line */
    n = UI.CharPos_X;         /* current character position */
  }

  LCD_CharPos(n, Line);       /* set char position */

  /* calculate banks */
  Line = Y_Start;                       /* get start bank */
  MaxBank = Line + CHAR_BANKS;          /* end bank + 1 */

  /* clear line */
  while (Line < MaxBank)           /* loop through banks */
  {
    LCD_DotPos(X_Start, Line);     /* set dot position */

    /* clear bank */
    n = X_Start;              /* reset counter */
    while (n < 96)            /* for all columns */
    {
      LCD_Data(0);            /* send empty byte */
      n++;                    /* next byte */
    }

    Line++;                   /* next bank */
  }
}



/*
 *  clear the display
 */ 

void LCD_Clear(void)
{
  uint8_t           Bank = 0;      /* bank counter */
  uint8_t           Pos;           /* column counter */

  /* we have to clear all dots manually :-( */

  LCD_DotPos(0, 0);           /* set start address */

  while (Bank < LCD_BANKS)         /* loop through all banks */
  {
    Pos = 0;                       /* start at the left */

    while (Pos < 96)               /* for all 96 columns */
    {
      LCD_Data(0);                 /* send empty byte */
      Pos++;                       /* next column */
    }

    Bank++;                        /* next bank */           
  }

  LCD_CharPos(1, 1);          /* reset character position */
}



/*
 *  set contrast
 *
 *  required:
 *  - value: 0-31
 */

void LCD_Contrast(uint8_t Contrast)
{
  uint8_t           Temp;          /* temp. value */ 

  /* set contrast */
  Temp = Contrast;
  Temp &= 0b00011111;              /* filter lower nibble (5 bits) */
  LCD_Cmd(CMD_VOP_L | Temp);       /* set lower nibble */

  Temp = Contrast;
  Temp >>= 5;                      /* shift upper nibble by 5 bits */
  Temp &= 0b00000111;              /* filter nibble (just 3 bits) */
  LCD_Cmd(CMD_VOP_H | Temp);       /* set upper nibble */

  NV.Contrast = Contrast;          /* update value */
}



/*
 *  initialize LCD
 */
 
void LCD_Init(void)
{
  #ifdef LCD_RESET
  /* reset display */
  LCD_PORT &= ~(1 << LCD_RESET);        /* set /RES low */
  wait3us();                            /* wait >1µs */
  LCD_PORT |= (1 << LCD_RESET);         /* set /RES high */
  wait3us();                            /* wait 3µs */
  #endif

  /* default: display off */
  /* default: horizontal addressing mode */

  /* enable charge pump */
  LCD_Cmd(CMD_PWR_CONTROL | FLAG_PWR_ON);

  /* set pixel mode: normal */
  LCD_Cmd(CMD_PIXEL_MODE | FLAG_PIXEL_NORMAL);

  /* set display mode: normal */
  LCD_Cmd(CMD_DISP_MODE | FLAG_DISP_NORMAL);

  /* set contrast: default value */
  LCD_Contrast(LCD_CONTRAST);

  /* horizontal mirroring (x-flip) is controlled by PCF8814's MX pin */

  /* vertical mirroring (y-flip) */
  #ifdef LCD_FLIP_Y
  LCD_Cmd(CMD_VERT_MIRROR | FLAG_Y_MIRROR_ON);    /* flip vertically */
  #endif

  /* switch display on */
  LCD_Cmd(CMD_DISPLAY | FLAG_DISPLAY_ON);

  /* display needs about ???ms */

  /* update maximums */
  UI.CharMax_X = LCD_CHAR_X;       /* characters per line */
  UI.CharMax_Y = LCD_CHAR_Y;       /* lines */
  UI.MaxContrast = 255;            /* maximum LCD contrast */
  #ifdef SW_SYMBOLS
  UI.SymbolSize_X = LCD_SYMBOL_CHAR_X;  /* x size in chars */
  UI.SymbolSize_Y = LCD_SYMBOL_CHAR_Y;  /* y size in chars */
  #endif

  LCD_Clear();                /* clear display */
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
  uint8_t           Index;         /* font index */
  uint16_t          Offset;        /* address offset */
  uint8_t           Bank;          /* bank number */
  uint8_t           x;             /* bitmap x byte counter */
  uint8_t           y = 1;         /* bitmap y byte counter */

  /* prevent x overflow */
  if (UI.CharPos_X > LCD_CHAR_X) return;

  /* get font index number from lookup table */
  Table = (uint8_t *)&FontTable;        /* start address */
  Table += Char;                        /* add offset for character */
  Index = pgm_read_byte(Table);         /* get index number */
  if (Index == 0xff) return;            /* no character bitmap available */

  /* calculate start address of character bitmap */
  Table = (uint8_t *)&FontData;        /* start address of font data */
  Offset = FONT_BYTES_N * Index;       /* offset for character */
  Table += Offset;                     /* address of character data */

  Bank = Y_Start;                  /* get start bank */

  /* read character bitmap and send it to display */
  while (y <= FONT_BYTES_Y)
  {
    LCD_DotPos(X_Start, Bank);          /* set start position */

    /* read and send all column bytes for this row */
    x = 1;
    while (x <= FONT_BYTES_X)
    {
      Index = pgm_read_byte(Table);     /* read byte */
      LCD_Data(Index);                  /* send byte */
      Table++;                          /* address for next byte */
      x++;                              /* next byte */
    }

    Bank++;                             /* next bank */
    y++;                                /* next row */
  }

  /* update character position */
  UI.CharPos_X++;                  /* next character in current line */
  X_Start += FONT_SIZE_X;          /* also update X dot position */
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
  LCD_CharPos(LCD_CHAR_X, LCD_CHAR_Y);       /* move to bottom right */

  if (Mode)              /* cursor on */
  {
    LCD_Char('>');
  }
  else                   /* cursor off */
  {
    LCD_Char(' ');
  }
}



/* ************************************************************************
 *   fancy stuff
 * ************************************************************************ */


#ifdef SW_SYMBOLS

/*
 *  display a component symbol
 *
 *  requires:
 *  - ID: symbol to display
 */

void LCD_Symbol(uint8_t ID)
{
  uint8_t           *Table;        /* pointer to symbol table */
  uint8_t           Data;          /* symbol data */
  uint16_t          Offset;        /* address offset */
  uint8_t           Bank;          /* bank number */
  uint8_t           x;             /* bitmap x byte counter */
  uint8_t           y = 1;         /* bitmap y byte counter */

  /* calculate start address of character bitmap */
  Table = (uint8_t *)&SymbolData;       /* start address of symbol data */
  Offset = SYMBOL_BYTES_N * ID;         /* offset for symbol */
  Table += Offset;                      /* address of symbol data */

  Bank = Y_Start;                  /* get start bank */

  /* read character bitmap and send it to display */
  while (y <= SYMBOL_BYTES_Y)
  {
    if (y > 1)                /* multi-bank bitmap */
    {
      LCD_DotPos(X_Start, Bank);        /* move to new bank */
    }

    /* read and send all column bytes for this row */
    x = 1;
    while (x <= SYMBOL_BYTES_X)
    {
      Data = pgm_read_byte(Table);      /* read byte */
      LCD_Data(Data);                   /* send byte */
      Table++;                          /* address for next byte */
      x++;                              /* next byte */
    }

    Bank++;                             /* next bank */
    y++;                                /* next row */
  }

  /* hint: we don't update the char position */
}

#endif



/* ************************************************************************
 *   clean-up of local constants
 * ************************************************************************ */


/* source management */
#undef LCD_DRIVER_C

#endif

/* ************************************************************************
 *   EOF
 * ************************************************************************ */
