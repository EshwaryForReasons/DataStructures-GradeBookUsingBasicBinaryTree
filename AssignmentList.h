
#pragma once

#include "BinaryTree.h"
#include "Assignment.h"

#include <map>
#include <vector>

class AssignmentList : public BinaryTree<Assignment>
{
public:

	void init_assignments();
	void update_assignments_file();

	void print_list();
	void add_assignment(AssignmentCategory category, const string& name, const DateTime& start_date, const DateTime& end_date, int max_points, int points);
	void edit_assignment(Assignment* assignment, AssignmentCategory category, const string& name, const DateTime& start_date, const DateTime& end_date, int max_points, int points);
	void calculate_grade();
	void delete_assignment(Assignment* assignment);

	Assignment* search_assignment(const string& name);

private:

	typedef map<AssignmentCategory, vector<Assignment*>> OrganizedMap;

	//Get a map organized by category. We can avoid a copy of a massive list by using the reference parameter
	void get_organized_map(OrganizedMap& organized_map);
};

