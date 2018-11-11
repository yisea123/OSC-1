#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "main.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAMath.h"
#include "FPGA/RShift.h"
#include "Hardware/CPU.h"
#include "Hardware/Hardware.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageService.h"
#include "Keyboard/Keyboard.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#endif


extern const PageBase pService;
extern const PageBase ppCalibrator;
extern const PageBase ppFunction;
extern const PageBase ppSound;
extern const PageBase ppRTC;
extern const PageBase ppInformation;


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void Draw_ResetSettings()
{
    Painter::BeginScene(Color::BACK);

    Painter::DrawTextInRectWithTransfersC(30, 110, 300, 200, "����������� ����� �������� �������� ������ ����/����.\n"
                                         "������� ����� ������ ������, ���� ����� �� �����.", Color::FILL);

    Painter::EndScene();
}
*/

static void OnPress_ResetSettings()
{
    Settings::Load(true);
    /*
    Keyboard::Disable();
    Display::SetDrawMode(DrawMode_Hand, Draw_ResetSettings);

    if (Keyboard::WaitPressingButton().Is(Key::Start))
    {
        Settings::Load(true);
    }

    Display::SetDrawMode(DrawMode::Auto, 0);
    Keyboard::Enable();
    */
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_BUTTON(         bResetSettings,                                                                                  //--- ������ - ����� �������� ---
    "����� �����", "Reset set",
    "����� �������� �� ��������� �� ���������",
    "Reset to default settings",
    pService, EmptyFuncBV, OnPress_ResetSettings, EmptyFuncVII
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_AutoSearch()
{
    FPGA_NEED_AUTO_FIND = 1;
}

DEF_BUTTON(         bAutoSearch,                                                                                      //--- ������ - ����� ������� ---
    "����� ����", "Find sign",
    "������������� ����������� ��������� ������������ ��� ������� � ������ 1",
    "Sets optimal settings for the oscilloscope signal on channel 1",
    pService, FuncActive, OnPress_AutoSearch, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Calibrator_Calibrator(bool)
{
    FPGA::SetCalibratorMode(CALIBRATOR_MODE);
}

DEF_CHOICE_3(       cCalibrator_Calibrator,                                                                 //--- ������ - ���������� - ���������� ---
    "����������", "Calibrator",
    "����� ������ �����������",
    "Mode of operation of the calibrator",
    "�����", "DC",
    "+4V", "+4V",
    "0V", "0V",
    CALIBRATOR_MODE, ppCalibrator, FuncActive, OnChanged_Calibrator_Calibrator, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Calibrator_Calibrate()
{
    return !(SET_CALIBR_MODE_A == CalibrationMode::Disable && CALIBR_MODE_B == CalibrationMode::Disable);
}

static void OnPress_Calibrator_Calibrate()
{
    FPGA::state.needCalibration = true;
}

DEF_BUTTON(         bCalibrator_Calibrate,                                                                 //--- ������ - ���������� - ����������� ---
    "�����������", "Calibrate",
    "������ ��������� ����������",
    "Running the calibration procedure",
    ppCalibrator, IsActive_Calibrator_Calibrate, OnPress_Calibrator_Calibrate, FuncDraw
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppCalibrator,                                                                                           // ������ - ���������� ///
    "����������", "CALIBRATOR",
    "����������� ������������ � ���������� ������������",
    "Control of the calibrator and calibration of an oscillograph",
    &cCalibrator_Calibrator,     // ������ - ���������� - ����������
    &bCalibrator_Calibrate,      // ������ - ���������� - �����������
    Page::Name::Service_Calibrator, &pService, FuncActive, EmptyPressPage
)

#ifdef OLD_RECORDER

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Recorder(bool)
{
    FPGA::EnableRecorderMode(RECORDER_MODE);
}

DEF_CHOICE_2(       cRecorder,                                                                                          //--- ������ - ����������� ---
    "�����������", "Recorder",
    "��������/��������� ����� ������������. ���� ����� �������� �� ��������� 50 ��/��� � ����� ���������.",
    "Turn on/off recorder mode. This mode is available for scanning 20ms/div and slower.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    RECORDER_MODE, pService, FuncActive, OnChanged_Recorder, FuncDraw
)

#else

// C����� - ����������� //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Page ppRecorder =
{
    Control::Type::Page, &pService, 0,
    {
        "�����������", "RECORDER",
        "������ � ��������������� ������� ��������",
        "Recording and playback of input signals"
    },
    PageSB_Service_Recorder,
    {
        (void *)&bRecorder_Exit,
        (void *)&bRecorder_SaveTo,
        (void *)&bRecorder_Choice,
        (void *)&bRecorder_Cursor
    },
    true
};

// ������ - ����������� - ����� ----------------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_Exit =
{
    Control::Type::SmallButton, &ppRecorder, 0,
    {
        "�����", "Exit",
        "����� �� ������ ������������",
        "Exit registrator mode"
    },
    OnPressSB_Exit,
    DrawSB_Exit
};

// ������ - ����������� - ��������� � ... ------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_SaveTo =
{
    Control::Type::SmallButton, &ppRecorder, IsActive_Recorder_SaveTo,
    {
        "��������� � ...", "Save in ...",
        "����� ����� �������� ������",
        "Selecting a storage location"
    },
    OnPress_Recorder_SaveTo,
    Draw_Recorder_SaveTo,
    {
        {Draw_Recorder_SaveTo_RAM, "������ ����������� �� ���������� ������", "data is stored in the internal memory"},
        {Draw_Recorder_SaveTo_EXT, "������ ����������� �� ������� ��", "data is stored in the data storage"}
    }
};

static bool IsActive_Recorder_SaveTo()
{
    return FDRIVE_IS_CONNECTED;
}

static void OnPress_Recorder_SaveTo()
{
    CircleIncreaseInt8((int8*)(&REC_PLACE_OF_SAVING), 0, PlaceOfSaving_Number - 1);
}

static void Draw_Recorder_SaveTo(int x, int y)
{
    static const pFuncVII funcs[PlaceOfSaving_Number] =
    {
        Draw_Recorder_SaveTo_RAM, Draw_Recorder_SaveTo_EXT
    };

    funcs[REC_PLACE_OF_SAVING](x, y);
}

static void Draw_Recorder_SaveTo_RAM(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_ROM);
    Painter::SetFont(Font::Type::_8);
}

static void Draw_Recorder_SaveTo_EXT(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, SYMBOL_FLASH_DRIVE_BIG);
    Painter::SetFont(Font::Type::_8);
}

// ������ - ����������� - ����� ----------------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_Choice =
{
    Control::Type::SmallButton, &ppRecorder, 0,
    {
        "�����", "Choice",
        "����� ������� ��� ���������",
        "Select signal to view"
    },
    OnPress_Recorder_Choice,
    Draw_Recorder_Choice
};

static void OnPress_Recorder_Choice()
{

}

static void Draw_Recorder_Choice(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 1, y, SYMBOL_FOLDER);
    Painter::SetFont(Font::Type::_8);
}

// ������ - ����������� - ������ ---------------------------------------------------------------------------------------------------------------------
static const SButton bRecorder_Cursor =
{
    Control::Type::SmallButton, &ppRecorder, 0,
    {
        "������", "Cursors",
        "����� �������",
        "Cursor selection"
    },
    OnPress_Recorder_Cursor,
    Draw_Recorder_Cursor
};

static void OnPress_Recorder_Cursor()
{
    CircleIncreaseInt8(&REC_NUM_CURSOR, 0, 1);
}

static void Draw_Recorder_Cursor(int x, int y)
{
    Painter::DrawText(x + 8, y + 5, REC_NUM_CURSOR ? "2" : "1");
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON_EXIT(  bFunction_Exit,                                                                             //--- ������ - ������� - ����� ---
    ppFunction, FuncActive, FuncPress, DrawSB_Exit
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_Screen()
{
    if (FFT_ENABLED)
    {
        Display::ShowWarning(Warning::ImpossibleEnableMathFunction);
    }
    else
    {
        Math::CircleIncrease<int8>((int8 *)&FUNC_MODE_DRAW, 0, 2);
    }
}

static void Draw_Function_Screen_Disable(int x, int y)
{
    Painter::DrawText(x + 2 + (LANG_EN ? 2 : 0), y + 5, DICT(DDis));
}

static void Draw_Function_Screen_Separate(int x, int y)
{
    Painter::DrawRectangle(x + 3, y + 5, 13, 9);
    Painter::DrawHLine(y + 9, x + 3, x + 16);
    Painter::DrawHLine(y + 10, x + 3, x + 16);
}

static void Draw_Function_Screen_Together(int x, int y)
{
    Painter::DrawRectangle(x + 3, y + 5, 13, 9);
}

static void Draw_Function_Screen(int x, int y)
{
    const pFuncVII funcs[3] =
    {
        Draw_Function_Screen_Disable,
        Draw_Function_Screen_Separate,
        Draw_Function_Screen_Together
    };
    funcs[FUNC_MODE_DRAW](x, y);
}

DEF_SMALL_BUTTON_HINTS_3(   bFunction_Screen,                                                                       //--- ������ - ������� - ����� ---
    "�����", "Display",
    "�������� ����� ����������� ��������������� �������",
    "Chooses the mode of display of a mathematical signal",
    ppFunction, FuncActive, OnPress_Function_Screen, Draw_Function_Screen,
    Draw_Function_Screen_Disable,   {"����� �������������� ������� ��������",
                                    "The conclusion of mathematical function is disconnected"},
    Draw_Function_Screen_Separate,  {"������� � �������������� ������� ��������� � ������ �����",
                                    "Signals and mathematical function are removed in different windows"},
    Draw_Function_Screen_Together,  {"������� � �������������� ������� ��������� � ����� ����",
                                    "Signals and mathematical function are removed in one window"}
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_Type()
{
    Math::CircleIncrease<int8>((int8 *)&MATH_FUNC, 0, 1);
}

static void Draw_Function_Type_Sum(int x, int y)
{
    Painter::DrawHLine(y + 9, x + 4, x + 14);
    Painter::DrawVLine(x + 9, y + 4, y + 14);
}

static void Draw_Function_Type_Mul(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 4, y + 3, SYMBOL_MATH_FUNC_MUL);
    Painter::SetFont(Font::Type::_8);
}

static void Draw_Function_Type(int x, int y)
{
    const pFuncVII funcs[2] = {Draw_Function_Type_Sum, Draw_Function_Type_Mul};
    funcs[MATH_FUNC](x, y);
}

DEF_SMALL_BUTTON_HINTS_2(   bFunction_Type,                                                                           //--- ������ - ������� - ��� ---
    "���", "Type",
    "����� �������������� �������",
    "Choice of mathematical function",
    ppFunction, FuncActive, OnPress_Function_Type, Draw_Function_Type,
    Draw_Function_Type_Sum, {"��������", "Addition"},
    Draw_Function_Type_Mul, {"���������", "Multiplication"}
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_ModeRegSet()
{
    Math::CircleIncrease<int8>((int8 *)&MATH_MODE_REG_SET, 0, 1);
}

static void Draw_Function_ModeRegSet_Range(int x, int y)
{
    Painter::DrawChar(x + 7, y + 5, LANG_RU ? 'M' : 'S');
}

static void Draw_Function_ModeRegSet_RShift(int x, int y)
{
    Painter::DrawText(x + 5 - (LANG_EN ? 3 : 0), y + 5, DICT(DShift));
}

static void Draw_Function_ModeRegSet(int x, int y)
{
    static const pFuncVII funcs[2] = {Draw_Function_ModeRegSet_Range, Draw_Function_ModeRegSet_RShift};
    funcs[MATH_MODE_REG_SET](x, y);
}

DEF_SMALL_BUTTON_HINTS_2(   bFunction_ModeRegSet,                                                   //--- ������ - ������� - ����� ����� ��������� ---
    "����� ����� ���������", "Mode regulator SET",
    "����� ������ ����� ��������� - ���������� ��������� ��� ���������",
    "Choice mode regulcator ��������� - management of scale or shift",
    ppFunction, FuncActive, OnPress_Function_ModeRegSet, Draw_Function_ModeRegSet,
    Draw_Function_ModeRegSet_Range,  {"���������� ���������", "Management of scale"},
    Draw_Function_ModeRegSet_RShift, {"���������� ���������", "Management of shift"}
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_RangeA()
{
    SET_RANGE_MATH = SET_RANGE_A;
    MATH_DIVIDER = SET_DIVIDER_A;
}

static void Draw_Function_RangeA(int x, int y)
{
    Painter::DrawChar(x + 8, y + 5, '1');
}

DEF_SMALL_BUTTON(   bFunction_RangeA,                                                                 //--- ������ - ������� - ������� 1-�� ������ ---
    "������� 1-�� ������", "Scale of the 1st channel",
    "���������� ������� ������� ������ ��� ����������� ����������",
    "Takes scale for a mathematical signal from the first channel",
    ppFunction, FuncActive, OnPress_Function_RangeA, Draw_Function_RangeA
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Function_RangeB()
{
    SET_RANGE_MATH = SET_RANGE_B;
    MATH_DIVIDER = SET_DIVIDER_B;
}

static void Draw_Function_RangeB(int x, int y)
{
    Painter::DrawChar(x + 8, y + 5, '2');
}

DEF_SMALL_BUTTON(   bFunction_RangeB,                                                                 //--- ������ - ������� - ������� 2-�� ������ ---
    "������� 2-�� ������", "Scale of the 2nd channel",
    "���������� ������� ������� ������ ��� ����������� ����������",
    "Takes scale for a mathematical signal from the second channel",
    ppFunction, FuncActive, OnPress_Function_RangeB, Draw_Function_RangeB
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Function()
{
    return !FFT_ENABLED;
}

static void OnPress_Function(bool)
{
    if (FFT_ENABLED)
    {
        Display::ShowWarning(Warning::ImpossibleEnableMathFunction);
    }
}

static void OnRegSet_Function(int delta)
{
    if (!FUNC_MODE_DRAW_IS_ENABLED)
    {
        return;
    }

    if (MATH_MODE_REG_SET_IS_RSHIFT)
    {
        uint16 prevRShift = SET_RSHIFT_MATH;
        uint16 rShift = prevRShift;
        if (delta > 0)
        {
            if (rShift < RShift::MAX)
            {
                rShift += 4 * STEP_RSHIFT;
                LIMIT_ABOVE(rShift, RShift::MAX);
                if (prevRShift < RShift::ZERO && rShift > RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }
                Sound::RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
        else if (delta < 0)
        {
            if (rShift > RShift::MIN)
            {
                rShift -= 4 * STEP_RSHIFT;
                LIMIT_BELOW(rShift, RShift::MIN);
                if (prevRShift > RShift::ZERO && rShift < RShift::ZERO)
                {
                    rShift = RShift::ZERO;
                }
                Sound::RegulatorShiftRotate();
                SET_RSHIFT_MATH = rShift;
            }
        }
    }
    if (MATH_MODE_REG_SET_IS_RANGE)
    {
        static int sum = 0;
        sum -= delta;

        float rShiftAbs = MathFPGA::RShift2Abs(SET_RSHIFT_MATH, SET_RANGE_MATH);

        if (sum > 2)
        {
            if (SET_RANGE_MATH < Range::Number - 1)
            {
                SET_RANGE_MATH = (Range::E)((uint8)(SET_RANGE_MATH + 1));  // SET_RANGE_MATH++;
                SET_RSHIFT_MATH = (uint16)MathFPGA::RShift2Rel(rShiftAbs, SET_RANGE_MATH);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
        else if (sum < -2)
        {
            if (SET_RANGE_MATH > 0)
            {
                SET_RANGE_MATH = (Range::E)((uint8)(SET_RANGE_MATH - 1));  // SET_RANGE_MATH--;
                SET_RSHIFT_MATH = (uint16)MathFPGA::RShift2Rel(rShiftAbs, SET_RANGE_MATH);
                Sound::RegulatorSwitchRotate();
            }
            sum = 0;
        }
    }
}

DEF_PAGE_SB(        ppFunction,                                                                                                // ������ - ������� ///
    "�������", "FUNCTION",
    "��������� � ����� �������������� ������� - �������� ��� ���������",
    "Installation and selection of mathematical functions - addition or multiplication",
    &bFunction_Exit,        // ������ - ������� - �����
    &bFunction_Screen,      // ������ - ������� - �����
    &bFunction_Type,        // ������ - ������� - ���
    &bFunction_ModeRegSet,  // ������ - ������� - ����� ����� ���������
    &bFunction_RangeA,      // ������ - ������� - ������� 1-�� ������
    &bFunction_RangeB,
    Page::Name::SB_Service_Function, &pService, IsActive_Function, OnPress_Function, FuncDrawPage, OnRegSet_Function
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cSound_Enable,                                                                                      //--- ������ - ���� - ���� ---
    "����", "Sound",
    "���������/���������� �����",
    "Inclusion/switching off of a sound",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    SOUND_ENABLED, ppSound, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gSound_Volume,                                                                                 //--- ������ - ���� - ��������� ---
    "���������", "Volume",
    "��������� ��������� �����",
    "Set the volume",
    SOUND_VOLUME, 0, 100, ppSound, FuncActive, FuncChanged, FuncBeforeDraw
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppSound,                                                                                                      // ������ - ���� ///
    "����", "SOUND",
    "� ���� ���� ����� ��������� ��������� �����",
    "In this menu, you can adjust the volume",
    &cSound_Enable,  // ������ - ���� - ����
    &gSound_Volume,  // ������ - ���� - ���������
    Page::Name::Service_Sound, &pService, FuncActive, EmptyPressPage
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int8 dServicetime = 0;
static int8 hours = 0, minutes = 0, secondes = 0, year = 0, month = 0, day = 0;
DEF_TIME(                                                                                                             //--- ������ - ����� - ����� ---
    tRTC_Time,
    "�����", "Time",
    "��������� �������� �������.\n������� ������:\n"
    "������ �� ������� ���� \"�����\". ��������� ���� ��������� �������� �������. ��������� ��������� ������ �� �������� ����������, �������������� "
    "�������� ���������� \"�����\", �������� ����, ������, �������, ���, �����, ��� �����. ���������� ������� ������������ �������� ��������. "
    "��������� ����� ��������� ���������� ����������� ��������. ����� �������� ����� \"���������\", ������ � ���������� ����� 0.5 ��� ������ �� "
    "������ ����������. ���� ��������� �������� ������� ��������� � ����������� ������ �������� �������. ������� ���������� ��������� ������ �� "
    "����� ������ �������� ������� � �������� ���� ��������� �������� ���    ���� ��� ���������� ������ �������� �������"
    ,
    "Setting the current time. \nPoryadok work:\n"
    "Click on the menu item \"Time\".The menu set the current time.By briefly pressing the button on the numeric keypad of conformity "
    "Control \"Time\", highlight the hours, minutes, seconds, year, month, or a number.The selected item is indicated by a flashing area. "
    "Turn the setting knob to set the desired value. Then highlight \"Save\", press and udrezhivat more than 0.5 seconds, the button on the panel "
    "Control. Menu Setting the current time will be closed to the conservation of the new current time. Pressing a button on the prolonged retention "
    "of any other element will lead to the closure of the current time setting menu without saving the new current time"
    ,
    ppRTC, FuncActive, dServicetime, hours, minutes, secondes, month, day, year
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Time_Correction()
{
    CPU::RTC_::SetCorrection((int8)NRST_CORRECTION_TIME);
}

DEF_GOVERNOR(       tRTC_Correction,                                                                              //--- ������ - ����� - ��������� ---
    "���������", "Correction",
    "��������� ��������������� ������������ ��� ����������� ���� �������",
    "Setting correction factor to compensate for time travel",
    NRST_CORRECTION_TIME, -63, 63, ppRTC, FuncActive, OnChanged_Time_Correction, FuncBeforeDraw
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(         ppRTC,                                                                                                       // ������ - ����� ///
    "�����", "TIME",
    "��������� � ��������� �������",
    "Set and setup time",
    &tRTC_Time,          // ������ - ����� - �����
    &tRTC_Correction,    // C����� - ����� - ���������
    Page::Name::Service_RTC, &pService, FuncActive, EmptyPressPage
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cLanguage,                                                                                                 //--- ������ - ���� ---
    "Language", "����",
    "��������� ������� ���� ����",
    "Allows you to select the menu language",
    "�������",    "RU",
    "����������", "EN",
    LANG, pService, FuncActive, FuncChangedChoice, FuncDraw
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Information_Draw()
{
    Painter::BeginScene(Color::BACK);
    int x = 100;
    int dY = 20;
    int y = 20;
    Painter::DrawRectangle(0, 0, 319, 239, Color::FILL);
    y += dY;
    Painter::DrawText(x, y, DICT(DInformation));
    y += dY;

    char buffer[100];

    sprintf(buffer, "%s : %s", DICT(DModel), LANG_RU ? MODEL_RU : MODEL_EN);
    Painter::DrawText(x, y, buffer);

    y += 2 * dY;

    Painter::DrawText(x, y, DICT(DSoftware));
    y += dY;
    sprintf(buffer, (const char *)DICT(DVersion), NUM_VER);
    Painter::DrawText(x, y, buffer);
    y += dY;

    //Painter::DrawFormText(x, y, Color::FILL, "CRC32 : %X", Hardware::CalculateCRC32());

    dY = -10;
    Painter::DrawStringInCenterRect(0, 190 + dY, 320, 20, "��� ��������� ������ ������� � ����������� ������ ������");
    Painter::DrawStringInCenterRect(0, 205 + dY, 320, 20, "����� ����������: ���./����. 8-017-262-57-50");
    Painter::DrawStringInCenterRect(0, 220 + dY, 320, 20, "������������: e-mail: mnipi-24(@)tut.by, ���. 8-017-262-57-51");

    Menu::Graphics::Draw();
    Painter::EndScene();
}

static void OnPress_Information(bool)
{
    Display::SetDrawMode(DrawMode::Auto, Information_Draw);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Information_Exit()
{
    Display::SetDrawMode(DrawMode::Auto, 0);
}

DEF_SMALL_BUTTON_EXIT(bInformation_Exit,                                                                       //--- ������ - ���������� - ����� ---
                      ppInformation, FuncActive, OnPress_Information_Exit, DrawSB_Exit
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_SB(ppInformation,                                                                                          // ������ - ���������� ///
    "����������", "INFORMATION",
    "���������� ���������� � �������",
    "Displays information about the device",
    &bInformation_Exit, // ������ - ���������� - �����
    0,
    0,
    0,
    0,
    0,
    Page::Name::SB_Service_Information, &pService, FuncActive, OnPress_Information, FuncDrawPage, FuncRegSetPage
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageService::pointer = &pService;

DEF_PAGE_9_GLOBAL(pService,                                                                                                            // ������ ///
    "������", "SERVICE",
    "�������������� ���������, ����������, ����� �������, �������������� �������",
    "Additional settings, calibration, signal search, mathematical functions",
    &bResetSettings,    // ������ - ����� ��������
    &bAutoSearch,       // ������ - ����� �������
    &ppCalibrator,      // ������ - ����������
    &cRecorder,         // ������ - �����������
    &ppFunction,        // ������ - �������
    &ppSound,           // ������ - ����
    &ppRTC,             // ������ - �����
    &cLanguage,         // ������ - ����
    &ppInformation,     // ������ - ����������
    Page::Name::Service, Menu::pageMain, FuncActive, EmptyPressPage
)























/*
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressPrevSettings()
{

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OnChangedColorMenuTitle()
{
    uint16 newColor1 = Painter::ReduceBrightness(COLOR(COLOR_MENU_TITLE), 0.50f);
    uint16 newColor2 = Painter::ReduceBrightness(COLOR(COLOR_MENU_TITLE), 1.50f);
    COLOR(Color::MENU_TITLE_DARK) = newColor1;
    COLOR(COLOR_MENU_TITLE_BRIGHT) = newColor2;
    Color_Log(COLOR_MENU_TITLE);
    Color_Log(Color::MENU_TITLE_DARK);
    Color_Log(COLOR_MENU_TITLE_BRIGHT);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OnChangedColorMenu2Item()
{
    uint16 newColor1 = Painter::ReduceBrightness(COLOR(COLOR_MENU_ITEM), 0.50f);
    uint16 newColor2 = Painter::ReduceBrightness(COLOR(COLOR_MENU_ITEM), 1.50f);
    COLOR(COLOR_MENU_ITEM_DARK) = newColor1;
    COLOR(COLOR_MENU_ITEM_BRIGHT) = newColor2;
    Color_Log(COLOR_MENU_ITEM);
    Color_Log(COLOR_MENU_ITEM_DARK);
    Color_Log(COLOR_MENU_ITEM_BRIGHT);
}


// ������ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pService;

// ������ - ����� ��������
const Button mbServicePreviousSettings =
{
    Control::Type::Button, &pService, 0,
    {
        "����� ��������", "Return settings",
        "���������� ��������� ������������, ������� ���� � ������ ������� \"����� �������\"",
        "Returns the oscilloscope settings that were in when you press \"Searching for signal\""
    },
    OnPressPrevSettings
};

static bool ActiveF_MathFormula()
{
    return MATH_FUNC_IS_MUL || MATH_FUNC_IS_SUM;
}

static void ChangeF_MathFormula()
{

}

static int8 curDigit = 0;
const Formula mfMathFormula =
{
    Control::Type::Formula, &ppFunction, ActiveF_MathFormula,
    {
        "�������", "Formulf",
        "����� �������� ������������ � ����� � �������������� �������",
        "Here you can set the coefficients and signs in a mathematical formula"
    },
    (int8 *)&MATH_FUNC, (int8 *)&set.math_koeff1add, (int8 *)&set.math_koeff2add, (int8 *)&set.math_koeff1mul, (int8 *)&set.math_koeff2mul, &curDigit, ChangeF_MathFormula
};
*/
