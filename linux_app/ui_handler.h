#ifndef UI_HANDLER_H
#define UI_HANDLER_H

/**
 * @file ui_handler.h
 * @brief En-tête pour le module de gestion de l'interface utilisateur
 * @author 
 * @date 07-04-2025
 * 
 * Ce fichier contient les prototypes des fonctions pour la gestion de
 * l'interface utilisateur en ligne de commande.
 */

#include <stdbool.h>

/**
 * @brief Initialisation du module d'interface utilisateur
 */
void UI_Init(void);

/**
 * @brief Nettoyage du module d'interface utilisateur
 */
void UI_Cleanup(void);

/**
 * @brief Affichage du message de bienvenue
 */
void UI_DisplayWelcome(void);

/**
 * @brief Affichage de l'aide (liste des commandes)
 */
void UI_DisplayHelp(void);

/**
 * @brief Affichage du prompt
 */
void UI_DisplayPrompt(void);

/**
 * @brief Lecture d'une commande
 * @param buffer Buffer pour stocker la commande
 * @param size Taille du buffer
 * @return true si la lecture a réussi, false sinon
 */
bool UI_ReadCommand(char *buffer, size_t size);

/**
 * @brief Affichage d'une réponse
 * @param response Réponse à afficher
 */
void UI_DisplayResponse(const char *response);

/**
 * @brief Affichage d'un message d'erreur
 * @param error Message d'erreur à afficher
 */
void UI_DisplayError(const char *error);

/**
 * @brief Effacement de l'écran
 */
void UI_ClearScreen(void);

#endif /* UI_HANDLER_H */
