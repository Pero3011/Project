#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <limits>
#include <fstream>
#include <iomanip>
//#include <Windows.h>

using namespace std;

//Global vars
int MAX_PARTS;
int MAX_MODELS;
int partsCount = 0;

//Structs
struct part {
	string name;
	double cost;
	string* applicableModels = new string[MAX_MODELS];
	int modelCount = 0;
};

part* parts = new part[MAX_PARTS];

struct service {
	string names;
	float cost;
};
struct schedule {
	string model;
	int year;
	int mileage;
	int months;
	vector<string> partNames; // Dynamic list of parts
	vector<string> serviceNames; //Dynamic list of services
};

//Vars
vector<service> services;
vector<schedule> schedules;


//void setColor(int color) {
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
//}

//Save & Load data
void loadParts();
void saveParts();

//Admin fn declaration

void part_edit();
void service_edit();
void updateTax();
void schedule_edit();

//System fn declaration
int  login(string username, string password);
void list1();
void logout();
void searchSystem();
void adminMenu();


void splitString(const string& str, char delimiter, string* outputArray, int& count) {
	vector<string> tokens;
	stringstream ss(str);
	string token;

	while (getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	count = tokens.size();

	for (size_t i = 0; i < tokens.size(); ++i) {
		outputArray[i] = tokens[i];
	}
}

//User fn declaration
//void breakdown(parts p[], service s[], Maintenance m[], car c[], int size_p, int size_s, int size_m, int size_c, float& tax, parts p_info[], service s_info[]);
//void user_input(string& model_name, int& model_year, int& mileage, int& month);
//void cost_estimate(Maintenance m[], parts p[], service s[], int size_m, string model_name, int model_year, int mileage, int month,
//	parts p_info[], service s_info[], int p_size, int s_size, int sizep, int sizes, float& tax, float totallabor);

int main()
{
    list1();
}

int login(string username, string password) {
    if (username == "admin" && password == "admin") {
        cout << "Login successful! (Admin)" << endl;
        return 1;
    }
    else {
        cout << "Login failed! Invalid credentials." << endl;
        return 0;
    }
}

void list1() {
    int choice;

    while (true) {
        cout << "\n";
        cout << "\t\t\t\t=========================================\n";
        cout << "\t\t\t\t|           Welcome to ElWarsha         |\n";
        cout << "\t\t\t\t|                                       |\n";
        cout << "\t\t\t\t=========================================\n";
        cout << "\n";
        cout << "\t\t\t\t\t[1] Login as Admin\n";
        cout << "\t\t\t\t\t[2] Login as User \n";
        cout << "\t\t\t\t\t[2] Exit\n";
        cout << "\n";
        cout << "\t\t\t\tPlease enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            int result = login(username, password);
            if (result == 1) {
                adminMenu(); // Admin
            }
        }
        else if (choice == 3) {
            cout << "Exiting...\n";
            exit(0);
        }
        else {
            cout << "Invalid choice!" << endl;
        }
    }
}

void logout() {
    cout << "\nYou have been logged out successfully. See you next time!\n";
}

void services_edit() {

	cout << "<--- Welcome to the Services section --->" << endl;
	int answer;
	do
	{
		cout << "Add a service or Update an exicting service(Add[1],update[2] or exit[3]): \n ";
		cin >> answer;
		if (answer == 1) /*checking the type of feature*/
		{
			//variables
			string name;
			float price;

			//input
			cin.ignore(numeric_limits<streamsize>::max(), '\n');    // Clear the input buffer

			cout << "enter the name of the new service:\n";
			getline(cin, name); //read multiple words from the user
			cout << "enter the cost of the new service:\n";
			cin >> price;
			transform(name.begin(), name.end(), name.begin(), ::tolower);//be5aly kolo lower case

			//update the array
			services.push_back({ name,price });

			//display results
			cout << "Successfully Added!!" << endl;
		}
		else if (answer == 2)
		{
			string answer;
			cout << "Choose a service to update by its name:\n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');    // Clear the input buffer
			getline(cin, answer);
			transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

			for (int i = 0; i < 5; i++)
			{
				float cost_update;
				if (answer == services[i].names)
				{

					cout << "Enter the new cost for " + services[i].names << endl;
					cin >> cost_update;
					services[i].cost = cost_update;	//update the cost

					cout << "Successfully Updated!!" << endl;

					break;

				}
				else if (i == (5 - 1) && answer != services[i].names)
				{
					cout << "Invalid service..Try Again!!" << endl;
				}

			}
		}
		else if (answer == 3) 
		{ 
			continue; 
		}
		else 
		{
			cout << "Invalid choice!!" << endl;
		}
	} while (answer != 3);
}

