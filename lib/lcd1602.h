/**
 * File:   lcd1602.h
 * Author: Zafer Altun<info@zaferaltun.com>
 * http://www.zaferaltun.com
 * 
 * Bu kütüphane 1602 LCD cihanızı 8 bit, 4 bit tek port ve 4 bit çoklu port modlarında kullanabilmenizi sağlar.
 * 
 * This library allows you to use 1602 LCD device in 8 bit parallel mode, 4 bit single port and 4 bit multi port modes.
 *  
 * @2019
 * 
 **/

/**
 * LCD Command Sheet, Original Documentation
 * 
    Function Set: 8-bit, 1 Line, 5x7 Dots	0x30	48
    Function Set: 8-bit, 2 Line, 5x7 Dots	0x38	56
    Function Set: 4-bit, 1 Line, 5x7 Dots	0x20	32
    Function Set: 4-bit, 2 Line, 5x7 Dots	0x28	40
    Entry Mode	0x06	6
    Display off Cursor off
    (clearing display without clearing DDRAM content)	0x08	8
    Display on Cursor on	0x0E	14
    Display on Cursor off	0x0C	12
    Display on Cursor blinking	0x0F	15
    Shift entire display left	0x18	24
    Shift entire display right	0x1C	30
    Move cursor left by one character	0x10	16
    Move cursor right by one character	0x14	20
    Clear Display (also clear DDRAM content)	0x01	1
    Set DDRAM address or cursor position on display	0x80 + address*	128 + address*
    Set CGRAM address or set pointer to CGRAM location	0x40 + address** + 8	64 + address** + 8
 **/

typedef struct LCD_1602 {
  unsigned char *regPort;
  unsigned int regRS;
  unsigned int regRW;
  unsigned int regEN;
  unsigned char *dataPort;
  unsigned int use4Bit;
} LCD_1602;

const unsigned int LCD_CMD_CLEAR = 0x01;
const unsigned int LCD_CMD_INCREMENT_CURSOR = 0x06;
const unsigned int LCD_CMD_DISPLAY_ON_CURSOR_OFF = 0x0C;
const unsigned int LCD_CMD_DISPLAY_CURSOR_ON = 0x0E;
const unsigned int LCD_CMD_LCD_DISPLAY_CURSOR_ON = 0x0F;
const unsigned int LCD_CMD_USE_4_BIT_1_LINE_5x7_MTX = 0x20;
const unsigned int LCD_CMD_USE_4_BIT_2_LINE_5x7_MTX = 0x28;
const unsigned int LCD_CMD_USE_8_BIT_2_LINE_5x7_MTX = 0x38;
const unsigned int LCD_CMD_FIRST_LINE_FIRST_POS = 0x80;
const unsigned int LCD_CMD_JUMP_TO_SECOND_LINE = 0x3C;
const unsigned int LCD_CMD_SECOND_LINE_FIRST_POS = 0xC0;
const unsigned int LCD_CMD_CUSTOM_CHAR_DEF_CGRAM_ADDR_0 = 0x40;
const unsigned int LCD_CMD_CUSTOM_CHAR_DEF_CGRAM_ADDR_1 = 0x48;
const unsigned int LCD_CMD_CUSTOM_CHAR_DEF_CGRAM_ADDR_2 = 0x50;
const unsigned int LCD_CMD_CUSTOM_CHAR_DEF_CGRAM_ADDR_3 = 0x58;
const unsigned int LCD_CMD_CUSTOM_CHAR_DEF_CGRAM_ADDR_4 = 0x60;
const unsigned int LCD_CMD_CUSTOM_CHAR_DEF_CGRAM_ADDR_5 = 0x68;
const unsigned int LCD_CMD_CUSTOM_CHAR_DEF_CGRAM_ADDR_6 = 0x70;
const unsigned int LCD_CMD_CUSTOM_CHAR_DEF_CGRAM_ADDR_7 = 0x78;

void lcdInit(LCD_1602 lcd);
void lcdClear(LCD_1602 lcd);
void lcdProcess(LCD_1602 lcd, unsigned int cmd, unsigned int isData);
void lcdCommand(LCD_1602 lcd, unsigned int cmd);
void lcdData(LCD_1602 lcd, const char str[16]);
void lcdDataInt(LCD_1602 lcd, int num);
void lcdDataFloat(LCD_1602 lcd, float num, int precision);
void lcdPutChar(LCD_1602 lcd, unsigned char str);
void lcdDelay(void);
void lcdSetRegPin(LCD_1602 lcd, unsigned char pin);
void lcdClearRegPin(LCD_1602 lcd, unsigned char pin);

