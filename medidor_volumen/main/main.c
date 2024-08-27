#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "freertos/queue.h"
#include "YFS201.h"

// macros necesarios para definicion de GPIO como leds
#define LED_1 GPIO_NUM_19
#define LED_2 GPIO_NUM_18
#define LED_3 GPIO_NUM_5
#define MASCARA_PIN_LED_1 (1ULL << LED_1)
#define MASCARA_PIN_LED_2 (1ULL << LED_2)
#define MASCARA_PIN_LED_3 (1ULL << LED_3)
#define RESOLUCION_HZ 1000000   // 1MHz, 1 tick=1us
#define PERIODO_YFS201 60000000 // 1 min
gpio_config_t led1 = {};
gpio_config_t led2 = {};
gpio_config_t led3 = {};

// MACRO MEDIDOR DE VOLUMEN
#define VOL_FIN 10 // L
QueueHandle_t queue = NULL;
gptimer_handle_t gptimer_1 = NULL;
float vol_min = 0;
/*---------------------------------------------------------------
       INITS
---------------------------------------------------------------*/
uint8_t init_gpio(void);
uint8_t init_timer(void);
volatile bool inicio = false;
void app_main(void)
{
    init_gpio();
    init_timer();
    YFS201_Init();
    gptimer_start(gptimer_1);
    //
    while (1)
    {

        if (inicio == true)
        {  
            YFS201_flujo();
            inicio = false;
            vol_min = YFS201_volumen();
            printf("Volumen min [L/min]: %f\n", vol_min);
            
              if (vol_min <= VOL_FIN)
            {
                if (vol_min <= 2)
                {
                    gpio_set_level(LED_1, 1);
                    gpio_set_level(LED_2, 0);
                    
                }
                if (vol_min >= 2 && vol_min <= 6)
                {
                    gpio_set_level(LED_1, 1);
                    gpio_set_level(LED_2, 1);
                    
                }
                if (vol_min >= 6 && vol_min <= 9)
                {
                    gpio_set_level(LED_1, 0);
                    gpio_set_level(LED_2, 1);
                    
                }

               
            }
            else
            {
                if (vol_min > VOL_FIN)
                {
                    gpio_set_level(LED_1, 0);
                    gpio_set_level(LED_2, 1);
                    
                }
            }
                      
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
// ppm print
static bool IRAM_ATTR alarma_act_leds(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
   
    inicio = true;

    return true;
}
uint8_t init_gpio(void)
{

    led1.pin_bit_mask = MASCARA_PIN_LED_1;
    led1.mode = GPIO_MODE_DEF_OUTPUT;
    led1.pull_up_en = GPIO_PULLUP_DISABLE;
    led1.pull_down_en = GPIO_PULLDOWN_DISABLE;
    led1.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&led1);

    led2.pin_bit_mask = MASCARA_PIN_LED_2;
    led2.mode = GPIO_MODE_DEF_OUTPUT;
    led2.pull_up_en = GPIO_PULLUP_DISABLE;
    led2.pull_down_en = GPIO_PULLDOWN_DISABLE;
    led2.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&led2);

    led3.pin_bit_mask = MASCARA_PIN_LED_3;
    led3.mode = GPIO_MODE_DEF_OUTPUT;
    led3.pull_up_en = GPIO_PULLUP_DISABLE;
    led3.pull_down_en = GPIO_PULLDOWN_DISABLE;
    led3.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&led3);

    return true;
}

uint8_t init_timer(void)
{
    //-----configuro TIMER a resolucion 1 us----------------//
    gptimer_config_t timer_config_2 = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = RESOLUCION_HZ,

    };
    gptimer_new_timer(&timer_config_2, &gptimer_1);

    gptimer_event_callbacks_t cbs_1 = {
        .on_alarm = alarma_act_leds, //
    };
    gptimer_register_event_callbacks(gptimer_1, &cbs_1, queue);

    gptimer_enable(gptimer_1);

    gptimer_alarm_config_t alarma_YFS201 = {
        .reload_count = 0,
        .alarm_count = PERIODO_YFS201, // 1 min
        .flags.auto_reload_on_alarm = true,
    };
    gptimer_set_alarm_action(gptimer_1, &alarma_YFS201);

    return true;
}
