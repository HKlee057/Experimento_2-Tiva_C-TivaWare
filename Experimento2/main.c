//*****************************************************************************
//Hector Alejandro Klée González
//Carné 17118
//Ejercicio 2 - TivaWare
//Electrónica Digital 2
//*****************************************************************************
//Librerías
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
//*****************************************************************************
//Variables utilizadas
//*****************************************************************************
uint8_t state;
uint8_t LED;
uint8_t BLUE;
uint8_t GREEN;
uint8_t RED;
//*****************************************************************************
//Inicialización de Timer0
//*****************************************************************************
void Timer0Init(void)
{
    // Habilitación del módulo de Timer0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    // Configuraciones del Timer
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() -1);
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);
}
//*****************************************************************************
//Handler Timer0
//*****************************************************************************
void Timer0IntHandler(void)
{
    // Limpiar bandera
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    // Comparación de variable de estado
    if (state == 1)
    {
        // En caso de ser 1, apagar todas las salidas y establecer el estado como 0
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0x00);
        state = 0;
    } else
    {
        // En caso de ser 0, establecer el estado como 1 y realizar comparacionen
        state = 1;
        // Si la variable ingresada es igual a RED, encender el pin correspondiente
        if(RED)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
        } else
        {
            // Caso contrario, apagar el pin que realiza el color
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0x00);
        }
        // Repetición de la comparación anterior dos veces (una para verde, otra para rojo)
        if(GREEN)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        } else
        {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_3,0x00);
        }
        if(BLUE)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        } else
        {
            GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,0x00);
        }
    }
}
//*****************************************************************************
//Inicialización de UART0
//*****************************************************************************
void UARTInit (void)
{
    // Habilitación de entradas y salidas del Puerto A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // Habilitación del módulo UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Habilitación de pines de control
    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);
    // Configuraciones del UART
    UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(),115200,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    IntEnable(INT_UART0);
    UARTEnable(UART0_BASE);
    // Mensaje a desplegar
    UARTCharPut(UART0_BASE, 'I');
    UARTCharPut(UART0_BASE, 'N');
    UARTCharPut(UART0_BASE, 'G');
    UARTCharPut(UART0_BASE, 'R');
    UARTCharPut(UART0_BASE, 'E');
    UARTCharPut(UART0_BASE, 'S');
    UARTCharPut(UART0_BASE, 'E');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'C');
    UARTCharPut(UART0_BASE, 'A');
    UARTCharPut(UART0_BASE, 'R');
    UARTCharPut(UART0_BASE, 'A');
    UARTCharPut(UART0_BASE, 'C');
    UARTCharPut(UART0_BASE, 'T');
    UARTCharPut(UART0_BASE, 'E');
    UARTCharPut(UART0_BASE, 'R');
}
//*****************************************************************************
//Handler UART0
//*****************************************************************************
void UART0IntHandler(void){
    // Limpiar bandera
    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT);
    // Lectura de dato ingresado en la consola
    LED = UARTCharGet(UART0_BASE);
    // Comparaciones realizadas dependiendo del dato ingresado
    if (LED == 'r'){
        RED =~ RED;
    }
    else if (LED == 'g'){
        GREEN =~ GREEN;
     }
     else if (LED == 'b'){
         BLUE =~ BLUE;
    }
}
//*****************************************************************************
//Main Void
//*****************************************************************************
int main(void)
{
    // Configuración de Reloj a 40 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL |  SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    // Habilitación de Puerto F y sus salidas respectivas
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    // Funciones de incialización tanto de Timer0 como UART0 (vistas al inicio)
    Timer0Init();
    UARTInit();
    // Loop infinitio
    while (1){

    }
}
