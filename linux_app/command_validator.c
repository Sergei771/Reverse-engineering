#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "command_validator.h"

/**
 * @file command_validator.c
 * @brief Module de validation de format des commandes (simplifié)
 * @author 
 * @date 07-04-2025
 * 
 * Ce fichier implémente les fonctions pour la validation des commandes
 * avant leur envoi au microcontrôleur.
 */

// Les listes prédéfinies ne sont plus utilisées pour la validation,
// mais pourraient servir pour une future auto-complétion.
/*
static const char *VALID_LED_COMMANDS[] = { ... };
static const char *VALID_PATTERN_COMMANDS[] = { ... };
static char validCommandsBuffer[1024];
*/

/**
 * @brief Initialisation (plus nécessaire pour la validation simple)
 */
void Command_Init(void)
{
    // Rien à faire ici maintenant
}

/**
 * @brief Validation du format d'une commande (plus flexible)
 * @param command Commande à valider
 * @return true si le format semble valide, false sinon
 */
bool Command_Validate(const char *command)
{
    if (command == NULL || strlen(command) == 0) {
        return false;
    }

    // Copie pour manipulation (et mise en majuscule)
    char upperCommand[128];
    size_t len = 0;
    while (command[len] != '\0' && len < sizeof(upperCommand) - 1) {
        upperCommand[len] = toupper((unsigned char)command[len]);
        len++;
    }
    upperCommand[len] = '\0';

    // --- Vérification des formats --- 

    // Commandes simples (un seul mot)
    if (strcmp(upperCommand, "HELP") == 0) return true;
    if (strcmp(upperCommand, "STATUS") == 0) return true;
    if (strcmp(upperCommand, "STOP") == 0) return true;
    if (strcmp(upperCommand, "CLEAR") == 0) return true;
    if (strcmp(upperCommand, "QUIT") == 0) return true;

    // Commandes LED: LED<N> ON/OFF
    if (strncmp(upperCommand, "LED", 3) == 0) {
        if (len >= 6 && // LED1 ON
            isdigit((unsigned char)upperCommand[3]) &&
            upperCommand[4] == ' ' &&
            (strcmp(upperCommand + 5, "ON") == 0 || strcmp(upperCommand + 5, "OFF") == 0))
        {
            // Vérifier si N est dans la plage valide (optionnel ici, fait par STM32)
            // int ledNum = upperCommand[3] - '0';
            // if (ledNum >= 1 && ledNum <= 3) return true;
            return true; // Accepter le format
        }
    }

    // Commandes CHENILLARD: CHENILLARD<N> ON ou CHENILLARD FREQUENCE<F>
    if (strncmp(upperCommand, "CHENILLARD", 10) == 0) {
        const char* args = upperCommand + 10;
        // CHENILLARD<N> ON
        if (strlen(args) >= 4 && // N ON
            isdigit((unsigned char)args[0]) &&
            strcmp(args + 1, " ON") == 0)
        {
            return true;
        }
        // CHENILLARD FREQUENCE<F>
        if (strncmp(args, " FREQUENCE", 10) == 0 &&
            strlen(args) == 11 && // " FREQUENCEF"
            isdigit((unsigned char)args[10]))
        {
             return true;
        }
    }

    // Raccourcis: PAT<N> ou FREQ<F>
    if (strncmp(upperCommand, "PAT", 3) == 0) {
        if (len == 4 && isdigit((unsigned char)upperCommand[3])) return true;
    }
    if (strncmp(upperCommand, "FREQ", 4) == 0) {
        if (len == 5 && isdigit((unsigned char)upperCommand[4])) return true;
    }

    // Si aucun format ne correspond
    return false;
}

/**
 * @brief Obtention de la liste des commandes valides (plus utilisé pour l'aide)
 * @return Chaîne vide ou NULL
 */
const char* Command_GetValidCommands(void)
{
    // Cette fonction n'est plus nécessaire car l'aide est codée en dur dans ui_handler
    return ""; // Retourner chaine vide
}
