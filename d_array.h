#pragma once

#include <iostream>
#include "attendance.h"

template <class T>
class d_array {

public:
	d_array();
	d_array(const d_array& other);
	~d_array();
	void insert(const T& value);
	void insert(const T& value, size_t pos);
	void remove();
	void remove(size_t pos);
	size_t size() const;
	size_t capacity() const;
	void sort(bool type = 0);
	
	void save(std::ofstream & out) const;
	void load(std::ifstream& in);
	void print() const;

	// ������ ���������� ��� ������ ������ � ������� person, ������� ��, ����� ������� ������������� (����� �����)
	// �������� �� �������� ������������ ��� ������ ��� ����� �������� �� person
	d_array<T> get_by_interval(time_t date_1, time_t date_2) const = delete;
	d_array<T> get_by_group(const char* group) const = delete;
	d_array<T> get_by_surname(const char* surname) const = delete;
	void add_attendance_group(const char* group, time_t time) = delete;
	void add_attendance_person(const char* surname, time_t time) = delete;
	void add_group(const char* group, const char** surnames, size_t people_count) = delete;
	void remove_group(const char* group) = delete;

private:
	void resize(size_t new_capacity);

	T* _array;
	size_t _length;
	size_t _capacity;
	const static size_t _init_capacity = 8;
};

template<class T>
d_array<T>::d_array() :
	_array(nullptr),
	_length(0),
	_capacity(_init_capacity)
{}
/*
	����������� �����������
	��������������� �������� ������ �� other
*/
template<class T>
d_array<T>::d_array(const d_array& other) :
	_length(other._length),
	_capacity(other._capacity)
{
	_array = new T[_capacity];
	for (size_t i = 0; i < _length; i++) _array[i] = other._array[i];
}

template<class T>
d_array<T>::~d_array()
{
	if(_array) delete[] _array;
}

/*
	������� � �����
	���� ��������� ������, ��������� ������
*/
template<class T>
void d_array<T>::insert(const T& value)
{
	if (_length == 0) _array = new T[_capacity];
	if (_length >= _capacity) resize(_capacity * 2);
	_array[_length++] = value;
}

/*
	������� �� �������
	��������� ������, ���� ����� ������ �����, �������� ��� �������� ������ � ��������� � �������������� ����� ������
*/
template<class T>
void d_array<T>::insert(const T& value, size_t pos)
{
	if (pos  < 1 || pos > _length) return;
	pos--;
	if (_length == _capacity) resize(_capacity * 2);
	for (size_t i = _length; i > pos; i--) {
		_array[i] = _array[i - 1];
	}
	_array[pos] = value;
}

/*
	�������� � �����
	��������� ���������� ������� � ��������� ������������ ������ �������, ���� ����� � 2 ���� ������ ������������
*/
template<class T>
void d_array<T>::remove()
{
	if (_length == 0) return;
	_length--;
	if (_length <= _capacity / 2) resize(_capacity / 2);
}

/*
	�������� � �������
	�������� �������� ������� ����� �� ���������� � ��������� ������������ ������ �������, ���� �����
*/
template<class T>
void d_array<T>::remove(size_t pos)
{
	if (pos < 1 || pos > _length) return;
	for (size_t i = pos - 1; i < _length - 1; i++) {
		_array[i] = _array[i + 1];
	}
	_length--;
	if (_length <= _capacity / 2) resize(_capacity / 2);
}

/*
	���-�� ��������� � �������
*/
template<class T>
size_t d_array<T>::size() const
{
	return _length;
}

/*
	������� ���-�� ���������� ����� ��� ������
*/
template<class T>
inline size_t d_array<T>::capacity() const
{
	return _capacity;
}

/*
	����������� ����������
*/
template<class T>
void d_array<T>::sort(bool type)
{
	T temp;

	for (size_t i = 0; i < _length - 1; i++) {
		for (size_t j = i + 1; j < _length; j++) {
			if (_array[i] > _array[j]) {
				temp = _array[i];
				_array[i] = _array[j];
				_array[j] = temp;
			}
		}
	}
}

/*
	���������� � �������� ����
*/
template<class T>
void d_array<T>::save(std::ofstream& out) const
{
	out.write((char*)&_length, sizeof(_length)); // ���������� ���-�� ���������
	out.write((char*)&_capacity, sizeof(_capacity)); // ���������� ������ ���������� �����
	for (size_t i = 0; i < _length; i++) {
		out.write((char*)&_array[i], sizeof(_array[i])); // ���������� ������
	}
}

/*
	������ �� ��������� �����
*/
template<class T>
void d_array<T>::load(std::ifstream& in)
{
	if (_array) delete _array; // ������� ������
	in.read((char*)&_length, sizeof(_length)); // ������ ���-�� ���������
	in.read((char*)&_capacity, sizeof(_capacity)); // ������ ���-�� ���������� �����
	_array = new T[_capacity]; // ������� ����� ������
	for (size_t i = 0; i < _length; i++) {
		in.read((char*)&_array[i], sizeof(_array[i])); // ������ ��� ��������
	}
}

/*
	����� � �������
*/
template<class T>
void d_array<T>::print() const
{
	for (size_t i = 0; i < _length; i++) std::cout << _array[i] << " ";
	std::cout << std::endl;
}

