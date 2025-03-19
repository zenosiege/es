#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
//gpio - general purpose input-output

class Rcc {
    public:
        Rcc() {
            rcc_clock_setup_pll(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
        };
};

//Rcc clock_system;

//==============================================================================
int main() {

    rcc_periph_clock_enable(RCC_GPIOD);

    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15 | GPIO13);

    for (uint32_t ptime{0}; true; ++ptime){

        gpio_toggle(GPIOD, GPIO13);

        if (ptime % 1'000'000 < 500'000) {
            gpio_set(GPIOD, GPIO15);
        }
        else {
            gpio_clear(GPIOD, GPIO15);
        }
        
    }
            
    // while (true) {
    //     gpio_toggle(GPIOD, GPIO15);
    //     for (volatile uint32_t i = 0; i<2'000'000; ++i);
    // }
}
