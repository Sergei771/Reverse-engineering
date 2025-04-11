#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "special_commands.h"
#include "ui_handler.h"

/**
 * @file special_commands.c
 * @brief Module de gestion des commandes spéciales
 * @author 
 * @date 07-04-2025
 * 
 * Ce fichier implémente les fonctions pour la gestion des commandes
 * spéciales (help, clear, quit).
 */

/* Définition des commandes spéciales */
static const char *CMD_HELP = "help";
static const char *CMD_CLEAR = "clear";
static const char *CMD_QUIT = "quit";

/**
 * @brief Initialisation du module de gestion des commandes spéciales
 */
void Special_Init(void)
{
    // Rien à initialiser pour le moment
}

/**
 * @brief Vérification si une commande est une commande spéciale
 * @param command Commande à vérifier
 * @return true si c'est une commande spéciale, false sinon
 */
bool Special_IsSpecialCommand(const char *command)
{
    if (command == NULL || strlen(command) == 0) {
        return false;
    }
    
    // Copie de la commande pour la normalisation
    char normalizedCommand[128];
    strncpy(normalizedCommand, command, sizeof(normalizedCommand) - 1);
    normalizedCommand[sizeof(normalizedCommand) - 1] = '\0';
    
    // Conversion en minuscules pour la comparaison
    for (int i = 0; normalizedCommand[i]; i++) {
        normalizedCommand[i] = tolower((unsigned char)normalizedCommand[i]);
    }
    
    // Vérification des commandes spéciales
    if (strcmp(normalizedCommand, CMD_HELP) == 0 ||
        strcmp(normalizedCommand, CMD_CLEAR) == 0 ||
        strcmp(normalizedCommand, CMD_QUIT) == 0) {
        return true;
    }
    
    return false;
}

/**
 * @brief Traitement d'une commande spéciale
 * @param command Commande à traiter
 * @return Code de la commande spéciale exécutée
 */
SpecialCommandCode Special_ProcessCommand(const char *command)
{
    if (command == NULL || strlen(command) == 0) {
        return SPECIAL_CMD_NONE;
    }
    
    // Copie de la commande pour la normalisation
    char normalizedCommand[128];
    strncpy(normalizedCommand, command, sizeof(normalizedCommand) - 1);
    normalizedCommand[sizeof(normalizedCommand) - 1] = '\0';
    
    // Conversion en minuscules pour la comparaison
    for (int i = 0; normalizedCommand[i]; i++) {
        normalizedCommand[i] = tolower((unsigned char)normalizedCommand[i]);
    }
    
    // Traitement des commandes spéciales
    if (strcmp(normalizedCommand, CMD_HELP) == 0) {
        UI_DisplayHelp();
        return SPECIAL_CMD_HELP;
    } else if (strcmp(normalizedCommand, CMD_CLEAR) == 0) {
        UI_ClearScreen();
        return SPECIAL_CMD_CLEAR;
    } else if (strcmp(normalizedCommand, CMD_QUIT) == 0) {
        printf("Au revoir !\n");
        return SPECIAL_CMD_QUIT;
    }
    
    return SPECIAL_CMD_NONE;
}
