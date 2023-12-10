
#pragma once

#include "DateTime.h"

#include <iostream>

using namespace std;

enum AssignmentCategory
{
	AC_NONE,
	AC_ClassParticipation,
	AC_Quizzes,
	AC_Labs,
	AC_Midterms,
	AC_FinalExam
};

class Assignment
{
public:

	Assignment() = default;
	Assignment(int _id, AssignmentCategory _category, const string& _name, const DateTime& _start_date, const DateTime& _end_date, int _max_points, int _points)
		: id(_id)
		, category(_category)
		, name(_name)
		, start_date(_start_date)
		, end_date(_end_date)
		, max_points(_max_points)
		, points(_points)
	{}

	static const string get_category_name(AssignmentCategory category);
	static float get_category_weight(AssignmentCategory category);

	bool operator<(const Assignment& other) const;
	bool operator>(const Assignment& other) const;

	void set_id(int new_id) {id = new_id;}
	void set_category(AssignmentCategory new_category) {category = new_category;}
	void set_name(string new_name) {name = new_name;}
	void set_start_date(DateTime new_start_date) {start_date = new_start_date;}
	void set_end_date(DateTime new_end_date) {end_date = new_end_date;}
	void set_max_points(int new_max_points) {max_points = new_max_points;}
	void set_points(int new_points) {points = new_points;}

	const int get_id() const {return id;}
	const AssignmentCategory get_category() const {return category;}
	const string& get_name() const {return name;}
	const DateTime& get_start_date() const {return start_date;}
	const DateTime& get_end_date() const {return end_date;}
	const int get_max_points() const {return max_points;}
	const int get_points() const {return points;}

private:

	int id;
	AssignmentCategory category;
	string name;
	DateTime start_date;
	DateTime end_date;
	int max_points;
	int points;
};

