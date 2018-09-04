#include "TShift.h"
#include "defines.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int TShift::Min()
{
#define k 0
#define mul 2

    static const int m[ENumPointsFPGA::Size][3] =
    {
        { -256 * mul + k,  -128 * mul + k, 0 * mul + k},  // 512
        { -512 * mul + k,  -256 * mul + k, 0 * mul + k},  // 1024
        {-1024 * mul + k,  -512 * mul + k, 0 * mul + k},  // 2048
        {-2048 * mul + k, -1024 * mul + k, 0 * mul + k},  // 4096
        {-4096 * mul + k, -2048 * mul + k, 0 * mul + k}   // 8192
    };

    if (SET_PEAKDET_DIS)
    {
        return m[(int)FPGA_ENUM_POINTS][TPOS];
    }

    return m[(int)FPGA_ENUM_POINTS][TPOS] * 2; // ��� �������� ��������� ��� �������� ������ ���� � ��� ���� ������

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int TShift::Zero()
{
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int TShift::Max()
{
    return 60000;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int TShift::InPoints()
{
    return SET_PEAKDET_EN ? value : (value * 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
TShift::TShift(int tshift)
{
    Set(tshift);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void TShift::Set(int tShift)
{
    value = tShift;
    LIMITATION(value, Min(), Max());
}
