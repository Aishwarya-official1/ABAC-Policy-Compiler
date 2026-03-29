#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct Condition {
    string category;
    string attribute;
    string value;
};

struct IRRule {
    vector<Condition> subjectConds;
    vector<Condition> objectConds;
    string action;
    string effect;
};

// Compare two condition vectors
bool sameConditions(const vector<Condition>& a, const vector<Condition>& b) {
    if (a.size() != b.size()) return false;

    for (int i = 0; i < a.size(); i++) {
        if (a[i].category != b[i].category ||
            a[i].attribute != b[i].attribute ||
            a[i].value != b[i].value) {
            return false;
        }
    }
    return true;
}

// Compare whether two IR rules have same conditions and action
bool sameRuleBase(const IRRule& r1, const IRRule& r2) {
    return sameConditions(r1.subjectConds, r2.subjectConds) &&
           sameConditions(r1.objectConds, r2.objectConds) &&
           r1.action == r2.action;
}

// Optimization + security analysis
void analyzeRules(const vector<IRRule>& rules) {
    ofstream out("security_analysis_report.txt");

    bool foundIssue = false;

    for (int i = 0; i < rules.size(); i++) {
        for (int j = i + 1; j < rules.size(); j++) {

            if (sameRuleBase(rules[i], rules[j])) {
                if (rules[i].effect == rules[j].effect) {
                    out << "Redundant rule detected: Rule " << i + 1
                        << " and Rule " << j + 1
                        << " are identical.\n";
                    foundIssue = true;
                } else {
                    out << "Conflict detected: Rule " << i + 1
                        << " and Rule " << j + 1
                        << " have same conditions but different effects.\n";
                    foundIssue = true;
                }
            }
        }
    }

    if (!foundIssue) {
        out << "No redundancy or conflict detected. Policy set is consistent.\n";
    }

    out.close();
}

// Remove redundant rules, keep first occurrence
vector<IRRule> removeRedundantRules(const vector<IRRule>& rules) {
    vector<IRRule> optimized;

    for (int i = 0; i < rules.size(); i++) {
        bool duplicate = false;

        for (int j = 0; j < optimized.size(); j++) {
            if (sameRuleBase(rules[i], optimized[j]) &&
                rules[i].effect == optimized[j].effect) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) {
            optimized.push_back(rules[i]);
        }
    }

    return optimized;
}

// Print optimized rules on screen
void printRules(const vector<IRRule>& rules) {
    for (int i = 0; i < rules.size(); i++) {
        cout << "Rule " << i + 1 << ": ";

        for (const auto& c : rules[i].subjectConds) {
            cout << c.category << "." << c.attribute << "=" << c.value << " ";
        }

        for (const auto& c : rules[i].objectConds) {
            cout << c.category << "." << c.attribute << "=" << c.value << " ";
        }

        cout << "action=" << rules[i].action
             << " effect=" << rules[i].effect << endl;
    }
}

int main() {
    vector<IRRule> rules;

    IRRule r1;
    r1.subjectConds.push_back({"subject", "role", "admin"});
    r1.objectConds.push_back({"object", "type", "file"});
    r1.action = "read";
    r1.effect = "permit";

    IRRule r2;
    r2.subjectConds.push_back({"subject", "role", "admin"});
    r2.objectConds.push_back({"object", "type", "file"});
    r2.action = "read";
    r2.effect = "permit";   // redundant

    IRRule r3;
    r3.subjectConds.push_back({"subject", "role", "admin"});
    r3.objectConds.push_back({"object", "type", "file"});
    r3.action = "read";
    r3.effect = "deny";     // conflict

    rules.push_back(r1);
    rules.push_back(r2);
    rules.push_back(r3);

    analyzeRules(rules);

    vector<IRRule> optimizedRules = removeRedundantRules(rules);

    cout << "Original number of rules: " << rules.size() << endl;
    cout << "Optimized number of rules: " << optimizedRules.size() << endl;
    cout << "\nOptimized Rules:\n";
    printRules(optimizedRules);

    cout << "\nSecurity analysis written to security_analysis_report.txt\n";

    return 0;
}


