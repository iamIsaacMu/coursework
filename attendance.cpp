#include "attendance.h"

const size_t person::_init_att_count = 16; // ������ ��������� �������� ���-�� ���������� ����� ������

/*
	����������� - �������������� ��� ����������
*/
person::person() :
	_surname(nullptr),
	_group(nullptr),
	_attendance(nullptr),
	_att_count(0),
	_att_capacity(_init_att_count)
{}

/*
	����������� - �������������� ���������� � ���������� �������� ���������� � ����
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
	���������� - ������� ��� ����
*/
person::~person()
{
	if (_surname) delete _surname;
	if (_group) delete _group;
	if (_attendance) delete _attendance;
}

/*
	���������� ���� ���������
*/
void person::add_date(time_t date)
{
	if (!_attendance) _attendance = new time_t[_att_capacity]; // ���� ������ ��������� ����, ������� ���
	if (_att_count >= _att_capacity) resize(); // ����������� ������ �������, ���� �� ��������
	_attendance[_att_count++] = date; // ��������� ���� � �����
}

/*
	�������� �� ���������� �������� �� ����� � �������� ���������� �������
*/
bool person::was_between(time_t date_1, time_t date_2) const
{
	for (size_t i = 0; i < _att_count; i++) {
		if (date_1 <= _attendance[i] && _attendance[i] <= date_2) return true; // ���� ���� ���� ���� �������� � ���������� ���������� true
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
	���������� � �������� ����
*/
void person::save(std::ofstream& out) const
{
	size_t surname_l = strlen(_surname) + 1;
	out.write((char*)&surname_l, sizeof(surname_l)); // ���������� ����� �������
	out.write(_surname, sizeof(char) * surname_l); // ���������� ���� �������

	size_t group_l = strlen(_group) + 1;
	out.write((char*)&group_l, sizeof(group_l)); // ���������� ����� ������
	out.write(_group, sizeof(char) * group_l); // ���������� ���� ������

	out.write((char*)&_att_capacity, sizeof(_att_capacity)); // ���������� ���-�� ��������� � ���-�� ���������� �����
	out.write((char*)&_att_count, sizeof(_att_count));
	for (size_t i = 0; i < _att_count; i++) out.write((char*)&(_attendance[i]), sizeof(_attendance[i])); // ���������� ���� ���������
}

/*
	�������� �� ��������� �����
*/
void person::load(std::ifstream& in)
{
	if (_surname) delete _surname; // ������� �������
	size_t surname_l;
	in.read((char*)&surname_l, sizeof(surname_l)); // ������ ����� �������
	_surname = new char[surname_l]; // ������� ������ ��� �������
	in.read(_surname, sizeof(char) * surname_l); // ������ �������

	if (_group) delete _group; // ������� ������
	size_t group_l;
	in.read((char*)&group_l, sizeof(group_l)); // ������ ����� ������
	_group = new char[group_l]; // ������� ������ ��� ������
	in.read(_group, sizeof(char) * group_l); // ������ ������
	
	if (_attendance) delete _attendance; // ������� ������ ���������
	in.read((char*)&_att_capacity, sizeof(_att_capacity)); // ������ ���-�� ��������� � ���-�� ���������� �����
	in.read((char*)&_att_count, sizeof(_att_count));
	_attendance = new time_t[_att_capacity]; // ������� ������ ���
	for (size_t i = 0; i < _att_count; i++) in.read((char*)&_attendance[i], sizeof(_attendance[i])); // ������ ������ ���������
}

/*
	����� � �������
	
	localtime - ��������� ���-�� ���������� � 01.01.1970 (��, ��� �������� � ������� ���� time_t)
	� ��������� ���������� ���, �����, ����, ���, ������, �������, �����������;

	asctime ��������� ��������� ���������� � ������� localtime � char* ������� ���� ����� ���� ����: ������: ������� ���

	� ������� ������� ������� � ������ � ��� ���� ���������
*/
void person::print() const
{
	std::cout << _surname << " " << _group << std::endl << " ";
	for (size_t i = 0; i < _att_count; i++) std::cout << asctime(localtime(&_attendance[i])) << " ";
	std::cout << std::endl;
}

/*
	���������� ������������
*/
person& person::operator=(const person& other)
{
	if (this == &other) return *this; // �������� �� ����������������
	if (_surname) delete _surname; // �������� �����
	if (_group) delete _group;
	if (_attendance) delete[] _attendance;
	_surname = new char[strlen(other._surname) + 1]; // ��������� ������ ��� ����� ����
	_group = new char[strlen(other._group) + 1];
	strcpy(_surname, other._surname); // ����������� ������� � ������
	strcpy(_group, other._group);
	_att_count = other._att_count; // ����������� ���-�� ��� � ���-�� ����� ��� ������
	_att_capacity = other._att_capacity;
	if (_att_count != 0) {
		_attendance = new time_t[_att_capacity]; // �������� ������� � ����������� ���� ��������� �� other
		for (size_t i = 0; i < _att_count; i++) _attendance[i] = other._attendance[i];
	}
	else {
		_attendance = nullptr;
	}
	return *this;
}

/*
	��������� �� ������� (this.surname < other.surname)
*/
bool person::smaller_surname(const person& other) const
{
	return strcmp(_surname, other._surname) < 0;
}

/*
	��������� �� ������� (this.surname > other.surname)
*/
bool person::bigger_surname(const person& other) const
{
	return strcmp(_surname, other._surname) > 0;
}

/*
	��������� �� ���������� (this.count < other.count)
*/
bool person::smaller_count(const person& other) const
{
	return _att_count < other._att_count;
}

/*
	��������� �� ���������� (this.count > other.count)
*/
bool person::bigger_count(const person& other) const
{
	return _att_count > other._att_count;
}

/*
	��������� ���-�� ����� ��� ������
*/
void person::resize()
{
	time_t* buf = new time_t[_att_capacity * 2]; // ����� ������ ��� �������� � 2 ���� ������
	_att_capacity *= 2;
	for (size_t i = 0; i < _att_count; i++) buf[i] = _attendance[i]; // ����������� �� ������� ������� � �����
	delete _attendance; // �������� �������
	_attendance = buf; // ������������� ������ � _attendance
}