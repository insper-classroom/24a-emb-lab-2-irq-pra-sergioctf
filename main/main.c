/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <math.h>

const int BTN_PIN_DOT = 13;
const int BTN_PIN_LINE = 12;
const int BUZZ_PIN = 18;
volatile int foi_dot = 0;
volatile int foi_line = 0;
volatile int freq_dot = 1000;
volatile int freq_line = 6000;

void btn_callback(uint gpio, uint32_t events) {
   
  if (events == 0x8){
    if (gpio == BTN_PIN_DOT) {
      foi_dot = 1;
    } else if(gpio == BTN_PIN_LINE){
      foi_line = 1;  
    }
  }
}

void reproduz(double tempo, int freq, int pino){
  float periodo = (1.0/freq) * (float) pow(10,6);
  float s = (periodo / (float)2) ;
  int giro = (tempo*1000)/(periodo);
  int i = 0;
  while(i<=giro){
    gpio_put(pino, 1);
    sleep_us((int)s);
    gpio_put(pino, 0);
    sleep_us((int)s);
    i++;
  }

}

int main(){
  stdio_init_all();

  gpio_init(BTN_PIN_DOT);
  gpio_set_dir(BTN_PIN_DOT, GPIO_IN);
  gpio_pull_up(BTN_PIN_DOT);

  gpio_init(BTN_PIN_LINE);
  gpio_set_dir(BTN_PIN_LINE, GPIO_IN);
  gpio_pull_up(BTN_PIN_LINE);

  gpio_init(BUZZ_PIN);
  gpio_set_dir(BUZZ_PIN, GPIO_OUT);

  // callback led r (first)
  gpio_set_irq_enabled_with_callback(BTN_PIN_DOT, GPIO_IRQ_EDGE_RISE, true,
                                     &btn_callback);

  gpio_set_irq_enabled_with_callback(BTN_PIN_LINE, GPIO_IRQ_EDGE_RISE, true,
                                     &btn_callback);

  // callback led g (nao usar _with_callback)
  while (true) {
    if (foi_dot == 1) {
      printf("DOT");
      reproduz(100,freq_dot,18);
      foi_dot = 0;
    } else if (foi_line == 1) {
      printf("LINE");
      reproduz(300,freq_line,18);
      foi_line = 0;
    }

  }
}
