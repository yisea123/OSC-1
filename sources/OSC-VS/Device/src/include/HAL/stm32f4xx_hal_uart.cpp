#include "stdafx.h"
#include "stm32f4xx_hal_uart.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *, uint8_t *, uint16_t)
{
    return HAL_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *, uint8_t *, uint16_t, uint32_t)
{
    return HAL_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *)
{
    return HAL_ERROR;
}
