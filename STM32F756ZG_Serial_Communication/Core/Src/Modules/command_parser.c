/**
  ******************************************************************************
  * @file           : command_parser.c
  * @brief          : Module de gestion des commandes
  * @author         : 
  * @date           : 07-04-2025
  ******************************************************************************
  * @description
  * Ce fichier implémente les fonctions pour l'analyse et l'exécution des commandes
  * reçues via l'UART pour le microcontrôleur STM32F756ZG.
  * 
  * Le module gère deux types de commandes :
  * 1. Commandes LED individuelles :
  *    - Format : "LED<num> <état>"
  *    - Exemple : "LED1 ON" ou "LED2 OFF"
  *    - <num> : 1 à 3
  *    - <état> : ON ou OFF
  * 
  * 2. Commandes de chenillard :
  *    - Format : "CHENILLARD<num> <fréquence>"
  *    - Exemple : "CHENILLARD1 1S" ou "CHENILLARD2 500MS"
  *    - <num> : 1 à 3
  *    - <fréquence> : 500MS, 1S ou 3S
  * 
  * Le module vérifie la validité des commandes et retourne des messages d'erreur
  * appropriés en cas de commande invalide.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Modules/command_parser.h"
#include "Modules/uart_handler.h"
#include "Modules/led_controller.h"
#include "Modules/pattern_controller.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // Pour atoi (si on l'utilise, sinon manuelle)
#include <ctype.h> // Pour toupper()

/* Constantes pour les commandes */
#define CMD_LED         "LED"
#define CMD_CHENILLARD  "CHENILLARD"
#define CMD_ON          "ON"
#define CMD_OFF         "OFF"
#define CMD_FREQ_PREFIX "FREQUENCE"
#define CMD_HELP        "HELP"
#define CMD_STATUS      "STATUS"
#define CMD_PAT         "PAT"
#define CMD_FREQ        "FREQ"
#define CMD_STOP        "STOP"
#define CMD_QUIT        "QUIT"
#define CMD_CLEAR       "CLEAR"

/* Variables privées ---------------------------------------------------------*/
static char commandBuffer[COMMAND_BUFFER_SIZE];  // Buffer pour stocker la commande en cours
static uint8_t bufferIndex = 0;                  // Index dans le buffer
static bool commandComplete = false;             // Indique si une commande est complète

/* Prototypes de fonctions privées -------------------------------------------*/
static bool Parse_LED_Command(const char* command);
static bool Parse_Chenillard_ON_Command(const char* command);
static bool Parse_Chenillard_Frequency_Command(const char* command);
static bool Parse_Shortcut_Command(const char* command);
static bool Execute_STOP_Command(void);
static bool Execute_STATUS_Command(void);
static void Send_Error_Message(const char* message);
static void Send_Success_Message(const char* message);

/**
  * @brief  Initialisation du module de gestion des commandes
  * @note   Cette fonction initialise toutes les variables du module :
  *         - Vide le buffer de commande
  *         - Réinitialise l'index du buffer
  *         - Désactive le flag de commande complète
  * @param  None
  * @retval None
  */
void Command_Parser_Init(void)
{
  memset(commandBuffer, 0, COMMAND_BUFFER_SIZE);
  bufferIndex = 0;
  commandComplete = false;
}

/**
 * @brief  Traite un caractère reçu
 * @param  c: Caractère reçu
 * @retval Aucun
 */
void Command_Parser_ProcessChar(char c)
{
  // Si une commande vient d'être traitée, on s'assure que le buffer est prêt
  if (commandComplete) {
      memset(commandBuffer, 0, COMMAND_BUFFER_SIZE);
      bufferIndex = 0;
      commandComplete = false;
  }

  // Convertir en majuscule
  char upperC = toupper((unsigned char)c);

  /* Ignorer le Line Feed */
  if (upperC == '\n')
  {
      return;
  }

  /* Gestion du retour chariot (fin de commande) */
  if (upperC == '\r')
  {
    commandBuffer[bufferIndex] = '\0'; // Terminer la chaîne
    commandComplete = true;
    return;
  }

  /* Gestion du retour arrière (Backspace) */
  if (upperC == '\b' || upperC == 127) // ASCII Backspace ou DEL
  {
    if (bufferIndex > 0)
    {
      bufferIndex--;
      commandBuffer[bufferIndex] = '\0'; // Effacer le dernier caractère
    }
    return;
  }

  /* Stockage du caractère si imprimable et place disponible */
  if (bufferIndex < COMMAND_BUFFER_SIZE - 1 && isprint((unsigned char)upperC))
  {
    commandBuffer[bufferIndex] = upperC;
    bufferIndex++;
  }
}

