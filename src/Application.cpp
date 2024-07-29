#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

const int WORD_SIZE = 127 + 1;
const int BUFFER_SIZE = 1023 + 1;

struct Employee
{
	char* surname;
	char* name;
	int age;
};

struct List {
	Employee** employees;
	int structEmployeesSize = 5;
};

struct SearchNameAndSurname {
	char* surname;
	char* name;
};

Employee* createNewEmployee(const char surname[WORD_SIZE], const char name[WORD_SIZE], int age);

void printEmployee(Employee* employee);

void printAllEmployees(List* listEmployees);

char* requestString(const char  message[BUFFER_SIZE]);

int requestInt(const char message[BUFFER_SIZE]);

int requestAge();

char requestLetter(const char  message[BUFFER_SIZE]);

SearchNameAndSurname requestNameAndSurname();

void resizeUp(List* listEmployees);

void addNewEmployee(List* listEmployees, Employee* newEmployee);

List* inicializationFromFile(char* inFileName);

bool namePredicate(Employee* employee, char* name);

bool surnameAndNamePredicate(Employee* employee, SearchNameAndSurname nameAndSurname);

bool surnamePredicate(Employee* employee, char* surname);

bool agePredicate(Employee* employee, int age);

bool letterPredicate(Employee* employee, char letter);

template <typename T>
void printSelectedEmployee(List* listEmployees, T field, bool(predicate)(Employee* employee, T));

template <typename T>
void printSelectedEmployeeToFile(List* listEmployees, T field,
	bool(predicate)(Employee* employee, T), char* fileNameForInformation);

template <typename T>
bool isEmployeeExists(List* listEmployees, T field, bool(predicate)(Employee* employee, T));

void editName(Employee* employee, char* newName);

void editSurname(Employee* employee, char* newSurname);

void editAge(Employee* employee, int newAge);

template <typename T>
void edit(List* listEmployees, SearchNameAndSurname nameAndSurname, T newField, void (func)(Employee* employee, T));

void freeMemoryEmployee(Employee* employee);

void tryResizeDown(List* listEmployees);

void deleteEmployeer(List* listEmployees, SearchNameAndSurname nameAndSurname);

void saveEmloyees(List* listEmployees, char* fileName);

