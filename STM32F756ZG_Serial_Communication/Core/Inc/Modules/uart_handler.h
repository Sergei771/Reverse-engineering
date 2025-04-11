/**
  ******************************************************************************
  * @file           : uart_handler.h
  * @brief          : En-tête pour le module de gestion UART
  * @author         : 
  * @date           : 07-04-2025
  ******************************************************************************
  * @description
  * Ce fichier contient les prototypes des fonctions pour la gestion de la
  * communication série via UART.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_HANDLER_H
#define __UART_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include <stdbool.h>

/* Définitions ---------------------------------------------------------------*/
#define UART_BUFFER_SIZE   64    // Taille du buffer de réception UART

/* Exported constants --------------------------------------------------------*/
#define UART_RX_BUFFER_SIZE     256
#define UART_TX_BUFFER_SIZE     256
#define UART_CMD_BUFFER_SIZE    128
#define UART_MAX_COMMAND_LENGTH 64

/* Exported functions prototypes ---------------------------------------------*/
void UART_Handler_Init(UART_HandleTypeDef *huart);
void UART_StartReceive(void);
void UART_RxCpltCallback(void);
void UART_SendString(const char *str);
void UART_SendResponse(const char *response);
bool UART_IsCommandAvailable(void);
char* UART_GetCommand(void);
bool UART_HasOverflow(void);

#ifdef __cplusplus
}
#endif

#endif /* __UART_HANDLER_H */