/**
  * @brief  Traitement des commandes complètes
  * @note   Cette fonction est appelée périodiquement pour traiter les commandes
  *         complètes. Elle :
  *         - Vérifie si une commande est prête
  *         - Analyse la commande selon son type
  *         - Réinitialise le buffer après traitement
  * @param  None
  * @retval None
  */
void Command_Parser_ProcessCommands(void)
{
  if (!commandComplete) { return; }

  bool commandProcessed = false;
  bool isEmptyCommand = (strlen(commandBuffer) == 0); // Vérifier si vide AVANT traitement

  // Traitement des commandes principales
  if (!isEmptyCommand) {
      if (strcmp(commandBuffer, CMD_STATUS) == 0) {
          commandProcessed = Execute_STATUS_Command();
      }
      else if (strcmp(commandBuffer, CMD_STOP) == 0) {
          commandProcessed = Execute_STOP_Command();
      }
      else if (strncmp(commandBuffer, CMD_LED, strlen(CMD_LED)) == 0) {
          commandProcessed = Parse_LED_Command(commandBuffer);
      }
      else if (strncmp(commandBuffer, CMD_CHENILLARD, strlen(CMD_CHENILLARD)) == 0) {
          const char* argPart = commandBuffer + strlen(CMD_CHENILLARD);
          // CHENILLARD<N> ON
          // Vérifier si le premier char après CHENILLARD est un chiffre et si la fin est " ON"
          if (strlen(argPart) >= 4 && isdigit((unsigned char)argPart[0]) && strcmp(argPart + 1, " " CMD_ON) == 0) {
              commandProcessed = Parse_Chenillard_ON_Command(commandBuffer);
          }
          // CHENILLARD FREQUENCE<F>
          else if (strncmp(argPart, " " CMD_FREQ_PREFIX, strlen(" " CMD_FREQ_PREFIX)) == 0) {
              commandProcessed = Parse_Chenillard_Frequency_Command(commandBuffer);
          }
           // Ignorer "CHENILLARD OFF" ou autre format invalide
          else {
             commandProcessed = false; // Sera traité comme inconnu plus bas
          }
      }
      else if (strncmp(commandBuffer, CMD_PAT, strlen(CMD_PAT)) == 0 ||
               strncmp(commandBuffer, CMD_FREQ, strlen(CMD_FREQ)) == 0)
      {
          commandProcessed = Parse_Shortcut_Command(commandBuffer);
      }

      // Si aucune commande n'a été reconnue ou traitée avec succès
      if (!commandProcessed)
      {
          Send_Error_Message("Commande inconnue ou format invalide");
      }
  } // Fin if (!isEmptyCommand)

  // Réinitialisation (ne plus faire dans ProcessChar)
  memset(commandBuffer, 0, COMMAND_BUFFER_SIZE);
  bufferIndex = 0;
  commandComplete = false; // Marquer comme traitée pour ProcessChar

  // Afficher le prompt seulement si ce n'était pas une commande vide
  if (!isEmptyCommand) {
    UART_SendString("STM32> ");
  }
}

/**
  * @brief  Analyse d'une commande LED
  * @note   Cette fonction analyse une commande de type LED.
  *         Format attendu : "LED<num> <état>"
  *         Exemple : "LED1 ON" ou "LED2 OFF"
  * @param  command: Commande à analyser
  * @retval true si la commande est valide, false sinon
  */
