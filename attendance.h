#pragma once
#define _CRT_SECURE_NO_WARNINGS

// для простоты использования сортировки
#define SURNAME 0
// для простоты использования сортировки
#define COUNT 1

#include <iostream>
#include <fstream>
#include <ctime>

class person {

public:
	person();
	person(const char* surname, const char* group);
	~person();
	void add_date(time_t date);
	bool was_between(time_t, time_t) const;
	const char* get_surname() const;
	const char* get_group() const;
	void save(std::ofstream& out) const;
	void load(std::ifstream& in);
	void print() const;

	person& operator=(const person& other);
	bool smaller_surname(const person& other) const;
	bool bigger_surname(const person& other) const;
	bool smaller_count(const person& other) const;
	bool bigger_count(const person& other) const;

private:
	void resize();

	char* _surname;
	char* _group;
	time_t* _attendance; // хранятся времена начала пары, на которых присутствовал этот человек, например
	size_t _att_count;
	size_t _att_capacity;
	const static size_t _init_att_count;
};