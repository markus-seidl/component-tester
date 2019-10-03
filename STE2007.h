/* ************************************************************************
 *
 *   STE2007 graphic display controller
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

#define STE2007_I2C_ADDR_0    0b00111100     /* SA1=0 SA0=0 0x3c */
#define STE2007_I2C_ADDR_1    0b00111101     /* SA1=0 SA0=1 0x3d */
#define STE2007_I2C_ADDR_2    0b00111110     /* SA1=1 SA0=0 0x3e */
#define STE2007_I2C_ADDR_3    0b00111111     /* SA1=1 SA0=1 0x3f */


/*
 *  I2C master code (optional)
 *  - send in standard or fast mode to enable high speed mode
 *  - frame: <start><master code><not-acknowledge>
 *  - STE2007 doesn't require master code for high speed mode
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
 *  set display: on/off
 */

#define CMD_DISPLAY           0b10101110     /* display on/off */

#define FLAG_DISPLAY_OFF      0b00000000     /* display off */
#define FLAG_DISPLAY_ON       0b00000001     /* display on */


/*
 *  set display mode: normal/reversed
 */

#define CMD_DISP_MODE         0b10100110     /* display normal/reversed */

#define FLAG_DISP_NORMAL      0b00000000     /* normal display */
#define FLAG_DISP_REVERSED    0b00000000     /* reversed display */


/*
 *  set pixel mode: normal/all-on
 */

#define CMD_PIXEL_MODE        0b10100100     /* pixel mode */

#define FLAG_PIXEL_NORMAL     0b00000000     /* normal */
#define FLAG_PIXEL_ALL        0b00000001     /* all pixels on */


/*
 *  set page address
 *  - valid range: 0 - 8
 *  - last page is 4 pixels high
 */

#define CMD_PAGE              0b10110000     /* set page address */


/*
 *  set upper nibble of column address (bits 4-6)
 *  - valid range: 0 - 7 (both nibbles 0 - 95)
 */

#define CMD_COLUMN_H          0b00010000     /* set upper nibble of column address */


/*
 *  set lower nibble of column address (bits 0-3)
 *  - valid range: 0 - 15 (both nibbles 0 - 95)
 */

#define CMD_COLUMN_L          0b00000000     /* set lower nibble column address */


/*
 *  set start line address
 *  - valid range: 0 - 63
 */ 

#define CMD_STARTLINE         0b01000000     /* set start line address */


/*
 *  select segment driver direction
 */

#define CMD_SEG_DIR           0b10100000     /* set segment driver direction */

#define FLAG_SEG_NORMAL       0b00000000     /* normal */
#define FLAG_SEG_REVERSED     0b00000001     /* reversed */


/*
 *  select common driver direction
 */

#define CMD_COM_DIR           0b11000000     /* set common driver direction */

#define FLAG_COM_NORMAL       0b00000000     /* normal */
#define FLAG_COM_REVERSED     0b00001000     /* reversed */


/*
 *  read ID
 *  - after issuing this command STE2007 sends the indentification byte
 */

#define CMD_READ_ID           0b11011011     /* read ID */


/*
 *  set power control
 *  - booster, voltage regulator and voltage folower
 */

#define CMD_PWR_CONTROL       0b00101000     /* set power control */

#define FLAG_PWR_OFF          0b00000000     /* all off */
#define FLAG_PWR_ON           0b00000111     /* all on */


/*
 *  set voltage range (V0R) 
 */

#define CMD_V0R               0b00100000     /* set V0R */

#define FLAG_V0R_0300         0b00000000     /* 3.00V */
#define FLAG_V0R_0428         0b00000001     /* 4.28V */
#define FLAG_V0R_0556         0b00000010     /* 5.56V */
#define FLAG_V0R_0684         0b00000011     /* 6.84V */
#define FLAG_V0R_0812         0b00000100     /* 8.12V (default) */
#define FLAG_V0R_0940         0b00000101     /* 9.40V */
#define FLAG_V0R_1068         0b00000110     /* 10.68V */
#define FLAG_V0R_1196         0b00000111     /* 11.96V */


/*
 *  adjust contrast setting (VOP)
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_VOP               0b11100001     /* set VOP */

/* byte #2: contrast value */
#define FLAG_VOP_P000         0b00000000     /* 0 (default) */
#define FLAG_VOP_P001         0b00000001     /* +1 */
/* ... */
#define FLAG_VOP_P127         0b01111111     /* +127 */
#define FLAG_VOP_M000         0b10000000     /* 0 */
#define FLAG_VOP_M001         0b10000001     /* -1 */
/* ... */
#define FLAG_VOP_M127         0b11111111     /* -127 */


/*
 *  set electronic volume (EV)
 *  - contrast setting for user
 *  - valid range: 0 - 31
 */

#define CMD_EV                0b10000000     /* set EV */


/*
 *  initialize/reset driver
 */

#define CMD_RESET             0b11100010     /* reset driver */


/*
 *  no operation (NOP)
 */

#define CMD_NOP               0b11100011     /* NOP */


