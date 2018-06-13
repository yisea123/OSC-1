#pragma once
#include "defines.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DISPLAY_ADD_STRING(x)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Display
{
public:
    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    static void Init();

    static void Update();

    static uint8 *GetBuffer();

    static uint8 *GetBufferEnd();

    static void ToggleBuffers();

private:
    /// �����
    static uint8 *frontBuffer;
    /// ������ �����. � ��� ���������� ���������, � ����� ����������� ���������� �� frontBuffer
    static uint8 *backBuffer;
};


extern LTDC_HandleTypeDef hltdc;
