#pragma once
#include "defines.h"

#define COMMON_ENUM     uint8 value; operator uint8() const { return value; }

struct Range
{
    enum
    {
        _2mV,
        _5mV,
        _10mV,
        _20mV,
        _50mV,
        _100mV,
        _200mV,
        _500mV,
        _1V,
        _2V,
        _5V,
        _10V,
        _20V,
        Size
    };
    Range(uint8 v = _2mV) : value(v) {};
    COMMON_ENUM;
    Range& operator++()
    {
        ++value;
        return *this;
    }
};

struct TBase
{
    enum
    {
        _2ns,
        _5ns,
        _10ns,
        _20ns,
        _50ns,
        _100ns,
        _200ns,
        _500ns,
        _1us,
        _2us,
        _5us,
        _10us,
        _20us,
        _50us,
        _100us,
        _200us,
        _500us,
        _1ms,
        _2ms,
        _5ms,
        _10ms,
        _20ms,
        _50ms,
        _100ms,
        _200ms,
        _500ms,
        _1s,
        _2s,
        _5,
        _10s,
        Size
    };
    TBase(uint8 v = _2ns) : value(v) {};
    COMMON_ENUM;
};

struct TrigInput
{
    enum
    {
        Full,
        HF,
        LF
    };
    TrigInput(uint8 v = Full) : value(v) {};
    COMMON_ENUM
};

enum Channel
{
    A,
    B,
    Ext,
    Math,
    NumChannels
};


struct MeasSource
{
    enum
    {
        A,
        B,
        A_B
    };
    COMMON_ENUM;
};

struct TrigPolarity
{
    enum
    {
        Rising,
        Falling
    };
    COMMON_ENUM;
};

struct Language
{
    enum
    {
        RU,
        EN,
        Num
    };
    COMMON_ENUM;
};

struct ModeDrawSignal
{
    enum
    {
        Lines,
        Points
    };
    COMMON_ENUM;
};


/// �������� ��������� �������
struct ThicknessSignal
{
    enum
    {
        _1,         ///< ������ �������� ������ �������� ���� �����
        _3          ///< ������ �������� ������ �������� ��� �����
    };
    COMMON_ENUM;
};

struct Bandwidth
{
    enum
    {
        Full,     ///< ���� ��� �������� ������� � ���� �����, �� ��� ���� ��������� ��������������� ������ �� �������-������-������.
        _20MHz,
        _100MHz,
        _200MHz,
        _350MHz,
        _650MHz,
        _750MHz
    };
    COMMON_ENUM;
};

struct ModeTrig
{
    enum
    {
        Auto,
        Wait,
        Once
    };
    ModeTrig(uint8 v = Auto) : value(v) {};
    uint8 value;
};

/// ��� ����� ��������� � ������-���������� - ����������� ��� �����
enum TesterControl
{
    Control_Voltage,
    Control_Current
};


enum TesterPolarity
{
    Polarity_Positive,
    Polarity_Negative
};

enum TesterStepU
{
    StepU_100mV,
    StepU_500mV
};

enum TesterStepI
{
    StepI_4mA,
    StepI_20mA
};

/// ����� �������� ���������
enum CursCntrl
{
    CursCntrl_1,        ///< ������
    CursCntrl_2,        ///< ������
    CursCntrl_1_2,      ///< ������
    CursCntrl_Disable   ///< �������
};

/// ��� ���������� �� ����������
struct ModeAveraging
{
    enum
    {
        Accurately,   ///< ��������� �����.
        Around        ///< ��������� ��������������.
    };
    ModeAveraging(uint8 v = Accurately) : value(v) {};
    COMMON_ENUM;
};

/// ����� ����� ����.
struct Background
{
    enum
    {
        Black,
        White
    };
    uint8 value;
    operator uint8() const
    {
        return value;
    }
};

/// ��������� ����� ������������� �� �������.
enum TPos
{
    TPos_Left,          ///< �������� � ������ ����.
    TPos_Center,        ///< �������� � ������.
    TPos_Right          ///< �������� � ������� ����.
};

/// ����� ����� �������, � ������� ��� ������.
enum ENumPointsFPGA
{
    FNP_512,
    FNP_1k,
    FNP_2k,
    FNP_4k,
    FNP_8k,
    FPGA_ENUM_POINTS_SIZE
};

/// ��� ������ ��� ������� ������ ������.
enum ModeBtnMemory
{
    ModeBtnMemory_Menu,     ///< ����� ������������ ��������������� �������� ����.
    ModeBtnMemory_Save      ///< ���������� ����������� ������ �� ������.
};

/// ��������.
enum Divider
{
    Divider_1,
    Divider_10
};

/// �������� �������������
struct TrigSource
{
    enum
    {
        A,    /// ����� 1
        B,    /// ����� 2
        Ext   /// �������
    };
    TrigSource(uint8 v = A) : value(v) {};
    uint8 value;
    operator uint8() const
    {
        return value;
    }
    operator Channel() const
    {
        return (Channel)value;
    }
};

