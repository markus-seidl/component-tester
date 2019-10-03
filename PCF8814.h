/* ************************************************************************
 *
 *   PCF8814 graphic display controller
 *
 *   (c) 2019 by Markus Reschke
 *
 * ************************************************************************ */


/* ************************************************************************
 *   I2C
 * ************************************************************************ */


/*
 *  I2C slave addreses
 */

#define PCF8814_I2C_ADDR_0    0b00111100     /* SA1=0 SA0=0 0x3c */
#define PCF8814_I2C_ADDR_1    0b00111101     /* SA1=0 SA0=1 0x3d */
#define PCF8814_I2C_ADDR_2    0b00111110     /* SA1=1 SA0=0 0x3e */
#define PCF8814_I2C_ADDR_3    0b00111111     /* SA1=1 SA0=1 0x3f */


/*
 *  I2C master code
 *  - send in standard or fast mode to enable high speed mode
 *  - frame: <start><master code><not-acknowledge>
 */

#define LCD_MASTER_CODE       0b00001000     /* master code */


/*
 *  I2C control byte
 */

#define LCD_CONTROL_BYTE      0b00000000     /* 0x00 */
/* continuation flag */
#define FLAG_CTRL_MULTI       0b00000000     /* stream of bytes follows */
#define FLAG_CTRL_SINGLE      0b10000000     /* only 1 byte follows */
/* D/C */
#define FLAG_CTRL_CMD         0b00000000     /* command follows */
#define FLAG_CTRL_DATA        0b01000000     /* data follows */



/* ************************************************************************
 *  LCD commands
 * ************************************************************************ */


/*
 *  horizontal addressing: set lower nibble of column address (bits 0-3)
 *  - valid range: 0 - 15 (both nibbles 0 - 95)
 */

#define CMD_COLUMN_L          0b00000000     /* set lower nibble column address */


/*
 *  horizontal addressing: set upper nibble of column address (bits 4-6)
 *  - valid range: 0 - 7 (both nibbles 0 - 95)
 */

#define CMD_COLUMN_H          0b00010000     /* set upper nibble of column address */


/*
 *  power control
 *  - charge pump
 */

#define CMD_PWR_CONTROL       0b00101000     /* set power control */

                                             /* charge pump */
#define FLAG_PWR_OFF          0b00000000     /* off */
#define FLAG_PWR_ON           0b00000100     /* on */


/*
 *  set bias
 */

#define CMD_BIAS              0b00110000     /* set bias */

                                             /* alpha */
#define FLAG_BIAS_015         0b00000000     /* 0.15 */
#define FLAG_BIAS_030         0b00000001     /* 0.30 */
#define FLAG_BIAS_045         0b00000010     /* 0.45 */
#define FLAG_BIAS_060         0b00000011     /* 0.60 */
#define FLAG_BIAS_075         0b00000100     /* 0.75 */
#define FLAG_BIAS_090         0b00000101     /* 0.90 */
#define FLAG_BIAS_105         0b00000110     /* 1.05 */
#define FLAG_BIAS_135         0b00000111     /* 1.35 */


/*
 *  charge pump multiplication factor
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_CHARGE_PUMP       0b00111101     /* set charge pump */

/* byte #2: multiplicator */
#define FLAG_CHARGE_PUMP_2    0b00000000     /* 2x */
#define FLAG_CHARGE_PUMP_3    0b00000001     /* 3x */
#define FLAG_CHARGE_PUMP_4    0b00000010     /* 4x */
#define FLAG_CHARGE_PUMP_5    0b00000011     /* 5x */


/*
 *  set initial display line
 *  - valid range: 0 - 63
 */ 

#define CMD_STARTLINE         0b01000000     /* set start row address */


/*
 *  adjust contrast setting (VOP): set upper nibble (bits 5-7)
 *  - valid range: 0 - 7 (both nibbles 0 - 255)
 */

#define CMD_VOP_H             0b00100000     /* set upper nibble of VOP */


/*
 *  adjust contrast setting (VOP): set lower nibble (bits 0-4)
 *  - valid range: 0 - 31 (both nibbles 0 - 255)
 */

