#include "head.h"
#include <lpc21xx.h>

void uart0_init(unsigned int baud)
{
    unsigned int result = 0, pclk;
    VPBDIV=1;
    if(VPBDIV == 0x00)
        pclk = 15000000;

    else if(VPBDIV == 0x01)
        pclk = 60000000;

    else if(VPBDIV == 0x02)
        pclk = 30000000;

    else
        pclk = 15000000;

    result = pclk/(16*baud);

    PINSEL0 |= 0x05;

    U0LCR = 0x83;

    U0DLL = result & 0xFF;
    U0DLM = (result >> 8) & 0xFF;

    U0LCR = 0x03;
}

#define THRE ((U0LSR>>5)&1)
#define RDR (U0LSR&1)

void uart0_tx(unsigned char data)
{
    while(THRE == 0);

    U0THR = data;
}

unsigned char uart0_rx()
{
    while(RDR == 0);

    return U0RBR;
}

void uart0_tx_string(char *ptr)
{
    while(*ptr != 0)
    {
        uart0_tx(*ptr);
        ptr++;
    }
}

void uart0_binary(int num)
{
    int pos,temp;

    for(pos=7;pos>=0;pos--)
    {
        temp = ((num>>pos)&1);
        uart0_tx(temp+48);
    }
}

void uart0_int(int num)
{
    int a[10],i=0;

    if(num==0)
    {
        uart0_tx('0');
        return;
    }

    if(num<0)
    {
        num=-num;
        uart0_tx('-');
    }

    while(num>0)
    {
        a[i]=num%10+48;
        num=num/10;
        i++;
    }

    for(i=i-1;i>=0;i--)
    {
        uart0_tx(a[i]);
    }
}

void uart0_float(float num)
{
    int whole;
    int decimal;

    whole=(int)num;
    decimal=(int)((num-whole)*100+0.5);

    uart0_int(whole);
    uart0_tx('.');

    if(decimal<10)
        uart0_tx('0');

    uart0_int(decimal);
		}
void uart0_hex(int num)
{
    char buf[10];

    sprintf(buf,"%x",num);

    uart0_tx_string(buf);
}
