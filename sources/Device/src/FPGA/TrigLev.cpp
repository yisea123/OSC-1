#include "TrigLev.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool TrigLev::pulse = false;
uint TrigLev::timeSwitchingTrig = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool TrigLev::SyncPulse()
{
    return pulse;
}
