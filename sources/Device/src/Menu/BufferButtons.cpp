#include "defines.h"
#include "BufferButtons.h"
#include "Log.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
KeyEvent BufferButtons::buffer[SIZE];
int       BufferButtons::start = 0;
int       BufferButtons::end = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BufferButtons::Push(KeyEvent str)
{
    if(end == SIZE)
    {
        //LOG_ERROR("����� ����������");
        return;
    }

    buffer[end] = str;
    end++;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
KeyEvent BufferButtons::Extract()
{
    KeyEvent result = buffer[start];

    start++;
    if(start == end)
    {
        start = end = 0;
    }

    return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool BufferButtons::IsEmpty()
{
    return end == 0;
}

