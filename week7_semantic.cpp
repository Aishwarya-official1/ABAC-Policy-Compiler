/*Subject Attributes → describe who is requesting

role → admin, employee

department → HR, finance

Object Attributes → describe what is being accessed

type → file, database

sensitivity → 1, 2, 3

Action Attributes → describe what action is requested

action → read, write, delete

Environment Attributes → describe context (not used in this code)

time → 9:00AM, 5:00PM

location → office, remote*/


/*symbol table
It defines:

✔ What attributes exist
✔ Which category they belong to
✔ What data type they expect*/



#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
using namespace std;

/* ---------------- STRUCTURES ---------------- */

struct Condition {
    string category;   // subject / object / action
    string attribute;  // role, type, sensitivity
    string value;      // value in string format
};

struct Policy {
    string name;
    vector<Condition> conditions;
    string effect;     // permit / deny
};

struct AttributeInfo {
    string category;
    string type;       // string / int
};

/* ---------------- SYMBOL TABLE ---------------- */

unordered_map<string, AttributeInfo> symbolTable;
void initializeSymbolTable() {//Policies are only allowed to use these attributes.
    symbolTable["role"] = {"subject", "string"};
    symbolTable["department"] = {"subject", "string"};
    symbolTable["type"] = {"object", "string"};
    symbolTable["sensitivity"] = {"object", "int"};
    symbolTable["action"] = {"action", "string"};
}

/* ---------------- HELPER FUNCTION ---------------- */

bool isInteger(string value) {
    for(char c : value) {
        if(!isdigit(c)) return false;
        
    }
    return true;
}

/* ---------------- SEMANTIC ANALYSIS ---------------- */

/*During semantic analysis,
 it is used to validate attribute existence and ensure type correctness.”*/

bool semanticAnalysis(vector<Policy>& policies) {

    bool hasError = false;//remembers if any semantic error occurred.
    set<string> policyNames;//stores policy names to check for duplicates.

    for(int i = 0; i < policies.size(); i++) {
        /*Loop over each policy.
Policy& p → reference to current policy for easy access.*/

        Policy& p = policies[i];

        // 1. Duplicate Policy Name Check
        if(policyNames.count(p.name)) {
            /*If policy name already exists → duplicate error.*/
            cout << "Semantic Error: Duplicate policy name '"
                 << p.name << "'\n";
            hasError = true;
        }
        policyNames.insert(p.name);//Add name to policyNames for future checks.

        // 2. Validate Each Condition
        for(auto& cond : p.conditions) {//Loop over all conditions of the policy.

            // Attribute existence check
            if(symbolTable.find(cond.attribute) == symbolTable.end()) {//In C++, if .find() does not find the key, it returns symbolTable.end().

                cout << "Semantic Error: Undefined attribute '"
                     << cond.attribute << "' in policy "
                     << p.name << endl;
                hasError = true;
                continue;//Skips the rest of the loop for this condition.
            }

            // Type checking
            string expectedType = symbolTable[cond.attribute].type;
            /*Looks up expected type from symbol table.
If expected type is int → checks if value is numeric using isInteger().
If not → type mismatch error.*/

            if(expectedType == "int" && !isInteger(cond.value)) {
                cout << "Semantic Error: Type mismatch for attribute '"
                     << cond.attribute << "' in policy "
                     << p.name << endl;
                hasError = true;
            }
        }

        // 3. Effect Validation
        /*Ensures policy effect is either "permit" or "deny".
Anything else → invalid.*/
        if(p.effect != "permit" && p.effect != "deny") {
            cout << "Semantic Error: Invalid effect in policy "
                 << p.name << endl;
            hasError = true;
        }
    }

    // 4. Conflict Detection
    for(int i = 0; i < policies.size(); i++) {
        /*Compare each pair of policies to detect conflicts.*/
        /*
Checks number of conditions is same.
Compares attribute names and values.
If any mismatch → same = false.
*/
        for(int j = i + 1; j < policies.size(); j++) {

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

    if(!hasError)
        cout << "Semantic Analysis Passed Successfully.\n";
    else
        cout << "Semantic Analysis Failed.\n";

    return !hasError;
}

/* ---------------- MAIN ---------------- */

int main() {

    initializeSymbolTable();

    vector<Policy> policies;

    // Example Policy 1 (Valid)
    // Policy p1;
    // p1.name = "P1";
    // //above creates a policy named P1.
    // p1.conditions.push_back({"subject","role","admin"});//subject.role == "admin"
    // p1.conditions.push_back({"object","type","file"});//object.type == "file"
    // p1.conditions.push_back({"action","action","read"});//action == "read"

    // p1.effect = "permit";

    // policies.push_back(p1);//Adds P1 to policy list.

    /*

full Policy P1 means:
If role = admin
AND type = file
AND action = read
Then permit*/

    // Example Policy 2 (Conflict)
    // Policy p2;
    // p2.name = "P2";
    // //Policy name is P2.
    // p2.conditions.push_back({"subject","role","admin"});
    // p2.conditions.push_back({"object","type","file"});
    // p2.conditions.push_back({"action","action","read"});
    // p2.effect = "deny";
    /*If role = admin
AND type = file
AND action = read
Then deny
*/


Policy p1;
p1.name = "P1";
p1.conditions.push_back({"object","sensitivity","high"});
p1.effect = "permit";

policies.push_back(p1);

    //policies.push_back(p2);

    /*
I gave the system two policies:

Policy P1:
Permit admin to read file.

Policy P2:
Deny admin to read file.

Same condition.
Different effect.
*/

    semanticAnalysis(policies);
    /*Check attribute existence → OK

Check types → OK

Check duplicate names → OK

Check conflict → not ok

Because P1 and P2 contradict.*/

    return 0;
}
