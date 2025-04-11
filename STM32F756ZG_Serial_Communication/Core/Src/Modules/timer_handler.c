/**
  ******************************************************************************
  * @file           : timer_handler.c
  * @brief          : Module de gestion des timers
  * @author         : 
  * @date           : 07-04-2025
  ******************************************************************************
  * @description
  * Ce fichier implémente les fonctions pour la gestion des timers du
  * microcontrôleur STM32F756ZG.
  * 
  * Le module gère trois timers avec des périodes différentes :
  * - TIM2 : 500ms
  * - TIM3 : 1s
  * - TIM4 : 3s
  * 
  * Chaque timer peut être :
  * - Initialisé
  * - Démarré
  * - Arrêté
  * - Vérifié (état actif/inactif)
  * 
  * Les timers sont configurés en mode :
  * - Comptage vers le haut
  * - Auto-rechargement
  * - Interruption à la fin du comptage
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "Modules/timer_handler.h"
#include "Modules/pattern_controller.h"
#include "stm32f7xx_hal.h"
#include <stdio.h>

/* Variables privées ---------------------------------------------------------*/
// Ces handles sont déjà déclarés dans tim.h
// Ne pas les redéclarer ici
// Tableaux des états des timers
static bool timerActive[TIMER_COUNT] = {false, false, false};

/**
  * @brief  Initialise les timers
  * @param  Aucun
  * @retval Aucun
  */
void Timer_Init(void)
{
  // Les timers sont initialisés par la fonction MX_TIMx_Init générée par CubeMX
  // Rien à faire ici car les timers sont déjà configurés
  
  // Assurez-vous que tous les timers sont arrêtés initialement
  for (int i = 0; i < TIMER_COUNT; i++) {
    timerActive[i] = false;
  }
  
  // Ne pas renvoyer de valeur dans une fonction void
}

/**
  * @brief  Démarre un timer
  * @param  timerType: Type du timer à démarrer
  * @retval true si le démarrage a réussi, false sinon
  */
bool Timer_Start(uint8_t timerType)
{
  TIM_HandleTypeDef* htim;
  
  /* Sélection du timer approprié */
  switch (timerType)
  {
    case PATTERN_FREQ_500MS:
      htim = &htim2;
      break;
    
    case PATTERN_FREQ_1S:
      htim = &htim3;
      break;
    
    case PATTERN_FREQ_3S:
      htim = &htim4;
      break;
    
    default:
      return false;
  }
  
  /* Démarrage du timer */
  if (HAL_TIM_Base_Start_IT(htim) != HAL_OK)
  {
    return false;
  }
  
  // --- DEBUG --- 
  // char debugMsg[50];
  // snprintf(debugMsg, sizeof(debugMsg), "Timer %d (TIM%d) START\r\n", timerType, (timerType == PATTERN_FREQ_500MS) ? 2 : ((timerType == PATTERN_FREQ_1S) ? 3 : 4));
  // UART_SendString(debugMsg);
  // --- Fin DEBUG ---
  
  /* Mise à jour de l'état */
  timerActive[timerType] = true;
  
  return true;
}

/**
  * @brief  Arrête un timer
  * @param  timerType: Type du timer à arrêter
  * @retval true si l'arrêt a réussi, false sinon
  */
bool Timer_Stop(uint8_t timerType)
{
  TIM_HandleTypeDef* htim;
  
  /* Sélection du timer approprié */
  switch (timerType)
  {
    case PATTERN_FREQ_500MS:
      htim = &htim2;
      break;
    
    case PATTERN_FREQ_1S:
      htim = &htim3;
      break;
    
    case PATTERN_FREQ_3S:
      htim = &htim4;
      break;
    
    default:
      return false;
  }
  
  /* Arrêt du timer */
  if (HAL_TIM_Base_Stop_IT(htim) != HAL_OK)
  {
    return false;
  }
  
  /* Mise à jour de l'état */
  timerActive[timerType] = false;
  
  return true;
}

/**
  * @brief  Vérifie si un timer est actif
  * @param  timerType: Type du timer à vérifier
  * @retval true si le timer est actif, false sinon
  */
bool Timer_IsActive(uint8_t timerType)
{
  if (timerType >= TIMER_COUNT)
  {
    return false;
  }
  
  return timerActive[timerType];
}

/**
  * @brief  Callbacks d'interruption des timers
  * @note   Ces fonctions sont appelées lorsqu'une interruption timer se produit
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // --- DEBUG --- 
  /*
  uint8_t timerId = 0;
  if (htim->Instance == TIM2) timerId = 2;
  else if (htim->Instance == TIM3) timerId = 3;
  else if (htim->Instance == TIM4) timerId = 4;
  if (timerId != 0) {
      char debugMsg[30];
      snprintf(debugMsg, sizeof(debugMsg), "TIM%d Interrupt!\r\n", timerId);
      UART_SendString(debugMsg);
  }
  */
  // --- Fin DEBUG ---

  if (htim->Instance == TIM2)
  {
    Pattern_TimerCallback(PATTERN_FREQ_500MS);
  }
  else if (htim->Instance == TIM3)
  {
    Pattern_TimerCallback(PATTERN_FREQ_1S);
  }
  else if (htim->Instance == TIM4)
  {
    Pattern_TimerCallback(PATTERN_FREQ_3S);
  }
}

