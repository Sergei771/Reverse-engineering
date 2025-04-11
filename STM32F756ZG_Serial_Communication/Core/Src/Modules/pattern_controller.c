/**
  ******************************************************************************
  * @file           : pattern_controller.c
  * @brief          : Module de gestion des chenillards
  * @author         : 
  * @date           : 07-04-2025
  ******************************************************************************
  * @description
  * Ce fichier implémente les fonctions pour la gestion des différents modèles
  * de chenillard pour le microcontrôleur STM32F756ZG.
  * 
  * Le module gère trois types de chenillards avec trois fréquences différentes :
  * - Pattern 1 : Allumage séquentiel des LED de gauche à droite
  * - Pattern 2 : Alternance des LED paires et impaires
  * - Pattern 3 : Allumage en chenille avec retour
  * 
  * Fréquences disponibles :
  * - 500ms : Changement rapide
  * - 1s    : Changement moyen
  * - 3s    : Changement lent
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Modules/pattern_controller.h"
#include "Modules/led_controller.h"
#include "Modules/timer_handler.h"
#include "Modules/uart_handler.h"
#include <stdio.h>

/* Variables privées ---------------------------------------------------------*/
static Pattern_Type activePattern = PATTERN_NONE;    // Pattern actuellement actif
static Pattern_Frequency currentFrequency = PATTERN_FREQ_1S;  // Fréquence actuelle
static uint8_t patternStep = 0;                      // Étape actuelle du pattern
static bool patternNeedsUpdate = false;              // Flag de mise à jour

/* Prototypes de fonctions privées -------------------------------------------*/
static void Pattern1_Update(void);  // Mise à jour du pattern 1
static void Pattern2_Update(void);  // Mise à jour du pattern 2
static void Pattern3_Update(void);  // Mise à jour du pattern 3

/**
  * @brief  Initialisation du module de gestion des chenillards
  * @note   Cette fonction initialise toutes les variables du module :
  *         - Désactive tous les patterns
  *         - Configure la fréquence par défaut à 1s
  *         - Réinitialise l'étape du pattern
  *         - Désactive le flag de mise à jour
  * @param  None
  * @retval None
  */
void Pattern_Controller_Init(void)
{
  activePattern = PATTERN_NONE;
  currentFrequency = PATTERN_FREQ_1S;
  patternStep = 0;
  patternNeedsUpdate = false;
}

/**
  * @brief  Démarrage d'un chenillard
  * @note   Cette fonction :
  *         - Vérifie la validité du pattern demandé
  *         - Arrête le pattern actif si nécessaire
  *         - Configure le nouveau pattern
  *         - Démarre le timer correspondant à la fréquence actuelle
  *         - Force une mise à jour immédiate pour voir l'effet
  * @param  pattern: Type de chenillard à démarrer (PATTERN_1, PATTERN_2, PATTERN_3)
  * @retval true si l'opération a réussi, false sinon
  */
bool Pattern_Start(Pattern_Type pattern)
{
  /* Vérification de la validité du type de chenillard */
  if (pattern <= PATTERN_NONE || pattern > PATTERN_COUNT)
  {
    return false;
  }
  
  /* Arrêt du chenillard actif si nécessaire */
  if (activePattern != PATTERN_NONE)
  {
    Pattern_Stop();
  }
  
  /* Activation du chenillard demandé */
  activePattern = pattern;
  patternStep = 0;
  
  /* Démarrage du timer correspondant à la fréquence actuelle */
  Timer_Start(currentFrequency);
  
  /* Mise à jour immédiate pour voir un effet */
  patternNeedsUpdate = true;
  
  return true;
}

/**
  * @brief  Arrêt du chenillard actif
  * @note   Cette fonction :
  *         - Arrête le timer actif
  *         - Désactive le pattern
  *         - Éteint toutes les LED
  * @param  None
  * @retval true si l'opération a réussi, false sinon
  */