#define CMD_VOP_L             0b10000000     /* set lower nibble of VOP */


/*
 *  set pixel mode: normal/all-on
 */

#define CMD_PIXEL_MODE        0b10100100     /* pixel mode */

#define FLAG_PIXEL_NORMAL     0b00000000     /* normal */
#define FLAG_PIXEL_ALL        0b00000001     /* all pixels on (default) */


/*
 *  set display mode: normal/inversed
 */

#define CMD_DISP_MODE         0b10100110     /* display normal/inversed */

#define FLAG_DISP_NORMAL      0b00000000     /* normal display */
#define FLAG_DISP_REVERSED    0b00000000     /* inversed display */


/*
 *  set display: on/off
 */

#define CMD_DISPLAY           0b10101110     /* display on/off */

#define FLAG_DISPLAY_OFF      0b00000000     /* display off */
#define FLAG_DISPLAY_ON       0b00000001     /* display on */


/*
 *  data order
 */

#define CMD_DATA_ORDER        0b10101000     /* set data order */

#define FLAG_DATA_LSB_TOP     0b00000000     /* LSB top, MSB bottom */
#define FLAG_DATA_MSB_TOP     0b00000001     /* MSB top, LSB bottom */


/*
 *  RAM addressing mode
 */

#define CMD_ADDR_MODE         0b10101010     /* set addressing mode */

#define FLAG_ADDR_HOR         0b00000000     /* horizontal addressing */
#define FLAG_ADDR_VER         0b00000001     /* vertical addressing */


/*
 *  partial display position (initial row)
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_PARTIAL_POS       0b10101100     /* set initial row */

/* byte #2: initial row */
#define FLAG_PARTIAL_ROW_00   0b00000000     /* row 0 */
#define FLAG_PARTIAL_ROW_08   0b00000001     /* row 8 */
#define FLAG_PARTIAL_ROW_16   0b00000010     /* row 16 */
#define FLAG_PARTIAL_ROW_24   0b00000011     /* row 24 */
#define FLAG_PARTIAL_ROW_32   0b00000100     /* row 32 */
#define FLAG_PARTIAL_ROW_40   0b00000101     /* row 40 */
#define FLAG_PARTIAL_ROW_48   0b00000110     /* row 48 */
#define FLAG_PARTIAL_ROW_56   0b00000111     /* row 56 */


/*
 *  n-line inversion
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_NLINE_INV         0b10101101     /* set n-line inversion */  

/* byte #2: frame inversion and n-line */

/* frame inversion */
#define FLAG_FRAME_INV_OFF    0b00000000     /* disabled, n-line counter runs continously */
#define FLAG_FRAME_INV_ON     0b10000000     /* enabled, n-line counter reset */

/* n-line */
#define FLAG_NLINE_OFF        0b00000000     /* n-line inversion disabled */
#define FLAG_NLINE_02         0b00000001     /* invert every 2 rows */
/* ... */
#define FLAG_NLINE_DEFAULT    0b00001101     /* default value */
/* ... */
#define FLAG_NLINE_62         0b00011111     /* invert every 62 rows */


/*
 *  vertical addressing: set bank
 *  - valid range: 0 - 8
 *  - last bank is 1 pixel high
 */

#define CMD_BANK              0b10110000     /* set bank */


/*
 *  vertical mirroring
 */

#define CMD_VERT_MIRROR       0b11000000     /* set vertical mirroring */

#define FLAG_Y_MIRROR_OFF     0b00000000     /* mirroring disabled */
#define FLAG_Y_MIRROR_ON      0b00001000     /* mirroring enabled */


/*
 *  set partial display (mux rate)
 */

#define CMD_PARTIAL_DISP      0b11010000     /* set partial display */

