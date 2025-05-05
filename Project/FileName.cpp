//Basic libraries
#include <iostream>
#include <string>
#include <fstream>

//Extra logic libraries
#include <sstream>
#include <cctype>
#include <algorithm>
#include <limits>

//UI libraries
#include <iomanip>
#include <Windows.h>

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
void loadservices();
void saveservices();
void loadschedule();
void saveschedule();

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
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}



//User fn declaration
//void breakdown(parts p[], service s[], Maintenance m[], car c[], int size_p, int size_s, int size_m, int size_c, float& tax, parts p_info[], service s_info[]);
//void user_input(string& model_name, int& model_year, int& mileage, int& month);
//void cost_estimate(Maintenance m[], parts p[], service s[], int size_m, string model_name, int model_year, int mileage, int month,
//  parts p_info[], service s_info[], int p_size, int s_size, int sizep, int sizes, float& tax, float totallabor);

int main()
{
    loadservices();
    loadschedule();
    loadParts();
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

//Start admin fn definition
void loadservices() {
    ifstream file("services.txt");
    if (!file) return;
    while (file >> ws && !file.eof()) {
        getline(file, services[servicesCount].names, '|');
        file >> services[servicesCount].cost;
        file.ignore();
        servicesCount++;
    }

    file.close();
}
void saveservices() {
    ofstream file("services.txt");
    for (int i = 0; i < servicesCount; i++) {
        file << services[i].names << '|' << services[i].cost << '\n';
    }
    file.close();
}
void displayServices() {
    if (servicesCount == 0) {
        cout << "No services available.\n";
        return;
    }

    cout << "\n=== List of Services ===\n";
    cout << "-------------------------------------\n";
    cout << "Service Name\t\t\tCost\n";
    cout << "-------------------------------------\n";

    for (int i = 0; i < servicesCount; i++) {
        cout << left << setw(30) << services[i].names
            << "LE " << fixed << setprecision(2) << services[i].cost << endl;
    }

    cout << "---------------------------------\n";
    cout << "Total Services: " << servicesCount << "\n\n";
}
void services_edit() {
    cout << "<--- Welcome to the Services section --->" << endl;
    int answer;
    do {
        cout << " [1] - Dispalay Services \n";
        cout << " [2] - Add a Service \n";
        cout << " [3] - Update a Service \n";
        cout << " [4] - Exit \n";
        cout << "Enter your choice : ";
        cin >> answer;
        if (answer == 1) {
            displayServices();
            cout << "-----------------------\n";
        }
        else if (answer == 2) {
            //variables
            string name;
            float price;

            //input
            cin.ignore();    // Clear the input buffer

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
        else if (answer == 3) {
            string answer;
            displayServices();

            if (servicesCount == 0) { continue; }
            cout << "Choose a service to update by its name:\n";
            cin.ignore();    // Clear the input buffer
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
        else if (answer == 4) {
            break;
        }
        else {
            cout << "Invalid choice!!" << endl;
        }
    } while (answer != 4);
    saveservices();
}


void loadParts() {
    ifstream file("parts.txt");
    if (!file) return;
    while (file >> ws && !file.eof()) {
        getline(file, parts[partsCount].name, '|');
        file >> parts[partsCount].cost;
        file.ignore();
        string modelsLine;
        getline(file, modelsLine);
        splitString(modelsLine, ',', parts[partsCount].applicableModels, parts[partsCount].modelCount);
        partsCount++;
    }
    file.close();
}
void saveParts() {
    ofstream file("parts.txt");
    for (int i = 0; i < partsCount; i++) {
        file << parts[i].name << '|' << parts[i].cost << '|';
        for (int j = 0; j < parts[i].modelCount; j++) {
            file << parts[i].applicableModels[j];
            if (j != parts[i].modelCount - 1) file << ',';
        }
        file << '\n';
    }
    file.close();
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
        cout << " [1] - Dispalay Parts \n";
        cout << " [2] - Add a Part \n";
        cout << " [3] - Update a Part \n";
        cout << " [4] - Exit \n";
        cout << "Enter your choice : ";
        cin >> answer;
        if (answer == 1) {
            display_parts();
            cout << "-------------------------------------" << endl;
        }
        else if (answer == 2) {
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
        else if (answer == 3) {
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
        else if (answer != 4) {
            cout << "Invalid choice!!";
        }
    } while (answer != 4);
    saveParts();
}


void loadschedule() {
    std::ifstream inFile("schedule.txt");

    schedulesCount = 0;
    schedules = new schedule[MAX_SCHEDULES];

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.find("Schedule #") != std::string::npos) {
            schedulesCount++;
        }
        else if (schedulesCount > 0) {
            schedule& curr = schedules[schedulesCount - 1];

            if (line.find("Model: ") == 0) {
                curr.model = line.substr(7);
            }
            else if (line.find("Year: ") == 0) {
                curr.year = std::stoi(line.substr(6));
            }
            else if (line.find("Mileage: ") == 0) {
                curr.mileage = std::stoi(line.substr(9));
            }
            else if (line.find("Interval: Every ") == 0) {
                curr.months = std::stoi(line.substr(16));
            }
            else if (line.find("Parts: ") == 0) {
                std::string partsList = line.substr(7);
                size_t pos = 0;
                while ((pos = partsList.find(',')) != std::string::npos) {
                    curr.partNames[curr.partNamesCount++] = partsList.substr(0, pos);
                    partsList.erase(0, pos + 1);
                }
                curr.partNames[curr.partNamesCount++] = partsList;
            }
            else if (line.find("Services: ") == 0) {
                std::string servicesList = line.substr(10);
                size_t pos = 0;
                while ((pos = servicesList.find(',')) != std::string::npos) {
                    curr.serviceNames[curr.serviceNamesCount++] = servicesList.substr(0, pos);
                    servicesList.erase(0, pos + 1);
                }
                curr.serviceNames[curr.serviceNamesCount++] = servicesList;
            }
        }
    }
    inFile.close();
}
void saveschedule() {
    ofstream outFile("schedule.txt");  // Create or overwrite the file

    if (!outFile.is_open()) {
        cerr << "Error: Could not open schedule.txt for writing!" << endl;
        return;
    }

    // Write header
    outFile << "=== Maintenance Schedules ===\n";
    outFile << "Total Schedules: " << schedulesCount << "\n\n";

    // Write each schedule
    for (int i = 0; i < schedulesCount; i++) {
        outFile << "Schedule #" << i + 1 << "\n";
        outFile << "Model: " << schedules[i].model << "\n";
        outFile << "Year: " << schedules[i].year << "\n";
        outFile << "Mileage: " << schedules[i].mileage << " km\n";
        outFile << "Interval: Every " << schedules[i].months << " months\n";

        // Write parts
        outFile << "Parts: ";
        for (int j = 0; j < schedules[i].partNamesCount; j++) {
            outFile << schedules[i].partNames[j];
            if (j < schedules[i].partNamesCount - 1) outFile << ", ";
        }
        outFile << "\n";

        // Write services
        outFile << "Services: ";
        for (int j = 0; j < schedules[i].serviceNamesCount; j++) {
            outFile << schedules[i].serviceNames[j];
            if (j < schedules[i].serviceNamesCount - 1) outFile << ", ";
        }
        outFile << "\n\n";
    }

    outFile.close();
    cout << "Schedules successfully saved to schedule.txt" << endl;
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
void schedule_edit() {
    cout << "<--- Welcome to Maintenance Schedule --->" << endl;
    int answer;
    do {
        cout << " [1] - Display Schedule \n";
        cout << " [2] - Add Mentenance \n";
        cout << " [3] - Update a Part or a Service \n";
        cout << " [4] - Exit \n";
        cout << "Enter your choice : ";
        cin >> answer;

        if (answer == 1) {
            displaySchedules();
            cout << "-------------------------------------" << endl;
        }
        else if (answer == 2) {
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
            display_parts();
            cin.ignore();   // Clear the input buffer
            for (int i = 0; i < partscounter && newEntry.partNamesCount < MAX_PARTS; i++) {
                cout << "Enter part name #" << i + 1 << ": ";
                getline(cin, partname);
                transform(partname.begin(), partname.end(), partname.begin(), ::tolower); //transform any string to lower case to avoid user errors
                newEntry.partNames[newEntry.partNamesCount++] = partname;
            }

            int servicecounter;
            cout << "How many services do you want to add? ";
            cin >> servicecounter;

            string servicename;
            displayServices();
            cin.ignore();    // Clear the input buffer
            for (int i = 0; i < servicecounter && newEntry.serviceNamesCount < MAX_SERVICES; i++) {
                cout << "Enter service name #" << i + 1 << ": ";
                getline(cin, servicename);
                transform(servicename.begin(), servicename.end(), servicename.begin(), ::tolower); //transform any string to lower case to avoid user errors
                newEntry.serviceNames[newEntry.serviceNamesCount++] = servicename;
            }
            schedulesCount++;
            saveschedule();
            displaySchedules();
        }
        else if (answer == 3) {
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
                display_parts();
                cout << "Choose the part you want to edit by its name: ";
                cin.ignore();    // Clear the input buffer
                getline(cin, answer);
                transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

                bool found = false;
                for (int i = 0; i < schedulesCount && !found; i++) {
                    for (int j = 0; j < schedules[i].partNamesCount; j++) {
                        if (answer == schedules[i].partNames[j]) {
                            string partname;
                            cout << "Enter the name of the new part: ";
                            cin.ignore();    // Clear the input buffer
                            getline(cin, partname);
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
                displayServices();
                cout << "Choose the service you want to edit by its name: ";
                cin.ignore();    // Clear the input buffer
                getline(cin, answer);
                transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

                bool found = false;
                cin.ignore();    // Clear the input buffer
                for (int i = 0; i < schedulesCount && !found; i++) {
                    for (int j = 0; j < schedules[i].serviceNamesCount; j++) {
                        if (answer == schedules[i].serviceNames[j]) {
                            string servicename;
                            cout << "Enter the name of the new service: ";
                            getline(cin, servicename);
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
            saveschedule();
        }
        else if (answer != 4) {
            cout << "Invalid choice!!";
        }
    } while (answer != 4);
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
//End admin fn definition

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
