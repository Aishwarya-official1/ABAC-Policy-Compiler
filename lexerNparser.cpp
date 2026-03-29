/*Companies use policy engines to decide:
Who can access what
Under which conditions
Example:
Banking apps
Hospital systems
College portals
Cloud dashboards
cloud security platforms*/

#include <iostream>
#include <vector>
#include <cctype>
using namespace std;

// ---------------- TOKEN DEFINITION ----------------
enum TokenType {
    KEYWORD, IDENTIFIER, STRING, OPERATOR,
    DOT, SEMICOLON
};

struct Token {
    TokenType type;
    string value;
};

// Convert token type to string for printing
string tokenTypeToString(TokenType type) {
    switch(type) {
        case KEYWORD: return "KEYWORD";
        case IDENTIFIER: return "IDENTIFIER";
        case STRING: return "STRING";
        case OPERATOR: return "OPERATOR";
        case DOT: return "DOT";
        case SEMICOLON: return "SEMICOLON";
        default: return "UNKNOWN";
    }
}

// ---------------- LEXER ----------------
vector<Token> lexer(string input) {
    vector<Token> tokens;

    for (int i = 0; i < input.length(); i++) {
        //Ignore spaces
        if (isspace(input[i]))
            continue;

        // identifies Keywords and identifiers
        if (isalpha(input[i])) {
            string word;
            while (isalnum(input[i]) || input[i] == '_') {
                word += input[i++];
            }
            i--;

            if (word == "permit" || word == "deny" ||
                word == "if" || word == "and")
                tokens.push_back({KEYWORD, word});
            else
                tokens.push_back({IDENTIFIER, word});
        }

        // String literals
        else if (input[i] == '"') {
            string str = "\"";
            i++;
            while (i < input.length() && input[i] != '"') {
                str += input[i++];
            }
            if (i >= input.length()) {
                cout << "Lexical Error: Unterminated string\n";//If loop ended because input finished (no closing quote found),
                exit(0);
            }
            str += "\"";
            tokens.push_back({STRING, str});
        }

        // Operator ==
        else if (input[i] == '=' && i + 1 < input.length() && input[i+1] == '=') {//If = is the last character, checking input[i+1] would crash the program.
            tokens.push_back({OPERATOR, "=="});
            i++;
        }

        else if (input[i] == '.')
            tokens.push_back({DOT, "."});
        else if (input[i] == ';')
            tokens.push_back({SEMICOLON, ";"});
        else {
            cout << "Lexical Error: Invalid character '" << input[i] << "'\n";
            exit(0);//immediately stops execution
        }
    }

    return tokens;
}

// ---------------- PARSER ----------------
vector<Token> tokens;
int pos = 0;//pos is pointer to current token.

void syntaxError(string msg) {
    cout << "Syntax Error: " << msg << endl;
    exit(0);
}

void parseAttribute() {
    /*What it does step-by-step:

Check first IDENTIFIER

Check DOT

Check second IDENTIFIER

If all correct → attribute is valid.*/
    if (tokens[pos].type == IDENTIFIER)
        pos++;
    else syntaxError("Expected identifier");

    if (pos < tokens.size() && tokens[pos].type == DOT)
        pos++;
    else syntaxError("Expected '.'");

    if (pos < tokens.size() && tokens[pos].type == IDENTIFIER)
        pos++;
    else syntaxError("Expected identifier after '.'");
}

void parseExpression() {
    /*Steps:

First parse attribute

Check operator ==

Check STRING

If all correct → expression valid.*/
    parseAttribute();

    if (pos < tokens.size() && tokens[pos].type == OPERATOR)
        pos++;
    else syntaxError("Expected '==' operator");

    if (pos < tokens.size() && tokens[pos].type == STRING)
        pos++;
    else syntaxError("Expected string literal");
}

void parseCondition() {
    /*What this does:

Parse first expression

While next token is "and"

Move forward

Parse another expression*/

    
    parseExpression();

    while (pos < tokens.size() &&
           tokens[pos].type == KEYWORD &&
           tokens[pos].value == "and") {
        pos++;
        parseExpression();
    }
}

void parsePolicy() {
    // First keyword: permit/deny
    if (tokens[pos].type == KEYWORD &&
        (tokens[pos].value == "permit" || tokens[pos].value == "deny"))
        pos++;
    else syntaxError("Expected 'permit' or 'deny'");

    // Second keyword: if
    if (tokens[pos].type == KEYWORD && tokens[pos].value == "if")
        pos++;
    else syntaxError("Expected 'if' after decision");

    // Parse conditions
    parseCondition();

    // Semicolon at end
    if (pos < tokens.size() && tokens[pos].type == SEMICOLON)
        pos++;
    else syntaxError("Expected ';' at end");

    if (pos != tokens.size())
        syntaxError("Extra tokens after ';'");

    cout << "Policy Accepted\n";
}

// ---------------- MAIN ----------------
int main() {
    string input;

    cout << "Enter policy: ";
    getline(cin, input);

    tokens = lexer(input);

    cout << "\nTokens:\n";
    for (auto t : tokens)
        cout << t.value << "  -->  " << tokenTypeToString(t.type) << endl;

    cout << "\nParsing...\n";
    parsePolicy();

    return 0;
}
