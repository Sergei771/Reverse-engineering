# STM32 Console Application

Application console Linux pour la communication avec un microcontrôleur STM32F756ZG via liaison série.

## Prérequis

- Kali Linux (ou autre distribution Linux)
- GCC (GNU Compiler Collection)
- Accès au port série (généralement `/dev/ttyACM0`)

## Installation des dépendances

```bash
sudo apt update
sudo apt install build-essential
```

## Compilation

### Compilation standard
```bash
make
```

### Compilation en mode debug
```bash
make DEBUG=1
```

## Installation

Pour installer l'application dans le système :
```bash
sudo make install
```

## Utilisation

### Lancement
```bash
stm32_console [port_serie]
```
Si aucun port n'est spécifié, `/dev/ttyACM0` sera utilisé par défaut.

### Commandes disponibles
- `help` : Affiche l'aide
- `clear` : Efface l'écran
- `quit` : Quitte l'application

## Nettoyage

Pour nettoyer les fichiers de compilation :
```bash
make clean
```

Pour un nettoyage complet (y compris les fichiers temporaires) :
```bash
make distclean
```

## Structure du projet

- `main.c` : Point d'entrée de l'application
- `serial_handler.[ch]` : Gestion de la communication série
- `ui_handler.[ch]` : Interface utilisateur en ligne de commande
- `command_validator.[ch]` : Validation des commandes
- `special_commands.[ch]` : Gestion des commandes spéciales

## Permissions

Assurez-vous que l'utilisateur a les droits d'accès au port série :
```bash
sudo usermod -a -G dialout $USER
```
(Redémarrez la session utilisateur après cette commande)

## Dépannage

Si vous rencontrez des problèmes de communication :
1. Vérifiez que le port série est correctement détecté
2. Assurez-vous que l'utilisateur a les droits nécessaires
3. Vérifiez la connexion physique avec le microcontrôleur
4. Compilez en mode debug pour plus d'informations 