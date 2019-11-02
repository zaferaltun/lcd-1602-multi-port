/* 
 * Author: Zafer Altun <info@zaferaltun.com>
 * http://www.zaferaltun.com
 * 
 * Bu proje 1602 LCD cihazını 4 bit çoklu port modunda esnek bir şekilde nasıl kullanacağınızı gösterir.
 * 
 * This project demonstrates how you can use LCD 1602 device in 4 bit multi port mode as effectively.
 * 
 * * @2019
 * 
 */

#define _XTAL_FREQ              4000000   // Varsayılan saat frekansı, 4Mhz

// CONFIG
#pragma config FOSC = INTRCIO   // Dahili osilatör kullan, RA4 ve RA5 I/O olarak kullanılabilir
#pragma config WDTE = OFF       // Watchdog Timer kapalı

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "../lib/delay.h"
#include "../lib/lcd1602.h"

LCD_1602 lcd;

void init() {

  // B ve C portlarını dijital olarak kullanabilmek için ilgili analog girişleri kapat
  ANS4 = 0;
  ANS5 = 0;
  ANS6 = 0;
  ANS7 = 0;
  ANS8 = 0;
  ANS9 = 0;
  ANS10 = 0;
  ANS11 = 0;

  // B ve C portunun tüm pinlerini çıkış olarak tayin et
  TRISB = 0b00000000;
  PORTB = 0;
  TRISC = 0b00000000;
  PORTC = 0;

  // LCD
  lcd.dataPort = &PORTC;  // data portu olarak PORTC 'yi kullan, otomatik olarak son 4 bit kullanılacak
  lcd.regPort = &PORTB;   // kontrol portu olarak PORTB 'yi kullan, hangi pinler olacağı regRS, regRW ve regEN de tayin edilecek
  lcd.regRS = 4;
  lcd.regRW = 5;
  lcd.regEN = 6;
  lcd.use4Bit = 1;        // 4 bit modunu aktif et
  lcdInit(lcd);
}

int main(int argc, char** argv) {

  init();

  while (1) {
    lcdClear(lcd);
    lcdCommand(lcd, LCD_CMD_FIRST_LINE_FIRST_POS);
    lcdData(lcd, "MERHABA DUNYALI!");
    lcdCommand(lcd, LCD_CMD_SECOND_LINE_FIRST_POS);
    lcdData(lcd, "(:");
    delayMS(1000);
    
    lcdClear(lcd);
    lcdData(lcd, "INTEGER ORNEGI:");
    lcdCommand(lcd, LCD_CMD_SECOND_LINE_FIRST_POS);
    lcdDataInt(lcd, 1453);
    delayMS(1000);
    
    lcdClear(lcd);
    lcdData(lcd, "FLOAT ORNEGI:");
    lcdCommand(lcd, LCD_CMD_SECOND_LINE_FIRST_POS);
    lcdDataFloat(lcd, 123.456, 2);
    delayMS(1000);
  }

  return (EXIT_SUCCESS);
}