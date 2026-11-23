#ifndef HEAD_H
#define HEAD_H

#include <LPC21xx.h>

/* ADC */

extern void adc_init(void);
unsigned int adc_read(unsigned int ch_num);

/* DELAY */

void delay_ms(unsigned int ms);
void delay_sec(unsigned int sec);

/* UART */

 void uart0_init(unsigned int baud);
 void uart0_tx(unsigned char data);
unsigned char uart0_rx(void);
void uart0_tx_string(char *ptr);
void uart0_int(int num);
void uart0_binary(int num);
void uart0_float(float num);
void uart0_hex(int num);

/* LCD */

void lcd_init(void);
void lcd_data(unsigned char data);
void lcd_cmnd(unsigned char cmnd);
void lcd_string(char *p);
void lcd_int(int num);

#endif
