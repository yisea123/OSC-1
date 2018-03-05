#include "Device.h"
#include "Log.h"
#include "Display/Display.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Hardware/Timer.h"
#include "Utils/Decoder.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/MathOSC.h"
#include <stdlib.h>
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DeviceMode Device::currentMode = Mode_Osci;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Device::Init()
{
    uint64 temp = (uint64)(rand() % 4);
    temp++;
    
    Hardware::Init();

    Tester::Init();

    Display::Init();

    FPGA::Init();

    set.Load();

    FPGA::LoadSettings();

    menu.Init();

    FPGA::OnPressStart();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Device::Update()
{
    START_MULTI_MEASUREMENT();
    menu.Update();
    Display::Update();
    Decoder::Update();
    FPGA::Update();
    menu.SaveSettings();
    Tester::Update();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
DeviceMode Device::CurrentMode()
{
    return currentMode;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Device::ChangeMode()
{
    mathOSC.CircleIncrease<uint8>((uint8 &)currentMode, 0, NumDeviceModes - 1);

    if (currentMode == Mode_Tester)
    {
        Tester::Enable();
    }
    else
    {
        Tester::Disable();
    }
}
