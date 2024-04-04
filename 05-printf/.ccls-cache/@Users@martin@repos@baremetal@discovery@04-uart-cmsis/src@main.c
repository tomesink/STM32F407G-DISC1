#include "../cmsis_f4/Include/stm32f407xx.h"

uint32_t SystemCoreClock = 16000000;
void SystemInit(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG
    SysTick_Config(SystemCoreClock / 1000); // Tick every 1 ms
}

#define _USART USART2

int putchar(int c)
{
    while (!(_USART->SR & USART_SR_TXE)) {
    }; // wait until we are able to transmit
    _USART->DR = c; // transmit the character
    return c;
}

int puts(const char* s)
{
    while (*s)
        putchar(*s++);

    putchar('\n');
    return 1;
}

int getchar()
{
    while (!(_USART->SR & USART_SR_RXNE)) {
    }; // wait until something received
    return _USART->DR; // find out what it is
}

int main(void)
{
    // Set pins PA2 (TX) and PA3 (RX) for serial communication
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable RCC for port A
    GPIOA->MODER |= GPIO_MODER_MODER2_1; // PA2 is Alt fn mode (serial
                                         // TX in this case)
    GPIOA->AFR[0]
        |= (7 << GPIO_AFRL_AFSEL2_Pos); // That alt fn is alt 7 for PA2
    GPIOA->MODER |= GPIO_MODER_MODER3_1; // PA3 is Alt fn mode (serial
                                         // RX in this case)
    GPIOA->AFR[0] |= (7
        << GPIO_AFRL_AFSEL3_Pos); // Alt fn for PA3 is same as for PA2

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // enable RCC for USART2

    // Set the baud rate, which requires a mantissa and fraction
    uint32_t baud_rate = 115200;
    uint16_t uartdiv = SystemCoreClock / baud_rate;
    _USART->BRR = (((uartdiv / 16) << USART_BRR_DIV_Mantissa_Pos)
        | ((uartdiv % 16) << USART_BRR_DIV_Fraction_Pos));

    // Mpw enable the USART peripheral
    _USART->CR1 |= USART_CR1_RE // enable receive
        | USART_CR1_TE // enable transmit
        | USART_CR1_UE // enable usart
        ;

    puts(
        "UART test - if you can read this, then it should be working!");
    puts("Now type something, and I will echo it to serial");
    while (1) {
        int c = getchar();
        putchar(c);
        if (c == '\r')
            putchar('\n');
    }
}
