#include "YFS201.h"

#define PIN_SENSOR GPIO_NUM_27
#define MASCARA_PIN (1ULL << PIN_SENSOR)
#define ESP_INTR_FLAG_DEFAULT 0


gpio_config_t YFS201 = {};

// del datasheet --> Frequency (Hz) = 7.5 * Flow rate (L/min)
float sensibilidad= 0.00390; //ppm*sensibilidad=lpm

volatile uint8_t ppm= 0;// pulsos por minuto


uint8_t auxiliar=0;
float volumen=0;

// se inicia la interrupción cuando se activa el boton. Finaiza del mismo modo
static void IRAM_ATTR isr_handler(void *args)
{
    // cada vez que detecta un flanco ascendente, significa que el sensor de efecto hall mide
    // lo que se traduce a un aumento de flujo
    // al trabajar contabilizando interrupciones se solventa el problema de adquirir ppm en el rango de 1 a 30 L/min
   
        ppm++;
   
}

uint8_t YFS201_Init(void)
{
    // configuro pin como entrada y su interrupcion por flanco ascendente
    YFS201.pin_bit_mask = MASCARA_PIN;
    YFS201.mode = GPIO_MODE_DEF_INPUT;
    YFS201.pull_up_en = GPIO_PULLUP_DISABLE;
    YFS201.pull_down_en = GPIO_PULLDOWN_DISABLE;
    YFS201.intr_type = GPIO_INTR_NEGEDGE;

    gpio_config(&YFS201);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    gpio_isr_handler_add(PIN_SENSOR, isr_handler, (void *)PIN_SENSOR);
    

    return true;
}






// envia los pulsos por minuto
uint8_t YFS201_flujo(void)
{   auxiliar=ppm;
    ppm=0;
    return auxiliar;
}

float YFS201_volumen(void)
{   
    volumen=sensibilidad*auxiliar;
    return volumen;
}



uint8_t YFS201_deInit(void)
{
    return true;
}