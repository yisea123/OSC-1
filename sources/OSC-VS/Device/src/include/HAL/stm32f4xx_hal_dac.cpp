#include "stdafx.h"
#include "stm32f4xx_hal_dac.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef HAL_DAC_SetValue(DAC_HandleTypeDef *, uint32_t, uint32_t, uint32_t)
{
    return HAL_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_DAC_Start(DAC_HandleTypeDef *, uint32_t)
{
    return HAL_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_DAC_Start_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t* pData, uint32_t Length, uint32_t Alignment)
{
    return HAL_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_DAC_Stop_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel)
{
    return HAL_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_DAC_DeInit(DAC_HandleTypeDef* hdac)
{
    return HAL_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_DAC_ConfigChannel(DAC_HandleTypeDef* hdac, DAC_ChannelConfTypeDef* sConfig, uint32_t Channel)
{
    return HAL_ERROR;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
HAL_StatusTypeDef HAL_DAC_Init(DAC_HandleTypeDef* hdac)
{
    return HAL_ERROR;
}
