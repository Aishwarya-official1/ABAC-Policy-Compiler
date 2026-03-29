#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<map>

using namespace std;



struct IRRule {
    string id;
    string effect;         // allow / deny
    string subjectRole;    // student, admin, faculty
    string resourceType;   // notes, marks, exam_paper
    string action;         // read, write, delete
    bool hasCondition;
    string conditionKey;   // department
    string conditionValue; // CSE
};




bool validateRules(const vector<IRRule>& rules);


//correctness checks
/*
Check:
1.effect is only allow or deny
2.subjectRole not empty
3.resourceType not empty
4.action not empty
5.if condition exists, key/value should not be empty*/

bool validateRules(const vector<IRRule>& rules) {
    for (const auto& rule : rules) {
        if (rule.effect != "allow" && rule.effect != "deny") {
            cout << "Error in " << rule.id << ": invalid effect\n";
            return false;
        }
        if (rule.subjectRole.empty()) {
            cout << "Error in " << rule.id << ": missing subject role\n";
            return false;
        }
        if (rule.resourceType.empty()) {
            cout << "Error in " << rule.id << ": missing resource type\n";
            return false;
        }
        if (rule.action.empty()) {
            cout << "Error in " << rule.id << ": missing action\n";
            return false;
        }
        if (rule.hasCondition && (rule.conditionKey.empty() || rule.conditionValue.empty())) {
            cout << "Error in " << rule.id << ": invalid condition\n";
            return false;
        }
    }
    return true;
}



//consistency check

string makeSignature(const IRRule& r) {
    string cond = r.hasCondition ? r.conditionKey + "=" + r.conditionValue : "no_condition";
    return r.subjectRole + "|" + r.resourceType + "|" + r.action + "|" + cond;
}


bool checkConsistency(const vector<IRRule>& rules) {
    map<string, string> seen; // signature -> effect

    for (const auto& rule : rules) {
        string sig = makeSignature(rule);

        if (seen.find(sig) != seen.end()) {
            if (seen[sig] == rule.effect) {
                cout << "Warning: duplicate rule found in " << rule.id << "\n";
            } else {
                cout << "Error: conflicting rule found in " << rule.id << "\n";
                return false;
            }
        } else {
            seen[sig] = rule.effect;
        }
    }
    return true;
}


void generatePseudoRules(const vector<IRRule>& rules, const string& filename);


void generatePseudoRules(const vector<IRRule>& rules, const string& filename) {
    ofstream fout(filename);
if (!fout) {
    cout << "Error opening file: " << filename << endl;
    return;
}
    for (const auto& rule : rules) {
        fout << "IF subject.role == \"" << rule.subjectRole << "\" "
             << "AND resource.type == \"" << rule.resourceType << "\" "
             << "AND action == \"" << rule.action << "\"";

        if (rule.hasCondition) {
            fout << " AND " << rule.conditionKey << " == \"" << rule.conditionValue << "\"";
        }

        fout << "\nTHEN " << (rule.effect == "allow" ? "ALLOW" : "DENY") << "\n\n";
    }

    fout.close();
}


void generateTable(const vector<IRRule>& rules, const string& filename) {
    ofstream fout(filename);
if (!fout) {
    cout << "Error opening file: " << filename << endl;
    return;
}

    fout << "RuleID\tEffect\tSubject\tResource\tAction\tCondition\n";

    for (const auto& rule : rules) {
        string cond = rule.hasCondition ? rule.conditionKey + "=" + rule.conditionValue : "None";
        fout << rule.id << "\t"
             << rule.effect << "\t"
             << rule.subjectRole << "\t"
             << rule.resourceType << "\t"
             << rule.action << "\t"
             << cond << "\n";
    }

    fout.close();
}

void generateJSON(const vector<IRRule>& rules, const string& filename) {
    ofstream fout(filename);
if (!fout) {
    cout << "Error opening file: " << filename << endl;
    return;
}

    fout << "{\n  \"policies\": [\n";

    for (size_t i = 0; i < rules.size(); i++) {
        const auto& r = rules[i];

        fout << "    {\n";
        fout << "      \"id\": \"" << r.id << "\",\n";
        fout << "      \"effect\": \"" << r.effect << "\",\n";
        fout << "      \"subject\": { \"role\": \"" << r.subjectRole << "\" },\n";
        fout << "      \"resource\": { \"type\": \"" << r.resourceType << "\" },\n";
        fout << "      \"action\": \"" << r.action << "\"";

        if (r.hasCondition) {
            fout << ",\n";
            fout << "      \"condition\": { \"" << r.conditionKey << "\": \"" << r.conditionValue << "\" }\n";
        } else {
            fout << "\n";
        }

        fout << "    }";
        if (i != rules.size() - 1) fout << ",";
        fout << "\n";
    }

    fout << "  ]\n}\n";

    fout.close();
}

int main() {
    vector<IRRule> rules = {
        {"R1", "allow", "student", "notes", "read", true, "department", "CSE"},
        {"R2", "deny", "student", "exam_paper", "read", false, "", ""},
        {"R3", "allow", "admin", "marks", "write", false, "", ""}
    };

    if (!validateRules(rules)) {
        cout << "Compilation failed: invalid rules\n";
        return 1;
    }

    if (!checkConsistency(rules)) {
        cout << "Compilation failed: inconsistent rules\n";
        return 1;
    }

    generatePseudoRules(rules, "compiled_policy_rules.txt");
    generateTable(rules, "compiled_policy_table.txt");
    generateJSON(rules, "compiled_policy.json");

    cout << "Policy compilation successful.\n";
    cout << "Generated files:\n";
    cout << "1. compiled_policy_rules.txt\n";
    cout << "2. compiled_policy_table.txt\n";
    cout << "3. compiled_policy.json\n";

    return 0;
}