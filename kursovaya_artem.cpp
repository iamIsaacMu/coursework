#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <ctime>
#include "d_array.h"
#include "attendance.h"

using namespace std;

void loop_person() {
    d_array<person> d;
    
    const char* filename = "file_person.bin"; // Имя файла
    
    /*
        Различные переменные, которые требуются для работы
    */
    char surname[128];
    char group[128];
    char** s;
    ifstream in;
    ofstream out;
    time_t t1, t2;
    time_t rawtime;
    time(&rawtime);
    struct tm* time_info1 = localtime(&rawtime);
    size_t parameter;
    size_t pos;
    
    int d1, m1, y1, d2, m2, y2, h, min;

    bool flag = 1;
    while (flag) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "0 - выход" << endl;
        cout << "1 - вставить" << endl;
        cout << "2 - вставить на позицию" << endl;
        cout << "3 - удалить" << endl;
        cout << "4 - удалить с позиции" << endl;
        cout << "5 - вывести на экран" << endl;
        cout << "6 - отсортировать по фамилии" << endl;
        cout << "7 - отсортировать по числу посещений занятий" << endl;
        cout << "8 - размер массива" << endl;
        cout << "9 - сохранить в бинарный файл" << endl;
        cout << "10 - загрузить из бинарного файла" << endl;
        cout << "11 - найти студентов" << endl;
        cout << "12 - добавить посещение" << endl;
        cout << "13 - добавить группу" << endl;
        cout << "14 - удалить группу" << endl;
        cin >> parameter;
        if (parameter > 14) {
            cout << "Неправильный выбор" << endl;
            continue;
        }
        switch (parameter)
        {
        case 0:
            cout << endl;
            return;
        case 1:
            cout << "Введите фамилию и группу через пробел" << endl;
            cin >> surname >> group;
            d.insert(person(surname, group));
            break;
        case 2:
            cout << "Введите позицию, фамилию и группу через пробел" << endl;
            cin >> pos >> surname >> group;
            d.insert(person(surname, group), pos);
            break;
        case 3:
            d.remove();
            break;
        case 4:
            cout << "Введите позицию" << endl;
            cin >> pos;
            d.remove(pos);
            break;
        case 5:
            d.print();
            break;
        case 6:
            d.sort(SURNAME); // SURNAME = 0
            break;
        case 7:
            d.sort(COUNT); // COUNT = 1
            break;
        case 8:
            cout << d.size() << endl;
            break;
        case 9:
            out.open(filename, ios::binary); // Открытие файла для записи
            d.save(out);
            out.close(); // Закрытие файла
            break;
        case 10:
            in.open(filename, ios::binary); // Открытие файла для чтения
            d.load(in);
            in.close(); // Закрытие файла
            break;
        case 11: // Поиск
            cout << "0 - Отмена" << endl;
            cout << "1 - По дате посещения" << endl;
            cout << "2 - По фамилии" << endl;
            cout << "3 - По группе" << endl;
            cin >> parameter;
            switch (parameter)
            {
            case 1:
                cout << "Введите даты начала и конца поиска (дд мм гг дд мм гг)" << endl;
                cin >> d1 >> m1 >> y1 >> d2 >> m2 >> y2;
                y1 += 100;
                y2 += 100;
                m1--;
                m2--;

                time_info1->tm_year = y1;
                time_info1->tm_mon = m1;
                time_info1->tm_mday = d1;
                time_info1->tm_hour = 0; // Устанавливаем самое начало суток
                time_info1->tm_min = 0;
                time_info1->tm_sec = 0;
                // mktime переводит структуру даты в кол-во милисекунд прошедших с 01.01.1970
                t1 = mktime(time_info1);
                
                time_info1->tm_year = y2;
                time_info1->tm_mon = m2;
                time_info1->tm_mday = d2;
                time_info1->tm_hour = 23; // Устанавливаем самый конец суток
                time_info1->tm_min = 59;
                time_info1->tm_sec = 59;
                t2 = mktime(time_info1);

                d.get_by_interval(t1, t2).print(); // Ищем по интервалу и выводим

                break;
            case 2:
                cout << "Введите фамилию" << endl;
                cin >> surname;
                d.get_by_surname(surname).print(); // Ищем по фамилии и выводим
                break;
            case 3:
                cout << "Введите группу" << endl;
                cin >> group;
                d.get_by_group(group).print(); // Ищем по группе и выводим
                break;
            default:
                break;
            }
            break;
        case 12:
            cout << "0 - Отмена" << endl;
            cout << "1 - Для одного студента" << endl;
            cout << "2 - Для группы" << endl;
            cin >> parameter;
            switch (parameter)
            {
            case 1:
                cout << "Введите дату занятия (дд мм гг чч мм)" << endl;
                cin >> d1 >> m1 >> y1 >> h >> min;
                m1--; // Месяцы идут с 0 а не с 1
                y1 += 100; // в структуре хранится кол-во лет с 1900 года
                time_info1->tm_year = y1;
                time_info1->tm_mon = m1;
                time_info1->tm_mday = d1;
                time_info1->tm_hour = h;
                time_info1->tm_min = min;
                time_info1->tm_sec = 0;
                t1 = mktime(time_info1);
                cout << "Введите фамилию" << endl;
                cin >> surname;
                d.add_attendance_person(surname, t1);
                break;
            case 2:
                cout << "Введите дату занятия (дд мм гг чч мм)" << endl;
                cin >> d1 >> m1 >> y1 >> h >> min;
                m1--;
                y1 += 100;
                time_info1->tm_year = y1;
                time_info1->tm_mon = m1;
                time_info1->tm_mday = d1;
                time_info1->tm_hour = h;
                time_info1->tm_min = min;
                time_info1->tm_sec = 0;
                t1 = mktime(time_info1);
                cout << "Введите группу" << endl;
                cin >> group;
                d.add_attendance_group(group, t1);
                break;
            default:
                break;
            }
            break;
        case 13:
            cout << "Введите группу" << endl;
            cin >> group;
            cout << "Введите количество студентов" << endl;
            cin >> parameter;
            s = new char*[parameter];
            cout << "Введите фамилии через пробел" << endl;
            for (size_t i = 0; i < parameter; i++) { // Последовательное чтение из консоли студентов
                s[i] = new char[30];
                cin >> surname;
                strcpy(s[i], surname); // Копирование фамилий в массив
            }
            d.add_group(group, (const char**)s, parameter);
            break;
        case 14:
            cout << "Введите группу" << endl;
            cin >> group;
            d.remove_group(group);
            break;
        default:
            break;
        }
    }
}
template<class _type>
void loop_simple() {
    
    d_array<_type> d;
    size_t parameter;
    ifstream in;
    ofstream out;
    _type el;
    size_t pos;

    const char* filename = "file.bin"; // Путь к файлу

    bool flag = 1;
    while (flag) {
        /*
        очистка буфера, если ее не будет, то при вводе строки,
        когда ожидалось число и т.д., программа зациклится
        */
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        cout << "0 - выход" << endl;
        cout << "1 - вставить" << endl;
        cout << "2 - вставить на позицию" << endl;
        cout << "3 - удалить" << endl;
        cout << "4 - удалить с позиции" << endl;
        cout << "5 - вывести на экран" << endl;
        cout << "6 - отсортировать" << endl;
        cout << "7 - размер массива" << endl;
        cout << "8 - сохранить в бинарный файл" << endl;
        cout << "9 - загрузить из бинарного файла" << endl;
        cin >> parameter;
        if (parameter > 9) {
            cout << "Неправильный выбор" << endl;
            continue;
        }
        switch (parameter)
        {
        case 0:
            cout << endl;
            return;
        case 1:
            cout << "Введите значение" << endl;
            cin >> el;
            d.insert(el);
            break;
        case 2:
            cout << "Введите позицию и значение" << endl;
            cin >> pos >> el;
            d.insert(el, pos);
            break;
        case 3:
            d.remove();
            break;
        case 4:
            cout << "Введите позицию" << endl;
            cin >> pos;
            d.remove(pos);
            break;
        case 5:
            d.print();
            break;
        case 6:
            d.sort();
            break;
        case 7:
            cout << d.size() << endl;
            break;
        case 8:
            out.open(filename, ios::binary); // Открытие файла лдя записи
            d.save(out);
            out.close(); // Закрытие файла для записи
            break;
        case 9:
            in.open(filename, ios::binary); // Открытие файла для чтения
            d.load(in);
            in.close(); // Закрыте файла для чтения
            break;
        default:
            break;
        }
    }
}


/*
    В демонстрационной программе 2 функции для работы с person и всеми остальными типами

    В главном меню выбираем тип для работы
*/
int main()
{
    setlocale(LC_ALL, "Russian");
    int param;
    bool flag = 1;
    while (flag) {
        cout << "Выберите с каким типом работать" << endl << endl;
        cout << "0 - выход" << endl;
        cout << "1 - int" << endl;
        cout << "2 - float" << endl;
        cout << "3 - person" << endl;
        cin >> param;
        switch (param)
        {
        case 0:
            flag = 0;
            break;
        case 1:
            loop_simple<int>();
            break;
        case 2:
            loop_simple<float>();
            break;
        case 3:
            loop_person();
            break;
        default:
            break;
        }
    }
}
