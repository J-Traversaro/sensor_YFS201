#ifndef YFS201_H
#define YFS201_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "driver/gpio.h"
#include "freertos/queue.h"

 

//en esta funcion se inicializa el pin 
//como pioOut, ademas se habilita int
// para poder registrar los pulsos enviados
//por el sensor 

/// @brief 
/// @param  
/// @return 
uint8_t YFS201_Init(void);

// estimando la sensibilidad del sensor, se puede conocer
// el flujo/min del fluido circundante

/// @brief 
/// @param  
/// @return 
uint8_t YFS201_flujo(void);

//integrando el valor obtenido en YFS201_flujo, se determina
// el volumen del fluido circundante

/// @brief 
/// @param  
/// @return 
float YFS201_volumen(void);




/// @brief 
/// @param  
/// @return 
uint8_t YFS201_deInit(void);

#endif 