static bool Parse_LED_Command(const char* command)
{
  size_t prefixLen = strlen(CMD_LED);

  // Vérification longueur minimale (LED1 ON = 6)
  if (strlen(command) < prefixLen + 4) { // LEDN ON
    Send_Error_Message("Format LED invalide (trop court)");
    return false;
  }

  // Extraction et validation Numéro
  char ledNumChar = command[prefixLen];
  if (ledNumChar < '1' || ledNumChar > ('0' + LED_COUNT)) {
      Send_Error_Message("Numero LED invalide (1-3)");
      return false;
  }
  uint8_t ledNumber = ledNumChar - '0';

  // Vérification Espace
  if (command[prefixLen + 1] != ' ') {
      Send_Error_Message("Format LED invalide (espace manquant)");
      return false;
  }

  // Extraction et validation Etat
  const char* stateStr = &command[prefixLen + 2];
  LED_State ledState;

  if (strcmp(stateStr, CMD_ON) == 0) {
    ledState = LED_ON;
  } else if (strcmp(stateStr, CMD_OFF) == 0) {
    ledState = LED_OFF;
  } else {
    Send_Error_Message("Etat LED invalide (ON/OFF attendu)");
    return false;
  }

  // Application et message
  if (!LED_SetState(ledNumber, ledState)) {
      Send_Error_Message("Impossible de changer LED (pattern actif?)");
      return false;
  }
  char successMsg[30];
  snprintf(successMsg, sizeof(successMsg), "LED %d mise a %s\r\n", ledNumber, stateStr);
  Send_Success_Message(successMsg);
  return true;
}

/**
  * @brief  Analyse une commande CHENILLARD<N> ON
  * @param  command: Commande à analyser
  * @retval true si la commande est valide et traitée, false sinon
  */
static bool Parse_Chenillard_ON_Command(const char* command)
{
  // Format: "CHENILLARD<N> ON"
  size_t prefixLen = strlen(CMD_CHENILLARD); // 10

  // Extraction Numéro
  char patternNumChar = command[prefixLen];
  if (patternNumChar < '1' || patternNumChar > ('0' + PATTERN_COUNT)) {
      Send_Error_Message("Numero chenillard invalide (1-3)");
      return false;
  }
  uint8_t patternNumber = patternNumChar - '0';

  // Vérification " ON"
  if (strcmp(&command[prefixLen + 1], " " CMD_ON) != 0) {
      Send_Error_Message("Format chenillard ON invalide");
      return false;
  }

  // Démarrage et message
  if (Pattern_Start(patternNumber)) {
      char msg[40];
      snprintf(msg, sizeof(msg), "Chenillard %d active\r\n", patternNumber);
      Send_Success_Message(msg);
      return true;
  } else {
      Send_Error_Message("Impossible de demarrer chenillard");
      return false;
  }
}

/**
  * @brief  Analyse une commande CHENILLARD FREQUENCE<F>
  * @param  command: Commande à analyser
  * @retval true si la commande est valide et traitée, false sinon
  */
static bool Parse_Chenillard_Frequency_Command(const char* command)
{
  // Format: "CHENILLARD FREQUENCE<F>"
  const char* prefix = CMD_CHENILLARD " " CMD_FREQ_PREFIX; // "CHENILLARD FREQUENCE"
  size_t prefixLen = strlen(prefix);

  if (strlen(command) != prefixLen + 1 || strncmp(command, prefix, prefixLen) != 0) {
      Send_Error_Message("Format Frequence invalide");
      return false;
  }

  // Extraction Numéro Fréquence (1, 2, 3)
  char freqNumChar = command[prefixLen];
  Pattern_Frequency patternFreq;
  const char* freqStr = NULL;

  switch (freqNumChar) {
      case '1': patternFreq = PATTERN_FREQ_500MS; freqStr = "500MS"; break;
      case '2': patternFreq = PATTERN_FREQ_1S; freqStr = "1S"; break;
      case '3': patternFreq = PATTERN_FREQ_3S; freqStr = "3S"; break;
      default:
          Send_Error_Message("Numero Frequence invalide (1-3)");
          return false;
  }

  // Mise à jour et message
  if (Pattern_SetFrequency(patternFreq)) {
      char msg[40];
      snprintf(msg, sizeof(msg), "Frequence reglee a %s\r\n", freqStr);
      Send_Success_Message(msg);
      return true;
  } else {
      Send_Error_Message("Impossible de regler frequence");
      return false;
  }
}

/**
  * @brief  Analyse une commande raccourci (PAT, FREQ, STOP)
  * @param  command: Commande à analyser
  * @retval true si la commande est valide et traitée, false sinon
  */
