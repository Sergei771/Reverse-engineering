/**
  ******************************************************************************
  * @file           : command_parser.h
  * @brief          : En-tête pour le module d'analyse des commandes
  * @author         : 
  * @date           : 07-04-2025
  ******************************************************************************
  * @description
  * Ce fichier contient les prototypes des fonctions pour l'analyse et le
  * traitement des commandes reçues via la communication série.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMAND_PARSER_H
#define __COMMAND_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

/* Exported constants --------------------------------------------------------*/
#define MAX_COMMAND_LENGTH 64
#define MAX_ARGS 5
#define COMMAND_BUFFER_SIZE   64      // Taille maximale du buffer de commande

/* Exported functions prototypes ---------------------------------------------*/
void Command_Parser_Init(void);
void Command_Parser_ProcessCommands(void);
void Command_Parser_ProcessChar(char c);

#ifdef __cplusplus
}
#endif

#endif /* __COMMAND_PARSER_H */

