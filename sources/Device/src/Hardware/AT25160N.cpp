#include "AT25160N.h"
#include "Log.h"
#include "Hardware/Timer.h"
#include "Utils/StringUtils.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static SPI_HandleTypeDef hSPI2 =
{
    SPI2,
    {
        SPI_MODE_MASTER,
        SPI_DIRECTION_2LINES,
        SPI_DATASIZE_8BIT,
        SPI_POLARITY_HIGH,
        SPI_PHASE_1EDGE,
        SPI_NSS_SOFT,
        SPI_BAUDRATEPRESCALER_2,
        SPI_FIRSTBIT_MSB,
        SPI_TIMODE_DISABLE,
        SPI_CRCCALCULATION_DISABLE,
        10
    },
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};

#define PIN_OUT     GPIOC, GPIO_PIN_3
#define PIN_IN      GPIOC, GPIO_PIN_2
#define PIN_CLK     GPIOB, GPIO_PIN_10
#define PIN_CS      GPIOB, GPIO_PIN_12

#define WREN    BIN_U8(00000110)        ///< Set Write Enable Latch
#define WRDI    BIN_U8(00000100)        ///< Reset Write Enable Latch
#define RDSR    BIN_U8(00000101)        ///< Read Status Register
#define READ    BIN_U8(00000011)        ///< Read Data from Memory Array
#define WRITE   BIN_U8(00000010)        ///< Write Data to Memory Array
#define WRSR    BIN_U8(00000001)        ///< Write Status Register


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AT25160N::Init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*  ���������� �������
    SPI2
    PB12 - NSS
    PB10 - SCK
    PC3  - MOSI
    PC2 -  MISO   */

    //__HAL_RCC_SPI2_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =
    {//     SCK         NSS
        GPIO_PIN_10 | GPIO_PIN_12,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };
    HAL_GPIO_Init(GPIOB, &isGPIO);

    //             MOSI
    isGPIO.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOC, &isGPIO);

    //             MISO
    isGPIO.Pin = GPIO_PIN_2;
    isGPIO.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOC, &isGPIO);

    SetPin(PIN_CS);
    ResetPin(PIN_OUT);
    ResetPin(PIN_CLK);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::Test()
{
    const uint size = 10;
    uint8 data[size];
    uint8 out[size];
    for(uint i = 0; i < size; i++)
    {
        data[i] = (uint8)rand();
    }

    SetWriteLatch();
    WriteData(0, data, size);
    ReadData(0, out, size);
    ResetWriteLatch();

    bool testIsOk = true;

    for(uint i = 0; i < size; i++)
    {
        if(data[i] != out[i])
        {
            testIsOk = false;
        }
    }

    if(testIsOk)
    {
        LOG_MESSAGE("Test is OK!!!");
    }
    else
    {
        LOG_MESSAGE("WARNING!!! Test is failed!!!");
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::SetWriteLatch()
{
    //WaitFinishWrite();

    ResetPin(PIN_CS);
    WriteByte(WREN);
    SetPin(PIN_CS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::ResetWriteLatch()
{
    WaitFinishWrite();

    ResetPin(PIN_CS);
    WriteByte(WRDI);
    SetPin(PIN_CS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 AT25160N::ReadStatusRegister()
{
    ResetPin(PIN_CS);
    WriteByte(RDSR);
    uint8 result = ReadByte();
    SetPin(PIN_CS);
    return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::WriteStatusRegister(uint8 data)
{
    ResetPin(PIN_CS);
    WriteByte(WRSR);
    WriteByte(data);
    SetPin(PIN_CS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::WaitFinishWrite()
{
    ResetPin(PIN_CS);

    int isBusy = 1;
    do
    {
        WriteByte(RDSR);
        uint8 status = ReadStatusRegister();
        isBusy = _GET_BIT(status, 0);
    } while (isBusy);

    SetPin(PIN_CS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::Save(Settings &)
{
    /*
    �������� ���������� ��������
    1. ��������� ����� ��������� � ������ ��� ������ ��������� ������.
    2. 
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::Load(Settings &)
{
    /*
    �������� �������� ��������
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::WriteByte(uint8 byte)
{
    for(int bit = 7; bit >= 0; bit--)
    {
        if (_GET_BIT(byte, bit))
        {
            SetPin(PIN_OUT);
        }
        SetPin(PIN_CLK);
        ResetPin(PIN_CLK);
        ResetPin(PIN_OUT);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 AT25160N::ReadByte()
{
    uint8 retValue = 0;

    for(int i = 0; i < 8; i++)
    {
        SetPin(PIN_CLK);
        retValue <<= 1;
        if(HAL_GPIO_ReadPin(PIN_IN) == GPIO_PIN_SET)
        {
            retValue |= 0x01;
        }
        ResetPin(PIN_CLK);
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::SetPin(GPIO_TypeDef *gpio, uint16 pin)
{
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::ResetPin(GPIO_TypeDef *gpio, uint16 pin)
{
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::WriteData(uint address, uint8 *data, uint size)
{
    WaitFinishWrite();

    ResetPin(PIN_CS);

    WriteByte(WRITE);
    WriteByte((address >> 8) & 0xff);
    WriteByte(address & 0xff);

    for(uint i = 0; i < size; i++)
    {
        uint8 byte = data[i];

        for (int bit = 7; bit >= 0; bit--)
        {
            if (_GET_BIT(byte, bit))
            {
                SetPin(PIN_OUT);
            }
            SetPin(PIN_CLK);
            ResetPin(PIN_CLK);
            ResetPin(PIN_OUT);
        }
    }
    
    SetPin(PIN_CS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::ReadData(uint address, uint8 *data, uint size)
{
    WaitFinishWrite();

    ResetPin(PIN_CS);

    WriteByte(READ);
    WriteByte((address >> 8) & 0xff);
    WriteByte(address & 0xff);

    for(uint i = 0; i < size; i++)
    {
        data[i] = ReadByte();
    }

    SetPin(PIN_CS);
}