int main(int argc, char* argv[], char* envp[])
{
	char* employeesFileName = nullptr;		
	char* searchEmployeesFileName = nullptr;

	if (argc == 3) {
		employeesFileName = argv[1];
		searchEmployeesFileName = argv[2];
	}
	else {
		std::cout << "Enter the name file with information employees: ";
		employeesFileName = new char[WORD_SIZE];
		searchEmployeesFileName = new char[WORD_SIZE];
		std::cin >> employeesFileName;
		std::cout << "Enter the name file to search for an employees: ";
		std::cin >> searchEmployeesFileName;
	}

    List* listEmployees = inicializationFromFile(employeesFileName);

	std::cout << "INFORMATION SYSTEM EMPLOYEES:\n";
	std::cout << "Select the menu item:\n";
	std::cout <<
		"1 - adding data of employee;\n"
		"2 - editing data of employee;\n"
		"3 - deleting data of employeer;\n"
		"4 - search by surname;\n"
		"5 - displaying information about all"
		" employees of the specified age;\n"
		"6 - displaying information about all"
		" employees whose last name begins"
		" with the specified letter;\n"
		"7 - save information employees;\n"
		"0 - out and save;\n";

	int menuItem;

	while (true) {

		menuItem = requestInt("\nEnter the menu item: ");

		if (menuItem == 1) {
			std::cout << "Add a employee:\n";
			char* surname = requestString("Enter surname:");
			char* name = requestString("Enter name:");
			int age = requestAge();

			Employee* newEmployee = createNewEmployee(surname, name, age);

			addNewEmployee(listEmployees, newEmployee);
			printAllEmployees(listEmployees);

			delete[] name, surname, newEmployee;
		}
		else if (menuItem == 2) {
			std::cout << "Editing the data of employee:\n";
			SearchNameAndSurname nameAndSurname = requestNameAndSurname();

			if (isEmployeeExists(listEmployees, nameAndSurname, surnameAndNamePredicate)) {

				std::cout << "\nHow do you want to edit the data?:\n"
					"1 - edit surname;\n"
					"2 - edit name;\n"
					"3 - edit age;\n"
					"over - exit from editing.\n";

				int editMenu = requestInt("Enter the edit menu item:");

				if (editMenu == 1) {
					char* newSurname = requestString("Enter new surname: ");
					edit(listEmployees, nameAndSurname, newSurname, editSurname);
					delete[] newSurname;
				}
				else if (editMenu == 2) {
					char* newName = requestString("Enter new name: ");
					edit(listEmployees, nameAndSurname, newName, editName);
					delete[] newName;
				}
				else if (editMenu == 3) {
					int newAge = requestAge();
					edit(listEmployees, nameAndSurname, newAge, editAge);
				}
				else
					std::cout << "Exit from editing:\n";
			}
			else
				std::cout << "Employee was not found!";

			delete[] nameAndSurname.name, nameAndSurname.surname;
		}
		else if (menuItem == 3) {

			std::cout << "Delete a employee data:\n";
			SearchNameAndSurname nameAndSurname = requestNameAndSurname();
			deleteEmployeer(listEmployees, nameAndSurname);

			delete[] nameAndSurname.name, nameAndSurname.surname;
		}
		else if (menuItem == 4) {

			std::cout << "Search by surname:\n";
			char* surname = requestString("Enter surname:");
			
			printSelectedEmployee(listEmployees, surname, surnamePredicate);
			printSelectedEmployeeToFile(listEmployees, surname,
				surnamePredicate, searchEmployeesFileName);

			delete[] surname;
		}
		else if (menuItem == 5) {

			std::cout << "Search by age:\n";
			int age = requestAge();

			printSelectedEmployee(listEmployees, age, agePredicate);
			printSelectedEmployeeToFile(listEmployees, age,
				agePredicate, searchEmployeesFileName);
		}
		else if (menuItem == 6) {

			std::cout << "Search by first letter for surname:\n";
			char letter = requestLetter("Enter the letter (A - Z):");
			
			printSelectedEmployee(listEmployees, letter, letterPredicate);
			printSelectedEmployeeToFile(listEmployees, letter,
				letterPredicate, searchEmployeesFileName);

		}
		else if (menuItem == 7) {
			saveEmloyees(listEmployees, employeesFileName);
		}
		else if (menuItem == 0)
		{
			saveEmloyees(listEmployees, employeesFileName);
			std::cout << "\nYou have left the program and save information!";
			break;
		}
	}

	if (argc != 3)
		delete[] employeesFileName, searchEmployeesFileName;

	return 0;
}

Employee* createNewEmployee(const char surname[WORD_SIZE], const char name[WORD_SIZE], int age)
{
	Employee* newEmployee = new Employee;

	newEmployee->surname = new char[WORD_SIZE];
	strcpy(newEmployee->surname, surname);

	newEmployee->name = new char[WORD_SIZE];
	strcpy(newEmployee->name, name);

	newEmployee->age = age;

	return newEmployee;

}

void printEmployee(Employee* employee) {

	std::cout << "\n**************************\n";
	std::cout << "Last name: " << employee->surname << '\n';
	std::cout << "Name: " << employee->name << '\n';
	std::cout << "Age: " << employee->age << '\n';
}

void printAllEmployees(List* listEmployees) {

	for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++) {
		if (listEmployees->employees[ii] != NULL)
			printEmployee(listEmployees->employees[ii]);
	}
}

char* requestString(const char  message[BUFFER_SIZE]) {
	std::cout << message << "\n";
	char* string = new char[WORD_SIZE];
	std::cin.getline(string, WORD_SIZE);
	return string;
}

int requestInt(const char message[BUFFER_SIZE]) {
	std::cout << message << "\n";
	int integer;
	std::cin >> integer;
	std::cin.ignore();
	return integer;
}

