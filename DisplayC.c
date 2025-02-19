#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "inc/ssd1306.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

uint32_t lastTime = 0; // Variável para debounce
ssd1306_t ssd; // Inicializa a estrutura do display
int adcPositionY = 28, adcPositionX = 60; // Posição inicial do cursor na tela
bool estadoLedVerde = false, estadoBordaExtra = false, estadoPWM = true; // Variáveis de estado
uint sliceLedAzul, sliceLedVermelho; // Variáveis para inicialização do PWM para acionar os leds

//rotina da interrupção
static void gpio_irq_handler(uint gpio, uint32_t events) {
  uint32_t currentTime = to_us_since_boot(get_absolute_time());
   
  if(currentTime - lastTime > 200000) { // Debounce

    if(gpio == 22) { // Botão do Joystick
        estadoLedVerde = !estadoLedVerde;
        gpio_put(11, estadoLedVerde);

        estadoBordaExtra = !estadoBordaExtra;
    }

    if(gpio == 5) { // Botão A
      estadoPWM = !estadoPWM;      
      pwm_set_enabled(sliceLedAzul, estadoPWM); //habilita o pwm no slice correspondente
      pwm_set_enabled(sliceLedVermelho, estadoPWM); //habilita o pwm no slice correspondente
    }

    lastTime = currentTime;
  }
}

int main() {
  adc_init();
  stdio_init_all(); // Inicializa a comunicação serial
  
  // Configura os pinos ADC
  adc_gpio_init(26);
  adc_gpio_init(27);  
  
  gpio_init(5); // Inicializa o pino 5 (botão A) para uso do botão
  gpio_set_dir(5, GPIO_IN);
  gpio_pull_up(5);

  gpio_init(22); // Inicializa o pino 22 (Joystick) para uso do botão
  gpio_set_dir(22, GPIO_IN);
  gpio_pull_up(22);

  gpio_init(11); // Inicializa o pino 11 para uso do led
  gpio_set_dir(11, GPIO_OUT);

  gpio_init(12); // Inicializa o pino 12 para uso do led
  gpio_set_dir(12, GPIO_OUT);

  gpio_init(13); // Inicializa o pino 13 para uso do led
  gpio_set_dir(13, GPIO_OUT);

  gpio_set_function(12, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM
  sliceLedAzul = pwm_gpio_to_slice_num(12); //obter o canal PWM da GPIO
  pwm_set_clkdiv(sliceLedAzul, 18.0); //define o divisor de clock do PWM
  pwm_set_wrap(sliceLedAzul, 65535); //definir o valor de wrap
  pwm_set_enabled(sliceLedAzul, true); //habilita o pwm no slice correspondente

  gpio_set_function(13, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM
  sliceLedVermelho = pwm_gpio_to_slice_num(13); //obter o canal PWM da GPIO
  pwm_set_clkdiv(sliceLedVermelho, 18.0); //define o divisor de clock do PWM
  pwm_set_wrap(sliceLedVermelho, 65535); //definir o valor de wrap
  pwm_set_enabled(sliceLedVermelho, true); //habilita o pwm no slice correspondente

  i2c_init(I2C_PORT, 400 * 1000); // I2C Initialisation. Using it at 400Khz.
  set_sys_clock_khz(128000, false); // Set the system clock to 128Mhz

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_send_data(&ssd); // Envia os dados para o display

  // Configura as interrupções  
  gpio_set_irq_enabled_with_callback(5, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);
  gpio_set_irq_enabled_with_callback(22, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);

  while(true) {
    // Lê o eixo Y
    adc_select_input(0); // ADC0 (GPIO 26)
    
    uint16_t adcPositionY = 55-adc_read()*55/4095;

    // Lê o eixo X
    adc_select_input(1); // ADC1 (GPIO 27)
    uint16_t adcPositionX = adc_read()*119/4095;

    if(adcPositionX >= 53 && adcPositionX <= 59) { // Desliga o PWM na faixa de 53 a 59 por conta da imprecisão do joystick
        pwm_set_gpio_level(13, 0); //ajustar o brilho do led azul
    } else {
      if(adcPositionX <= 52)
        pwm_set_gpio_level(13, (52-adcPositionX)*65535/52); //ajustar o brilho do led vermelho
      else
        pwm_set_gpio_level(13, (adcPositionX - 60) * 65535 / (119 - 60)); //ajustar o brilho do led vermelho
    }

    if(adcPositionY >= 25 && adcPositionY <= 32) { // Desliga o PWM na faixa de 25 a 32 por conta da imprecisão do joystick
      pwm_set_gpio_level(12, 0); // Desliga o PWM na faixa de 25 a 32
    } else {
      if(adcPositionY <= 24)
          pwm_set_gpio_level(12, (24-adcPositionY)*65535/24); // Ajustar o brilho do led azul
      else
          pwm_set_gpio_level(12, (adcPositionY-33)*65535/(55-33)); // Ajustar o brilho do led azul
    }

    // Garante que o cursor não ultrapasse os limites da tela
    if(adcPositionX < 0)
      adcPositionX = 0;
    if(adcPositionX > 127)
      adcPositionX = 127;
    if(adcPositionY < 0)    
      adcPositionY = 0;
    if(adcPositionY > 63)
      adcPositionY = 63;
    
    // Desenha o cursor na tela
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, adcPositionY, adcPositionX, 8, 8, true, true);

    // Desenha a borda extra na tela
    if(estadoBordaExtra)
      ssd1306_rect(&ssd, 1, 1, 125, 61, true, false);

    // Desenha a borda principal na tela
    ssd1306_rect(&ssd, 0, 0, 127, 63, true, false);
    
    // Manda os dados pro display
    ssd1306_send_data(&ssd);
  }
}