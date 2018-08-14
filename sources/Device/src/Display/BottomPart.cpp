#include "BottomPart.h"
#include "Settings/Settings.h"
#include "Grid.h"
#include "Painter.h"
#include "Utils/MathOSC.h"
#include "FPGA/FPGAMath.h"
#include "Tables.h"
#include "FPGA/FPGA.h"
#include "Display/Painter.h"
#include "Utils/StringUtils.h"
#include "FlashDrive/FlashDrive.h"
#include "Utils/Dictionary.h"
#include "Hardware/CPU.h"
#include "Data/Reader.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BottomPart::Draw()
{
    int y0 = Display::HEIGHT - 19;
    int y1 = Display::HEIGHT - 10;
    int x = -1;

    Painter::DrawHLine(Grid::ChannelBottom(), 1, Grid::Left() - Measures::GetDeltaGridLeft() - 2, Color::FILL);
    Painter::DrawHLine(Grid::FullBottom(), 1, Grid::Left() - Measures::GetDeltaGridLeft() - 2);
    WriteTextVoltage(A, x + 2, y0);
    WriteTextVoltage(B, x + 2, y1);
    Painter::DrawVLine(x + 95, Grid::Bottom() + 2, Display::HEIGHT - 2, Color::FILL);

    x += 98;
    const int SIZE = 100;
    char buffer[SIZE] = {0};

    TBase tBase = SET_TBASE;
    int16 tShift = (int16)SET_TSHIFT;

    snprintf(buffer, SIZE, "�\xa5%s", Tables_GetTBaseString(tBase));
    Painter::DrawText(x, y0, buffer);

    buffer[0] = 0;
    char bufForVal[20];
    snprintf(buffer, SIZE, "\xa5%s", FPGA::GetTShiftString(tShift, bufForVal));
    Painter::DrawText(x + 35, y0, buffer);

    buffer[0] = 0;
    pString source[3] = {"1", "2", "\x82"};
    if (MODE_WORK_IS_DIR)
    {
        snprintf(buffer, 100, "�\xa5\x10%s", source[(uint8)TRIG_SOURCE]);
    }

    Painter::DrawText(x, y1, buffer, Color::Trig());

    buffer[0] = 0;
    static pString couple[] =
    {
        "\x92",
        "\x91",
        "\x92",
        "\x92"
    };
    static pString polar[] =
    {
        "\xa7",
        "\xa6"
    };
    static pString filtr[] =
    {
        "\xb5\xb6",
        "\xb5\xb6",
        "\xb3\xb4",
        "\xb1\xb2"
    };
    if (MODE_WORK_IS_DIR)
    {
        snprintf(buffer, SIZE, "\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[TRIG_INPUT], polar[TRIG_POLARITY]);
        Painter::DrawText(x + 18, y1, buffer);
        Painter::DrawChar(x + 45, y1, filtr[TRIG_INPUT][0]);
        Painter::DrawChar(x + 53, y1, filtr[TRIG_INPUT][1]);
    }

    buffer[0] = '\0';
    const char mode[] =
    {
        '\xb7',
        '\xa0',
        '\xb0'
    };
    if (MODE_WORK_IS_DIR)
    {
        snprintf(buffer, 100, "\xa5\x10%c", mode[START_MODE]);
        Painter::DrawText(x + 63, y1, buffer);
    }

    Painter::DrawVLine(x + 79, Grid::Bottom() + 2, Display::HEIGHT - 2, Color::FILL);

    Painter::DrawHLine(Grid::Bottom(), Grid::Right() + 2, Display::WIDTH - 2);
    Painter::DrawHLine(Grid::ChannelBottom(), Grid::Right() + 2, Display::WIDTH - 2);

    x += 82;
    y0 = y0 - 3;
    y1 = y1 - 6;
    int y2 = y1 + 6;
    Painter::SetFont(TypeFont_5);

    if (MODE_WORK_IS_DIR)
    {
        WriteStringAndNumber(LANG_RU ? "������" : "accum", (int16)x, (int16)y0, NUM_ACCUM);
        WriteStringAndNumber(LANG_RU ? "������" : "ave", (int16)x, (int16)y1, NUM_AVE);
        WriteStringAndNumber(LANG_RU ? "��\x93���" : "mn\x93max", (int16)x, (int16)y2, NUM_MIN_MAX);
    }

    x += 42;
    Painter::DrawVLine(x, Grid::Bottom() + 2, Display::HEIGHT - 2);

    Painter::SetFont(TypeFont_8);

    if (MODE_WORK_IS_DIR)
    {
        char mesFreq[20] = "\x7c=";
        float freq = FPGA::FreqMeter_GetFreq();
        if (freq == -1.0f)
        {
            strcat(mesFreq, "******");
        }
        else
        {
            strcat(mesFreq, Freq2String(freq, false, buffer));
        }
        Painter::DrawText(x + 3, Grid::Bottom() + 2, mesFreq);
    }

    DrawTime(x + 3, Grid::Bottom() + 11);

    Painter::DrawVLine(x + 55, Grid::Bottom() + 2, Display::HEIGHT - 2);

    Painter::SetFont(TypeFont_UGO2);

    // ������
    if (FDrive::IsConnected())
    {
        Painter::Draw4SymbolsInRect(x + 57, Grid::Bottom() + 2, SYMBOL_FLASH_DRIVE);
    }

    /*
    if (CONNECTED_TO_USB || CABLE_USB_IS_CONNECTED)
    {
        Painter::Draw4SymbolsInRect(x + 72, Grid::Bottom(); + 2, SYMBOL_USB, CONNECTED_TO_USB ? Color::WHITE : Color::FLASH_01);
    }
    */

    Painter::SetColor(Color::FILL);
    // ������� ��������
    if (!SET_PEAKDET_DIS)
    {
        Painter::DrawChar(x + 38, Grid::Bottom() + 11, '\x12');
        Painter::DrawChar(x + 46, Grid::Bottom() + 11, '\x13');
    }

    if (MODE_WORK_IS_DIR)
    {
        Painter::SetFont(TypeFont_5);
        WriteStringAndNumber("�����.:", (int16)(x + 57), (int16)(Grid::Bottom() + 10), (int)ENUM_SMOOTHING + 1);
        Painter::SetFont(TypeFont_8);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int BottomPart::WriteChannel(Channel ch, int x, int y)
{
    Painter::DrawText(x, y, ch == A ? "1:" : "2:", Color::Chan(ch));

    x += 7;

    static const char symbols[3] = {SYMBOL_COUPLE_AC, SYMBOL_COUPLE_DC, SYMBOL_COUPLE_GND};

    char string[2] = {symbols[SET_COUPLE(ch)], 0};

    Painter::DrawText(x, y, string);

    x += 8;

    Painter::DrawText(x, y, NameRange(SET_RANGE(ch)));

    x += 22;

    CHAR_BUF(buf, 20);

    MathOSC::Voltage2String(RSHIFT_2_ABS(SET_RSHIFT(ch), SET_RANGE(ch)), true, buf);

    Painter::DrawText(x, y, buf);

    return x + 47;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BottomPart::WriteTBase(int x, int y)
{
    Painter::DrawText(x, y, NameTBase(SET_TBASE), Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BottomPart::WriteTextVoltage(Channel ch, int x, int y)
{
    if (!SET_ENABLED(ch))
    {
        return;
    }

    static pString couple[] = {"\x92", "\x91", "\x90"};

    Color color = Color::Chan(ch);

    bool inverse = SET_INVERSE(ch);
    Divider divider = SET_DIVIDER(ch);
    Range range = SET_RANGE(ch);

    const int widthField = 91;
    const int heightField = 8;
    Color colorDraw = inverse ? Color::WHITE : color;
    if (inverse)
    {
        Painter::FillRegion(x, y, widthField, heightField, color);
    }
    const int SIZE = 100;
    char buffer[SIZE];
    snprintf(buffer, SIZE, "%s\xa5%s\xa5%s", (ch == A) ? DICT(D1ch) : DICT(D2ch), couple[SET_COUPLE(ch)], sChannel_Range2String(range, divider));
    Painter::DrawText(x + 1, y, buffer, colorDraw);
    char bufferTemp[SIZE];
    snprintf(bufferTemp, SIZE, "\xa5%s", sChannel_RShift2String((uint16)SET_RSHIFT(ch), range, divider, buffer));
    Painter::DrawText(x + 46, y, bufferTemp);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BottomPart::WriteStringAndNumber(const char *text, int16 x, int16 y, int number)
{
    const int SIZE = 100;
    char buffer[SIZE];
    Painter::DrawText(x, y, text, Color::FILL);
    if (number == 0)
    {
        snprintf(buffer, SIZE, "-");
    }
    else
    {
        snprintf(buffer, SIZE, "%d", number);
    }
    Painter::DrawTextRelativelyRight(x + 41, y, buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BottomPart::DrawTime(int x, int y)
{
    int dField = 10;
    int dSeparator = 2;

    PackedTime time = CPU::RTC_::GetPackedTime();

    char buffer[20];

    Painter::SetColor(Color::FILL);

    if (MODE_WORK_IS_ROM || MODE_WORK_IS_RAM)
    {
        if (DS)
        {
            y -= 9;
            time.day = TIME_DAY_DS;
            time.hours = TIME_HOURS_DS;
            time.minutes = TIME_MINUTES_DS;
            time.seconds = TIME_SECONDS_DS;
            time.month = TIME_MONTH_DS;
            time.year = TIME_YEAR_DS;
            Painter::DrawText(x, y, Int2String((int)time.day, false, 2, buffer));
            Painter::DrawText(x + dField, y, ":");
            Painter::DrawText(x + dField + dSeparator, y, Int2String((int)time.month, false, 2, buffer));
            Painter::DrawText(x + 2 * dField + dSeparator, y, ":");
            Painter::DrawText(x + 2 * dField + 2 * dSeparator, y, Int2String((int)time.year + 2000, false, 4, buffer));
            y += 9;
        }
        else
        {
            return;
        }
    }

    Painter::DrawText(x, y, Int2String((int)time.hours, false, 2, buffer));
    Painter::DrawText(x + dField, y, ":");
    Painter::DrawText(x + dField + dSeparator, y, Int2String((int)time.minutes, false, 2, buffer));
    Painter::DrawText(x + 2 * dField + dSeparator, y, ":");
    Painter::DrawText(x + 2 * dField + 2 * dSeparator, y, Int2String((int)time.seconds, false, 2, buffer));
}