#include "hal.h"
#include <inttypes.h>
#include <stdbool.h>

void systick_handler(void) { s_ticks++; }

int main(void)
{
    systick_init(18750000 / 1000);
    uint16_t led = PIN('D', 13); // Orange LED
    RCC->AHB1ENR |= BIT(PINBANK(led)); // Enable GPIO clock for LED
    gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode

    uint32_t timer, period = 1500; // Declare timer and 500ms period
    for (;;) {
        if (timer_expired(&timer, period, s_ticks)) {
            static bool on; // This block is executed
            gpio_write(led, on); // Every `period` milliseconds
            on = !on; // Toggle LED state
        }
        // Here we could perform other activities!
    }
    return 0;
}
