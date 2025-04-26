#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

//gpio - general purpose input-output

/*
1. Настроить режиме PWM1 в канале 4 таймера 4

1.5. Увеличить частоту сигнала на выходе таймера 4 в 100 раз. (За счет делителя)

2. Настроить таймер 6 на переполнение каждые 5 секунд.

3. Связать переполнение таймера 6 с обновлением (программно) регистра сравнения таймера 4.

4. Увеличивать содержимое регистра сравнения на 100 при каждом обновлении таймера 6.

5. При достижении предела счета таймера 4 (1000) перейти к величине сравнения 100.
*/

// uint16_t важно - учитывается разрядность счётчиков
constexpr uint16_t timer6_clock_freq_hz{1'000}; // Частота тактирования таймера 6
constexpr uint16_t step_interval_ms{2'000}; // Шаг времени, с которым изменяется яркость

constexpr uint32_t pwm_clock_freq_hz{1'000'000}; // Частота тактирования таймера ШИМ
constexpr uint16_t pwm_interval_us{10'000}; // Период ШИМ-сигнала, мкс

uint16_t oc3_val {pwm_interval_us / 2};
uint16_t oc4_val {pwm_interval_us / 2 + 200};

void pwm_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOD);
    // AF - шта? это alternate function
    gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO15);


    gpio_set_af(GPIOD, GPIO_AF2, GPIO15);

    rcc_periph_clock_enable(RCC_TIM4);
    timer_set_prescaler(TIM4, rcc_get_timer_clk_freq(TIM4) / pwm_clock_freq_hz -1);
    timer_set_period(TIM4, pwm_interval_us - 1); // период счета (число-1)

    timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_PWM1); // PWM1 включается здесь. Изначально здесь стоял TOGGLE
    timer_set_oc_value(TIM4, TIM_OC4, oc4_val); // с нулем он будет просто мигать
    timer_enable_oc_output(TIM4, TIM_OC4); // разрешаем выход

    timer_set_oc_mode(TIM4, TIM_OC3, TIM_OCM_PWM1); // PWM1 включается здесь. Изначально здесь стоял TOGGLE
    timer_set_oc_value(TIM4, TIM_OC3, oc3_val); // с нулем он будет просто мигать
    timer_enable_oc_output(TIM4, TIM_OC3); // разрешаем выход


    timer_enable_counter(TIM4);

    rcc_periph_clock_enable(RCC_TIM6);
    timer_set_prescaler(TIM6, rcc_get_timer_clk_freq(TIM6) / timer6_clock_freq_hz - 1); //rcc_get_timer_clk_freq(TIM6) - спросили прямо "чё за частота?"
    timer_set_period(TIM6, step_interval_ms - 1);

    timer_enable_irq(TIM6, TIM_DIER_UIE);
    nvic_enable_irq(NVIC_TIM6_DAC_IRQ);

    timer_enable_counter(TIM6);
    // timer_enable_irq(TIM4, TIM_DIER_CC1IE);
    // nvic_enable_irq(NVIC_TIM4_IRQ); // прерывания

    
    // OC - Output Compare
    
    

}

int main() {

    pwm_setup();
    // while(true) не используем
    while(true) {
        
    }

}

void tim6_dac_isr(void) {
    // что здесь?
    timer_clear_flag(TIM6, TIM_SR_UIF);

    if (oc3_val >= 900) oc3_val = 100;
    else oc3_val += 100;

    if (oc4_val >= 900) oc4_val = 100;
    else oc4_val += 100;

    timer_set_oc_value(TIM4, TIM_OC4, oc4_val);
    timer_set_oc_value(TIM4, TIM_OC3, oc3_val);
}

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