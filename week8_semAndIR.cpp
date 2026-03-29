#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

/* ------------------ Structures ------------------ */

struct Condition {
    string category;   // subject / object / action
    string attribute;  // role, type, sensitivity
    string value;      // value in string form
};

struct Policy {
    string name;
    vector<Condition> conditions;
    string effect; // permit / deny
};

struct AttributeInfo {
    string category;
    string type; // string / int
};

struct IRRule {
    vector<Condition> subjectConds;
    vector<Condition> objectConds;
    string action;
    string effect;
};

/* ------------------ Symbol Table ------------------ */

unordered_map<string, AttributeInfo> symbolTable;

void initializeSymbolTable() {
    symbolTable["role"] = {"subject", "string"};
    symbolTable["department"] = {"subject", "string"};
    symbolTable["type"] = {"object", "string"};
    symbolTable["sensitivity"] = {"object", "int"};
    symbolTable["action"] = {"action", "string"};
}

/* ------------------ Semantic Analysis ------------------ */

bool isInteger(string value) {
    for(char c : value) {
        if(!isdigit(c)) return false;
    }
    return true;
}

bool semanticAnalysis(vector<Policy>& policies) {

    bool hasError = false;
    set<string> policyNames;

    for(int i = 0; i < policies.size(); i++) {

        Policy& p = policies[i];

        // Duplicate policy name
        if(policyNames.count(p.name)) {
            cout << "Semantic Error: Duplicate policy name '" 
                 << p.name << "'\n";
            hasError = true;
        }
        policyNames.insert(p.name);

        // Validate conditions
        for(auto& cond : p.conditions) {

            // Attribute existence
            if(symbolTable.find(cond.attribute) == symbolTable.end()) {
                cout << "Semantic Error: Undefined attribute '"
                     << cond.attribute << "' in policy "
                     << p.name << endl;
                hasError = true;
                continue;
            }

            // Type checking
            string expectedType = symbolTable[cond.attribute].type;

            if(expectedType == "int" && !isInteger(cond.value)) {
                cout << "Semantic Error: Type mismatch for attribute '"
                     << cond.attribute << "' in policy "
                     << p.name << endl;
                hasError = true;
            }
        }

        // Effect validation
        if(p.effect != "permit" && p.effect != "deny") {
            cout << "Semantic Error: Invalid effect in policy "
                 << p.name << endl;
            hasError = true;
        }
    }

    // Conflict detection
    for(int i = 0; i < policies.size(); i++) {
        for(int j = i+1; j < policies.size(); j++) {

            if(policies[i].conditions.size() ==
               policies[j].conditions.size()) {

                bool same = true;

                for(int k = 0; k < policies[i].conditions.size(); k++) {
                    if(policies[i].conditions[k].attribute !=
                       policies[j].conditions[k].attribute ||
                       policies[i].conditions[k].value !=
                       policies[j].conditions[k].value) {
                        same = false;
                        break;
                    }
                }

                if(same && policies[i].effect != policies[j].effect) {
                    cout << "Semantic Conflict: Policies "
                         << policies[i].name << " and "
                         << policies[j].name
                         << " contradict each other\n";
                    hasError = true;
                }
            }
        }
    }

    return !hasError;
}

/* ------------------ IR Generation ------------------ */

// Function to convert policies into Intermediate Representation (IR) rules
vector<IRRule> generateIR(vector<Policy>& policies) {

    vector<IRRule> irRules;   // This will store all the generated IR rules

    // Loop through each policy
    for(auto& p : policies) {

        IRRule rule;              // Create a new IR rule
        rule.effect = p.effect;   // Copy the effect (permit/deny) from the policy

        // Loop through all conditions of the policy
        for(auto& cond : p.conditions) {

            // Check which category the condition belongs to
            if(cond.category == "subject")
                // If subject condition, store it in subject conditions list
                rule.subjectConds.push_back(cond);

            else if(cond.category == "object")
                // If object condition, store it in object conditions list
                rule.objectConds.push_back(cond);

            else if(cond.category == "action")
                // If action condition, store only the action value
                rule.action = cond.value;
        }

        // After processing all conditions, add the rule to IR rules list
        irRules.push_back(rule);
    }

    // Return the generated IR rules
    return irRules;
}


// Function to print all IR rules in a readable format
void printIR(vector<IRRule>& irRules) {

    cout << "\n-- IR RULE TABLE --\n";

    // Loop through all IR rules
    for(int i = 0; i < irRules.size(); i++) {

        cout << "\nRule " << i+1 << ":\n";

        // Print subject conditions
        cout << "Subject Conditions:\n";
        for(auto& c : irRules[i].subjectConds)
            cout << "  " << c.attribute << " = " << c.value << endl;

        // Print object conditions
        cout << "Object Conditions:\n";
        for(auto& c : irRules[i].objectConds)
            cout << "  " << c.attribute << " = " << c.value << endl;

        // Print action and effect of the rule
        cout << "Action: " << irRules[i].action << endl;
        cout << "Effect: " << irRules[i].effect << endl;
    }
}


/* ------------------ MAIN ------------------ */

int main() {

    initializeSymbolTable();

    vector<Policy> policies;

    // Sample Policy 1
    Policy p1;
    p1.name = "P1";
    p1.conditions.push_back({"subject","role","admin"});
    p1.conditions.push_back({"object","type","file"});
    p1.conditions.push_back({"action","action","read"});
    p1.effect = "permit";

    policies.push_back(p1);

    // Sample Policy 2 (Conflict Example)
    // Policy p2;
    // p2.name = "P2";
    // p2.conditions.push_back({"subject","role","admin"});
    // p2.conditions.push_back({"object","type","file"});
    // p2.conditions.push_back({"action","action","read"});
    // p2.effect = "deny";

    // policies.push_back(p2);

    Policy p2;
p2.name = "P2";
p2.conditions.push_back({"subject","role","user"});
p2.conditions.push_back({"object","type","file"});
p2.conditions.push_back({"action","action","read"});
p2.effect = "deny";

policies.push_back(p2);


    cout << "Running Semantic Analysis...\n";

    if(semanticAnalysis(policies)) {
        cout << "\nSemantic Analysis Passed.\n";

        vector<IRRule> ir = generateIR(policies);
        printIR(ir);
    }
    else {
        cout << "\nSemantic Analysis Failed.\n";
    }

    return 0;
}
