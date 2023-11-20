#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class User {
public:
    std::string username;
    std::string password;
    bool isAdmin;

    std::string name;
    std::string dateOfBirth;
    std::string vaccinationStatus;
    std::string address;
    std::string mobileNumber;
    std::string covidTestResults;

    User() : isAdmin(false) {}

    User(const std::string& uname, const std::string& pwd, bool admin,
        const std::string& n, const std::string& dob, const std::string& vacc,
        const std::string& addr, const std::string& mobile, const std::string& testResults)
        : username(uname), password(pwd), isAdmin(admin),
        name(n), dateOfBirth(dob), vaccinationStatus(vacc),
        address(addr), mobileNumber(mobile), covidTestResults(testResults) {}

    std::string serialize() const {
        return username + " " + password + " " + (isAdmin ? "1" : "0") +
            " " + name + " " + dateOfBirth + " " + vaccinationStatus +
            " " + address + " " + mobileNumber + " " + covidTestResults;
    }
};

void createUser();
bool loginUser(User& loggedInUser);
void viewUserInfo(const User& user, bool displayAll = false);
void userMenu(User& user);
void adminFunctions(User& adminUser);

void editUserDocuments(User& adminUser);
void changeVaccinationStatus(User& adminUser);
void uploadQRCode(User& adminUser);
void viewUserReports(const User& adminUser);

void viewTestResults(const User& user);
void viewDocuments(const User& user);
void fileReport(const User& user);

std::vector<User> users;

void saveUserDetailsToFile(const std::vector<User>& users) {
    std::ofstream userFile("user_details.txt");

    if (userFile.is_open()) {
        for (const User& user : users) {
            userFile << user.serialize() << "\n";
        }
        userFile.close();
    }
    else {
        std::cerr << "Error: Unable to open the user details file for writing.\n";
    }
}

void loadUserDetailsFromFile() {
    std::ifstream userFile("user_details.txt");

    if (userFile.is_open()) {
        users.clear();

        std::string line;
        while (std::getline(userFile, line)) {
            std::istringstream iss(line);
            std::string uname, pwd, isAdminStr, n, dob, vacc, addr, mobile, testResults;
            iss >> uname >> pwd >> isAdminStr >> n >> dob >> vacc >> addr >> mobile >> testResults;
            bool isAdmin = (isAdminStr == "1");
            users.emplace_back(uname, pwd, isAdmin, n, dob, vacc, addr, mobile, testResults);
        }

        userFile.close();
    }
    else {
        std::cerr << "Warning: Unable to open the user details file for reading. This could be normal for the first run.\n";
    }
}

