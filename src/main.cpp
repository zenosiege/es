#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

// ПРЕРЫВАНИЕ ПО КНОПОЧКЕ (INPUT CAPTURE)
uint32_t duty;
uint32_t period;

void ic_setup(void) {
    
    rcc_periph_clock_enable(RCC_GPIOA);
    // AF - шта? это alternate function
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0);


    gpio_set_af(GPIOA, GPIO_AF2, GPIO0);

    rcc_periph_clock_enable(RCC_TIM5);
    timer_set_prescaler(TIM5, 16000 - 1);
    timer_set_period(TIM5, 10000 - 1); // период счета (число-1)

    timer_ic_set_input(TIM5, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_polarity(TIM5, TIM_IC1, TIM_IC_RISING); // направление перехода (на что реагировать - переход из нуля в единицу или наоборот (или оба))

    timer_ic_set_input(TIM5, TIM_IC2, TIM_IC_IN_TI1);
    timer_ic_set_polarity(TIM5, TIM_IC2, TIM_IC_FALLING); 

    timer_ic_enable(TIM5, TIM_IC1);
    timer_ic_enable(TIM5, TIM_IC2);


    timer_slave_set_trigger(TIM5, TIM_SMCR_TS_TI1FP1);
    timer_slave_set_mode(TIM5, TIM_SMCR_SMS_RM);

    timer_enable_irq(TIM5, TIM_DIER_CC1IE);
    nvic_enable_irq(NVIC_TIM5_IRQ);

    timer_enable_counter(TIM5);

}

int main() {
    volatile uint32_t d{};
    volatile uint32_t p{};
    
    ic_setup();
    // while(true) не используем
    while(true) {
        volatile uint32_t d = duty;
        volatile uint32_t p = period;
    }

}

void tim5_isr (void) {
    timer_clear_flag(TIM5, TIM_SR_CC1IF);
    duty = TIM_CCR2(TIM5); //импульс
    period = TIM_CCR1(TIM5); //период
}
// void tim5_isr (void) {
//     static bool start_cond{true};
//     timer_clear_flag(TIM5, TIM_SR_CC1IF); //IF - Interact FLag

//     if (start_cond) start = TIM_CCR1(TIM5);
//     else stop = TIM_CCR1(TIM5);
//     start_cond = not start_cond;
// }

// void tim6_dac_isr(void) {
//     // что здесь?
//     timer_clear_flag(TIM6, TIM_SR_UIF);

//     if (oc3_val >= 900) oc3_val = 100;
//     else oc3_val += 100;

//     if (oc4_val >= 900) oc4_val = 100;
//     else oc4_val += 100;

//     timer_set_oc_value(TIM4, TIM_OC4, oc4_val);
//     timer_set_oc_value(TIM4, TIM_OC3, oc3_val);
// }

// ща мы попросим линию таймера самой управлять вводом-выводом
// TIM4_CH4 - для PD15

// реакция от процессора на сигнал
// Interrupt Service Routine (ISR) - прерывания

/*
void tim1_dac_isr (void) {
    timer_clear_flag(TIM1, TIM_SR_CC1IF); //UIF скорее всего относился к другому. Делаем CC1IF
    static uint16_t brightness = 0;
    static uint8_t direction = 0;
    // потом продолжишь, код у тебя есть энивэй

    if (direction) {
        brightness += 10;

        if (brightness >= 1000) {direction = 0;}

    }

    else {
        brightness -= 10;
        if (brightness <= 0) {direction = 1;}
    }

    timer_set_oc_value(TIM1, TIM_OC4, brightness);
}
*/