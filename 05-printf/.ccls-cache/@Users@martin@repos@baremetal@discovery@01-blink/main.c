#include <inttypes.h>
#include <stdbool.h>

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(bank) ((struct gpio*)(0x40020000 + 0x400 * (bank)))

// Enum values are per datasheet: 0, 1, 2, 3
enum { GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG };

static inline void gpio_set_mode(uint16_t pin, uint8_t mode)
{
    struct gpio* gpio = GPIO(PINBANK(pin)); // GPIO bank
    int n = PINNO(pin); // Pin number
    gpio->MODER &= ~(3U << (n * 2)); // Clear existing setting
    gpio->MODER |= (mode & 3) << (n * 2); // Set new mode
}

static inline void gpio_write(uint16_t pin, bool val)
{
    struct gpio* gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

struct rcc {
    volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR,
        RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR,
        RESERVED2, APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR,
        AHB3LPENR, RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR, CSR,
        RESERVED6[2], SSCGR, PLLI2SCFGR;
};
#define RCC ((struct rcc*)0x40023800)

// Delay function for blink
static inline void spin(volatile uint32_t count)
{
    while (count--)
        (void)0;
}

void main(void)
{
    uint16_t led = PIN('D', 13); // Orange LED
    RCC->AHB1ENR |= BIT(PINBANK(led)); // Enable GPIO clock for LED
    gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode

    for (;;) {
        gpio_write(led, true);
        spin(999999);
        gpio_write(led, false);
        spin(999999);
    }
}
