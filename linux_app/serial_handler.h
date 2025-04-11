#ifndef SERIAL_HANDLER_H
#define SERIAL_HANDLER_H

/**
 * @file serial_handler.h
 * @brief En-tête pour le module de gestion de la communication série
 * @author 
 * @date 07-04-2025
 * 
 * Ce fichier contient les prototypes des fonctions pour la gestion de la
 * communication série avec le microcontrôleur STM32F756ZG.
 */

#include <stdbool.h>

/**
 * @brief Initialisation du module de communication série
 */
void Serial_Init(void);

/**
 * @brief Ouverture du port série
 * @param port Nom du port série à ouvrir
 * @return true si l'ouverture a réussi, false sinon
 */
bool Serial_Open(const char *port);

/**
 * @brief Fermeture du port série
 */
void Serial_Close(void);

/**
 * @brief Envoi d'une commande au microcontrôleur
 * @param command Commande à envoyer
 * @return true si l'envoi a réussi, false sinon
 */
bool Serial_SendCommand(const char *command);

/**
 * @brief Réception d'une réponse du microcontrôleur
 * @param response Buffer pour stocker la réponse
 * @param size Taille du buffer
 * @return true si la réception a réussi, false sinon
 */
bool Serial_ReceiveResponse(char *response, size_t size);

/**
 * @brief Configuration du port série
 * @param baudrate Vitesse de communication
 * @return true si la configuration a réussi, false sinon
 */
bool Serial_Configure(int baudrate);

#endif /* SERIAL_HANDLER_H */
