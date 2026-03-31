#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct User {
    string username;
    string password;
    string role;
    string department;
};

struct PolicyRule {
    string effect;         // permit / deny
    string subjectRole;    // admin / student / faculty
    string resourceType;   // file / marks / notes
    string action;         // read / write
    bool hasDepartmentCondition;
    string department;
};

bool authenticate(const vector<User>& users, const string& username,
                  const string& password, User& loggedInUser) {
    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            loggedInUser = user;
            return true;
        }
    }
    return false;
}

string enforcePolicy(const User& user, const vector<PolicyRule>& rules,
                     const string& resourceType, const string& action) {

    string decision = "deny"; // default

    for (const auto& rule : rules) {
        if (rule.subjectRole == user.role &&
            rule.resourceType == resourceType &&
            rule.action == action) {

            if (rule.hasDepartmentCondition &&
                rule.department != user.department) {
                continue;
            }

            // deny overrides
            if (rule.effect == "deny")
                return "deny";

            decision = "permit";
        }
    }

    return decision;
}
vector<User> loadUsers(const string& filename) {
    vector<User> users;
    ifstream fin(filename);

    if (!fin) {
        cout << "Error opening users file.\n";
        return users;
    }

    User u;
    while (fin >> u.username >> u.password >> u.role >> u.department) {
        users.push_back(u);
    }

    fin.close();
    return users;
}

int main() {
    vector<User> users = loadUsers("users.txt");

if (users.empty()) {
    cout << "No users loaded.\n";
    return 1;
}

    vector<PolicyRule> rules = {
    {"permit", "admin", "file", "read", false, ""},
    {"permit", "admin", "file", "write", false, ""},
    {"permit", "student", "notes", "read", true, "CSE"},
    {"deny", "student", "exam_paper", "read", false, ""},
    {"permit", "faculty", "marks", "write", true, "ECE"},
    {"deny", "guest", "file", "read", false, ""}
};

    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    User loggedInUser;
    if (!authenticate(users, username, password, loggedInUser)) {
        cout << "Authentication failed.\n";
        return 1;
    }

    cout << "\nLogin successful.\n";
    cout << "Role: " << loggedInUser.role << endl;
    cout << "Department: " << loggedInUser.department << endl;

    string resourceType, action;
    cout << "\nEnter resource type: ";
    cin >> resourceType;
    cout << "Enter action: ";
    cin >> action;

    string decision = enforcePolicy(loggedInUser, rules, resourceType, action);

    cout << "\nAccess decision: " << decision << endl;

    return 0;
}