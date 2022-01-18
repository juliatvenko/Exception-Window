#pragma warning( disable : 6385 )
#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

class ExceptionWindow
{
private:
    string* arr;
    int size;
    COORD start;
    COORD end;

    void SetCursor(short x, short y);
    void DrawRectangle();
public:
    ExceptionWindow();
    ExceptionWindow(int x1, int y1, int x2, int y2);
    ~ExceptionWindow();
    void CreateStringArray();
    void PrintStringArray();
};

//����������� �� �������������
ExceptionWindow::ExceptionWindow()
{
    this->start.X = 0; 
    this->start.Y = 0;
    this->end.X = 0; 
    this->end.Y = 0;
    this->size = 0;
    this->arr = NULL;
}

//�����������
ExceptionWindow::ExceptionWindow(int x1, int y1, int x2, int y2)
{
    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 || x1 > x2 || y1 > y2)
    {
        throw ("����������� ������i ����������");
    }       
    this->start.X = x1; 
    this->start.Y = y1;
    this->end.X = x2; 
    this->end.Y = y2;
    this->size = 0;
    this->arr = NULL;
}

//����������
ExceptionWindow::~ExceptionWindow()
{
    //��������, �� ����� �� �������
    if (arr)
    {
        delete[] arr;
    }
        
}

//��������� ������ ������, ���� ��������� ���� ������� � ����� �����
void ExceptionWindow::CreateStringArray()
{
    cout << "����i�� �i���i��� ���i���: ";
    cin >> size;

    if (size < 0)
    {
        throw ("����������i ��i��i ���i");
    }

    if (size > end.Y - start.Y)
    {
        throw ("����� ������ ����i�");
    }

    //������� ������� ������� ������������ ����� ��� �������� �� ���������
    cin.ignore();
    arr = new string[size];
    for (int i = 0; i < size; ++i)
    {
        //�������� ������ ����� � ��������
        getline(cin, arr[i]);
    }
        
}

//������������ ������ ������� �������
void ExceptionWindow::SetCursor(short x, short y)
{
    //��������� ����������� ������
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    //������������ ������� ���������
    COORD coordinates = { x, y };
    //���������� ������� �� �������� �������
    SetConsoleCursorPosition(handle, coordinates);
}

//��������� ������ � �����
void ExceptionWindow::PrintStringArray()
{
    //������� �������� ������ �� ���
    for (int i = 0; i < size; ++i)
    {
        if ((int)arr[i].length() > (end.X - start.X))
        {
            throw ("������� �������� � ������ " + to_string(i + 1) + ", ������� ����� �i���� �� ������� �i���");
        }
            
    }
        
    //��������� ������������
    DrawRectangle();

    //��������� ������ � ����� ����
    for (int i = 0, k = start.Y; i < size; ++i, ++k)
    {
        SetCursor(start.X, k);
        cout << arr[i];
    }

    //���� ���������� ��������� ��������� ������ �� ������� ����� ����� ���� �����
    SetCursor(0, end.Y + 1);
}

//�������������� �����, � ����� ��� ����� ���� ������
void ExceptionWindow::DrawRectangle()
{
    //�������� ������
    system("cls");

    //������� ��������� ���������� ��������� ������, � ����� �� ���� ����
    int x = start.X - 1;
    int y = start.Y - 1;

    int width = end.X - start.X + 1;
    int height = end.Y - start.Y + 1;

    //���� ������� start.Y = 0, �� ������ ���� ����� ��������� ���� ����������
    if (x >= 0 && y >= 0) 
    {
        //���������� ������� �� ������� (x1-1, y1-1)
        SetCursor(x, y);

        //��������� ������� ��� �����
        for (int i = 0; i <= width; i++) 
            cout << '-';
    }

    //���������� ������� � ����� (x1, y2)
    SetCursor(x, height + y);

    //��������� ������ ��� �����
    for (int i = 0; i <= width; i++) 
        cout << '-';

    // � ���� ������������ ����� (x1, i) �� (x2, i), � ��� ���� ����� ������� ���������� ����, ��� ����� i = y + 1
    // ���� ���� i = y, ������ ������ ������� "-" ����� (x1, y1) �� (x2, y1)
    for (int i = y + 1; i < height + y; i++) 
    { 

        //���� ������� start.� = 0, �� ��� ������� ������������ ��������� ���� ����������
        //��������� ��� ��� �����
        if (x >= 0 && y >= 0) 
        {
            // ���������� ������� � ����� (x1, i), �� � ������� �� ������� �� ����� (x1, y1) ��������������� ����
            SetCursor(x, i);
            cout << '|';
        }

        // ���������� ������� � ����� (x2, i), �� � ������� �� ������� �� ����� (x1, y2) ��������������� ����
        //��������� ����� ��� �����
        SetCursor(x + width, i);
        cout << '|';
    }
}

int main()
{
    //������������ ������� � �������� �������� � ������ 
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int x1, y1, x2, y2;
    cout << "����i�� ���������� (x1, y1) : ";
    cin >> x1 >> y1;
    cout << "����i�� ���������� (x2, y2) : ";
    cin >> x2 >> y2;

    //���� ������������ ���������� ��������
    try
    {
        ExceptionWindow window(x1, y1, x2, y2);
        window.CreateStringArray();
        window.PrintStringArray();
    }
    catch (string myException)
    {
        cout << myException;
    }
    return 0;
}