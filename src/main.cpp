#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>


// НАСТРОЙКА UART
void uart_setup(uint32_t usart, uint32_t baud, uint32_t bits, uint32_t parity, uint32_t stopbits) {
    // Настройка альт. функций линий порта РА
    rcc_periph_clock_enable(RCC_GPIOA);
    // AF - шта? это alternate function
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3); 
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);
    
    // Настройка УСАПП_1
    
    rcc_periph_clock_enable(RCC_USART2);
    
    usart_set_baudrate(usart, baud);
    usart_set_databits(usart, bits);
    usart_set_parity(usart, parity);
    usart_set_stopbits(usart, stopbits);
    
    usart_set_mode(usart, USART_MODE_TX_RX);
    usart_set_flow_control(usart, USART_FLOWCONTROL_NONE);
    
    usart_enable(usart);
    
    

}

char f = 'F';
char u = 'U';
char n = 'N';
char y = 'Y';
char newline = '\n';
int main() {
    uart_setup(USART2, 115200, 8, USART_PARITY_NONE, USART_STOPBITS_1);
    // while(true) не используем
    while(true) {
        if (usart_get_flag(USART2, USART_SR_RXNE)) {
            uint16_t data = usart_recv(USART2);
            usart_send_blocking(USART2, data);
        } 
        
        //for (volatile uint32_t i = 0; i < 500000; ++i); // задержки в попугаях
    }

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