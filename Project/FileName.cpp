#include <string>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <limits>
#include <fstream>
#include <iomanip>
//#include <Windows.h>

using namespace std;

//Global vars
int MAX_PARTS = 50; 
int MAX_MODELS = 50;  
int MAX_SERVICES = 50; 
int MAX_SCHEDULES = 50; 
int partsCount = 0;
int servicesCount = 0;
int schedulesCount = 0;

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
service* services = new service[MAX_SERVICES]; 

struct schedule {
    string model;
    int year;
    int mileage;
    int months;
    string* partNames = new string[MAX_PARTS]; 
    int partNamesCount = 0;
    string* serviceNames = new string[MAX_SERVICES]; 
    int serviceNamesCount = 0;
};

schedule* schedules = new schedule[MAX_SCHEDULES]; 



//Save & Load data
void loadParts();
void saveParts();

//Admin fn declaration
void parts_edit();
void services_edit();
void displayServices();
float updateTaxRate();
void schedule_edit();
void displaySchedules();

//System fn declaration
int  login(string username, string password);
void list1();
void logout();
void searchSystem();
void adminMenu();

//Assisting fn
void splitString(const string& str, char delimiter, string* outputArray, int& count) {
    stringstream ss(str);
    string token;
    count = 0;

    while (getline(ss, token, delimiter) && count < MAX_MODELS) {
        outputArray[count++] = token;
    }
}
//void setColor(int color) {
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
//}



//User fn declaration
//void breakdown(parts p[], service s[], Maintenance m[], car c[], int size_p, int size_s, int size_m, int size_c, float& tax, parts p_info[], service s_info[]);
//void user_input(string& model_name, int& model_year, int& mileage, int& month);
//void cost_estimate(Maintenance m[], parts p[], service s[], int size_m, string model_name, int model_year, int mileage, int month,
//  parts p_info[], service s_info[], int p_size, int s_size, int sizep, int sizes, float& tax, float totallabor);

int main()
{
    list1();
    // Clean up dynamic memory
    delete[] parts;
    delete[] services;
    delete[] schedules;
    return 0;
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
        cout << "\t\t\t\t|                                       |\n";
        cout << "\t\t\t\t|           Welcome to ElWarsha         |\n";
        cout << "\t\t\t\t|                                       |\n";
        cout << "\t\t\t\t=========================================\n";
        cout << "\n";
        cout << "\t\t\t\t\t[1] Login as Admin\n";
        cout << "\t\t\t\t\t[2] Login as User \n";
        cout << "\t\t\t\t\t[3] Exit\n";
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
    do {
        cout << "Add a service or Update an existing service(Add[1],update[2] or exit[3]): \n ";
        cin >> answer;
        if (answer == 1) {
            //variables
            string name;
            float price;

            //input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');    // Clear the input buffer

            cout << "enter the name of the new service:\n";
            getline(cin, name); //read multiple words from the user
            cout << "enter the cost of the new service:\n";
            cin >> price;
            transform(name.begin(), name.end(), name.begin(), ::tolower); //transform any string to lower case to avoid user errors

            //update the array
            if (servicesCount < MAX_SERVICES) {
                services[servicesCount].names = name;
                services[servicesCount].cost = price;
                servicesCount++;
                cout << "Successfully Added!!" << endl;
                cout << endl;
                displayServices();
            }
            else {
                cout << "Maximum services reached!" << endl;
            }
        }
        else if (answer == 2) {
            string answer;
            displayServices();
            cout << "Choose a service to update by its name:\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');    // Clear the input buffer
            getline(cin, answer);
            transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

            bool found = false;
            for (int i = 0; i < servicesCount; i++) {
                if (answer == services[i].names) {
                    float cost_update;
                    cout << "Enter the new cost for " + services[i].names << endl;
                    cin >> cost_update;
                    services[i].cost = cost_update;    //update the cost
                    cout << "Successfully Updated!!" << endl;
                    found = true;
                    displayServices();
                    break;
                }
            }
            if (!found) {
                cout << "Invalid service..Try Again!!" << endl;
            }
        }
        else if (answer == 3) {
            break;
        }
        else {
            cout << "Invalid choice!!" << endl;
        }
    } while (answer != 3);
}