/// ����� ������.
enum ModeWork
{
    ModeWork_Dir,           ///< �������� �����.
    ModeWork_RAM,           ///< � ���� ������ ����� ����������� ��������� ����������� ���������.
    ModeWork_ROM,           ///< � ���� ������ ����� ��������� �� flash-������ ��������� ������������� ����� �����������.
    ModeWork_None           ///< ������������ � ������ Data.c. �����, ����� �������, ��� ������ �� �������� �� �� ����� �����.
};

/// ����� ������ ������ ���������
struct ShowStrNavi
{
    enum
    {
        Temp,   ///< ���������� �� ��������� ������
        All,    ///< ������ ����������
        None    ///< ������� �� ����������
    };
    COMMON_ENUM;
};

enum FFTmaxDB
{
    FFTmaxDB_40,
    FFTmaxDB_60,
    FFTmaxDB_80
};

enum FuncModeDraw
{
    FuncModeDraw_Disable,
    FuncModeDraw_Separate,
    FuncModeDraw_Together
};

/// \brief ��� ������������ ��� �������.
/// ���� � ���, ��� ������ ��� �� ��������� ��-�� ������� ������������� ( ? ), ������� �� ������ �������������� �������� ��� ������ �� ��� ������.
enum BalanceADCtype
{
    BalanceADC_Disable,     ///< ������������ ���������.
    BalanceADC_Settings,    ///< ������������ �������� ������������, ������� �������� �������������.
    BalanceADC_Hand         ///< ������������ �������� ������������, �������� �������.
};

/// ��� �������� ���
enum StretchADCtype
{
    StretchADC_Disable,
    StretchADC_Real,
    StretchADC_Hand
};

enum DisplayOrientation
{
    Direct,
    Back
};

/// ����� ������ �� �����.
struct ModeCouple
{
    enum
    {
        DC,      ///< �������� ����
        AC,      ///< �������� ����
        GND,     ///< ���� �������.
        Size
    };
    COMMON_ENUM;
    ModeCouple(uint v = 0) : value((uint8)v) {};
};

enum Resistance
{
    Resistance_1Mom,
    Resistance_50Om
};

/// ����� ����� �������.
enum TimeCounting
{
    TimeCounting_100ms,
    TimeCounting_1s,
    TimeCounting_10s
};

/// ������� ����������� ��������� ��� ����� �������.
enum FreqClc
{
    FreqClc_100kHz,
    FreqClc_1MHz,
    FreqClc_10MHz,
    FreqClc_100MHz
};

/// ���������� ��������.
enum NumberPeriods
{
    NumberPeriods_1,
    NumberPeriods_10,
    NumberPeriods_100
};

enum PeakDetMode
{
    PeakDet_Disabled,
    PeakDet_Enabled,
    PeakDet_Average
};

/// ���������� ��������� ��� ������� ������������ � ������������� ��������.
struct ENumMinMax
{
    enum
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128
    };
    COMMON_ENUM;
};

enum ColorScheme
{
    ColorScheme_WhiteLetters,   ///< � ���� ������ ��������� ��������� ���� ������� ����� - �� ����� ������ ����� �����
    ColorScheme_BlackLetters    ///< � ���� ������ ��������� ��������� ���� ������� ������ - �� ����� ������� ��������
};

/// ���������� ���������� �� ����������.
struct ENumAverage
{
    enum
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _256,
        _512
    };
    COMMON_ENUM;
};

/// ����� �������.
enum StartMode
{
    StartMode_Auto,     ///< ��������������.
    StartMode_Wait,     ///< ������.
    StartMode_Single    ///< �����������.
};

/// ����� �������� ��������.
enum CursLookMode
{
    CursLookMode_None,      ///< ������� �� ������.
    CursLookMode_Voltage,   ///< ������� ������ �� ����������� �������������.
    CursLookMode_Time,      ///< ������� ������ �� �������� �������������.
    CursLookMode_Both       ///< ������� ������ �� �������� � �����������, � ����������� �� ����, ����� ������� ������� ���������.
};

/// ����� ������� ������ �������. ����� �������, �� � ����� ������������ �� �������� ����� ���������.
enum CursActive
{
    CursActive_U,
    CursActive_T,
    CursActive_None
};

/// ������������ ����������� ��������.
enum CursMovement
{
    CursMovement_Pixels,    ///< �� �������� ������
    CursMovement_Percents   ///< �� ���������
};

/// ���������� ����������.
struct ENumAccum
{
    enum
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _Infinity
    };
    COMMON_ENUM;
};

struct ModeAccumulation
{
    enum
    {
        NoReset,   /// � ���� ������ ������������ ������ N ���������.
        Reset      /// � ���� ������ ���������� N ��������� � ����� ������������.
    };
    COMMON_ENUM;
};

