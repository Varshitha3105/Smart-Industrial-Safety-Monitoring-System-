#include "head.h"
#include<lpc21xx.h>

/* LED DEFINITIONS */

#define GREEN  (1<<17)
#define YELLOW (1<<18)
#define RED    (1<<19)

/* SENSOR DEFINITIONS */

#define PIR   (1<<30)
#define FLAME (1<<5)

/* STATUS DEFINITIONS */

#define SAFE       0
#define WARNING    1
#define DANGER     2
#define EMERGENCY  3

/* GLOBAL VARIABLES */

volatile unsigned int emergency_flag = 0;

unsigned int adcval;
unsigned int temperature;

unsigned int pir_status;
unsigned int flame_status;

unsigned int machine_status;


/* EXTERNAL INTERRUPT ISR */

void eint0_handler(void)__irq
{
    emergency_flag = 1;

    EXTINT = 1<<0;

    VICVectAddr = 0;
}


/* EINT CONFIGURATION */

void config_eint()
{
    PINSEL1 &= ~(3<<0);
    PINSEL1 |= (1<<0);

    EXTMODE = 1<<0;

    EXTPOLAR = 0;

    EXTINT = 1<<0;
}


/* VIC CONFIGURATION */

void config_vic_eint()
{
    VICIntSelect &= ~(1<<14);

    VICVectCntl0 = (1<<5)|14;

    VICVectAddr0 = (unsigned int)eint0_handler;

    VICIntEnable |= 1<<14;
}


/* GPIO INITIALIZATION */

void gpio_init()
{
    /* Flame sensor P0.5 as GPIO */

    PINSEL0 &= ~(3<<10);

    /* LED pins as GPIO */

    PINSEL1 &= ~((3<<2)|(3<<4)|(3<<6));

    /* PIR P0.30 as GPIO */

    PINSEL1 &= ~(3<<28);

    /* LED pins as output */

    IODIR0 |= GREEN|YELLOW|RED;

    /* PIR and Flame pins as input */

    IODIR0 &= ~(PIR|FLAME);

    /* Initially LEDs OFF */

    IOCLR0 = GREEN|YELLOW|RED;
}


/* TEMPERATURE READING */

void temperature_read()
{
    adcval = adc_read(0);

    /*
       ADC reference = 3.3V

       LM35 output = 10mV per degree Celsius

       Temperature in degree C * 10:

       ADC value * 3300 / 1023

       Example:
       365 means 36.5 degree C
    */

    temperature = (adcval*3300)/1023;
}


/* PIR SENSOR READING */

void pir_read()
{
    if(IOPIN0 & PIR)
        pir_status = 1;

    else
        pir_status = 0;
}


/* FLAME SENSOR READING */

void flame_read()
{
    /* Assuming active LOW flame sensor */

    if((IOPIN0 & FLAME) == 0)
        flame_status = 1;

    else
        flame_status = 0;
}


/* MACHINE SAFETY CHECKING */

void safety_check()
{
    if(emergency_flag == 1)
    {
        machine_status = EMERGENCY;
    }

    else if(flame_status == 1 || temperature >= 500)
    {
        machine_status = DANGER;
    }

    else if(temperature >= 400)
    {
        machine_status = WARNING;
    }

    else
    {
        machine_status = SAFE;
    }
}


/* LED CONTROL */

void led_control()
{
    IOCLR0 = GREEN|YELLOW|RED;

    if(machine_status == SAFE)
    {
        IOSET0 = GREEN;
    }

    else if(machine_status == WARNING)
    {
        IOSET0 = YELLOW;
    }

    else
    {
        IOSET0 = RED;
    }
}


/* LCD DISPLAY */

void lcd_display()
{
    /* FIRST LINE */

    lcd_cmnd(0x80);

    lcd_string("TEMP: ");

    lcd_int(temperature/10);

    lcd_data('.');

    lcd_int(temperature%10);

    lcd_string(" C    ");


    /* SECOND LINE */

    lcd_cmnd(0xC0);

    if(machine_status == SAFE)
    {
        lcd_string("STATUS: SAFE    ");
    }

    else if(machine_status == WARNING)
    {
        lcd_string("STATUS: WARNING ");
    }

    else if(machine_status == DANGER)
    {
        lcd_string("STATUS: DANGER  ");
    }

    else
    {
        lcd_string("STATUS: EMERG   ");
    }
}


/* UART DISPLAY */

void uart_display()
{
    uart0_tx_string("\r\n--------------------------");

    uart0_tx_string("\r\nSMART INDUSTRIAL SAFETY");

    uart0_tx_string("\r\n--------------------------");

    /* TEMPERATURE */

    uart0_tx_string("\r\nTemperature: ");

    uart0_int(temperature/10);

    uart0_tx('.');

    uart0_int(temperature%10);

    uart0_tx_string(" C");

    /* PIR STATUS */

    uart0_tx_string("\r\nPIR Sensor: ");

    if(pir_status)
        uart0_tx_string("Worker Detected");

    else
        uart0_tx_string("No Movement");

    /* FLAME STATUS */

    uart0_tx_string("\r\nFlame Sensor: ");

    if(flame_status)
        uart0_tx_string("FIRE DETECTED");

    else
        uart0_tx_string("NO FIRE");

    /* MACHINE STATUS */

    uart0_tx_string("\r\nMachine Status: ");

    if(machine_status == SAFE)
        uart0_tx_string("SAFE");

    else if(machine_status == WARNING)
        uart0_tx_string("WARNING");

    else if(machine_status == DANGER)
        uart0_tx_string("DANGER");

    else
        uart0_tx_string("EMERGENCY");

    /* LED STATUS */

    uart0_tx_string("\r\nLED Status: ");

    if(machine_status == SAFE)
        uart0_tx_string("GREEN LED ON");

    else if(machine_status == WARNING)
        uart0_tx_string("YELLOW LED ON");

    else
        uart0_tx_string("RED LED ON");

    uart0_tx_string("\r\n--------------------------\r\n");
}


/* MAIN FUNCTION */

int main()
{
    /* PCLK = 15MHz assuming CCLK = 60MHz */

    VPBDIV = 0x00;

    /* GPIO INITIALIZATION */

    gpio_init();

    /* ADC INITIALIZATION */

    adc_init();

    /* UART INITIALIZATION */

    uart0_init(9600);

    /* LCD INITIALIZATION */

    lcd_init();

    /* EXTERNAL INTERRUPT */

    config_eint();

    config_vic_eint();


    /* STARTING MESSAGE */

    lcd_cmnd(0x01);

    lcd_string("INDUSTRIAL SAFE");

    lcd_cmnd(0xC0);

    lcd_string("SYSTEM STARTED");

    uart0_tx_string("\r\nSMART INDUSTRIAL SAFETY SYSTEM STARTED\r\n");

    delay_sec(2);


    while(1)
    {
        /* READ TEMPERATURE */

        temperature_read();

        /* READ PIR SENSOR */

        pir_read();

        /* READ FLAME SENSOR */

        flame_read();

        /* CHECK SAFETY CONDITIONS */

        safety_check();

        /* CONTROL LEDs */

        led_control();

        /* LCD DISPLAY */

        lcd_display();

        /* UART DISPLAY */

        uart_display();

        delay_ms(500);
    }
}
