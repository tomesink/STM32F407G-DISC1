#ifndef HAL_H
#define HAL_H

#include <inttypes.h>
#include <stdbool.h>

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

// GPIO
struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIO(bank) ((struct gpio*)(0x40020000 + 0x400 * (bank)))

// Enum values are per datasheet: 0, 1, 2, 3
enum { GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG };


struct rcc {
    volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR,
        RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR,
        RESERVED2, APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR,
        AHB3LPENR, RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR, CSR,
        RESERVED6[2], SSCGR, PLLI2SCFGR;
};
#define RCC ((struct rcc*)0x40023800)

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

// Delay function for blink
static inline void spin(volatile uint32_t count)
{
    while (count--)
        (void)0;
}


// Systick
struct systick {
    volatile uint32_t CTRL, LOAD, VAL, CALIB;
};
#define SYSTICK ((struct systick *) 0xe000e010)

static volatile uint32_t s_ticks; // volatile is important!!

static inline void systick_init(uint32_t ticks)
{
    if ((ticks - 1) > 0xffffff)
        return; // Systick timer is 24 bit
    SYSTICK->LOAD = ticks - 1;
    SYSTICK->VAL = 0;
    SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2); // Enable systick
    RCC->APB2ENR |= BIT(14); // Enable SYSCFG
}

void delay(unsigned ms)
{ // This function waits "ms" milliseconds
    uint32_t until
        = s_ticks + ms; // Time in a future when we need to stop
    while (s_ticks < until)
        (void)0; // Loop until then
}

// t: expiration time, prd: period, now: current time. Return true if
// expired
bool timer_expired(uint32_t* t, uint32_t prd, uint32_t now)
{
    if (now + prd < *t)
        *t = 0; // Time wrapped? Reset timer
    if (*t == 0)
        *t = now + prd; // First poll? Set expiration
    if (*t > now)
        return false; // Not expired yet, return
    *t = (now - *t) > prd ? now + prd
                          : *t + prd; // Next expiration time
    return true; // Expired, return true
}





#endif
