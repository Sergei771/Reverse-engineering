/**
  ******************************************************************************
  * @file           : led_controller.h
  * @brief          : En-tête pour le module de contrôle des LED
  * @author         : 
  * @date           : 07-04-2025
  ******************************************************************************
  * @description
  * Ce fichier contient les prototypes des fonctions pour le contrôle des LED
  * du microcontrôleur STM32F756ZG.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_CONTROLLER_H
#define __LED_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

/* Exported constants --------------------------------------------------------*/
#define LED_COUNT 3

/* Définition des états des LED */
typedef enum {
  LED_OFF = 0,
  LED_ON = 1
} LED_State;

/* Définitions ---------------------------------------------------------------*/
#define LED_ON      1
#define LED_OFF     0
#define LED_ERROR   2      // Valeur d'erreur pour les fonctions LED

/* Exported functions prototypes ---------------------------------------------*/
void LED_Controller_Init(void);
bool LED_SetState(uint8_t ledNumber, LED_State state);
LED_State LED_GetState(uint8_t ledNumber);
bool LED_Toggle(uint8_t ledNumber);
bool LED_IsValidNumber(uint8_t ledNumber);
void LED_ForceState(uint8_t ledNumber, LED_State state); // Prototype ajouté (non statique)

#ifdef __cplusplus
}
#endif

#endif /* __LED_CONTROLLER_H */

/************************ (C) COPYRIGHT Manus *****END OF FILE****/
