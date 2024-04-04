#include "hal.h"

static volatile uint32_t s_ticks;
void SysTick_Handler(void) { s_ticks++; }

uint32_t SystemCoreClock = FREQ;
void SystemInit(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG
    SysTick_Config(SystemCoreClock / 1000); // Tick every 1 ms
}

int main(void)
{
    uint16_t led = PIN('D', 13); // Blue LED
    gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode
    // uart_init(UART_DEBUG, 115200); // Initialise UART
    volatile uint32_t timer = 0, period = 500; // Declare timers
    for (;;) {
        if (timer_expired(&timer, period, s_ticks)) {
            static bool on; // This block is executed
            gpio_write(led, on); // Every `period` milliseconds
            on = !on; // Toggle LED state
            // uart_write_buf(UART_DEBUG, "hi\r\n", 4); // Write message
        }
        // Here we could perform other activities!
    }
    return 0;
}
