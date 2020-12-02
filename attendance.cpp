#include "attendance.h"

const size_t person::_init_att_count = 16; // Задаем начальное значение кол-ва выделенных ячеек памяти

/*
	Конструктор - инициализируем все переменные
*/
person::person() :
	_surname(nullptr),
	_group(nullptr),
	_attendance(nullptr),
	_att_count(0),
	_att_capacity(_init_att_count)
{}

/*
	Конструктор - инициализируем переменные и записываем значения аргументов в поля
*/
person::person(const char* surname, const char* group) :
	_attendance(nullptr),
	_att_count(0),
	_att_capacity(_init_att_count)
{
	_surname = new char[strlen(surname) + 1];
	strcpy(_surname, surname);
	_group = new char[strlen(group) + 1];
	strcpy(_group, group);
}

/*
	Деструктор - удаляем все поля
*/
person::~person()
{
	if (_surname) delete _surname;
	if (_group) delete _group;
	if (_attendance) delete _attendance;
}

/*
	Добавление даты посещения
*/
void person::add_date(time_t date)
{
	if (!_attendance) _attendance = new time_t[_att_capacity]; // Если массив посещений пуст, создаем его
	if (_att_count >= _att_capacity) resize(); // Увеличиваем размер массива, если он заполнен
	_attendance[_att_count++] = date; // Добавляем дату в конец
}

/*
	Проверка на нахождение студента на парах в заданный промежуток времени
*/
bool person::was_between(time_t date_1, time_t date_2) const
{
	for (size_t i = 0; i < _att_count; i++) {
		if (date_1 <= _attendance[i] && _attendance[i] <= date_2) return true; // Если хоть одна пара началась в промежутке возвращаем true
	}
	return false;
}

const char* person::get_surname() const
{
	return _surname;
}

const char* person::get_group() const
{
	return _group;
}

/*
	Сохранение в бинарный файл
*/
void person::save(std::ofstream& out) const
{
	size_t surname_l = strlen(_surname) + 1;
	out.write((char*)&surname_l, sizeof(surname_l)); // Записываем длину фамилии
	out.write(_surname, sizeof(char) * surname_l); // Записываем саму фамилию

	size_t group_l = strlen(_group) + 1;
	out.write((char*)&group_l, sizeof(group_l)); // Записываем длину группы
	out.write(_group, sizeof(char) * group_l); // Записываем саму группу

	out.write((char*)&_att_capacity, sizeof(_att_capacity)); // Записываем кол-во посещений и кол-во выделенных ячеек
	out.write((char*)&_att_count, sizeof(_att_count));
	for (size_t i = 0; i < _att_count; i++) out.write((char*)&(_attendance[i]), sizeof(_attendance[i])); // Записываем сами посещения
}

/*
	Загрузка из бинарного файла
*/
void person::load(std::ifstream& in)
{
	if (_surname) delete _surname; // удаляем фамилию
	size_t surname_l;
	in.read((char*)&surname_l, sizeof(surname_l)); // читаем длину фамилии
	_surname = new char[surname_l]; // создаем массив для фамилии
	in.read(_surname, sizeof(char) * surname_l); // читаем фамилию

	if (_group) delete _group; // удаляем группу
	size_t group_l;
	in.read((char*)&group_l, sizeof(group_l)); // читаем длину группы
	_group = new char[group_l]; // создаем массив для группы
	in.read(_group, sizeof(char) * group_l); // читаем группу
	
	if (_attendance) delete _attendance; // удаляем массив посещений
	in.read((char*)&_att_capacity, sizeof(_att_capacity)); // читаем кол-во посещений и кол-во выделенных ячеек
	in.read((char*)&_att_count, sizeof(_att_count));
	_attendance = new time_t[_att_capacity]; // создаем массив дат
	for (size_t i = 0; i < _att_count; i++) in.read((char*)&_attendance[i], sizeof(_attendance[i])); // читаем каждое посещение
}

/*
	Вывод в консоль
	
	localtime - переводит кол-во милисекунд с 01.01.1970 (то, что хранится в массиве типа time_t)
	в структуру содержащую год, месяц, день, час, минуты, секунды, милисекунды;

	asctime переводит структуру полученную с помощью localtime в char* формата день месяц дата часы: минуты: секунды год

	В функции выводим фамилию и группу и все даты посещений
*/
void person::print() const
{
	std::cout << _surname << " " << _group << std::endl << " ";
	for (size_t i = 0; i < _att_count; i++) std::cout << asctime(localtime(&_attendance[i])) << " ";
	std::cout << std::endl;
}

/*
	Копирующее присваивание
*/
person& person::operator=(const person& other)
{
	if (this == &other) return *this; // Проверка на самоприсваивание
	if (_surname) delete _surname; // Удаление полей
	if (_group) delete _group;
	if (_attendance) delete[] _attendance;
	_surname = new char[strlen(other._surname) + 1]; // Выделение памяти под новые поля
	_group = new char[strlen(other._group) + 1];
	strcpy(_surname, other._surname); // Копирование фамилии и группы
	strcpy(_group, other._group);
	_att_count = other._att_count; // Копирование кол-ва дат и кол-ва ячеек для записи
	_att_capacity = other._att_capacity;
	if (_att_count != 0) {
		_attendance = new time_t[_att_capacity]; // Создание массива и копирование всех элементов из other
		for (size_t i = 0; i < _att_count; i++) _attendance[i] = other._attendance[i];
	}
	else {
		_attendance = nullptr;
	}
	return *this;
}

/*
	Сравнение по фамилии (this.surname < other.surname)
*/
bool person::smaller_surname(const person& other) const
{
	return strcmp(_surname, other._surname) < 0;
}

/*
	Сравнение по фамилии (this.surname > other.surname)
*/
bool person::bigger_surname(const person& other) const
{
	return strcmp(_surname, other._surname) > 0;
}

/*
	Сравнение по посещениям (this.count < other.count)
*/
bool person::smaller_count(const person& other) const
{
	return _att_count < other._att_count;
}

/*
	Сравнение по посещениям (this.count > other.count)
*/
bool person::bigger_count(const person& other) const
{
	return _att_count > other._att_count;
}

/*
	Изменение кол-ва ячеек для записи
*/
void person::resize()
{
	time_t* buf = new time_t[_att_capacity * 2]; // Новый массив дат размером в 2 раза больше
	_att_capacity *= 2;
	for (size_t i = 0; i < _att_count; i++) buf[i] = _attendance[i]; // Копирование из старого массива в новый
	delete _attendance; // удаление старого
	_attendance = buf; // присваиваение нового в _attendance
}