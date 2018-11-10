#pragma once
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_gpio_ex.h"


#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define  GPIO_MODE_INPUT            0x00000000U   /*!< Input Floating Mode                   */
#define  GPIO_MODE_OUTPUT_PP        0x00000001U   /*!< Output Push Pull Mode                 */
#define  GPIO_MODE_OUTPUT_OD        0x00000011U   /*!< Output Open Drain Mode                */
#define  GPIO_MODE_AF_PP            0x00000002U   /*!< Alternate Function Push Pull Mode     */
#define  GPIO_MODE_AF_OD            0x00000012U   /*!< Alternate Function Open Drain Mode    */

#define  GPIO_NOPULL                0x00000000U   /*!< No Pull-up or Pull-down activation  */
#define  GPIO_PULLUP                0x00000001U   /*!< Pull-up activation                  */
#define  GPIO_PULLDOWN              0x00000002U   /*!< Pull-down activation                */

#define  GPIO_SPEED_FREQ_LOW         0x00000000U  /*!< IO works at 2 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_MEDIUM      0x00000001U  /*!< range 12,5 MHz to 50 MHz, please refer to the product datasheet */
#define  GPIO_SPEED_FREQ_HIGH        0x00000002U  /*!< range 25 MHz to 100 MHz, please refer to the product datasheet  */
#define  GPIO_SPEED_FREQ_VERY_HIGH   0x00000003U  /*!< range 50 MHz to 200 MHz, please refer to the product datasheet  */

#define  GPIO_MODE_IT_RISING         0x10110000U   /*!< External Interrupt Mode with Rising edge trigger detection          */
#define  GPIO_MODE_IT_FALLING        0x10210000U   /*!< External Interrupt Mode with Falling edge trigger detection         */
#define  GPIO_MODE_IT_RISING_FALLING 0x10310000U   /*!< External Interrupt Mode with Rising/Falling edge trigger detection  */

#define GPIO_AF6_SPI3               ((uint8_t)0x06)  /* SPI3/I2S3 Alternate Function mapping  */

#define GPIO_MODE_ANALOG            0x00000003U   /*!< Analog Mode  */


typedef enum
{
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
}GPIO_PinState;


typedef struct
{
    uint32_t Pin;       /*!< Specifies the GPIO pins to be configured.
                        This parameter can be any value of @ref GPIO_pins_define */

    uint32_t Mode;      /*!< Specifies the operating mode for the selected pins.
                        This parameter can be a value of @ref GPIO_mode_define */

    uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                        This parameter can be a value of @ref GPIO_pull_define */

    uint32_t Speed;     /*!< Specifies the speed for the selected pins.
                        This parameter can be a value of @ref GPIO_speed_define */

    uint32_t Alternate;  /*!< Peripheral to be connected to the selected pins. 
                         This parameter can be a value of @ref GPIO_Alternate_function_selection */
}GPIO_InitTypeDef;


void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void  HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);