bool Pattern_Stop(void)
{
  /* Vérification qu'un chenillard est actif */
  if (activePattern == PATTERN_NONE)
  {
    return false;
  }
  
  /* Arrêt du timer */
  Timer_Stop(currentFrequency);
  
  /* Désactivation du chenillard */
  activePattern = PATTERN_NONE;
  
  /* Extinction de toutes les LED */
  for (uint8_t i = 1; i <= LED_COUNT; i++)
  {
    LED_ForceState(i, LED_OFF);
  }
  
  return true;
}

/**
  * @brief  Changement de la fréquence du chenillard
  * @note   Cette fonction :
  *         - Vérifie la validité de la nouvelle fréquence
  *         - Si un chenillard est actif :
  *           * Arrête l'ancien timer
  *           * Change la fréquence
  *           * Démarre le nouveau timer
  *         - Sinon, met juste à jour la fréquence pour utilisation future
  * @param  freq: Nouvelle fréquence (PATTERN_FREQ_500MS, PATTERN_FREQ_1S, PATTERN_FREQ_3S)
  * @retval true si l'opération a réussi, false sinon
  */
bool Pattern_SetFrequency(Pattern_Frequency freq)
{
  /* Vérification de la validité de la fréquence */
  if (freq < PATTERN_FREQ_500MS || freq > PATTERN_FREQ_3S)
  {
    return false;
  }
  
  /* Si un chenillard est actif, on doit changer le timer */
  if (activePattern != PATTERN_NONE)
  {
    /* Arrêt du timer actuel */
    Timer_Stop(currentFrequency);
    
    /* Mise à jour de la fréquence */
    currentFrequency = freq;
    
    /* Démarrage du nouveau timer */
    Timer_Start(currentFrequency);
  }
  else
  {
    /* Mise à jour de la fréquence pour utilisation future */
    currentFrequency = freq;
  }
  
  return true;
}

/**
  * @brief  Récupération du chenillard actif
  * @note   Retourne le type de chenillard actuellement actif
  * @param  None
  * @retval Type de chenillard actif (PATTERN_NONE si aucun)
  */
Pattern_Type Pattern_GetActive(void)
{
  return activePattern;
}

/**
  * @brief  Récupération de la fréquence actuelle
  * @note   Retourne la fréquence actuellement configurée
  * @param  None
  * @retval Fréquence actuelle
  */
Pattern_Frequency Pattern_GetFrequency(void)
{
  return currentFrequency;
}

/**
  * @brief  Vérification si un chenillard est actif
  * @note   Utilisé pour empêcher le contrôle individuel des LED
  *         pendant qu'un chenillard est actif
  * @param  None
  * @retval true si un chenillard est actif, false sinon
  */
bool Pattern_IsActive(void)
{
  return (activePattern != PATTERN_NONE);
}

/**
  * @brief  Mise à jour du chenillard actif
  * @note   Cette fonction est appelée périodiquement pour mettre à jour
  *         l'état des LED selon le pattern actif. Elle :
  *         - Vérifie si une mise à jour est nécessaire
  *         - Appelle la fonction de mise à jour appropriée
  *         - Réinitialise le flag de mise à jour
  * @param  None
  * @retval None
  */
void Pattern_Controller_Update(void)
{
  /* Vérification si une mise à jour est nécessaire */
  if (!patternNeedsUpdate || activePattern == PATTERN_NONE)
  {
    return;
  }
  
  /* Mise à jour du chenillard en fonction du type */
  switch (activePattern)
  {
    case PATTERN_1:
      Pattern1_Update();
      break;
    
    case PATTERN_2:
      Pattern2_Update();
      break;
    
    case PATTERN_3:
      Pattern3_Update();
      break;
    
    default:
      /* Ne devrait jamais arriver */
      break;
  }
  
  /* Réinitialisation du flag de mise à jour */
  patternNeedsUpdate = false;
}

/**
  * @brief  Callback appelé par le timer
  * @note   Cette fonction est appelée par le module de gestion des timers
  *         lorsqu'un timer expire. Elle :
  *         - Vérifie que le timer correspond à la fréquence actuelle
  *         - Active le flag de mise à jour si nécessaire
  * @param  timerType: Type de timer qui a expiré
  * @retval None
  */
