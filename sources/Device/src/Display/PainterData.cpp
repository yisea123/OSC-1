#include "PainterData.h"
#include "defines.h"
#include "Colors.h"
#include "Grid.h"
#include "Log.h"
#include "Painter.h"
#include "Data/Storage.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/MathOSC.h"
#include "Utils/Math.h"
#include "Data/Reader.h"
#include "Symbols.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StructDataDrawing *PainterData::dataStruct = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PainterData::DrawData()
{
    if(MODE_WORK_IS_DIR)
    {
        DrawData_ModeDir();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawData_ModeDir()
{
    uint8 *dataA = 0;
    uint8 *dataB = 0;

    Storage::GetData(&dataA, &dataB);

    DrawChannel(A, dataA);
    DrawChannel(B, dataB);   

    DrawMemoryWindow();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawChannel(Channel ch, uint8 data[FPGA_MAX_NUM_POINTS])
{
    int center = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top();
    int left = Grid::Left();

    if (!SET_ENABLED(ch))
    {
        return;
    }
    
    float scale = (float)Grid::Height() / (MAX_VALUE - MIN_VALUE);

    if (MODE_DRAW_SIGNAL_IS_LINES)
    {
        if (THICKNESS_SIGNAL_IS_3)
        {
            Painter::SetColor(Color::ChanHalf(ch));

            int x = left;

            for (int i = 1; i < 281; i++)
            {
                int value = (int)(center - ((data[i] - AVE_VALUE) * scale) + 0.5f);
                int valuePrev = (int)(center - ((data[i - 1] - AVE_VALUE) * scale) + 0.5f);

                if (value == valuePrev)
                {
                    Painter::DrawHLine(value, x - 1, x + 1);
                    Painter::DrawVLine(x++, value - 1, value + 1);
                }
                else
                {
                    if(valuePrev > value)   { ++value;  }
                    else                    { --value;  }

                    if(valuePrev < value)
                    {
                        Swap(&valuePrev, &value);
                    }

                    Painter::DrawVLine(x - 1, value, valuePrev);
                    Painter::DrawVLine(x + 1, value, valuePrev);
                    Painter::DrawVLine(x++, valuePrev + 1, value - 1);
                }
            }
        }

        Painter::SetColor(Color::Chan(ch));

        int x = left;
               
        for (int i = 1; i < 281; i++)
        {
            int value = (int)(center - (data[i] - AVE_VALUE) * scale + 0.5f);
            int valuePrev = (int)(center - (data[i - 1] - AVE_VALUE) * scale + 0.5f);

            if(value == valuePrev)
            {
                Painter::SetPoint(x++, valuePrev);
            }
            else
            {
                Painter::DrawVLine(x++, valuePrev, valuePrev > value ? (value + 1) : (value - 1));
            }
        }
    }
    else
    {
        Painter::SetColor(Color::Chan(ch));

        for (int i = 0; i < 280; i++)
        {
            float value = center - (data[i] - AVE_VALUE) * scale;
            Painter::SetPoint(left + i, (uint8)ROUND(value));
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawMemoryWindow()
{
    //Draw
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawDataInRect(int x, int y, int width, int height, Channel ch)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawTPos(int leftX, int rightX)
{
    int x[] = {leftX, (rightX - leftX) / 2 + leftX, rightX};
    int x0 = x[TPOS];
    Painter::FillRegion(x0 - 3, 10, 6, 6, Color::BACK);
    Painter::DrawChar(x0 - 3, 10, SYMBOL_TPOS_1, Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawTShift(int leftX, int rightX, int numBytes)
{
    float scale = (float)(rightX - leftX + 1) / ((float)numBytes - (numBytes == 281 ? 1 : 0));
    int xShift = (int)(1.5f + (TPOS_IN_BYTES - TSHIFT_IN_POINTS) * scale) - 1;
    if (SET_PEAKDET_EN && TPOS_IS_RIGHT)
    {
        --xShift;
    }
    if (FPGA_POINTS_512)
    {
        ++xShift;                           /// \todo �������
    }
    LIMIT_ABOVE(xShift, rightX - 2);

    int dX01 = 1, dX02 = 2, dX11 = 3, dY11 = 7, dY12 = 6;

    if (xShift < leftX - 2)
    {
        xShift = leftX - 2;
        dX01 = 3; dX02 = 1; dY12 = 6;
    }
    else if (xShift > rightX - 1)
    {
        xShift = rightX - 2;
        dX11 = 1;
    }
    else
    {
        dY11 = 5; dY12 = 7;
    }

    Painter::FillRegion((int)xShift - 1, 1, 6, 6, Color::BACK);
    Painter::FillRegion((int)xShift, 2, 4, 4, Color::FILL);
    Painter::DrawLine((int)xShift + dX01, 3, (int)xShift + dX11, dY11 - 2, Color::BACK);
    Painter::DrawLine((int)xShift + dX02, 4, (int)xShift + 2, dY12 - 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int PainterData::Ordinate(uint8 x, float scale)
{
    if (x == NONE_VALUE)
    {
        return -1;
    }

    LimitationRet<uint8>((uint8)(x - MIN_VALUE), 0, (MAX_VALUE - MIN_VALUE));

    return (int)((17.0f - scale * x) + 0.5f);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::SendToDisplayDataInRect(Channel ch, int x, int *min, int *max, int width)
{
    LIMIT_ABOVE(width, 255);

#undef SIZE_BUFFER
#define SIZE_BUFFER (255 * 2)
    uint8 points[SIZE_BUFFER];

    for (int i = 0; i < width; i++)
    {
        points[i * 2] = (uint8)max[i];
        points[i * 2 + 1] = (uint8)(min[i] < 0 ? 0 : min[i]);
    }

    Painter::DrawVLineArray(x, (int)width, points, Color::Chan(ch));
}