/* mux rate */
#define FLAG_PARTIAL_MODE_65  0b00000000     /* 1:65 (default) */
#define FLAG_PARTIAL_MODE_56  0b00000001     /* 1:56 */
#define FLAG_PARTIAL_MODE_48  0b00000010     /* 1:48 */
#define FLAG_PARTIAL_MODE_40  0b00000011     /* 1:40 */
#define FLAG_PARTIAL_MODE_32  0b00000100     /* 1:32 */
#define FLAG_PARTIAL_MODE_24  0b00000101     /* 1:24 */
#define FLAG_PARTIAL_MODE_16  0b00000110     /* 1:16 */
#define FLAG_PARTIAL_MODE_08  0b00000111     /* 1:8 */


/*
 *  read ID 1
 *  - returns always 0
 */

#define CMD_READ_ID1          0b11011010     /* read ID 1 */


/*
 *  read ID 2
 *  - returns always 1
 */

#define CMD_READ_ID2          0b11011011     /* read ID 2 */


/*
 *  read ID 3
 */

#define CMD_READ_ID3          0b11011100     /* read ID 3 */


/*
 *  read ID 4
 */

#define CMD_READ_ID4          0b11011101     /* read ID 4 */


/*
 *  temperature sense (read temperature)
 *  - T = (1.875 * value - 40)°C
 */

#define CMD_READ_T            0b11011110     /* read temperature */


/*
 *  read voltage monitor
 *  - get status of charge pump
 */

#define CMD_READ_V_MONITOR    0b11011111     /* read voltage monitor */

/* charge pump status */
#define FLAG_CHARGE_PUMP_BAD  0b00000000     /* not working correctly */
#define FLAG_CHARGE_PUMP_OK   0b00000001     /* working correctly */


/*
 *  row control
 */

#define CMD_ROW_CONTROL       0b11100000     /* set row control */

/* bottom row swap */
#define FLAG_ROW_SWAP_OFF     0b00000000     /* bottom rows aren't mirrored (default) */
#define FLAG_ROW_SWAP_ON      0b00000001     /* bottom rows are mirrored */


/*
 *  software reset
 */

#define CMD_RESET             0b11100010     /* internal reset */


/*
 *  no operation (NOP)
 */

#define CMD_NOP               0b11100011     /* NOP */


/*
 *  display data length for 3-line SPI
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_DATA_LENGTH       0b11101000     /* set data length */

/* byte #2: 8-bit value */


/*
 *  temperature compensation: set TC slopes A and B
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_TC_SLOPES_AB      0b00111000     /* select slopes for A&B */

/* byte #2: slopes for A&B */

/* slope for segment A */
#define FLAG_TC_A_000         0b00000000     /* 0.00  0mV/K */
#define FLAG_TC_A_025         0b00000001     /* 0.25 -4mV/K */
#define FLAG_TC_A_050         0b00000010     /* 0.50 -8mV/K */
#define FLAG_TC_A_075         0b00000011     /* 0.75 -12mV/K */
#define FLAG_TC_A_100         0b00000100     /* 1.00 -16mV/K */
#define FLAG_TC_A_125         0b00000101     /* 1.25 -20mV/K */
#define FLAG_TC_A_175         0b00000110     /* 1.75 -24mV/K */
#define FLAG_TC_A_250         0b00000111     /* 2.50 -40mV/K */

/* slope for segment B */
#define FLAG_TC_B_000         0b00000000     /* 0.00  0mV/K */
#define FLAG_TC_B_025         0b00010000     /* 0.25 -4mV/K */
#define FLAG_TC_B_050         0b00100000     /* 0.50 -8mV/K */
#define FLAG_TC_B_075         0b00110000     /* 0.75 -12mV/K */
#define FLAG_TC_B_100         0b01000000     /* 1.00 -16mV/K */
#define FLAG_TC_B_125         0b01010000     /* 1.25 -20mV/K */
#define FLAG_TC_B_175         0b01100000     /* 1.75 -24mV/K */
#define FLAG_TC_B_250         0b01110000     /* 2.50 -40mV/K */


/*
 *  temperature compensation: set TC slopes C and D
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_TC_SLOPES_CD      0b00111001     /* select slopes for C&D */

/* byte #2: slopes for C&D */

