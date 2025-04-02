#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
//gpio - general purpose input-output

int main() {

    rcc_periph_clock_enable(RCC_GPIOD);
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);

    rcc_periph_clock_enable(RCC_TIM6);
    timer_set_prescaler(TIM6, 8000-1);
    timer_set_period(TIM6, 1000-1);

    timer_enable_irq(TIM6, TIM_DIER_UIE);
    nvic_enable_irq(NVIC_TIM6_DAC_IRQ);

    timer_enable_counter(TIM6);

    // while(true+) не используем
    while(true) {
        
    }

}

// реакция от процессора на сигнал

void tim6_dac_isr (void){
    timer_clear_flag(TIM6, TIM_SR_UIF);
    gpio_toggle(GPIOD, GPIO15);
}