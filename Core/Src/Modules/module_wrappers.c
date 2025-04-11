/**
  ******************************************************************************
  * @file           : module_wrappers.c
  * @brief          : Fonctions wrapper pour harmoniser les noms entre main.c et les modules
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Modules/led_controller.h"
#include "Modules/pattern_controller.h"
#include "Modules/timer_handler.h"
#include "Modules/uart_handler.h"
#include "Modules/command_parser.h"
#include "usart.h"

/* Fonctions d'adaptation (wrappers) -----------------------------------------*/

/**
  * @brief  Wrapper pour LED_Controller_Init
  */
void LED_Init(void)
{
    LED_Controller_Init();
}

/**
  * @brief  Wrapper pour Pattern_Init
  */
void PATTERN_Init(void)
{
    Pattern_Controller_Init();
}

/**
  * @brief  Wrapper pour Timer_Init
  */
void TIMER_Init(void)
{
    Timer_Init();
}

/**
  * @brief  Wrapper pour UART_Handler_Init
  */
/* bool UART_Init(void) // Wrapper supprimé
{
    return UART_Init();
}
*/

/**
  * @brief  Wrapper pour Command_Parser_Init
  */
void COMMAND_Init(void)
{
    Command_Parser_Init();
}

/**
  * @brief  Wrapper pour Command_Parser_ProcessCommands
  */
void COMMAND_Process(void)
{
    Command_Parser_ProcessCommands();
}

/**
  * @brief  Wrapper pour Pattern_Process
  */
void PATTERN_Process(void)
{
    Pattern_Controller_Update();
} 