int main() {
    loadUserDetailsFromFile();

    int choice;

    do {
        std::cout << "1. Create Account\n";
        std::cout << "2. Login\n";
        std::cout << "3. Admin Login\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            createUser();
            break;
        case 2: {
            User loggedInUser;
            if (loginUser(loggedInUser)) {
                viewUserInfo(loggedInUser);
                if (loggedInUser.isAdmin) {
                    adminFunctions(loggedInUser);
                }
                else {
                    userMenu(loggedInUser);
                }
            }
            else {
                std::cout << "Login failed. Please try again.\n";
            }
            break;
        }
        case 3: {
            std::string adminUsername, adminPassword;
            adminUsername = "admin";
            adminPassword = "admin";

            std::cout << "Enter admin username: ";
            std::cin >> adminUsername;
            std::cout << "Enter admin password: ";
            std::cin >> adminPassword;

            if (adminUsername == "admin" && adminPassword == "admin") {
                std::cout << "Admin login successful. Welcome, " << adminUsername << "!\n";
                adminFunctions(users.back());
            }
            else {
                std::cout << "Admin login failed. Please try again.\n";
            }
            break;
        }
        case 4:
            std::cout << "Exiting program.\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 4);

    saveUserDetailsToFile(users);

    return 0;
}

void createUser() {
    User newUser;
    std::cout << "Enter username: ";
    std::cin >> newUser.username;

    std::cout << "Enter password: ";
    std::cin >> newUser.password;

    newUser.isAdmin = false;

    std::cout << "Enter full name: ";
    std::cin.ignore();
    std::getline(std::cin, newUser.name);

    std::cout << "Enter date of birth (DD/MM/YYYY): ";
    std::cin >> newUser.dateOfBirth;

    std::cout << "Enter vaccination status (E.g. Vaccinated, Unvaccinated, or Partial): ";
    std::cin >> newUser.vaccinationStatus;

    std::cout << "Enter email address: ";
    std::cin.ignore();
    std::getline(std::cin, newUser.address);

    std::cout << "Enter mobile number: ";
    std::cin >> newUser.mobileNumber;

    std::cout << "Enter COVID test results (E.g. Positive, or Negative): ";
    std::cin >> newUser.covidTestResults;

    users.push_back(newUser);

    std::cout << "Account created successfully.\n";
}

bool loginUser(User& loggedInUser) {
    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    for (auto& user : users) {
        if (user.username == username && user.password == password) {
            std::cout << "Login successful. Welcome, " << username << "!\n";
            loggedInUser = user;
            return true;
        }
    }

    return false;
}

void viewUserInfo(const User& user, bool displayAll) {
    if (displayAll) {
        std::cout << "Username: " << user.username << "\n";
        std::cout << "Admin: " << (user.isAdmin ? "Yes" : "No") << "\n";
        std::cout << "Full Name: " << user.name << "\n";
        std::cout << "Date of Birth (DD/MM/YYYY): " << user.dateOfBirth << "\n";
        std::cout << "Vaccination Status (E.g. Vaccinated, Unvaccinated, or Partial): " << user.vaccinationStatus << "\n";
        std::cout << "Email Address: " << user.address << "\n";
        std::cout << "Mobile Number: " << user.mobileNumber << "\n";
        std::cout << "COVID Test Results (E.g. Positive, or Negative): " << user.covidTestResults << "\n";
    }
}

void userMenu(User& user) {
    int userChoice;
    bool hasLoggedIn = false;

    do {
        std::cout << "\nUser Menu:\n";
        std::cout << "1. View Documents\n";
        std::cout << "2. View Test Results\n";
        std::cout << "3. File Report\n";
        std::cout << "4. Logout\n";
        std::cout << "Enter your choice: ";
        std::cin >> userChoice;

        switch (userChoice) {
        case 1:
            viewDocuments(user);
            break;
        case 2:
            viewTestResults(user);
            break;
        case 3:
            fileReport(user);
            break;
        case 4:
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }

        if (userChoice == 2) {
            hasLoggedIn = true;
        }

    } while (userChoice != 4);
}

void adminFunctions(User& adminUser) {
    int adminChoice;
    do {
        std::cout << "\nAdmin Menu:\n";
        std::cout << "1. Edit User Documents\n";
        std::cout << "2. Change Vaccination Status\n";
        std::cout << "3. Upload QR Code\n";
        std::cout << "4. View User Reports\n";
        std::cout << "5. Logout\n";
        std::cout << "Enter your choice: ";
        std::cin >> adminChoice;

        switch (adminChoice) {
        case 1:
            std::cout << "Editing user documents...\n";
            editUserDocuments(adminUser);
            break;
        case 2:
            std::cout << "Changing vaccination status...\n";
            changeVaccinationStatus(adminUser);
            break;
        case 3:
            std::cout << "Uploading QR code...\n";
            uploadQRCode(adminUser);
            break;
        case 4:
            std::cout << "Viewing user reports...\n";
            viewUserReports(adminUser);
            break;
        case 5:
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (adminChoice != 5);
}

void editUserDocuments(User& adminUser) {
    std::string targetUsername;
    std::cout << "Enter the username of the user whose documents you want to edit: ";
    std::cin >> targetUsername;

    for (auto& user : users) {
        if (user.username == targetUsername) {
            std::cout << "Editing documents for user: " << targetUsername << "\n";
            std::cout << "Enter new full name: ";
            std::cin.ignore();
            std::getline(std::cin, user.name);

            std::cout << "Enter new date of birth (DD/MM/YYYY): ";
            std::cin >> user.dateOfBirth;

            std::cout << "Enter new vaccination status (E.g. Vaccinated, Unvaccinated, or Partial): ";
            std::cin >> user.vaccinationStatus;

            std::cout << "Enter new email address: ";
            std::cin.ignore();
            std::getline(std::cin, user.address);

            std::cout << "Enter new mobile number: ";
            std::cin >> user.mobileNumber;

            std::cout << "Enter new COVID test results (E.g. Positive, or Negative): ";
            std::cin >> user.covidTestResults;

            std::cout << "Documents updated successfully.\n";

            saveUserDetailsToFile(users);
            return;
        }
    }

    std::cout << "User not found.\n";
}

void changeVaccinationStatus(User& adminUser) {
    std::string targetUsername;
    std::cout << "Enter the username of the user whose vaccination status you want to change: ";
    std::cin >> targetUsername;

    for (auto& user : users) {
        if (user.username == targetUsername) {
            std::cout << "Changing vaccination status for user: " << targetUsername << "\n";
            std::cout << "Enter new vaccination status: ";
            std::cin >> user.vaccinationStatus;
            std::cout << "Vaccination status updated successfully.\n";

            saveUserDetailsToFile(users);
            return;
        }
    }

    std::cout << "User not found.\n";
}


void uploadQRCode(User& adminUser) {
    std::string targetUsername;
    std::cout << "Enter the username of the user to upload the QR code: ";
    std::cin >> targetUsername;

    for (auto& user : users) {
        if (user.username == targetUsername) {
            std::cout << "Uploading QR code for user: " << targetUsername << "\n";
            std::cout << "QR code uploaded successfully.\n";
            return;
        }
    }

    std::cout << "User not found.\n";
}

void viewDocuments(const User& user) {
        std::cout << "Viewing documents for user: " << user.username << "\n";
        std::cout << "Email Address: " << user.address << "\n";
        std::cout << "Mobile Number: " << user.mobileNumber << "\n";
        std::cout << "Full Name: " << user.name << "\n";
        std::cout << "Date of Birth: " << user.dateOfBirth << "\n";

}

void viewTestResults(const User& user) {
    std::cout << "Viewing test results for user: " << user.username << "\n";
    std::cout << "Vaccination Status: " << user.vaccinationStatus << "\n";
    std::cout << "COVID Test Results: " << user.covidTestResults << "\n";

}

void viewUserReports(const User& adminUser) {
    std::ifstream reportFile("error_reports.txt");
    std::string line;

    if (reportFile.is_open()) {
        std::cout << "User reports:\n";
        while (std::getline(reportFile, line)) {
            std::cout << line << '\n';
        }
        reportFile.close();
    }
    else {
        std::cerr << "Error: Unable to open the report file.\n";
    }
}

void fileReport(const User& user) {
    std::string report;

    std::cout << "Please type what went wrong with the application:\n";
    std::cin.ignore();
    std::getline(std::cin, report);

    std::ofstream reportFile("error_reports.txt", std::ios::app);

    if (reportFile.is_open()) {
        reportFile << "User: " << user.username << "\n";
        reportFile << "Report:\n" << report << "\n";
        reportFile << "---------------------------------\n";
        reportFile.close();

        std::cout << "Report filed successfully. Thank you for your feedback!\n";
    }
    else {
        std::cerr << "Error: Unable to open the report file.\n";
    }
}