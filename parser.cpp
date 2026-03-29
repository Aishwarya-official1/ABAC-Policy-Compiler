#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    STRING,
    OPERATOR,
    DOT,
    SEMICOLON
};

struct Token {
    TokenType type;
    string value;
};

vector<Token> tokens;
int pos = 0;

// ---------------- LEXER ----------------

vector<Token> lexer(string input) {
    vector<Token> result;

    for (int i = 0; i < input.length(); i++) {

        if (isspace(input[i]))
            continue;

        // Keywords and Identifiers
        if (isalpha(input[i])) {
            string word;

            while (isalnum(input[i]) || input[i] == '_') {
                word += input[i++];
            }
            i--;

            if (word == "permit" || word == "deny" ||
                word == "if" || word == "and")
                result.push_back({KEYWORD, word});
            else
                result.push_back({IDENTIFIER, word});
        }

        // String literal
        else if (input[i] == '"') {
            string str = "\"";
            i++;

            while (i < input.length() && input[i] != '"') {
                str += input[i++];
            }

            if (i >= input.length()) {
                cout << "Lexical Error: Unterminated string\n";
                exit(0);
            }

            str += "\"";
            result.push_back({STRING, str});
        }

        // Operator ==
        else if (input[i] == '=' && i + 1 < input.length() && input[i + 1] == '=') {
            result.push_back({OPERATOR, "=="});
            i++;
        }

        else if (input[i] == '.')
            result.push_back({DOT, "."});

        else if (input[i] == ';')
            result.push_back({SEMICOLON, ";"});

        else {
            cout << "Lexical Error: Invalid character '" << input[i] << "'\n";
            exit(0);
        }
    }

    return result;
}

// ---------------- PARSER ----------------

void syntaxError(string message) {
    cout << "Syntax Error: " << message << endl;
    exit(0);
}

void match(TokenType expected) {
    if (pos < tokens.size() && tokens[pos].type == expected)
        pos++;
    else
        syntaxError("Unexpected token");
}

void parseAttribute() {

    /*Expected:
IDENTIFIER . IDENTIFIER
Example:
subject.role
object.type
action.name*/
    if (tokens[pos].type == IDENTIFIER) {
        pos++;
    } else {
        syntaxError("Expected identifier");
    }

    if (pos < tokens.size() && tokens[pos].type == DOT) {
        pos++;
    } else {
        syntaxError("Expected '.'");
    }

    if (pos < tokens.size() && tokens[pos].type == IDENTIFIER) {
        pos++;
    } else {
        syntaxError("Expected identifier after '.'");
    }
}

void parseExpression() {


    parseAttribute();//does like:subject.role


    if (pos < tokens.size() && tokens[pos].type == OPERATOR) {
        pos++;
    } else {
        syntaxError("Expected '==' operator");
    }

    if (pos < tokens.size() && tokens[pos].type == STRING) {
        pos++;
    } else {
        syntaxError("Expected string literal");
    }
}

void parseCondition() {
    parseExpression();//Handles multiple expressions connected with and.

    while (pos < tokens.size() &&
           tokens[pos].type == KEYWORD &&
           tokens[pos].value == "and") {
        pos++;
        parseExpression();
    }
}

void parsePolicy() {

    // Decision
    if (pos < tokens.size() &&
        tokens[pos].type == KEYWORD &&
        (tokens[pos].value == "permit" || tokens[pos].value == "deny")) {
        pos++;
    } else {
        syntaxError("Expected 'permit' or 'deny'");
    }

    // 'if'
    if (pos < tokens.size() &&
        tokens[pos].type == KEYWORD &&
        tokens[pos].value == "if") {
        pos++;
    } else {
        syntaxError("Expected 'if' after decision");
    }

    parseCondition();

//Handles expressions like:
//subject.role == "admin" and object.type == "file"

    if (pos < tokens.size() && tokens[pos].type == SEMICOLON) {
        pos++;
    } else {
        syntaxError("Expected ';' at end");
    }

    if (pos != tokens.size()) {
        syntaxError("Extra tokens after ';'");
    }

    cout << "Policy Accepted\n";
}

// ---------------- MAIN ----------------

int main() {

    string input;

    cout << "Enter policy: ";
    getline(cin, input);

    tokens = lexer(input);

    parsePolicy();

    return 0;
}