float updateTaxRate()
{
	float taxRate = 0.15;
	cout << "Current Tax Rate: " << taxRate * 100 << "%\n";
	cout << "Enter New Tax Rate (%): ";
	float newTax;
	cin >> newTax;
	taxRate = newTax / 100; // Convert percentage to decimal
	cout << "Tax Rate Updated Successfully to " << taxRate * 100 << "%\n";
	return taxRate;
}

void schedule_edit()
{
	cout << "<--- Welcome to Maintenance Schedule --->" << endl;
	string answer;
	do
	{

		cout << "\nType 'add' to add maintenance,or 'update' to update existing maintenance, or 'exit' to quit: ";
		cin >> answer;

		if (answer == "add") {
			schedule newEntry;
			cout << "Enter the car model: ";
			cin >> newEntry.model;
			cout << "Enter the model year: ";
			cin >> newEntry.year;
			cout << "Enter the mileage of the car: ";
			cin >> newEntry.mileage;
			cout << "Enter maintenance interval (in months): ";
			cin >> newEntry.months;

			int partscounter;
			cout << "How many parts do you want to add? ";
			cin >> partscounter;

			string partname;
			for (int i = 0; i < partscounter; i++) {
				cout << "Enter part name #" << i + 1 << ": ";
				cin >> partname;
				newEntry.partNames.push_back(partname);
			}

			int servicecounter;
			cout << "How many services do you want to add? ";
			cin >> servicecounter;

			string servicename;
			for (int i = 0; i < servicecounter; i++) {
				cout << "Enter service name #" << i + 1 << ": ";
				cin >> servicename;
				newEntry.serviceNames.push_back(servicename);
			}

			schedules.push_back(newEntry); // Add to global schedule
		}
		else if (answer == "update")
		{
			cout << "What do you want to update the service or parts: ";
			cin >> answer;

			if (answer == "parts")
			{
				for (auto& s : schedules)
				{
					cout << "Parts:\n";
					for (auto& part : s.partNames)
					{
						cout << " - " << part << endl;
					}
				}

				cout << "Choose the part you want to edit by its name: ";
				cin >> answer;

				for (auto& s : schedules)
				{
					for (auto& part : s.partNames)
					{
						if (answer == part)
						{
							string partname;
							cout << "Enter the name of the new part: ";
							cin >> partname;
							part = partname;
						}
					}
				}

			}
			else if (answer == "service")
			{
				for (auto& s : schedules)
				{
					cout << "Services:\n";
					for (auto& service : s.serviceNames)
					{
						cout << " - " << service << endl;
					}
				}

				cout << "Choose the service you want to edit by its name: ";
				cin >> answer;

				for (auto& s : schedules)
				{
					for (auto& service : s.serviceNames)
					{
						if (answer == service)
						{
							string servicename;
							cout << "Enter the name of the new service: ";
							cin >> servicename;
							service = servicename;
						}
					}
				}
			}
		}
	} while (answer != "exit");

}

void display_parts(int MAX_PARTS) {

	for (int i = 0; i < MAX_PARTS; i++) {
		cout << parts[i].name << "\t| " << parts[i].cost << "\t |";

		int cnt = 0;
		while (parts[i].applicableModels[cnt] != "") {
			if (cnt > 0) cout << " , ";
			cout << parts[i].applicableModels[cnt];
			cnt++;
		}
		cout << endl;
	}

}

