/**
 * File:   lcd1602.h
 * Author: Zafer Altun<info@zaferaltun.com>
 * http://www.zaferaltun.com
 * 
 * Bu kütüphane ana projede tanımlanacak olan _XTAL_FREQ değerine göre gecikme vermenizi sağlar.
 * 
 * This library allows you to give delay according to the value of _XTAL_FREQ in your main project.
 * 
 * * @2019
 * 
 **/

void delayUS(unsigned int us) {
  while (us) {
    __delay_us(1);
    us--;
  }
}

void delayMS(unsigned int us) {
  while (us) {
    __delay_ms(1);
    us--;
  }
}