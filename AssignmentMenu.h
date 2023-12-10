
#pragma once

#include "Menu.h"

enum ASSIGNMENT_MENU_OPTION
{
	ASSIGNMENT_MENU_LIST,
	ASSIGNMENT_MENU_ADD,
	ASSIGNMENT_MENU_EDIT,
	ASSIGNMENT_MENU_CALCULATE_GRADE,
	ASSIGNMENT_MENU_DELETE,
	ASSIGNMENT_MENU_EXIT
};

/**
	Handles all menu functionality and interacts with assignmentList appropriately
*/
class AssignmentMenu : public Menu
{
public:

	AssignmentMenu();
	~AssignmentMenu();

	//Starts handling user input
	void begin();

private:

	typedef void (AssignmentMenu::*CallbackFunction)();

	void list_assignments();
	void add_assignment();
	void edit_assignment();
	void calculate_grade();
	void delete_assignment();

	//If true, return original function
	bool check_restart(bool b_condition, const string& message, CallbackFunction function);

	void get_input(const string& message, string& output);
		
	class AssignmentList* assignments;

	Menu* category_selection_menu;
};

