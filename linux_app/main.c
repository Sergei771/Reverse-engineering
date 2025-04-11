#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include "serial_handler.h"
#include "command_validator.h"
#include "ui_handler.h"
#include "special_commands.h"

/**
 * @file main.c
 * @brief Programme principal de l'application console Linux
 * @author 
 * @date 07-04-2025
 * 
 * Ce fichier contient le point d'entrée du programme et la boucle principale
 * pour l'application console Linux qui communique avec le microcontrôleur
 * STM32F756ZG via une liaison série.
 */

#define MAX_COMMAND_LENGTH 128

/* Prototypes de fonctions privées */
static void initialize(void);
static void cleanup(void);

/**
 * @brief Point d'entrée principal du programme
 * @param argc Nombre d'arguments
 * @param argv Tableau des arguments
 * @return Code de retour du programme
 */
int main(int argc, char *argv[])
{
    char command[MAX_COMMAND_LENGTH];
    bool running = true;
    char *port = "/dev/ttyACM0"; // Port série par défaut
    
    // Traitement des arguments de ligne de commande
    if (argc > 1) {
        port = argv[1];
    }
    
    // Initialisation des modules
    initialize();
    
    // Ouverture du port série
    if (!Serial_Open(port)) {
        UI_DisplayError("Impossible d'ouvrir le port série");
        return EXIT_FAILURE;
    }
    
    // Affichage du message de bienvenue et des commandes disponibles
    UI_DisplayWelcome();
    UI_DisplayHelp();
    
    // Boucle principale
    while (running) {
        // Affichage du prompt et lecture de la commande
        UI_DisplayPrompt();
        if (UI_ReadCommand(command, MAX_COMMAND_LENGTH)) {
            
            // Traitement des commandes spéciales
            if (Special_IsSpecialCommand(command)) {
                if (Special_ProcessCommand(command) == SPECIAL_CMD_QUIT) {
                    running = false;
                }
                continue;
            }
            
            // Validation de la commande
            if (Command_Validate(command)) {
                // Envoi de la commande au microcontrôleur
                if (Serial_SendCommand(command)) {
                    // Attente et affichage de la réponse
                    char response[256];
                    if (Serial_ReceiveResponse(response, sizeof(response))) {
                        UI_DisplayResponse(response);
                    } else {
                        UI_DisplayError("Pas de réponse du microcontrôleur");
                    }
                } else {
                    UI_DisplayError("Erreur lors de l'envoi de la commande");
                }
            } else {
                UI_DisplayError("Commande invalide");
            }
        }
    }
    
    // Nettoyage avant de quitter
    cleanup();
    
    return EXIT_SUCCESS;
}

/**
 * @brief Initialisation des modules de l'application
 */
static void initialize(void)
{
    UI_Init();
    Serial_Init();
    Command_Init();
    Special_Init();
}

/**
 * @brief Nettoyage avant de quitter l'application
 */
static void cleanup(void)
{
    Serial_Close();
    UI_Cleanup();
}
