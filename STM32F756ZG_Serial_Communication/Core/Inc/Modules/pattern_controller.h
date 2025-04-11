/**
  ******************************************************************************
  * @file           : pattern_controller.h
  * @brief          : En-tête pour le module de gestion des chenillards
  * @author         : 
  * @date           : 07-04-2025
  ******************************************************************************
  * @description
  * Ce fichier contient les prototypes des fonctions pour la gestion des
  * différents modèles de chenillard.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PATTERN_CONTROLLER_H
#define __PATTERN_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

/* Exported constants --------------------------------------------------------*/
#define PATTERN_COUNT 3

/* Définition des types de chenillard */
typedef enum {
  PATTERN_NONE = 0,
  PATTERN_1 = 1,
  PATTERN_2 = 2,
  PATTERN_3 = 3
} Pattern_Type;

/* Définition des fréquences de chenillard */
typedef enum {
  PATTERN_FREQ_500MS = 0,
  PATTERN_FREQ_1S = 1,
  PATTERN_FREQ_3S = 2
} Pattern_Frequency;

/* Exported functions prototypes ---------------------------------------------*/
void Pattern_Controller_Init(void);
bool Pattern_Start(Pattern_Type pattern);
bool Pattern_Stop(void);
bool Pattern_SetFrequency(Pattern_Frequency freq);
Pattern_Type Pattern_GetActive(void);
Pattern_Frequency Pattern_GetFrequency(void);
bool Pattern_IsActive(void);
void Pattern_Controller_Update(void);
void Pattern_TimerCallback(Pattern_Frequency timerType);

#ifdef __cplusplus
}
#endif

#endif /* __PATTERN_CONTROLLER_H */

