/**
  ******************************************************************************
  * @file           : led_controller.c
  * @brief          : Module de contrôle des LED
  * @author         : 
  * @date           : 07-04-2025
  ******************************************************************************
  * @description
  * Ce fichier implémente les fonctions pour le contrôle des LED du
  * microcontrôleur STM32F756ZG.
  * 
  * Le module gère trois LED connectées aux broches suivantes :
  * - LED1 : PB0 (GPIOB, Pin 0)
  * - LED2 : PB7 (GPIOB, Pin 7)
  * - LED3 : PB14 (GPIOB, Pin 14)
  * 
  * Chaque LED peut être :
  * - Allumée (LED_ON)
  * - Éteinte (LED_OFF)
  * 
  * Le module vérifie également qu'aucun chenillard n'est actif avant
  * de permettre le contrôle individuel des LED.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "Modules/led_controller.h"
#include "Modules/pattern_controller.h"
#include "stm32f7xx_hal.h"

/* Définition des broches LED -------------------------------------------------*/
#define LED1_PIN GPIO_PIN_0
#define LED2_PIN GPIO_PIN_7
#define LED3_PIN GPIO_PIN_14
#define LED_PORT GPIOB

/* Variables privées ---------------------------------------------------------*/
static GPIO_InitTypeDef GPIO_InitStruct;  // Structure de configuration GPIO

/**
  * @brief  Initialise le contrôleur de LED
  * @param  Aucun
  * @retval Aucun
  */
void LED_Controller_Init(void)
{
  /* Activation de l'horloge du port GPIOB */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /* Configuration des broches */
  GPIO_InitStruct.Pin = LED1_PIN | LED2_PIN | LED3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  
  /* Initialisation des broches */
  HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
  
  /* Extinction de toutes les LED */
  LED_SetState(1, LED_OFF);
  LED_SetState(2, LED_OFF);
  LED_SetState(3, LED_OFF);
}

/**
  * @brief  Vérification de la validité d'un numéro de LED
  * @note   Cette fonction vérifie si le numéro de LED fourni est valide.
  *         Les numéros valides sont compris entre 1 et LED_COUNT.
  * @param  ledNumber: Numéro de la LED à vérifier
  * @retval true si le numéro est valide, false sinon
  */
bool LED_IsValidNumber(uint8_t ledNumber)
{
  return (ledNumber >= 1 && ledNumber <= LED_COUNT);
}

/**
  * @brief  Conversion d'un numéro de LED en broche GPIO
  * @note   Cette fonction convertit un numéro de LED en la broche GPIO correspondante.
  *         LED1 -> PB0, LED2 -> PB7, LED3 -> PB14
  * @param  ledNumber: Numéro de la LED
  * @retval Broche GPIO correspondante, ou 0 si le numéro est invalide
  */
static uint16_t LED_NumberToPin(uint8_t ledNumber)
{
  switch (ledNumber)
  {
    case 1:
      return LED1_PIN;
    case 2:
      return LED2_PIN;
    case 3:
      return LED3_PIN;
    default:
      return 0;
  }
}

/**
  * @brief  Modification interne de l'état d'une LED (sans vérifier Pattern_IsActive)
  * @param  ledNumber: Numéro de la LED à modifier
  * @param  state: Nouvel état de la LED (LED_ON ou LED_OFF)
  * @retval None
  */
void LED_ForceState(uint8_t ledNumber, LED_State state)
{
  /* Vérification de la validité du numéro de LED */
  if (!LED_IsValidNumber(ledNumber))
  {
    return; // Ne rien faire si le numéro est invalide
  }

  /* Conversion du numéro de LED en broche GPIO */
  uint16_t pin = LED_NumberToPin(ledNumber);
  if (pin == 0)
  {
    return; // Ne rien faire si le pin est invalide
  }

  /* Modification de l'état de la LED */
  HAL_GPIO_WritePin(LED_PORT, pin, (state == LED_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
  * @brief  Modification de l'état d'une LED
  * @note   Cette fonction modifie l'état d'une LED spécifique.
  *         Elle :
  *         - Vérifie la validité du numéro de LED
  *         - Vérifie qu'aucun chenillard n'est actif
  *         - Modifie l'état de la LED
  * @param  ledNumber: Numéro de la LED à modifier
  * @param  state: Nouvel état de la LED (LED_ON ou LED_OFF)
  * @retval true si l'opération a réussi, false sinon
  */
bool LED_SetState(uint8_t ledNumber, LED_State state)
{
  /* Vérification de la validité du numéro de LED */
  if (!LED_IsValidNumber(ledNumber))
  {
    return false;
  }
  
  /* Vérification qu'aucun chenillard n'est actif */
  if (Pattern_IsActive())
  {
    return false;
  }
  
  /* Conversion du numéro de LED en broche GPIO */
  uint16_t pin = LED_NumberToPin(ledNumber);
  if (pin == 0)
  {
    return false;
  }
  
  /* Modification de l'état de la LED */
  HAL_GPIO_WritePin(LED_PORT, pin, (state == LED_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  
  return true;
}

/**
  * @brief  Récupération de l'état d'une LED
  * @note   Cette fonction permet de connaître l'état actuel d'une LED.
  * @param  ledNumber: Numéro de la LED
  * @retval État de la LED (LED_ON ou LED_OFF), ou LED_ERROR en cas d'erreur
  */
LED_State LED_GetState(uint8_t ledNumber)
{
  /* Vérification de la validité du numéro de LED */
  if (!LED_IsValidNumber(ledNumber))
  {
    return LED_ERROR;
  }
  
  /* Conversion du numéro de LED en broche GPIO */
  uint16_t pin = LED_NumberToPin(ledNumber);
  if (pin == 0)
  {
    return LED_ERROR;
  }
  
  /* Lecture de l'état de la LED */
  GPIO_PinState pinState = HAL_GPIO_ReadPin(LED_PORT, pin);
  return (pinState == GPIO_PIN_SET) ? LED_ON : LED_OFF;
}

/**
  * @brief  Inversion de l'état d'une LED
  * @param  ledNumber: Numéro de la LED (1-3)
  * @retval true si l'opération a réussi, false sinon
  */
bool LED_Toggle(uint8_t ledNumber)
{
  /* Vérification de la validité du numéro de LED */
  if (!LED_IsValidNumber(ledNumber))
  {
    return false;
  }
  
  /* Inversion de l'état de la LED */
  HAL_GPIO_TogglePin(LED_PORT, LED_NumberToPin(ledNumber));
  
  return true;
}