void displayServices() {
    if (servicesCount == 0) {
        cout << "No services available.\n";
        return;
    }

    cout << "\n=== List of Services ===\n";
    cout << "---------------------------------\n";
    cout << "Service Name\t\tCost\n";
    cout << "---------------------------------\n";

    for (int i = 0; i < servicesCount; i++) {
        cout << left << setw(24) << services[i].names
            << "LE " << fixed << setprecision(2) << services[i].cost << endl;
    }

    cout << "---------------------------------\n";
    cout << "Total Services: " << servicesCount << "\n\n";
}

float updateTaxRate() {
    float taxRate = 0.15;
    cout << "Current Tax Rate: " << taxRate * 100 << "%\n";
    cout << "Enter New Tax Rate (%): ";
    float newTax;
    cin >> newTax;
    taxRate = newTax / 100; // Convert percentage to decimal
    cout << "Tax Rate Updated Successfully to " << taxRate * 100 << "%\n";
    return taxRate;
}

void schedule_edit() {
    cout << "<--- Welcome to Maintenance Schedule --->" << endl;
    int answer;
    do {
        cout << "Type '1' to add maintenance, or '2' to update existing maintenance, or '3' to quit: ";
        cin >> answer;

        if (answer == 1) {
            if (schedulesCount >= MAX_SCHEDULES) {
                cout << "Maximum schedules reached!" << endl;
                continue;
            }

            schedule& newEntry = schedules[schedulesCount];
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
            for (int i = 0; i < partscounter && newEntry.partNamesCount < MAX_PARTS; i++) {
                cout << "Enter part name #" << i + 1 << ": ";
                cin >> partname;
                transform(partname.begin(), partname.end(), partname.begin(), ::tolower); //transform any string to lower case to avoid user errors
                newEntry.partNames[newEntry.partNamesCount++] = partname;
            }

            int servicecounter;
            cout << "How many services do you want to add? ";
            cin >> servicecounter;

            string servicename;
            for (int i = 0; i < servicecounter && newEntry.serviceNamesCount < MAX_SERVICES; i++) {
                cout << "Enter service name #" << i + 1 << ": ";
                cin >> servicename;
                transform(servicename.begin(), servicename.end(), servicename.begin(), ::tolower); //transform any string to lower case to avoid user errors
                newEntry.serviceNames[newEntry.serviceNamesCount++] = servicename;
            }
            schedulesCount++;
            displaySchedules();
        }
        else if (answer == 2) {
            int answer;

            displaySchedules();

            cout << "What do you want to update the parts[1] or services[2]: ";
            cin >> answer;
            if (answer == 1) {
                for (int i = 0; i < schedulesCount; i++) {
                    cout << "Parts for schedule " << i + 1 << ":\n";
                    for (int j = 0; j < schedules[i].partNamesCount; j++) {
                        cout << " - " << schedules[i].partNames[j] << endl;
                    }
                }

                string answer;
                cout << "Choose the part you want to edit by its name: ";
                cin >> answer;
                transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

                bool found = false;
                for (int i = 0; i < schedulesCount && !found; i++) {
                    for (int j = 0; j < schedules[i].partNamesCount; j++) {
                        if (answer == schedules[i].partNames[j]) {
                            string partname;
                            cout << "Enter the name of the new part: ";
                            cin >> partname;
                            transform(partname.begin(), partname.end(), partname.begin(), ::tolower); //transform any string to lower case to avoid user errors
                            schedules[i].partNames[j] = partname;
                            found = true;
                            displaySchedules();
                            break;
                        }
                    }
                }
                if (!found) {
                    cout << "Part not found!" << endl;
                }
            }
            else if (answer == 2) {
                for (int i = 0; i < schedulesCount; i++) {
                    cout << "Services for schedule " << i + 1 << ":\n";
                    for (int j = 0; j < schedules[i].serviceNamesCount; j++) {
                        cout << " - " << schedules[i].serviceNames[j] << endl;
                    }
                }

                string answer;
                cout << "Choose the service you want to edit by its name: ";
                cin >> answer;
                transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

                bool found = false;
                for (int i = 0; i < schedulesCount && !found; i++) {
                    for (int j = 0; j < schedules[i].serviceNamesCount; j++) {
                        if (answer == schedules[i].serviceNames[j]) {
                            string servicename;
                            cout << "Enter the name of the new service: ";
                            cin >> servicename;
                            transform(servicename.begin(), servicename.end(), servicename.begin(), ::tolower); //transform any string to lower case to avoid user errors
                            schedules[i].serviceNames[j] = servicename;
                            found = true;
                            displaySchedules();
                            break;
                        }
                    }
                }
                if (!found) {
                    cout << "Service not found!" << endl;
                }
            }
        }
    } while (answer != 3);
}

