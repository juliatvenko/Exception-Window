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

//конструктор за замовчуванням
ExceptionWindow::ExceptionWindow()
{
    this->start.X = 0; 
    this->start.Y = 0;
    this->end.X = 0; 
    this->end.Y = 0;
    this->size = 0;
    this->arr = NULL;
}

//конструктор
ExceptionWindow::ExceptionWindow(int x1, int y1, int x2, int y2)
{
    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0 || x1 > x2 || y1 > y2)
    {
        throw ("Неправильно вказанi координати");
    }       
    this->start.X = x1; 
    this->start.Y = y1;
    this->end.X = x2; 
    this->end.Y = y2;
    this->size = 0;
    this->arr = NULL;
}

//деструктор
ExceptionWindow::~ExceptionWindow()
{
    //перевірка, чи масив не порожній
    if (arr)
    {
        delete[] arr;
    }
        
}

//створення масиву стрічок, який необхідно буде вивести в межах рамки
void ExceptionWindow::CreateStringArray()
{
    cout << "Введiть кiлькiсть стрiчок: ";
    cin >> size;

    if (size < 0)
    {
        throw ("Неправильнi вхiднi данi");
    }

    if (size > end.Y - start.Y)
    {
        throw ("Надто багато рядкiв");
    }

    //пропуск частини символів попереднього рядка для переходу на наступний
    cin.ignore();
    arr = new string[size];
    for (int i = 0; i < size; ++i)
    {
        //дозволяє читати рядки з пробілами
        getline(cin, arr[i]);
    }
        
}

//встановлення заданої позиції курсора
void ExceptionWindow::SetCursor(short x, short y)
{
    //отримання дескриптора консолі
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    //встановлення значень координат
    COORD coordinates = { x, y };
    //переміщення курсора на відповідну позицію
    SetConsoleCursorPosition(handle, coordinates);
}

//виведення стрічок в рамці
void ExceptionWindow::PrintStringArray()
{
    //обробка ситуації виходу за межі
    for (int i = 0; i < size; ++i)
    {
        if ((int)arr[i].length() > (end.X - start.X))
        {
            throw ("Виникла проблема з рядком " + to_string(i + 1) + ", довжина рядка бiльша за довжину вiкна");
        }
            
    }
        
    //малювання прямокутника
    DrawRectangle();

    //виведення стрічок у межах вікна
    for (int i = 0, k = start.Y; i < size; ++i, ++k)
    {
        SetCursor(start.X, k);
        cout << arr[i];
    }

    //після завершення виведення поставити курсор на початок рядка зразу після рамки
    SetCursor(0, end.Y + 1);
}

//вимальовування рамки, в межах якої мають бути стрічки
void ExceptionWindow::DrawRectangle()
{
    //очищення консолі
    system("cls");

    //оскільки задаються координати виведення тексту, а рамка має бути вище
    int x = start.X - 1;
    int y = start.Y - 1;

    int width = end.X - start.X + 1;
    int height = end.Y - start.Y + 1;

    //якщо введено start.Y = 0, то верхню межу рамки неможливо буде намалювати
    if (x >= 0 && y >= 0) 
    {
        //переміщення курсору на позицію (x1-1, y1-1)
        SetCursor(x, y);

        //виведення верхньої межі рамки
        for (int i = 0; i <= width; i++) 
            cout << '-';
    }

    //переміщення курсору в точку (x1, y2)
    SetCursor(x, height + y);

    //виведення нижньої межі рамки
    for (int i = 0; i <= width; i++) 
        cout << '-';

    // і задає розташування точок (x1, i) та (x2, i), з цих двох точок зміщення відбувається вниз, при цьому i = y + 1
    // адже якщо i = y, будуть витерті символи "-" точок (x1, y1) та (x2, y1)
    for (int i = y + 1; i < height + y; i++) 
    { 

        //якщо введено start.Х = 0, то ліву сторону прямокутника неможливо буде намалювати
        //виведення лівої межі рамки
        if (x >= 0 && y >= 0) 
        {
            // переміщення курсору в точку (x1, i), де і відповідає за зміщення від точки (x1, y1) перпендикулярно вниз
            SetCursor(x, i);
            cout << '|';
        }

        // переміщення курсору в точку (x2, i), де і відповідає за зміщення від точки (x1, y2) перпендикулярно вниз
        //виведення правої межі рамки
        SetCursor(x + width, i);
        cout << '|';
    }
}

int main()
{
    //розпізнавання введеної і виведеної кирилиці в консолі 
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int x1, y1, x2, y2;
    cout << "Введiть координату (x1, y1) : ";
    cin >> x1 >> y1;
    cout << "Введiть координату (x2, y2) : ";
    cin >> x2 >> y2;

    //блок перехоплення виняткових ситуацій
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