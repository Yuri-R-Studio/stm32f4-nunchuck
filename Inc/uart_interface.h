/*
 * uart_interface.h
 *
 *  Created on: 8Aug.,2017
 *      Author: yuri
 */

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

#include "error_code.h"

typedef enum {
	ERROR_MESSAGE,
	PRINT_LOG,
	COMMAND_MAX
}command_code_t;

typedef struct
{
	command_code_t command_code;
	const char* input_message;
	const char* output_message;
} command_message_t;

/**
 * Write the message to the UART
 * @param pTxData
 * @param size
 * @return
 */
error_code_t uart_write(uint8_t const *pTxData, const uint16_t size);

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle);
void uart_interface_task(void const * argument);
#endif /* UART_INTERFACE_H_ */
