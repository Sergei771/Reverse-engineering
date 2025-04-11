/**
  ******************************************************************************
  * @file           : timer_handler.h
  * @brief          : En-tête pour le module de gestion des timers
  * @author         : 
  * @date           : 07-04-2025
  ******************************************************************************
  * @description
  * Ce fichier contient les prototypes des fonctions pour la gestion des
  * différents timers utilisés dans le projet.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_HANDLER_H
#define __TIMER_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

/* Exported constants --------------------------------------------------------*/
#define TIMER_COUNT 3

/* Exported functions prototypes ---------------------------------------------*/
void Timer_Init(void);
bool Timer_Start(uint8_t timerType);
bool Timer_Stop(uint8_t timerType);
bool Timer_IsActive(uint8_t timerType);
void Timer_Callback(uint8_t timerType);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_HANDLER_H */