int requestAge() {

	int age;
	do {
		age = requestInt("Enter age (1 - 120)");
	} while (!(age >= 1 && age <= 120));

	return age;
}

char requestLetter(const char  message[BUFFER_SIZE]) {

	char letter;
	do {
		std::cout << message << "\n";
		std::cin >> letter;
	} while (!(letter <= 'Z' && letter >= 'A'));
	
	return letter;
}

SearchNameAndSurname requestNameAndSurname() {

	char* surname = requestString("Enter the surname:");
	char* name = requestString("Enter the name:");

	return { surname, name };
}

void resizeUp(List* listEmployees) {

	int newStructEmployeerSize = 2 * listEmployees->structEmployeesSize;

	Employee** newEmployees = new Employee * [newStructEmployeerSize];

	for (int ii = 0; ii < newStructEmployeerSize; ii++)
		newEmployees[ii] = nullptr;

	for (int ii = 0, jj = 0; ii < listEmployees->structEmployeesSize; ii++) {
		if (listEmployees->employees[ii] != NULL) {
			newEmployees[jj++] = listEmployees->employees[ii];
		}
	}

	delete[] listEmployees->employees;

	listEmployees->structEmployeesSize = newStructEmployeerSize;
	listEmployees->employees = newEmployees;
}

void addNewEmployee(List* listEmployees, Employee* newEmployee) {

	for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++) {
		if (listEmployees->employees[ii] == NULL) {
			listEmployees->employees[ii] = newEmployee;
			return;
		}
	}

	resizeUp(listEmployees);

	for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++) {
		if (listEmployees->employees[ii] == NULL) {
			listEmployees->employees[ii] = newEmployee;
			break;
		}
	}
}

List* inicializeEmployee() {

	List* listEmployees = new List;
	listEmployees->employees = new Employee * [listEmployees->structEmployeesSize];

	for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++)
		listEmployees->employees[ii] = nullptr;

	addNewEmployee(listEmployees, createNewEmployee("Socolov", "Gleb", 20));
	addNewEmployee(listEmployees, createNewEmployee("Zunin", "Seva", 23));
	addNewEmployee(listEmployees, createNewEmployee("Ploshkin", "Makar", 18));
	addNewEmployee(listEmployees, createNewEmployee("Kovalev", "Vitya", 23));
	addNewEmployee(listEmployees, createNewEmployee("Shishkin", "Fedr", 27));

	return  listEmployees;
}

List* inicializationFromFile(char* inFileName) {

	int const BUFFER_SIZE = 127 + 1;

	List* listEmployees = new List;
	listEmployees->employees = new Employee * [listEmployees->structEmployeesSize];

	for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++)
		listEmployees->employees[ii] = nullptr;

	std::ifstream inFile;
	inFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		inFile.open(inFileName);
	}
	catch (const std::ios_base::failure& error)
	{
		std::cerr << "\033[31mError opening the file" << inFileName
			<< "!!!\n\a" << error.what() << "\033[0m\n";
		exit(1);
	}

	char* bufferSurname;
	char* bufferName;
	char* bufferAge;
	char* bufferSize;

	try
	{
		bufferSurname = new char[BUFFER_SIZE]; 
		bufferName = new char[BUFFER_SIZE];
		bufferAge = new char[BUFFER_SIZE];
		bufferSize = new char[BUFFER_SIZE];
	}
	catch (const std::bad_alloc& exception)
	{
		std::cerr << "\033[31mNot enough memory!!!\n\a" << exception.what() << "\033[0m\n";
		exit(2);
	}

	inFile.getline(bufferSize, BUFFER_SIZE);
	int size = atoi(bufferSize);

	for (int i = 0; i < size; i++) {
		try
		{
			inFile.getline(bufferSurname, BUFFER_SIZE);
			inFile.getline(bufferName, BUFFER_SIZE);
			inFile.getline(bufferAge, BUFFER_SIZE);
			addNewEmployee(listEmployees, createNewEmployee(bufferSurname, bufferName, atoi(bufferAge)));

		}
		catch (const std::ios_base::failure& error)
		{
			std::cerr << "\033[31mError reading the file!!!" << inFileName
				<< "!!!\n\a" << error.what() << "\033[0m\n";
			exit(3);
		}
	}
	try
	{
		inFile.close();
	}
	catch (const std::ios_base::failure& error)
	{
		std::cerr << "\033[31mError closing the file!!!" << inFileName
			<< "!!!\n\a" << error.what() << "\033[0m\n";
		exit(4);
	}

	delete[] bufferSurname, bufferName, bufferAge;

	return listEmployees;
}

