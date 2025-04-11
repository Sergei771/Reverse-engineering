#ifndef SPECIAL_COMMANDS_H
#define SPECIAL_COMMANDS_H

/**
 * @file special_commands.h
 * @brief En-tête pour le module de gestion des commandes spéciales
 * @author 
 * @date 07-04-2025
 * 
 * Ce fichier contient les prototypes des fonctions pour la gestion des
 * commandes spéciales (help, clear, quit).
 */

#include <stdbool.h>

/* Définition des codes de retour des commandes spéciales */
typedef enum {
    SPECIAL_CMD_NONE = 0,
    SPECIAL_CMD_HELP,
    SPECIAL_CMD_CLEAR,
    SPECIAL_CMD_QUIT
} SpecialCommandCode;

/**
 * @brief Initialisation du module de gestion des commandes spéciales
 */
void Special_Init(void);

/**
 * @brief Vérification si une commande est une commande spéciale
 * @param command Commande à vérifier
 * @return true si c'est une commande spéciale, false sinon
 */
bool Special_IsSpecialCommand(const char *command);

/**
 * @brief Traitement d'une commande spéciale
 * @param command Commande à traiter
 * @return Code de la commande spéciale exécutée
 */
SpecialCommandCode Special_ProcessCommand(const char *command);

#endif /* SPECIAL_COMMANDS_H */
