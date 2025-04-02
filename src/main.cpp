#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
//gpio - general purpose input-output

int main() {

    rcc_periph_clock_enable(RCC_GPIOD);
    // AF - шта? это alternate function
    gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO15);


    gpio_set_af(GPIOD, GPIO_AF2, GPIO15);

    rcc_periph_clock_enable(RCC_TIM4);
    timer_set_prescaler(TIM4, 8000-1);
    timer_set_period(TIM4, 1000-1); // период счета (число-1)

    timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_TOGGLE);
    timer_set_oc_value(TIM4, TIM_OC4, 500); //половина периода

    timer_enable_oc_output(TIM4, TIM_OC4);
    timer_enable_counter(TIM4);

    // while(true) не используем
    while(true) {
        
    }

}
// ща мы попросим линию таймера самой управлять вводом-выводом
// TIM4_CH4 - для PD15

// реакция от процессора на сигнал

void tim6_dac_isr (void){
    timer_clear_flag(TIM6, TIM_SR_UIF);
    gpio_toggle(GPIOD, GPIO15);
}