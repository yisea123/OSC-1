#pragma once
#include "defines.h"
#include "structs.h"
#include "SettingsTime.h"
#include "Display/Display.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsDisplay Settings Display
 *  @{
 */


/// ��� �������� � �������� �� ���������
struct LinkingRShift
{
    enum E
    {
        Voltage,      ///< �������� ��������� � ����������
        Position      ///< �������� ��������� � �������
    } value;
};

/// ��� ����� �� ������.
struct TypeGrid
{
    enum E
    {
        _1,
        _2,
        _3,
        _4,
        Size
    } value;
    operator uint8() const { return (uint8)value; };
};

/// ����������� FPS.
struct ENumSignalsInSec
{
    enum E
    {
        _25,
        _10,
        _5,
        _2,
        _1
    } value;
    ENumSignalsInSec(E v = _25) : value(v) {};
    operator uint8() const { return (uint8)value; };
};

/// ���������� ����� ��� ������� �����������.
struct ENumSmoothing
{
    enum E
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
    } value;
    operator uint8() const { return (uint8)value; };
};

struct ModeAccumulation
{
    enum E
    {
        NoReset,   /// � ���� ������ ������������ ������ N ���������.
        Reset      /// � ���� ������ ���������� N ��������� � ����� ������������.
    } value;
    operator uint8() const { return (uint8)value; };
};

/// ���������� ����������.
struct ENumAccum
{
    enum E
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
    } value;
    operator uint8() const { return (uint8)value; };
};

/// ���������� ���������� �� ����������.
struct ENumAverage
{
    enum E
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
    } value;
    operator uint8() const { return (uint8)value; };
};

struct ColorScheme
{
    enum E
    {
        WhiteLetters,   ///< � ���� ������ ��������� ��������� ���� ������� ����� - �� ����� ������ ����� �����
        BlackLetters    ///< � ���� ������ ��������� ��������� ���� ������� ������ - �� ����� ������� ��������
    } value;
};


#define SHIFT_IN_MEMORY             (set.disp_shiftInMemory)
#define TIME_MESSAGES               (set.disp_timeMessages)
#define ENUM_SIGNALS_IN_SEC         (set.disp_ENumSignalsInSec)
#define NUM_SIGNALS_IN_SEC          (sDisplay_NumSignalsInS())
#define LAST_AFFECTED_CH            (set.disp_lastAffectedChannel)
#define LAST_AFFECTED_CH_IS_A       (LAST_AFFECTED_CH.IsA())
#define DISPLAY_ORIENTATION         (set.dbg_Orientation)

#define TYPE_GRID                   (set.disp_typeGrid)
#define TYPE_GRID_1                 (TYPE_GRID == TypeGrid::_1)
#define TYPE_GRID_2                 (TYPE_GRID == TypeGrid::_2)
#define TYPE_GRID_3                 (TYPE_GRID == TypeGrid::_3)
#define TYPE_GRID_4                 (TYPE_GRID == TypeGrid::_4)

#define ENUM_ACCUM                  (set.disp_ENumAccum)
#define ENUM_ACCUM_INF              (ENUM_ACCUM == ENumAccum_Infinity)
#define NUM_ACCUM                   (1 << (int)ENUM_ACCUM)                   /* ���������� ����� ���������� */
#define IN_ACCUM_MODE               (ENUM_ACCUM > ENumAccum_1)

#define MODE_ACCUM                  (set.disp_modeAccumulation)
#define MODE_ACCUM_RESET            (MODE_ACCUM == ModeAccumulation::Reset)
#define MODE_ACCUM_NO_RESET         (MODE_ACCUM == ModeAccumulation::NoReset)

#define MODE_AVE                    (set.disp_modeAveraging)
#define ENUM_AVE                    (set.disp_ENumAverage)
#define NUM_AVE                     (1 << (int)ENUM_AVE)
#define IN_AVERAGING_MODE           (ENUM_AVE > ENumAverage::_1 && (!IN_P2P_MODE))

#define ENUM_SMOOTHING              (set.disp_ENumSmoothing)
#define NUM_SMOOTHING               (sDisplay_NumPointSmoothing())
#define SMOOTHING_ENABLED           (ENUM_SMOOTHING != ENumSmoothing::Disable)

#define ALT_MARKERS                 (set.disp_altMarkers)
#define ALT_MARKERS_SHOW            (ALT_MARKERS == AltMarkers::Show)
#define ALT_MARKERS_HIDE            (ALT_MARKERS == AltMarkers::Hide)
#define ALT_MARKERS_AUTO            (ALT_MARKERS == AltMarkers::AutoHide)

#define LINKING_RSHIFT              (set.disp_linkingRShift)

#define MENU_AUTO_HIDE              (set.disp_menuAutoHide)

#define BRIGHTNESS_DISPLAY          (set.disp_brightness)

#define BRIGHTNESS_GRID             (set.disp_brightnessGrid)

#define SET_NUM_BYTES_ON_DISPLAY    (SET_PEAKDET_EN ? 281 * 2 : 281)


/// ���������� ����������� ������� ������
int sDisplay_NumSignalsInS();
/// ������������� ����������� ������� ������
void sDisplay_SetNumSignalsInS(int maxFPS);
/// ���������� ����� ����� ������������� ������� (����� ������� - �����������)
int sDisplay_NumPointSmoothing();
/// ���������� ����� ������ � ��������� ����� �� ������ � ����������� ������
BitSet64 sDisplay_PointsOnDisplay();
/// \brief ��������� ����� ������ � ��������� ����� � ����������� ������
BitSet64 sDisplay_BytesOnDisplay();
/// ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������
int sDisplay_TimeMenuAutoHide();
/// ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
bool sDisplay_IsSeparate();
/// brightness = 1..100
void sDisplay_SetBrightness(int16 brightness);

int sDisplay_NumAverage();
/// ��� �������� ������ �� ������ � ������ �������� ��������� ��� ����� � ��� ���� ������, ��� ����, ������ ��� ������ ����� ������������ ����� �������
int sDisplay_ShiftInMemoryInPoints();

#define SHIFT_IN_MEMORY_IN_POINTS sDisplay_ShiftInMemoryInPoints()

/** @}  @}
 */
