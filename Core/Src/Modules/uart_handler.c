/**
  ******************************************************************************
  * @file           : uart_handler.c
  * @brief          : Module de gestion de la communication UART
  * @author         : 
  * @date           : 07-04-2025
  ******************************************************************************
  * @description
  * Ce fichier implémente les fonctions pour la gestion de la communication UART
  * avec le microcontrôleur STM32F756ZG.
  * 
  * Le module gère :
  * - L'initialisation de l'UART
  * - La réception de données
  * - L'envoi de données
  * - La gestion des erreurs de communication
  * 
  * Configuration UART :
  * - Baudrate : 115200
  * - Format : 8 bits de données, pas de parité, 1 bit de stop
  * - Mode : Asynchrone
  * - Buffer de réception : 64 octets
  * 
  * Le module utilise des interruptions pour la réception des données
  * et gère un buffer circulaire pour stocker les caractères reçus.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "Modules/uart_handler.h"
#include "Modules/command_parser.h"
#include <string.h>

/* Définitions privées ------------------------------------------------------*/
#define UART_TIMEOUT 100  // Timeout pour les transmissions UART en ms

/* Variables privées ---------------------------------------------------------*/
// Ce handle est déjà déclaré dans usart.h
// Ne pas le redéclarer ici
// static uint8_t receivedChar;               // Variable inutilisée supprimée
static uint8_t rxBuffer[UART_BUFFER_SIZE]; // Buffer de réception
static uint8_t rxIndex = 0;                // Index dans le buffer
// static bool rxReady = false;               // Variable inutilisée supprimée
static bool rxOverflow = false;            // Drapeau de dépassement de buffer

/**
  * @brief  Initialisation du module UART
  * @note   Cette fonction initialise l'UART avec les paramètres suivants :
  *         - Baudrate : 115200
  *         - Format : 8N1
  *         - Pas de parité
  *         - 1 bit de stop
  *         - Pas de contrôle de flux
  *         Elle configure également :
  *         - Les interruptions de réception
  *         - Le buffer de réception
  *         - Les flags d'état
  * @param  None
  * @retval true si l'initialisation a réussi, false sinon
  */
bool UART_Init(void)
{
  /* Configuration de l'UART */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  
  /* Initialisation de l'UART */
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    return false;
  }
  
  /* Activation des interruptions de réception */
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  
  /* Démarrage de la réception */
  HAL_UART_Receive_IT(&huart3, &rxBuffer[rxIndex], 1);
  
  return true;
}

/**
 * @brief  Envoie une chaîne de caractères par UART
 * @param  str: Chaîne à envoyer
 * @retval Aucun
 */
void UART_SendString(const char* str)
{
  HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), UART_TIMEOUT);
  // Ne pas renvoyer de valeur dans une fonction void
}

/**
  * @brief  Callback de réception UART
  * @note   Cette fonction est appelée lorsqu'un caractère est reçu.
  *         Elle :
  *         - Traite le caractère reçu
  *         - Gère le dépassement de buffer
  *         - Transmet le caractère au parseur de commandes
  *         - Redémarre la réception
  * @param  huart: Handle de l'UART
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Vérification que c'est bien notre UART */
  if (huart->Instance != USART3)
  {
    return;
  }
  
  /* Traitement du caractère reçu */
  char receivedChar = (char)rxBuffer[rxIndex];
  
  /* Gestion du dépassement de buffer */
  if (rxIndex >= UART_BUFFER_SIZE - 1)
  {
    rxOverflow = true;
    rxIndex = 0;
  }
  else
  {
    rxIndex++;
  }
  
  /* Transmission du caractère au parseur de commandes */
  Command_Parser_ProcessChar(receivedChar);
  
  /* Redémarrage de la réception */
  HAL_UART_Receive_IT(&huart3, &rxBuffer[rxIndex], 1);
}

/**
  * @brief  Vérification d'un dépassement de buffer
  * @note   Cette fonction permet de vérifier si un dépassement de buffer
  *         s'est produit depuis le dernier appel.
  * @param  None
  * @retval true si un dépassement s'est produit, false sinon
  */
bool UART_HasOverflow(void)
{
  bool overflow = rxOverflow;
  rxOverflow = false;
  return overflow;
}

/**
  * @brief  Réinitialisation du module UART
  * @note   Cette fonction réinitialise complètement le module UART :
  *         - Vide le buffer de réception
  *         - Réinitialise l'index
  *         - Désactive le flag de dépassement
  *         - Redémarre la réception
  * @param  None
  * @retval None
  */
void UART_Reset(void)
{
  /* Réinitialisation des variables */
  memset(rxBuffer, 0, UART_BUFFER_SIZE);
  rxIndex = 0;
  rxOverflow = false;
  
  /* Redémarrage de la réception */
  HAL_UART_Receive_IT(&huart3, &rxBuffer[rxIndex], 1);
}

