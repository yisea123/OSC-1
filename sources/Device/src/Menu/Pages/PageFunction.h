#pragma once
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PageFunction
{
public:
    static const PageBase *pointer;

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageFrequencyCounter
    {
    public:
        static const PageBase *pointer;

        /// \todo ����� ���������� ��������. ������ ���������� �� ������������ ��������

    /// ���������� ��������� �� ��������� - ���������� - ����� ����� F
        static const Choice *GetChoiceTimeF();
        /// ���������� ��������� �� ��������� - ���������� - ����� �������
        static const Choice *GetChoiceFreqClc();
        /// ���������� ��������� �� ��������� - ���������� - ���-�� ��������
        static const Choice *GetChoiceNumPeriods();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageFFT
    {
    public:
        static const PageBase *pointer;
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageMultimeter
    {
    public:
        static void Init();

        static void OnChanged_Mode(bool);

        static const PageBase *pointer;
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageTester
    {
    public:

        static void Init();

        static void OnChanged_Control(bool);

        static const PageBase *pointer;
    };

    //----------------------------------------------------------------------------------------------------------------------------------------------------
    class PageRecorder
    {
    public:
        static const PageBase *pointer;

        class PageSource
        {
        public:
            static const PageBase *pointer;
        };
    };
};