void parts_edit() {

	cout << "<--- Welcome to the Parts section --->" << endl;
	int answer;
	do
	{
		cout << " 1 - Add a Part : \n ";
		cout << " 2 - Update a Part : \n ";
		cout << " 3 - Exit \n ";
		cin >> answer;
		if (answer == 1)
		{
			MAX_PARTS++;
			cin.ignore();

			cout << "\n=== Add New Part ===\n";

			cout << "Enter Part Name: ";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');    // Clear the input buffer
			getline(cin, parts[partsCount].name);

			cout << "Enter Part Cost: ";
			while (!(cin >> parts[partsCount].cost) || parts[partsCount].cost < 0) {
				cout << "Invalid input. Please enter a valid positive cost: ";
				cin.clear();
				cin.ignore();
			}

			cin.ignore();

			cout << "Enter Applicable Model: ";
			string model;
			getline(cin, model);
			parts[partsCount].applicableModels[0] = model;
			parts[partsCount].modelCount = 1;

			partsCount++;

			cout << "\nPart added successfully ! \n";
			cout << "----------------------------------------\n";
			cout << "Name   : " << parts[partsCount - 1].name << "\n";
			cout << "Cost   : " << fixed << setprecision(2) << parts[partsCount - 1].cost << " L.E\n";
			cout << "Model  : " << parts[partsCount - 1].applicableModels[0] << "\n";
			cout << "----------------------------------------\n";

		}
		else if (answer == 2)
		{
			string partName, model;

			display_parts(MAX_PARTS);
			cin.ignore();
			cout << "Enter Part Name to Update: ";
			getline(cin, partName);

			cout << "Enter Car Model to Update: ";
			getline(cin, model);

			while (partName.empty()) {
				cout << "Part name cannot be empty. Please enter a valid Part Name: ";
				getline(cin, partName);
			}

			while (model.empty()) {
				cout << "Model name cannot be empty. Please enter a valid Car Model: ";
				getline(cin, model);
			}

			bool found = false;
			int wanted = -1;

			for (int i = 0; i < partsCount; ++i) {
				if (parts[i].name == partName) {
					int cnt = 0;
					while (parts[i].applicableModels[cnt] != "") {
						if (parts[i].applicableModels[cnt] == model) {
							found = true;
							wanted = i;
							break;
						}
						cnt++;
					}
				}
			}

			if (found) {
				cout << "Part found! Now updating the part...\n";

				cout << "Enter new Part Name (or press Enter to keep it unchanged): ";
				string newName;
				getline(cin, newName);
				if (!newName.empty()) {
					parts[wanted].name = newName;
				}

				cout << "Enter new Part Cost (or press Enter to keep it unchanged): ";
				float newcost;
				cin >> newcost;
				if (newcost > 0) {
					parts[wanted].cost = newcost;
				}
				else {
					cout << "Invalid cost input.\n";
				}

				cout << "Enter applicable models separated by commas (e.g. Toyota,Honda,Ford)(or press Enter to keep it unchanged):";

				cin.ignore();
				string newmodel;
				getline(cin, newmodel);

				if (!newmodel.empty()) {

					parts[wanted].modelCount = 0;
					for (int i = 0; i < MAX_MODELS; ++i)
						parts[wanted].applicableModels[i] = "";

					splitString(newmodel, ',', parts[wanted].applicableModels, parts[wanted].modelCount);
				}


				cout << "\nPart updated successfully!\n";
			}
			else {
				cout << "Part not found with the name '" << partName << "' and model '" << model << "'.\n";
			}
		}
		else { cout << "Invalid choice!!"; break; }
	} while (answer != 3);
}

void adminMenu() {
	while (true) {

		cout << "\n";
		cout << "================== Admin Menu ==================\n";
		cout << "1. Add/Update Services\n";
		cout << "2. Add/Update Schedule\n";
		cout << "3. Add/Update Parts\n";
		cout << "4. Update Tax Rate\n";
		cout << "5. Search System\n";
		cout << "6. Logout\n";
		cout << "=================================================\n";
		cout << "Choose: ";

		int choice;
		cin >> choice;

		switch (choice) {
		case 1: services_edit();
			break;
		case 2: schedule_edit();
			break;
		case 3: parts_edit();
			break;
		case 4: logout();
			break;
		default: cout << "Invalid choice, try again.\n";
		}


	}
}