/*
	��������� ���-�� ���������� �����
*/
template<class T>
void d_array<T>::resize(size_t new_capacity)
{
	T* new_array = new T[new_capacity]; // ������� ����� ������ ������� new_capacity
	_capacity = new_capacity;
	for (size_t i = 0; i < _length; i++) new_array[i] = _array[i]; // �������� �������� �� ������� ������� � �����
	delete[] _array; // ������� ������
	_array = new_array; // ����������� � _array ����� ������
}

/* ����� ������ ������ � ������� person */

/*
	����� � �������
*/
template<> // ������������� ��� person
void d_array<person>::print() const
{
	for (size_t i = 0; i < _length; i++) _array[i].print();
}

/*
	���������� � �������� ����
*/
template<> // ������������� ��� person
void d_array<person>::save(std::ofstream& out) const
{
	out.write((char*)&_length, sizeof(_length)); // ��� �� ������ ����� � ���-�� �����
	out.write((char*)&_capacity, sizeof(_capacity));
	for (size_t i = 0; i < _length; i++) _array[i].save(out); // ����� ������ ���������� ��� ������� person
}

/*
	�������� �� ��������� �����
*/
template<> // ������������� ��� person
void d_array<person>::load(std::ifstream& in)
{
	if (_array) delete _array; // ������� ������
	in.read((char*)&_length, sizeof(_length)); // ������ ����� � ���-�� �����
	in.read((char*)&_capacity, sizeof(_capacity));
	_array = new person[_capacity];
	for (size_t i = 0; i < _length; i++) {
		_array[i].load(in); // �������� ����� �������� � ������� person
	}
}

/*
	����� ��������� ���������� ������� � ���������
*/
template<> 
d_array<person> d_array<person>::get_by_interval(time_t date_1, time_t date_2) const
{
	d_array<person> res; // ������� ������ ��������� 
	for (size_t i = 0; i < _length; i++) {
		if (_array[i].was_between(date_1, date_2)) res.insert(_array[i]); //��������� �������� � ������ ���� �� ��� �� ���� � �������� ��������� 
	}
	return res;
}

/*
	����� ��������� ������������ ������
*/
template<> 
d_array<person> d_array<person>::get_by_group(const char* group) const
{
	d_array<person> res; // ������� ������ ���������
	for (size_t i = 0; i < _length; i++) {
		if (strcmp(_array[i].get_group(), group) == 0) res.insert(_array[i]); // ��������� � ������ ���� ��� ������ ��������� � ��������
	}
	return res;
}

/*
	����� ��������� � ������������ ��������
*/
template<>
d_array<person> d_array<person>::get_by_surname(const char* surname) const
{
	d_array<person> res; // ������� ������ ���������
	for (size_t i = 0; i < _length; i++) {
		if (strcmp(_array[i].get_surname(), surname) == 0) res.insert(_array[i]); // ��������� � ������ ���� ��� ������� ��������� � ��������
	}
	return res;
}

/*
	���������� ������ ��������� ���������
*/
template<>
void d_array<person>::add_attendance_group(const char* group, time_t time)
{
	for (size_t i = 0; i < _length; i++) {
		if (strcmp(_array[i].get_group(), group) == 0) _array[i].add_date(time); // ���� ������ �������� ��������� � �������� �������� ��� ���������
	}
}

/*
	���������� ������ �������� ���������
*/
template<>
void d_array<person>::add_attendance_person(const char* surname, time_t time)
{
	for (size_t i = 0; i < _length; i++) {
		if (strcmp(_array[i].get_surname(), surname) == 0) _array[i].add_date(time); // ���� ������� �������� ��������� � �������� �������� ��� ���������
	}
}

/*
	���������� ��������� ���������
*/
template<>
void d_array<person>::sort(bool type)
{
	person temp;

	for (size_t i = 0; i < _length - 1; i++) {
		for (size_t j = i + 1; j < _length; j++) { // ���������� ����� ��������� � ����������� �� ���� ��������� (�� ������� ��� ���-�� ���������)
			if (type == 0 ? _array[i].bigger_surname(_array[j]) : _array[i].bigger_count(_array[j])) { // type = 0 - �������, type = 1 - ������
				temp = _array[i];
				_array[i] = _array[j];
				_array[j] = temp;
			}
		}
	}
}

/*
	���������� ������
	��������� ������ ��������� ������ ������ �� �������, ������� ��������� � ���� ������
*/
template<>
void d_array<person>::add_group(const char* group, const char** surnames, size_t people_count) {
	for (size_t i = 0; i < people_count; i++) {
		insert(person(surnames[i], group)); // ��������� ������� �������� �� ������� surnames
	}
}

/*
	�������� ������
*/
template<>
void d_array<person>::remove_group(const char* group) {
	for (size_t i = 0; i < _length;) {
		if (strcmp(_array[i].get_group(), group) == 0) { // ���� � �������� ������ ������� � ��������, �� ������� ���
			remove(i + 1);
		}
		else i++; // ����� ���� � ���������� ��������
		// ������� ������������� ������ � ������ ����������, ������ ��� ��� �������� �������� ���������� � �� ����� ������� �������� �������� �����
	}
}