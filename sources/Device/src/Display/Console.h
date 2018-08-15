#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Console
{
public:
    static void Draw();

    static void AddString(char *string);

    static int NumberOfLines();

private:
    /// true ��������, ��� ��� ������� ������ ������� � ��������� � �� ����� ������ ������ (��� ����������, ����� ���������� ��� �� ����������)
    static bool inProcessDrawConsole;
    /// ���������� ����������� ����� � �������
    static int stringInConsole;
};