bool namePredicate(Employee* employee, char* name)
{
	return strcmp(employee->name, name) == 0;
}

bool surnameAndNamePredicate(Employee* employee, SearchNameAndSurname nameAndSurname)
{
	if (strcmp(employee->name, nameAndSurname.name) == 0 && strcmp(employee->surname, nameAndSurname.surname) == 0)
		return true;
	else
		return false;
}

bool surnamePredicate(Employee* employee, char* surname)
{
	return strcmp(employee->surname, surname) == 0;
}

bool agePredicate(Employee* employee, int age)
{
	return employee->age == age;
}

bool letterPredicate(Employee* employee, char letter)
{
	return employee->surname[0] == letter;
}

template <typename T>
void printSelectedEmployee(List* listEmployees, T field, bool(predicate)(Employee* employee, T)) {
	bool isFound = false;
	Employee** employees = listEmployees->employees;

	for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++) {
		if (employees[ii] != NULL && predicate(employees[ii], field)) {
			isFound = true;
			printEmployee(employees[ii]);
		}
	}
	if (!isFound)
		std::cout << "\nEmployee are not found!\n";
}

template <typename T>
void printSelectedEmployeeToFile(List* listEmployees, T field, 
	bool(predicate)(Employee* employee, T), char* fileNameForInformation) {

	bool isFound = false;
	Employee** employees = listEmployees->employees;

	std::ofstream outFile;
	outFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);

	try
	{
		outFile.open(fileNameForInformation);
	}
	catch (const std::ios_base::failure& error)
	{
		std::cerr << "\033[31mError open file" << fileNameForInformation
			<< "!!!\n\a" << error.what() << "\033[0m\n";
		exit(1);
	}

	try
	{
		for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++) {
			if (employees[ii] != NULL && predicate(employees[ii], field)) {
				isFound = true;
				outFile << "Surname: " << employees[ii]->surname << '\n';
				outFile << "Name: " << employees[ii]->name << '\n';
				outFile << "Age: " << employees[ii]->age << '\n';
				outFile << "*********" << '\n';
			}
		}
		if (!isFound)
			std::cout << "\nEmployee are not found!\n";
		
	}
	catch (const std::ios_base::failure& error)
	{
		std::cerr << "\033[31mError write file" << fileNameForInformation
			<< "!!!\n\a" << error.what() << "\033[0m\n";
		exit(2);
	}

	try
	{
		outFile.close();
	}
	catch (const std::ios_base::failure& error)
	{
		std::cerr << "\033[31mОшибка закрытия файла" << fileNameForInformation
			<< "!!!\n\a" << error.what() << "\033[0m\n";
		exit(3);
	}
}

template <typename T>
bool isEmployeeExists(List* listEmployees, T field, bool(predicate)(Employee* employee, T)) {

	Employee** employees = listEmployees->employees;

	for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++) {
		if (employees[ii] != NULL && predicate(employees[ii], field)) {
			return true;
		}
	}
	return false;
}

void editName(Employee* employee, char* newName)
{
	strcpy(employee->name, newName);
}

void editSurname(Employee* employee, char* newSurname)
{
	strcpy(employee->surname, newSurname);
}

void editAge(Employee* employee, int newAge)
{
	employee->age = newAge;
}