/*
 *  set image location (initial row)
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_IMG_LOC           0b10101100     /* set image location */

/* byte #2: image location value */
#define FLAG_IMG_LOC_00       0b00000000     /* 0 lines */
#define FLAG_IMG_LOC_08       0b00000001     /* 8 lines */
#define FLAG_IMG_LOC_16       0b00000010     /* 16 lines */
#define FLAG_IMG_LOC_24       0b00000011     /* 24 lines */
#define FLAG_IMG_LOC_32       0b00000100     /* 32 lines */
#define FLAG_IMG_LOC_48       0b00000101     /* 48 lines */
#define FLAG_IMG_LOC_56       0b00000110     /* 56 lines */
#define FLAG_IMG_LOC_64       0b00000111     /* 64 lines */


/*
 *  set bias ratio
 */

#define CMD_BIAS_RATIO        0b00110000     /* set bias ratio */

#define FLAG_BIAS_RATIO_10    0b00000000     /* 1/10 - 81 lines */
#define FLAG_BIAS_RATIO_09    0b00000001     /* 1/9  - 65 lines */
#define FLAG_BIAS_RATIO_08    0b00000010     /* 1/8  - 49 lines */
#define FLAG_BIAS_RATIO_07    0b00000011     /* 1/7  - 33 lines */
#define FLAG_BIAS_RATIO_06    0b00000100     /* 1/6  - 25 lines */
#define FLAG_BIAS_RATIO_05    0b00000101     /* 1/5  - 17 lines */
#define FLAG_BIAS_RATIO_04    0b00000110     /* 1/4  -  9 lines */


/*
 *  set temperature compensation coefficient
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_TEMP_COMP         0b00111000     /* set temperature compensation */

/* byte #2: coefficient */
#define FLAG_TEMP_COMP_0000   0b00000000     /* 0 ppm */
#define FLAG_TEMP_COMP_0300   0b00000001     /* -300 ppm */
#define FLAG_TEMP_COMP_0600   0b00000010     /* -600 ppm */
#define FLAG_TEMP_COMP_0900   0b00000011     /* -900 ppm */
#define FLAG_TEMP_COMP_1070   0b00000100     /* -1070 ppm */
#define FLAG_TEMP_COMP_1200   0b00000101     /* -1200 ppm */
#define FLAG_TEMP_COMP_1500   0b00000110     /* -1500 ppm */
#define FLAG_TEMP_COMP_1800   0b00000111     /* -1800 ppm */


/*
 *  set charge pump multiplication factor
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_CHARGE_PUMP       0b00111101     /* set charge pump */

/* byte #2: multiplicator */
#define FLAG_CHARGE_PUMP_5    0b00000000     /* 5x */
#define FLAG_CHARGE_PUMP_4    0b00000001     /* 4x */
#define FLAG_CHARGE_PUMP_3    0b00000010     /* 3x */


/*
 *  set refresh rate
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_REFRESH_RATE      0b11101111     /* set refresh rate */

/* byte #2: refresh rate */
#define FLAG_REFRESH_RATE_80  0b00000000     /* 80Hz */
#define FLAG_REFRESH_RATE_75  0b00000001     /* 75Hz */
#define FLAG_REFRESH_RATE_70  0b00000010     /* 70Hz */
#define FLAG_REFRESH_RATE_65  0b00000011     /* 65Hz */


/*
 *  set icon mode
 */

#define CMD_ICON_MODE         0b11111000     /* set icon mode */

#define FLAG_ICON_OFF         0b00000000     /* off */
#define FLAG_ICON_ON          0b00000001     /* on */


/*
 *  N-line inversion
 *  - 2 byte command
 */

/* byte #1: command */
#define CMD_NLINE_INV         0b10101101     /* set n-line inversion */

/* byte #2:  */
#define FLAG_NLINE_OFF        0b00000000     /* N-line inversion disabled (default) */
#define FLAG_NLINE_02         0b00000001     /* row 2 */
/* ... */
#define FLAG_NLINE_32         0b00011111     /* row 32 */

#define FLAG_XOR_OFF          0b00000000     /* XOR disabled */
#define FLAG_XOR_ON           0b00100000     /* XOR enabled */


/*
 *  number of lines
 *  - multiplexing rate
 */

#define CMD_LINES             0b11010000     /* set number of lines */

#define FLAG_LINES_68         0b00000000     /* 68 lines (default) */
#define FLAG_LINES_65         0b00000001     /* 65 lines */
#define FLAG_LINES_49         0b00000010     /* 49 lines */
#define FLAG_LINES_33         0b00000011     /* 33 lines */
#define FLAG_LINES_P33        0b00000100     /* 33 lines partial display */
#define FLAG_LINES_P25        0b00000101     /* 25 lines partial display */
#define FLAG_LINES_P17        0b00000110     /* 17 lines partial display */
#define FLAG_LINES_P09        0b00000111     /* 9 lines partial display */



/* ************************************************************************
 *   EOF
 * ************************************************************************ */
