
#include "Assignment.h"

const string Assignment::get_category_name(AssignmentCategory category)
{
	switch (category)
	{
	case AC_NONE:
		return "NONE";
	case AC_ClassParticipation:
		return "Class Participation";
	case AC_Quizzes:
		return "Quizzes";
	case AC_Labs:
		return "Labs";
	case AC_Midterms:
		return "Midterms";
	case AC_FinalExam:
		return "Final Exam";
	}

	return "NONE";
}

float Assignment::get_category_weight(AssignmentCategory category)
{
	switch (category)
	{
	case AC_NONE:
		return 0.0f;
	case AC_ClassParticipation:
		return 0.1f;
	case AC_Quizzes:
		return 0.1f;
	case AC_Labs:
		return 0.3f;
	case AC_Midterms:
		return 0.2f;
	case AC_FinalExam:
		return 0.3f;
	}

	return 0.0f;
}

bool Assignment::operator<(const Assignment& other) const
{
	//First assignment type, then name
	return name < other.name;
}

bool Assignment::operator>(const Assignment& other) const
{
	return name > other.name;
}
