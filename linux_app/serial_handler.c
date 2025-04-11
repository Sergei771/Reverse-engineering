#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include "serial_handler.h"

/**
 * @file serial_handler.c
 * @brief Module de gestion de la communication série
 * @author 
 * @date 07-04-2025
 * 
 * Ce fichier implémente les fonctions pour la gestion de la communication
 * série avec le microcontrôleur STM32F756ZG.
 */

/* Variables privées */
static int serialFd = -1;
static struct termios oldtio;
static const int DEFAULT_BAUDRATE = B115200;

/**
 * @brief Initialisation du module de communication série
 */
void Serial_Init(void)
{
    serialFd = -1;
}

/**
 * @brief Ouverture du port série
 * @param port Nom du port série à ouvrir
 * @return true si l'ouverture a réussi, false sinon
 */
bool Serial_Open(const char *port)
{
    // Fermeture du port s'il est déjà ouvert
    if (serialFd >= 0) {
        Serial_Close();
    }
    
    // Ouverture du port série
    serialFd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialFd < 0) {
        perror("Erreur lors de l'ouverture du port série");
        return false;
    }
    
    // Sauvegarde des paramètres actuels
    if (tcgetattr(serialFd, &oldtio) != 0) {
        perror("Erreur lors de la récupération des paramètres du port série");
        close(serialFd);
        serialFd = -1;
        return false;
    }
    
    // Configuration du port série
    if (!Serial_Configure(DEFAULT_BAUDRATE)) {
        close(serialFd);
        serialFd = -1;
        return false;
    }
    
    // Vidage des buffers
    tcflush(serialFd, TCIOFLUSH);
    
    return true;
}

/**
 * @brief Fermeture du port série
 */
void Serial_Close(void)
{
    if (serialFd >= 0) {
        // Restauration des paramètres d'origine
        tcsetattr(serialFd, TCSANOW, &oldtio);
        
        // Fermeture du port
        close(serialFd);
        serialFd = -1;
    }
}

/**
 * @brief Envoi d'une commande au microcontrôleur
 * @param command Commande à envoyer
 * @return true si l'envoi a réussi, false sinon
 */
bool Serial_SendCommand(const char *command)
{
    if (serialFd < 0) {
        return false;
    }
    
    // Préparation de la commande avec retour chariot simple
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s\r", command); // Envoyer seulement \r
    
    // Envoi de la commande
    ssize_t bytesWritten = write(serialFd, buffer, strlen(buffer));
    if (bytesWritten < 0) {
        perror("Erreur lors de l'envoi de la commande");
        return false;
    }
    
    // Attente que tous les octets soient envoyés
    tcdrain(serialFd);
    
    return true;
}

/**
 * @brief Réception d'une réponse du microcontrôleur
 * @param response Buffer pour stocker la réponse
 * @param size Taille du buffer
 * @return true si la réception a réussi, false sinon
 */
bool Serial_ReceiveResponse(char *response, size_t size)
{
    if (serialFd < 0 || response == NULL || size == 0) {
        return false;
    }

    memset(response, 0, size);
    size_t totalBytesRead = 0;
    ssize_t bytesReadNow = 0;
    fd_set readfds;
    struct timeval timeout;
    bool keepReading = true;

    while (keepReading && totalBytesRead < size - 1) {
        FD_ZERO(&readfds);
        FD_SET(serialFd, &readfds);

        // Timeout court (ex: 50ms) pour détecter la fin de la transmission
        timeout.tv_sec = 0;
        timeout.tv_usec = 50 * 1000; // 50 ms

        int selectResult = select(serialFd + 1, &readfds, NULL, NULL, &timeout);

        if (selectResult < 0) { // Erreur select
            perror("Erreur select() en lecture série");
            return false;
        } else if (selectResult == 0) { // Timeout -> Fin de la réponse
            keepReading = false;
        } else { // Données disponibles
            bytesReadNow = read(serialFd, response + totalBytesRead, size - 1 - totalBytesRead);

            if (bytesReadNow < 0) { // Erreur lecture
                // EAGAIN ou EWOULDBLOCK signifie juste pas de données maintenant, mais select dit qu'il y en a?
                // C'est étrange, mais on peut considérer que c'est la fin pour l'instant.
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    keepReading = false;
                } else {
                    perror("Erreur read() en lecture série");
                    return false;
                }
            } else if (bytesReadNow == 0) { // EOF (port fermé?)
                 keepReading = false; // Probablement la fin
            } else { // Données lues
                totalBytesRead += bytesReadNow;
                // On continue de lire tant que select trouve des données rapidement
            }
        }
    }

    // Assurer la terminaison nulle
    response[totalBytesRead] = '\0';

    // Nettoyer les \r ou \n finaux si présents (optionnel, mais propre)
    while (totalBytesRead > 0 && (response[totalBytesRead - 1] == '\n' || response[totalBytesRead - 1] == '\r')) {
        response[--totalBytesRead] = '\0';
    }

    // Retourner true si on a lu au moins quelque chose
    return (totalBytesRead > 0);
}

/**
 * @brief Configuration du port série
 * @param baudrate Vitesse de communication
 * @return true si la configuration a réussi, false sinon
 */
bool Serial_Configure(int baudrate)
{
    if (serialFd < 0) {
        return false;
    }
    
    struct termios newtio;
    
    // Copie des paramètres actuels
    memcpy(&newtio, &oldtio, sizeof(struct termios));
    
    // Configuration des paramètres de contrôle
    newtio.c_cflag = baudrate | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    newtio.c_cc[VMIN] = 0;   // Non-blocking read
    newtio.c_cc[VTIME] = 0;  // Non-blocking read
    
    // Application des nouveaux paramètres
    if (tcsetattr(serialFd, TCSANOW, &newtio) != 0) {
        perror("Erreur lors de la configuration du port série");
        return false;
    }
    
    // Passage en mode non-bloquant
    fcntl(serialFd, F_SETFL, O_NDELAY);
    
    return true;
}


