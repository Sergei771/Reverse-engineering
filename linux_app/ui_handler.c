#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "ui_handler.h"
#include "command_validator.h"

/**
 * @file ui_handler.c
 * @brief Module de gestion de l'interface utilisateur
 * @author 
 * @date 07-04-2025
 * 
 * Ce fichier implémente les fonctions pour la gestion de l'interface
 * utilisateur en ligne de commande.
 */

/* Variables privées */
static struct termios oldTermios;
static const char *PROMPT = "STM32> ";
static const char *APP_NAME = "Application Console STM32F756ZG";
static const char *APP_VERSION = "1.0";

/* Prototypes de fonctions privées */
static void UI_SaveTerminalSettings(void);
static void UI_RestoreTerminalSettings(void);

/**
 * @brief Initialisation du module d'interface utilisateur
 */
void UI_Init(void)
{
    // Sauvegarde des paramètres du terminal
    UI_SaveTerminalSettings();
}

/**
 * @brief Nettoyage du module d'interface utilisateur
 */
void UI_Cleanup(void)
{
    // Restauration des paramètres du terminal
    UI_RestoreTerminalSettings();
}

/**
 * @brief Affichage du message de bienvenue
 */
void UI_DisplayWelcome(void)
{
    printf("\n");
    printf("==========================================================\n");
    printf("  %s v%s\n", APP_NAME, APP_VERSION);
    printf("  Communication série bidirectionnelle avec STM32F756ZG\n");
    printf("==========================================================\n");
    printf("\n");
}

/**
 * @brief Affichage de l'aide (liste des commandes)
 */
void UI_DisplayHelp(void)
{
    printf("Commandes disponibles:\n");
    printf("  LED<1-3> ON|OFF  : Controle une LED specifique.\n");
    printf("  PAT<1-3>         : Demarre le chenillard N (utilise la frequence courante).\n");
    printf("  FREQ<1-3>        : Definit la frequence (1:500ms, 2:1s, 3:3s) pour les chenillards.\n");
    printf("  STOP             : Arrete le chenillard actif.\n");
    printf("  STATUS           : Affiche l'etat des LEDs, du chenillard et de la frequence.\n");
    printf("  HELP             : Affiche cette aide.\n");
    printf("  CLEAR            : Efface l'ecran du terminal.\n");
    printf("  QUIT             : Quitte l'application.\n");
    printf("Note: Les commandes sont insensibles a la casse.\n");
    printf("\n");
}

/**
 * @brief Affichage du prompt
 */
void UI_DisplayPrompt(void)
{
    printf("%s", PROMPT);
    fflush(stdout);
}

/**
 * @brief Lecture d'une commande
 * @param buffer Buffer pour stocker la commande
 * @param size Taille du buffer
 * @return true si la lecture a réussi, false sinon
 */
bool UI_ReadCommand(char *buffer, size_t size)
{
    if (buffer == NULL || size == 0) {
        return false;
    }
    
    // Lecture de la ligne
    if (fgets(buffer, size, stdin) == NULL) {
        return false;
    }
    
    // Suppression du retour à la ligne
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return true;
}

/**
 * @brief Affichage d'une réponse
 * @param response Réponse à afficher
 */
void UI_DisplayResponse(const char *response)
{
    if (response != NULL) {
        printf("Réponse: %s\n", response);
    }
}

/**
 * @brief Affichage d'un message d'erreur
 * @param error Message d'erreur à afficher
 */
void UI_DisplayError(const char *error)
{
    if (error != NULL) {
        fprintf(stderr, "Erreur: %s\n", error);
    }
}

/**
 * @brief Effacement de l'écran
 */
void UI_ClearScreen(void)
{
    // Séquence d'échappement ANSI pour effacer l'écran et positionner le curseur en haut à gauche
    printf("\033[2J\033[H");
    fflush(stdout);
}

/**
 * @brief Sauvegarde des paramètres du terminal
 */
static void UI_SaveTerminalSettings(void)
{
    tcgetattr(STDIN_FILENO, &oldTermios);
}

/**
 * @brief Restauration des paramètres du terminal
 */
static void UI_RestoreTerminalSettings(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);
}

