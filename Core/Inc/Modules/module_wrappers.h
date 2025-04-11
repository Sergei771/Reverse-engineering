/**
  ******************************************************************************
  * @file           : module_wrappers.h
  * @brief          : Header pour module_wrappers.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODULE_WRAPPERS_H
#define __MODULE_WRAPPERS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Prototypes des fonctions -------------------------------------------------*/
void LED_Init(void);
void PATTERN_Init(void);
void TIMER_Init(void);
void COMMAND_Init(void);
void COMMAND_Process(void);
void PATTERN_Process(void);

#ifdef __cplusplus
}
#endif

#endif /* __MODULE_WRAPPERS_H */ 