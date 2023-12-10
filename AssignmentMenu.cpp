
#include "AssignmentMenu.h"

#include "AssignmentList.h"
#include "Menu.h"
#include "Utils.h"

#include <thread>
#include <iostream>
#include <conio.h>

static void block_and_go_back()
{
	cout << "Press q to go back." << endl;

	int input = _getch();

	if (input != Q_KEY)
		block_and_go_back();
	else
		cout << "Going back..." << endl;
}

AssignmentMenu::AssignmentMenu()
{
	set_name("Assignment Menu");
	add_option("List Assignments");
	add_option("Add Assignment");
	add_option("Edit Assignment");
	add_option("Calculate Grade");
	add_option("Delete Assignment");
	add_option("Exit");

	assignments = new AssignmentList();
	assignments->init_assignments();

	category_selection_menu = new Menu();
	category_selection_menu->set_name("Select a Category to Add Assignment");
	for (int i = 1; i < 6; ++i)
		category_selection_menu->add_option(Assignment::get_category_name(AssignmentCategory(i)));
}

AssignmentMenu::~AssignmentMenu()
{
	delete assignments;
}

void AssignmentMenu::begin()
{
	bool b_exit = false;

	while (!b_exit)
	{
		clear_console();
		int selected_option = display_menu();
		clear_console();

		switch (selected_option)
		{
		case ASSIGNMENT_MENU_LIST:
			list_assignments();
			break;
		case ASSIGNMENT_MENU_ADD:
			add_assignment();
			break;
		case ASSIGNMENT_MENU_EDIT:
			edit_assignment();
			break;
		case ASSIGNMENT_MENU_CALCULATE_GRADE:
			calculate_grade();
			break;
		case ASSIGNMENT_MENU_DELETE:
			delete_assignment();
			break;
		case ASSIGNMENT_MENU_EXIT:
		{
			cout << "Goodbye!" << endl;
			b_exit = true;
			break;
		}
		}

		if(!b_exit)
			block_and_go_back();
		this_thread::sleep_for(1s);
	}
}

void AssignmentMenu::list_assignments()
{
	assignments->print_list();
}

void AssignmentMenu::add_assignment()
{
	const int category = category_selection_menu->display_menu();

	cout << "***** Add " << Assignment::get_category_name(AssignmentCategory(category + 1)) << " Assignment *****\n" << endl;

	string name;
	string start_date;
	string end_date;
	string max_points;
	string points;

	get_input("Assignment Name: ", name);
	get_input("Start Date (mm/dd/yyyy hh:mm:ss): ", start_date);
	get_input("End Date (mm/dd/yyyy hh:mm:ss): ", end_date);
	get_input("Max Points: ", max_points);
	get_input("Points: ", points);

	if (check_restart(assignments->search_assignment(name), "Name conflict! Please use a unique name!", &AssignmentMenu::add_assignment))
		return;

	assignments->add_assignment(AssignmentCategory(category + 1), name, DateTime(start_date), DateTime(end_date), stoi(max_points), stoi(points));

	cout << "Assignment Added Successfully!" << endl;
}

void AssignmentMenu::edit_assignment()
{
	cout << "***** Edit Assignment *****\n" << endl;

	assignments->print_list();

	string original_name;
	cout << "Enter Assignment Name: ";
	getline(cin, original_name);

	Assignment* assignment = assignments->search_assignment(original_name);

	if (check_restart(!assignment, "Assignment was not found!", &AssignmentMenu::edit_assignment))
		return;

	clear_console();

	category_selection_menu->set_name("Select New Category:");
	const int category = category_selection_menu->display_menu();
	category_selection_menu->set_name("Select a Category to Add Assignment");

	string name = assignment->get_name();
	string start_date = assignment->get_start_date().to_string();
	string end_date = assignment->get_end_date().to_string();
	string max_points = std::to_string(assignment->get_max_points());
	string points = std::to_string(assignment->get_points());

	get_input("Assignment Name (" + assignment->get_name() + "): ", name);
	get_input("Start Date (" + assignment->get_start_date().to_string() + "): ", start_date);
	get_input("End Date (" + assignment->get_end_date().to_string() + "): ", end_date);
	get_input("Max Points (" + std::to_string(assignment->get_max_points()) + "): ", max_points);
	get_input("Points (" + std::to_string(assignment->get_points()) + "): ", points);

	assignments->edit_assignment(assignment, AssignmentCategory(category + 1), name, DateTime(start_date), DateTime(end_date), stoi(max_points), stoi(points));

	cout << "Assignment Edited Successfully!" << endl;
}

void AssignmentMenu::calculate_grade()
{
	cout << "***** Calculate Grade *****\n" << endl;

	assignments->calculate_grade();
}

void AssignmentMenu::delete_assignment()
{
	cout << "***** Delete Assignment *****\n" << endl;

	assignments->print_list();

	string name;
	cout << "Enter Assignment Name To Delete: ";
	getline(cin, name);

	Assignment* assignment = assignments->search_assignment(name);

	if (check_restart(!assignment, "Assignment was not found!", &AssignmentMenu::delete_assignment))
		return;

	assignments->delete_assignment(assignment);

	cout << "Assignment Deleted Successfully!" << endl;
}

bool AssignmentMenu::check_restart(bool b_condition, const string& message, CallbackFunction function)
{
	if (b_condition)
	{
		cout << message << endl;
		cout << "Press any key to try again." << endl;

		string temp;
		getline(cin, temp);

		(this->*function)();
	}

	return b_condition;
}

void AssignmentMenu::get_input(const string& message, string& output)
{
	cout << message;

	string temp;
	getline(cin, temp);

	if (!temp.empty())
		output = temp;

	//Strip the input of the newline character to prevent problems with formatting later
	if (output.find("\r") != string::npos || output.find("\n") != string::npos)
		output.erase(output.size() - 1);
}