/* slope for segment C */
#define FLAG_TC_C_000         0b00000000     /* 0.00  0mV/K */
#define FLAG_TC_C_025         0b00000001     /* 0.25 -4mV/K */
#define FLAG_TC_C_050         0b00000010     /* 0.50 -8mV/K */
#define FLAG_TC_C_075         0b00000011     /* 0.75 -12mV/K */
#define FLAG_TC_C_100         0b00000100     /* 1.00 -16mV/K */
#define FLAG_TC_C_125         0b00000101     /* 1.25 -20mV/K */
#define FLAG_TC_C_175         0b00000110     /* 1.75 -24mV/K */
#define FLAG_TC_C_250         0b00000111     /* 2.50 -40mV/K */

/* slope for segment D */
#define FLAG_TC_D_000         0b00000000     /* 0.00  0mV/K */
#define FLAG_TC_D_025         0b00010000     /* 0.25 -4mV/K */
#define FLAG_TC_D_050         0b00100000     /* 0.50 -8mV/K */
#define FLAG_TC_D_075         0b00110000     /* 0.75 -12mV/K */
#define FLAG_TC_D_100         0b01000000     /* 1.00 -16mV/K */
#define FLAG_TC_D_125         0b01010000     /* 1.25 -20mV/K */
#define FLAG_TC_D_175         0b01100000     /* 1.75 -24mV/K */
#define FLAG_TC_D_250         0b01110000     /* 2.50 -40mV/K */


/*
 *  temperature compensation: enable/disable TC
 */

#define CMD_TC_MODE           0b1110101      /* set TC mode */

#define FLAG_TC_OFF           0b0000000      /* disable TC */
#define FLAG_TC_ON            0b0000001      /* enable TC (default) */


/*
 *  oscillator selection
 */

#define CMD_OSCILLATOR        0b00111010     /* select oscillator */

#define FLAG_OSC_INT          0b00000000     /* internal oscillator (default) */
#define FLAG_OSC_EXT          0b00000001     /* external oscillator */


/*
 *  set factory defaults
 */

#define CMD_DEFAULTS          0b00111110     /* set factory defaults */

#define FLAG_DEFAULTS_OFF     0b00000000     /* disable defaults */
#define FLAG_DEFAULTS_ON      0b00000001     /* enable defaults */


/*
 *  frame frequency
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_FRAME_FREQ        0b11101111     /* set frame frequency */

/* byte #2: frame frequency */
#define FLAG_FRAME_FREQ_80    0b00000000     /* 80Hz (default) */
#define FLAG_FRAME_FREQ_70    0b00000001     /* 70Hz */
#define FLAG_FRAME_FREQ_60    0b00000010     /* 60Hz */
#define FLAG_FRAME_FREQ_40    0b00000011     /* 40Hz */


/*
 *  OTP programming
 */

#define CMD_OTP_PROG          0b11110000     /* set OTP mode */

#define FLAG_CAL_MODE_EXIT    0b00000000     /* exit calibration mode */
#define FLAG_CAL_MODE_ENTER   0b00000001     /* enter calibration mode */
#define FLAG_OTP_PROG_OFF     0b00000000     /* disable OTP programming */
#define FLAG_OTP_PROG_ON      0b00000010     /* enable OTP programming */


/*
 *  load 0: write 0 to OTP shift register
 *  - when in calibration mode
 */

#define CMD_LOAD_0            0b11011000     /* write 0 to OTP */


/*
 *  load 1: write 1 to OTP shift register
 *  - when in calibration mode
 */

#define CMD_LOAD_1            0b11011001     /* write 1 to OTP */


/*
 *  horizontal mirroring
 *  - pseudo command, treated as NOP
 *  - mirroring is set by PCF8814's MX pin
 */

#define CMD_HOR_MIRROR        0b10100000     /* set horizontal mirroring */

#define FLAG_X_MIRROR_OFF     0b00000000     /* mirroring disabled */
#define FLAG_X_MIRROR_ON      0b00000001     /* mirroring enabled */



/* ************************************************************************
 *   EOF
 * ************************************************************************ */
