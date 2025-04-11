#ifndef COMMAND_VALIDATOR_H
#define COMMAND_VALIDATOR_H

/**
 * @file command_validator.h
 * @brief En-tête pour le module de validation des commandes
 * @author 
 * @date 07-04-2025
 * 
 * Ce fichier contient les prototypes des fonctions pour la validation
 * des commandes avant leur envoi au microcontrôleur.
 */

#include <stdbool.h>

/**
 * @brief Initialisation du module de validation des commandes
 */
void Command_Init(void);

/**
 * @brief Validation d'une commande
 * @param command Commande à valider
 * @return true si la commande est valide, false sinon
 */
bool Command_Validate(const char *command);

/**
 * @brief Obtention de la liste des commandes valides
 * @return Chaîne de caractères contenant la liste des commandes valides
 */
const char* Command_GetValidCommands(void);

#endif /* COMMAND_VALIDATOR_H */