void displaySchedules() {
    if (schedulesCount == 0) {
        cout << "No maintenance schedules available.\n";
        return;
    }

    cout << "\n=== Maintenance Schedules ===\n";

    for (int i = 0; i < schedulesCount; i++) {
        cout << "\nSchedule #" << i + 1 << "\n";
        cout << "---------------------------------\n";
        cout << "Model: " << schedules[i].model << "\n";
        cout << "Year: " << schedules[i].year << "\n";
        cout << "Mileage: " << schedules[i].mileage << " km\n";
        cout << "Interval: Every " << schedules[i].months << " months\n";

        // Display parts
        cout << "\nParts to Replace:\n";
        if (schedules[i].partNamesCount == 0) {
            cout << " - None\n";
        }
        else {
            for (int j = 0; j < schedules[i].partNamesCount; j++) {
                cout << " - " << schedules[i].partNames[j] << "\n";
            }
        }

        // Display services
        cout << "\nServices to Perform:\n";
        if (schedules[i].serviceNamesCount == 0) {
            cout << " - None\n";
        }
        else {
            for (int j = 0; j < schedules[i].serviceNamesCount; j++) {
                cout << " - " << schedules[i].serviceNames[j] << "\n";
            }
        }

        cout << "---------------------------------\n";
    }

    cout << "\nTotal Schedules: " << schedulesCount << "\n\n";
}

void display_parts() {
    for (int i = 0; i < partsCount; i++) {
        cout << parts[i].name << "\t| " << parts[i].cost << "\t |";

        int cnt = 0;
        while (cnt < parts[i].modelCount && parts[i].applicableModels[cnt] != "") {
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
    do {
        cout << " 1 - Add a Part : \n ";
        cout << " 2 - Update a Part : \n ";
        cout << " 3 - Exit \n ";
        cin >> answer;
        if (answer == 1) {
            if (partsCount >= MAX_PARTS) {
                cout << "Maximum parts reached!" << endl;
                continue;
            }

            cin.ignore();
            cout << "\n=== Add New Part ===\n";

            cout << "Enter Part Name: ";
            string name;
            getline(cin, name);

            cout << "Enter Part Cost: ";
            double cost;
            while (!(cin >> cost) || cost < 0) {
                cout << "Invalid input. Please enter a valid positive cost: ";
                cin.clear();
                cin.ignore();
            }

            cin.ignore();
            cout << "Enter Applicable Model: ";
            string model;
            getline(cin, model);

            parts[partsCount].name = name;
            parts[partsCount].cost = cost;
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
        else if (answer == 2) {
            string partName, model;

            display_parts();
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
                    while (cnt < parts[i].modelCount && parts[i].applicableModels[cnt] != "") {
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
                string costInput;
                getline(cin, costInput);
                if (!costInput.empty()) {
                    double newcost = stod(costInput);
                    if (newcost > 0) {
                        parts[wanted].cost = newcost;
                    }
                    else {
                        cout << "Invalid cost input.\n";
                    }
                }

                cout << "Enter applicable models separated by commas (e.g. Toyota,Honda,Ford)(or press Enter to keep it unchanged):";
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
        else if (answer != 3) {
            cout << "Invalid choice!!";
        }
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
        case 4: updateTaxRate();
            break;
        case 5: searchSystem();
            break;
        case 6: logout();
            return;
        default: cout << "Invalid choice, try again.\n";
        }
    }
}

void searchSystem() {
    // Implementation of search functionality would go here
    cout << "Search functionality not yet implemented." << endl;
}
