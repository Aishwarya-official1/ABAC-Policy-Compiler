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
    string effect;
    string subjectRole;
    string resourceType;
    string action;
    bool hasDepartmentCondition;
    string department;
    int priority;
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
                     const string& resourceType, const string& action,
                     string& explanation, string& matchedRuleStr) {

    int bestPriority = -1;
    string decision = "deny";
    explanation = "No matching rule. Default deny applied.\n";
    matchedRuleStr = "None";

    for (const auto& rule : rules) {

        if (rule.subjectRole == user.role &&
            rule.resourceType == resourceType &&
            rule.action == action) {

            if (rule.hasDepartmentCondition &&
                rule.department != user.department) {
                continue;
            }

            if (rule.priority > bestPriority) {
                bestPriority = rule.priority;
                decision = rule.effect;

                matchedRuleStr = "role=" + rule.subjectRole +
                                 ", resource=" + rule.resourceType +
                                 ", action=" + rule.action +
                                 ", priority=" + to_string(rule.priority);

                if (rule.hasDepartmentCondition) {
                    matchedRuleStr += ", department=" + rule.department;
                }
            }
        }
    }

    if (bestPriority != -1) {
        if (decision == "permit")
            explanation = "Access permitted based on highest priority rule.\n";
        else
            explanation = "Access denied based on highest priority rule.\n";
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

void logAccess(const User& user, const string& resourceType,
               const string& action, const string& decision) {

    ofstream fout("access_log.txt", ios::app);

    fout << "user=" << user.username
         << " role=" << user.role
         << " department=" << user.department
         << " resource=" << resourceType
         << " action=" << action
         << " result=" << decision << "\n";

    fout.close();
}

int main() {
    vector<User> users = loadUsers("users.txt");

    if (users.empty()) {
        cout << "No users loaded.\n";
        return 1;
    }

    vector<PolicyRule> rules = {
        {"permit", "admin", "file", "read", false, "", 5},
        {"permit", "admin", "file", "write", false, "", 5},
        {"permit", "student", "notes", "read", true, "CSE", 3},
        {"deny", "student", "exam_paper", "read", false, "", 10},
        {"permit", "faculty", "marks", "write", true, "ECE", 4},
        {"deny", "guest", "file", "read", false, "", 10}
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

    string explanation, matchedRule;

    string decision = enforcePolicy(loggedInUser, rules,
                                   resourceType, action,
                                   explanation, matchedRule);

    cout << "\nAccess decision: " << decision << endl;
    cout << "\nExplanation:\n" << explanation;
    cout << "Matched Rule: " << matchedRule << endl;

    logAccess(loggedInUser, resourceType, action, decision);

    return 0;
}