void lcdInit(LCD_1602 lcd) {
  // 4 bit mod için özel komutları gönder
  if (lcd.use4Bit == 1) {
    lcdCommand(lcd, 0x33);
    lcdCommand(lcd, 0x32);
  }

  // Rutini başlat
  lcdCommand(lcd, lcd.use4Bit == 1 ? LCD_CMD_USE_4_BIT_2_LINE_5x7_MTX : LCD_CMD_USE_8_BIT_2_LINE_5x7_MTX);
  lcdCommand(lcd, LCD_CMD_FIRST_LINE_FIRST_POS);
  lcdCommand(lcd, LCD_CMD_DISPLAY_ON_CURSOR_OFF);
  lcdCommand(lcd, LCD_CMD_INCREMENT_CURSOR);

  lcdClear(lcd);
}

void lcdClear(LCD_1602 lcd) {
  lcdCommand(lcd, LCD_CMD_CLEAR);
}

void lcdProcess(LCD_1602 lcd, unsigned int cmd, unsigned int isData) {
  if (lcd.use4Bit == 1) {
    // 4 bit modunda 1 byte'ı, 2 parça 4 bit'lik veri şeklinde gönder

    // Önce ilk 4 biti gönder
    *lcd.dataPort = (((cmd & 0xF0) >> 4) << 4) | (*lcd.dataPort & 0x0F);
    
    // Data byte 'ı komut ise RS = 0 (low), gösterilecek veri ise RS = 1 (high) olmalı
    if (isData == 1) {
      lcdSetRegPin(lcd, lcd.regRS);
    } else {
      lcdClearRegPin(lcd, lcd.regRS);
    }

    // RW bitini kapat
    lcdClearRegPin(lcd, lcd.regRW);
    
    // E bitini bir süre açtıktan sonra tekrar kapat
    lcdSetRegPin(lcd, lcd.regEN);
    lcdDelay();
    lcdClearRegPin(lcd, lcd.regEN);

    // Sonra son 4 biti gönder
    *lcd.dataPort = ((cmd & 0x0F) << 4) | (*lcd.dataPort & 0x0F);
    
    // E bitini bir süre açtıktan sonra tekrar kapat
    lcdSetRegPin(lcd, lcd.regEN);
    lcdDelay();
    lcdClearRegPin(lcd, lcd.regEN);

    lcdDelay();
  } else {
    // Data byte 'ı komut ise RS = 0 (low), gösterilecek veri ise RS = 1 (high) olmalı
    if (isData == 1) {
      lcdSetRegPin(lcd, lcd.regRS);
    } else {
      lcdClearRegPin(lcd, lcd.regRS);
    }
    
    // RW 'yi kapat
    lcdClearRegPin(lcd, lcd.regRW);
    
    // Datayı yaz
    *lcd.dataPort = cmd;
    
    // E bitini bir süre açtıktan sonra tekrar kapat
    lcdSetRegPin(lcd, lcd.regEN);
    lcdDelay();
    lcdClearRegPin(lcd, lcd.regEN);
  }
}

void lcdCommand(LCD_1602 lcd, unsigned int cmd) {
  lcdProcess(lcd, cmd, 0);
}

void lcdData(LCD_1602 lcd, const char str[16]) {
  int c = 0;
  while (str[c] != '\0') {
    lcdPutChar(lcd, str[c]);
    c++;
  }
}

void lcdDataInt(LCD_1602 lcd, int num){
  char buf[16];
  sprintf(buf, "%d", num);
  lcdData(lcd, buf);
}

void lcdDataFloat(LCD_1602 lcd, float num, int precision) {
  int status;
  char* str = ftoa(num, &status);
  int c = -1;
  int di = -1;
  while (str[c + 1] != '\0' && c < 16){
    c++;
    if (str[c] == '.'){
      di = c;  
    } else if (di != -1 && c - di > precision){
      if (c - di - 1 == precision){
        str[c - 1] = str[c] >= '5' ? str[c - 1] + 1 : str[c - 1]; // round up if possible
      }
      str[c] = '\0';
    }
  }
  
  lcdData(lcd, str);
}

void lcdPutChar(LCD_1602 lcd, unsigned char str) {
  lcdProcess(lcd, str, 1);
}

void lcdDelay(void) {
  delayUS(200);
}

void lcdSetRegPin(LCD_1602 lcd, unsigned char pin) {
  *lcd.regPort |= 1 << pin;
}

void lcdClearRegPin(LCD_1602 lcd, unsigned char pin) {
  *lcd.regPort &= ~(1 << pin);
}
