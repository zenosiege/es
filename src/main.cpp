#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
//gpio - general purpose input-output

int main() {

    rcc_periph_clock_enable(RCC_GPIOD);
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);

    rcc_periph_clock_enable(RCC_TIM6);
    timer_set_prescaler(TIM6, 8000-1);
    timer_set_period(TIM6, 1000-1);
    timer_enable_counter(TIM6);

    while(true) {
        if(timer_get_counter(TIM6) < 800){
            gpio_set(GPIOD, GPIO15);
        }
        else {
            gpio_clear(GPIOD, GPIO15);
        }
    }

}