#include <iostream>
#include <vector>
#include <cctype>
using namespace std;

enum TokenType {
    KEYWORD, IDENTIFIER, STRING, OPERATOR,
    DOT, SEMICOLON
};

struct Token {
    TokenType type;
    string value;
};

// Helper to convert token type to string
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

vector<Token> lexer(string input) {
    vector<Token> tokens;

    for (int i = 0; i < input.length(); i++) {

        if (isspace(input[i]))
            continue;//ignore

        // Keywords and Identifiers
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
            while (input[i] != '"') {
                str += input[i++];
            }
            str += "\"";
            tokens.push_back({STRING, str});
        }

        // Operator ==
        else if (input[i] == '=' && input[i + 1] == '=') {
            tokens.push_back({OPERATOR, "=="});
            i++;
        }

        else if (input[i] == '.')
            tokens.push_back({DOT, "."});

        else if (input[i] == ';')
            tokens.push_back({SEMICOLON, ";"});
    }

    return tokens;
}

int main() {
    string input;

    cout << "Enter policy: ";
    getline(cin, input);//reads an entire line of text from the keyboard and stores it in the variable input.

    vector<Token> tokens = lexer(input);

    cout << "\nTokens:\n";

    for (auto t : tokens) {
        cout << t.value << "  -->  " << tokenTypeToString(t.type) << endl;
    }

    return 0;
}