template <typename T>
void edit(List* listEmployees, SearchNameAndSurname nameAndSurname, T newField, void (func)(Employee* employee, T)) {

	Employee** employees = listEmployees->employees;

	std::cout << "Selected employee:\n";
	printSelectedEmployee(listEmployees, nameAndSurname, surnameAndNamePredicate);
	bool isFound = false;

	for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++) {

		if (employees[ii] != NULL && surnameAndNamePredicate(employees[ii], nameAndSurname)) {
			func(employees[ii], newField);
			std::cout << "\nEdited data employee:\n";
			printEmployee(employees[ii]);
			isFound = true;
		}
	}
	if (!isFound)
		std::cout << "\nNo employee with that last name!";
}

void freeMemoryEmployee(Employee* employee) {

	delete[] employee->name, employee->surname;
	delete employee;
}

void tryResizeDown(List* listEmployees) {

	int countNull = 0;

	for (int ii = 0, jj = 0; ii < listEmployees->structEmployeesSize; ii++) {
		if (listEmployees->employees[ii] == NULL) {
			countNull++;
		}
	}

	if (countNull <= listEmployees->structEmployeesSize / 2)
		return;

	int newStructEmployeesSize = 0.7 * listEmployees->structEmployeesSize;

	Employee** newEmployee = new Employee * [newStructEmployeesSize];

	for (int ii = 0; ii < newStructEmployeesSize; ii++)
		newEmployee[ii] = nullptr;

	for (int ii = 0, jj = 0; ii < listEmployees->structEmployeesSize; ii++) {
		if (listEmployees->employees[ii] != NULL) {
			newEmployee[jj++] = listEmployees->employees[ii];
		}
	}

	delete[] listEmployees->employees;

	listEmployees->structEmployeesSize = newStructEmployeesSize;
	listEmployees->employees = newEmployee;
	std::cout << "\n Current array size:" << listEmployees->structEmployeesSize << '\n';
}

void deleteEmployeer(List* listEmployees, SearchNameAndSurname nameAndSurname) {

	Employee** employees = listEmployees->employees;

	bool isEmployeeFound = false;
	for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++) {
		if (employees[ii] != NULL && surnameAndNamePredicate(employees[ii], nameAndSurname)) {
			isEmployeeFound = true;
			freeMemoryEmployee(employees[ii]);
			employees[ii] = nullptr;

			std::cout << "\nYou have successfully deleted data: " << nameAndSurname.surname
				<< ' ' << nameAndSurname.name << "\n";
		}
	}

	if (!isEmployeeFound)
		std::cout << "\nNo person with that last name and surname!";
	else
		tryResizeDown(listEmployees);
}

void saveEmloyees(List* listEmployees, char* fileName) {

	int size = 0;
	for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++) {
		if (listEmployees->employees[ii] != NULL) {
			size++;
		}
	}

	std::ofstream outFile;
	outFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);

	try
	{
		outFile.open(fileName);
	}
	catch (const std::ios_base::failure& error)
	{
		std::cerr << "\033[31mError open file" << fileName
			<< "!!!\n\a" << error.what() << "\033[0m\n";
		exit(1);
	}

	try
	{
		outFile << size << '\n';
		for (int ii = 0; ii < listEmployees->structEmployeesSize; ii++) {
			if (listEmployees->employees[ii] != NULL) {
				outFile << listEmployees->employees[ii]->surname << '\n';
				outFile << listEmployees->employees[ii]->name << '\n';
				outFile << listEmployees->employees[ii]->age << '\n';
			}
		}
	}
	catch (const std::ios_base::failure& error)
	{
		std::cerr << "\033[31mError write file" << fileName
			<< "!!!\n\a" << error.what() << "\033[0m\n";
		exit(2);
	}

	try
	{
		outFile.close();
	}
	catch (const std::ios_base::failure& error)
	{
		std::cerr << "\033[31mОшибка закрытия файла" << fileName
			<< "!!!\n\a" << error.what() << "\033[0m\n";
		exit(3);
	}
}