/// ���������� ����� ��� ������� �����������.
struct ENumSmoothing
{
    enum
    {
        Disable,
        _2points,
        _3points,
        _4points,
        _5points,
        _6points,
        _7points,
        _8points,
        _9points,
        _10points
    };
    COMMON_ENUM;
};

/// ����������� FPS.
struct ENumSignalsInSec
{
    enum
    {
        _25,
        _10,
        _5,
        _2,
        _1
    };
    ENumSignalsInSec(uint8 v = _25) : value(v) {};
    COMMON_ENUM;
};

/// ��� ����� �� ������.
struct TypeGrid
{
    enum
    {
        _1,
        _2,
        _3,
        _4,
        Size
    };
    COMMON_ENUM;
};

/// ��� �������� � �������� �� ���������
struct LinkingRShift
{
    enum
    {
        Voltage,      ///< �������� ��������� � ����������
        Position      ///< �������� ��������� � �������
    };
    COMMON_ENUM;
};

/// ����� ����������� �������������� ������� �������� �������� �� ����������.
struct AltMarkers
{
    enum
    {
        Hide,        /// ������ �� ��������.
        Show,        /// ������ ��������.
        AutoHide     /// �������� � ������� ����� timeShowLevels.
    };
    COMMON_ENUM;
};

/// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
struct MenuAutoHide
{
    enum
    {
        None = 0,   ///< �������.
        _5 = 5,   ///< ����� 5 ������.
        _10 = 10,  ///< ����� 10 ������.
        _15 = 15,  ///< ����� 15 ������.
        _30 = 30,  ///< ����� 30 ������.
        _60 = 60   ///< ����� 60 ������.
    };
    COMMON_ENUM;
};

struct MeasuresNumber
{
    enum
    {
        _1,                       ///< 1 ��������� ����� �����.
        _2,                       ///< 2 ��������� ����� �����.
        _1_5,                     ///< 1 ������ � 5 �����������.
        _2_5,                     ///< 2 ������ �� 5 ���������.
        _3_5,                     ///< 3 ������ �� 5 ���������.
        _6_1,                     ///< 6 ����� �� 1 ���������.
        _6_2                      ///< 6 ����� �� 2 ���������.
    };
    uint8 value;
    operator uint8() const
    {
        return value;
    }
};

/// ������� �� ������� ��� ������ ���������.
enum ModeViewSignals
{
    ModeViewSignals_AsIs,       ///< ���������� ������� ��� ����.
    ModeViewSignals_Compress    ///< ������� ����� � ���������.
};

/// ��� ������� ��� ������ �������������.
enum SampleType
{
    SampleType_Real,    ///< �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
    SampleType_Equal    ///< ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
};

/// ������� ��/���.
enum FunctionTime
{
    FunctionTime_Time,
    FunctionTime_ShiftInMemory
};

/// ��� �������� � �������� �� �����������
enum LinkingTShift
{
    LinkingTShift_Time,     ///< �������� ��������� � �������
    LinkingTShift_Position  ///< �������� ��������� � �������
};

enum TrigModeFind
{
    TrigModeFind_Hand,      ///< ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������.
    TrigModeFind_Auto       ///< ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
};

enum CalibratorMode
{
    Calibrator_Freq,
    Calibrator_DC,
    Calibrator_GND
};

/// ����� ����������.
enum CalibrationMode
{
    CalibrationMode_x1,
    CalibrationMode_x10,
    CalibrationMode_Disable
};

enum ScaleFFT
{
    ScaleFFT_Log,           ///< ��� �������� �������� ��������������� ����� ������ �������.
    ScaleFFT_Linear         ///< ��� �������� �������� �������� ����� ������ �������.
};

struct SourceFFT
{
    enum
    {
        ChannelA,
        ChannelB,
        Both
    };
};

enum WindowFFT
{
    WindowFFT_Rectangle,
    WindowFFT_Hamming,
    WindowFFT_Blackman,
    WindowFFT_Hann
};

enum Function
{
    Function_Sum,
    Function_Mul
};

enum ModeRegSet
{
    ModeRegSet_Range,
    ModeRegSet_RShift
};

/// ����� ������������ ������.
enum FileNamingMode
{
    FileNamingMode_Mask,        ///< ��������� �� �����.
    FileNamingMode_Manually     ///< ��������� �������.
};

/// ��� ��������� ������ �� ������.
enum ModeSaveSignal
{
    ModeSaveSignal_BMP,
    ModeSaveSignal_TXT
};

/// ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
enum ModeShowIntMem
{
    ModeShowIntMem_Direct,  ///< ���������� ������ ��������� �������.
    ModeShowIntMem_Saved,   ///< ���������� ����������� ������.
    ModeShowIntMem_Both     ///< ���������� ��� �������.
};

const char *NameRange(Range range);
const char *NameTBase(TBase tBase);