static bool Parse_Shortcut_Command(const char* command)
{
    // STOP
    if (strcmp(command, CMD_STOP) == 0) {
        return Execute_STOP_Command(); // Appelle la fonction dédiée
    }
    // PAT<N>
    else if (strncmp(command, CMD_PAT, strlen(CMD_PAT)) == 0 && strlen(command) == strlen(CMD_PAT) + 1) {
        char patternNumChar = command[strlen(CMD_PAT)];
        if (patternNumChar >= '1' && patternNumChar <= ('0' + PATTERN_COUNT)) {
            uint8_t patternNumber = patternNumChar - '0';
            if (Pattern_Start(patternNumber)) {
                char msg[40];
                snprintf(msg, sizeof(msg), "Chenillard %d active\r\n", patternNumber);
                Send_Success_Message(msg);
                return true;
            } else {
                Send_Error_Message("Impossible de demarrer chenillard");
                return false;
            }
        } else {
             Send_Error_Message("Numero PAT invalide (1-3)");
             return false;
        }
    }
    // FREQ<F>
    else if (strncmp(command, CMD_FREQ, strlen(CMD_FREQ)) == 0 && strlen(command) == strlen(CMD_FREQ) + 1) {
        char freqNumChar = command[strlen(CMD_FREQ)];
        Pattern_Frequency patternFreq;
        const char* freqStr = NULL;

        switch (freqNumChar) {
            case '1': patternFreq = PATTERN_FREQ_500MS; freqStr = "500MS"; break;
            case '2': patternFreq = PATTERN_FREQ_1S; freqStr = "1S"; break;
            case '3': patternFreq = PATTERN_FREQ_3S; freqStr = "3S"; break;
            default:
                 Send_Error_Message("Numero FREQ invalide (1-3)");
                 return false;
        }

        if (Pattern_SetFrequency(patternFreq)) {
            char msg[40];
            snprintf(msg, sizeof(msg), "Frequence reglee a %s\r\n", freqStr);
            Send_Success_Message(msg);
            return true;
        } else {
             Send_Error_Message("Impossible de regler frequence");
             return false;
        }
    }

    Send_Error_Message("Raccourci invalide");
    return false;
}

/**
  * @brief  Execute le STOP command
  * @retval true si la commande est traitée avec succès, false sinon
  */
static bool Execute_STOP_Command(void)
{
    if (Pattern_Stop()) {
        Send_Success_Message("Chenillard arrete\r\n");
        return true;
    } else {
        Send_Error_Message("Aucun chenillard actif a arreter");
        // Considérer comme traité pour éviter "commande inconnue"
        return true;
    }
}

/**
  * @brief  Execute le STATUS command
  * @retval true si la commande est traitée avec succès, false sinon
  */
static bool Execute_STATUS_Command(void)
{
    char buffer[80];
    UART_SendString("--- Statut ---\r\n");

    // LEDs
    for (uint8_t i = 1; i <= LED_COUNT; i++) {
        snprintf(buffer, sizeof(buffer), "LED %d: %s\r\n", i, (LED_GetState(i) == LED_ON) ? CMD_ON : CMD_OFF);
        UART_SendString(buffer);
    }

    // Chenillard
    Pattern_Type activePat = Pattern_GetActive();
    Pattern_Frequency currentFreq = Pattern_GetFrequency();
    const char* freqStr = (currentFreq == PATTERN_FREQ_500MS) ? "500MS" : ((currentFreq == PATTERN_FREQ_1S) ? "1S" : "3S");
    if (activePat == PATTERN_NONE) {
        snprintf(buffer, sizeof(buffer), "Chenillard: INACTIF (Freq select: %s)\r\n", freqStr);
    } else {
        snprintf(buffer, sizeof(buffer), "Chenillard: ACTIF (Pattern: %d, Freq: %s)\r\n", activePat, freqStr);
    }
    UART_SendString(buffer);

    // Overflow UART?
    if (UART_HasOverflow()) {
        UART_SendString("Attention: Debordement buffer UART detecte!\r\n");
    }

    return true;
}

/**
  * @brief  Envoi d'un message de succès
  */
static void Send_Success_Message(const char* message)
{
    char buffer[100];
    // Assurer que le message original finit par \r\n pour la clarté
    size_t msgLen = strlen(message);
    if (msgLen > 2 && message[msgLen-2] == '\r' && message[msgLen-1] == '\n') {
        snprintf(buffer, sizeof(buffer), "[OK] %s", message);
    } else {
        snprintf(buffer, sizeof(buffer), "[OK] %s\r\n", message);
    }
    UART_SendString(buffer);
}

/**
  * @brief  Envoi d'un message d'erreur
  */
static void Send_Error_Message(const char* message)
{
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "[ERR] %s\r\n", message);
    UART_SendString(buffer);
}