void Pattern_TimerCallback(Pattern_Frequency timerType)
{
  /* Vérification que le timer correspond au pattern actif */
  if (activePattern != PATTERN_NONE && timerType == currentFrequency)
  {
    patternNeedsUpdate = true;
  }
}

/**
  * @brief  Mise à jour du chenillard de type 1
  * @note   Pattern 1 : Allumage séquentiel des LED de gauche à droite
  *         Étape 0 : LED1 ON, LED2 OFF, LED3 OFF
  *         Étape 1 : LED1 OFF, LED2 ON, LED3 OFF
  *         Étape 2 : LED1 OFF, LED2 OFF, LED3 ON
  *         Puis retour à l'étape 0
  * @param  None
  * @retval None
  */
static void Pattern1_Update(void)
{
  /* Extinction de toutes les LED */
  for (uint8_t i = 1; i <= LED_COUNT; i++)
  {
    LED_ForceState(i, LED_OFF);
  }
  
  /* Allumage de la LED correspondant à l'étape actuelle */
  LED_ForceState(patternStep + 1, LED_ON);
  
  /* Passage à l'étape suivante */
  patternStep = (patternStep + 1) % LED_COUNT;
}

/**
  * @brief  Mise à jour du chenillard de type 2
  * @note   Pattern 2 : Alternance des LED paires et impaires
  *         Étape 0 : LED1 ON, LED2 OFF, LED3 ON
  *         Étape 1 : LED1 OFF, LED2 ON, LED3 OFF
  *         Puis retour à l'étape 0
  * @param  None
  * @retval None
  */
static void Pattern2_Update(void)
{
  /* Alternance entre LED paires et impaires */
  if (patternStep == 0)
  {
    /* Étape 0 : LED impaires allumées */
    LED_ForceState(1, LED_ON);
    LED_ForceState(2, LED_OFF);
    LED_ForceState(3, LED_ON);
  }
  else
  {
    /* Étape 1 : LED paires allumées */
    LED_ForceState(1, LED_OFF);
    LED_ForceState(2, LED_ON);
    LED_ForceState(3, LED_OFF);
  }
  
  /* Passage à l'étape suivante */
  patternStep = (patternStep + 1) % 2;
}

/**
  * @brief  Mise à jour du chenillard de type 3
  * @note   Pattern 3 : Allumage en chenille avec retour
  *         Étape 0 : LED1 ON, LED2 OFF, LED3 OFF
  *         Étape 1 : LED1 ON, LED2 ON, LED3 OFF
  *         Étape 2 : LED1 ON, LED2 ON, LED3 ON
  *         Étape 3 : LED1 OFF, LED2 ON, LED3 ON
  *         Étape 4 : LED1 OFF, LED2 OFF, LED3 ON
  *         Étape 5 : LED1 OFF, LED2 OFF, LED3 OFF
  *         Puis retour à l'étape 0
  * @param  None
  * @retval None
  */
static void Pattern3_Update(void)
{
  /* Extinction de toutes les LED */
  for (uint8_t i = 1; i <= LED_COUNT; i++)
  {
    LED_ForceState(i, LED_OFF);
  }
  
  /* Allumage des LED selon l'étape */
  switch (patternStep)
  {
    case 0:
      LED_ForceState(1, LED_ON);
      break;
    case 1:
      LED_ForceState(1, LED_ON);
      LED_ForceState(2, LED_ON);
      break;
    case 2:
      LED_ForceState(1, LED_ON);
      LED_ForceState(2, LED_ON);
      LED_ForceState(3, LED_ON);
      break;
    case 3:
      LED_ForceState(2, LED_ON);
      LED_ForceState(3, LED_ON);
      break;
    case 4:
      LED_ForceState(3, LED_ON);
      break;
    default:
      break;
  }
  
  /* Passage à l'étape suivante */
  patternStep = (patternStep + 1) % 6;
}

