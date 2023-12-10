
#include "AssignmentList.h"
#include "Utils.h"

#include <fstream>
#include <sstream>
#include <iomanip>

static const string ASSIGNMENTS_DATA = "assignment_data.csv";

void AssignmentList::init_assignments()
{
	clear();

	ifstream in_file(ASSIGNMENTS_DATA);

	// Placeholder variables for data in the file
	string id;
	string category;
	string name;
	string start_date;
	string end_date;
	string max_points;
	string points;
	string line = "";

	bool b_header = true;

	//Construct a User and put it in the user_list for every user in the file
	while (getline(in_file, line))
	{
		if (b_header)
		{
			b_header = false;
			continue;
		}

		stringstream ss(line);
		getline(ss, id, ',');
		getline(ss, category, ',');
		getline(ss, name, ',');
		getline(ss, start_date, ',');
		getline(ss, end_date, ',');
		getline(ss, max_points, ',');
		getline(ss, points, ',');

		insert(new Assignment(stoi(id), AssignmentCategory(stoi(category)), name, DateTime(start_date), DateTime(end_date), stoi(max_points), stoi(points)));
	}

	in_file.close();
}

void AssignmentList::update_assignments_file()
{
	fstream file(ASSIGNMENTS_DATA, ios::out);

	if (file.fail())
		cout << "Failed to open " << ASSIGNMENTS_DATA << " file!" << endl;

	//Add header
	file << "Id,Group Id,Name,Start Date, End Date,Possible Points,Points" << endl;

	OrganizedMap organized_map;
	get_organized_map(organized_map);

	for (auto const pair : organized_map)
		for (Assignment* assignment : pair.second)
			file << assignment->get_id() << "," << (int)assignment->get_category() << "," << assignment->get_name() << "," << assignment->get_start_date().to_string() << "," << assignment->get_end_date().to_string() << "," << std::to_string(assignment->get_max_points()) << "," << std::to_string(assignment->get_points()) << endl;

	file.close();
}

void AssignmentList::print_list()
{
	//Organize the list into a map of vectors. Each key represents an assignment category. We do this because the output of a binary search tree will be unorganized
	OrganizedMap organized_map;
	get_organized_map(organized_map);

	for (const auto pair : organized_map)
	{
		cprint(Assignment::get_category_name(pair.first), GREEN);

		for (Assignment* assignment : pair.second)
			cout << "-- " << assignment->get_name() << endl;
	}
}

void AssignmentList::add_assignment(AssignmentCategory category, const string& name, const DateTime& start_date, const DateTime& end_date, int max_points, int points)
{
	int new_id = 0;

	//Find the first open number in the data. Otherwise, find the largest id and make the new assignment one larger
	queue<Assignment*> list = traverse();
	int i = 1;
	while (!list.empty())
	{
		if (list.front()->get_id() != i)
			new_id = i;
		
		list.pop();
		i++;
	}

	if (new_id == 0 && list.empty())
		new_id = i + 1;

	insert(new Assignment(new_id, category, name, start_date, end_date, max_points, points));
	update_assignments_file();
}

void AssignmentList::edit_assignment(Assignment* assignment, AssignmentCategory category, const string& name, const DateTime& start_date, const DateTime& end_date, int max_points, int points)
{
	//After editing, the binary search tree is no longer sorted. To resolve this, we just recreate it
	queue<Assignment*> list = traverse();

	assignment->set_category(category);
	assignment->set_name(name);
	assignment->set_start_date(start_date);
	assignment->set_end_date(end_date);
	assignment->set_max_points(max_points);
	assignment->set_points(points);
	
	clear();

	while (!list.empty())
	{
		insert(list.front());
		list.pop();
	}

	update_assignments_file();
}

void AssignmentList::calculate_grade()
{
	OrganizedMap organized_map;
	get_organized_map(organized_map);

	map<AssignmentCategory, float> grade_by_group;

	for (const auto pair : organized_map)
	{
		int total_points = 0;
		int total_max_points = 0;

		for (Assignment* assignment : pair.second)
		{
			total_points += assignment->get_points();
			total_max_points += assignment->get_max_points();
		}

		grade_by_group.emplace(pair.first, (float)total_points / (float)total_max_points);
	}

	float final_grade = 0.0f;
	float weights_accounted = 0.0f;

	for (const auto pair : grade_by_group)
	{
		const float weight = Assignment::get_category_weight(pair.first);
		final_grade += pair.second * weight;
		weights_accounted += weight;
	}

	//If a category has no assignments, then we need to reweight the others
	final_grade *= (1 / weights_accounted) * 100.0f;

	cout << "Final Grade: " << (final_grade >= 90.0f ? "A" : final_grade >= 80.0f ? "B" : final_grade >= 65.0f ? "C" : final_grade >= 55.0f ? "D" : "F") << " " << final_grade << "%" << endl;
}

void AssignmentList::delete_assignment(Assignment* assignment)
{
	//Deleting in the case of two children in a binary search tree is rather complicated. Instead, I store all the nodes and reconstruct the tree, skipping the node to be deleted

	queue<Assignment*> list = traverse();
	clear();
	while (!list.empty())
	{
		Assignment* temp = list.front();
		list.pop();

		if(temp == assignment)
			continue;
		insert(temp);
	}

	update_assignments_file();
}

Assignment* AssignmentList::search_assignment(const string& name)
{
	auto continue_check_function = [&name](void* current)
	{
		return ((Node<Assignment>*)current)->data->get_name() != name;
	};

	auto comparision_function = [&name](void* current)
	{
		return ((Node<Assignment>*)current)->data->get_name() > name;
	};

	Node<Assignment>* node = search_node(continue_check_function, comparision_function);
	return node ? node->data : nullptr;
}

void AssignmentList::get_organized_map(AssignmentList::OrganizedMap& organized_map)
{
	queue<Assignment*> list = traverse();
	while (!list.empty())
	{
		Assignment* temp = list.front();
		list.pop();

		organized_map[temp->get_category()].push_back(temp);
	}
}
