/*
 * uart_interface.c
 *
 *  Created on: 8Aug.,2017
 *      Author: yuri
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "error_code.h"
#include "uart_interface.h"
#include "i2c_interface.h"
#include "string.h"

uint8_t RxBuffer_uart_1 [128];
uint8_t RxBuffer_index_uart_1 = 0;
uint8_t message_avaliable = 0;
uint8_t RxBuf_1 = 0;


//List of commands from the terminal
static const command_message_t command_message_list[COMMAND_MAX] = {
	{ERROR_MESSAGE, "", "Message not found\r\n"},
	{PRINT_LOG, "print_log", "Printing all logs\r\n"}
};


extern UART_HandleTypeDef huart6;
extern sample_log_t i2c_device_logging;

/**
 * Write the message to the UART
 * @param pTxData
 * @param size
 * @return
 */
error_code_t uart_write(uint8_t const *pTxData, const uint16_t size)
{                                                                           // Make sure DMA isn't already running will return busy otherwise
        if (size > 0) {                                                                                     // Make sure not trying to Tx datalength = 0
            HAL_StatusTypeDef rc = HAL_UART_Transmit(&huart6, (uint8_t*) pTxData, (uint16_t) size, 10);

            if (rc != HAL_OK) {                                                                             // If starting DMA returned error
                return ERROR_UART_SEND;
            } else {
            	return ERROR_OK;                                                                      // If DMA started successfully make uart not ready
            }
        }

    return ERROR_OK;
}

/**
 * This callback will fill up the buffer comment
 * @param UartHandle
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if (UartHandle->Instance == USART6){
        HAL_UART_Receive_IT(&huart6, &RxBuf_1, 1);
        if (RxBuf_1 == '\r' || RxBuf_1 == '\n') message_avaliable = 1;
        if (RxBuffer_index_uart_1 != 0xff && RxBuf_1 != '\0'){
            RxBuffer_uart_1[RxBuffer_index_uart_1++] = RxBuf_1;
        }
    }else{
        //error handler
    }
}
/**
 * Flush the uart
 */
void flush_uart(void){
	uint8_t buf;
	HAL_UART_Receive_IT(&huart6,&buf,1);
}

/**
 * check if there is a new message
 * @return
 */
uint8_t is_message_avaliable(){
	return message_avaliable;
}
/**
 * It interprets the command line
 * @return the index of the command
 */
command_code_t read_command_line(){
	message_avaliable = 0;
	RxBuffer_index_uart_1 = 0;
	for(command_code_t index = 1; index < COMMAND_MAX; index ++){
		if (strncmp(command_message_list[index].input_message, RxBuffer_uart_1,
				strlen(command_message_list[index].input_message)) == 0) {
			return index;
		}
	}

	return ERROR_MESSAGE;
}


/**
 * Prints the whole log in roll style
 */
void print_log(){
	uint8_t temp_buffer[128];
	uint8_t first_point = i2c_device_logging.index;
	for(uint8_t i = 0; i < SAMPLE_SIZE; i++){
		snprintf ((char*)temp_buffer, sizeof((char*)temp_buffer), "accel_x:%d, accel_y:%d, accel_z:%d, button_c:%d, button_z:%d\r\n",
				i2c_device_logging.sample[((first_point + i)%SAMPLE_SIZE)].accel_x,
				i2c_device_logging.sample[((first_point + i)%SAMPLE_SIZE)].accel_y,
				i2c_device_logging.sample[((first_point + i)%SAMPLE_SIZE)].accel_z,
				i2c_device_logging.sample[((first_point + i)%SAMPLE_SIZE)].button_c,
				i2c_device_logging.sample[((first_point + i)%SAMPLE_SIZE)].button_z);
		uart_write(temp_buffer,strlen((char*)temp_buffer));
		vTaskDelay(50);
	}
}


//	Task that answers the uart communication
void uart_interface_task(void const * argument)
{
	uart_write((uint8_t*)"Boart Test\r\n",12);
	flush_uart();
	for(;;){


		vTaskDelay(100);

		if (is_message_avaliable()){
			command_code_t code = read_command_line();
			uart_write((uint8_t*)command_message_list[code].output_message,strlen(command_message_list[code].output_message));

			switch(code){
			case PRINT_LOG:
				print_log();
				break;

			default:
				break;
			}
		}
	}
}
