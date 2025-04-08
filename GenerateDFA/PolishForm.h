#pragma once
#include <string>
#include <stack>
using namespace std;

class PolishForm {
public:
    //Polish sausage creator
    string convertToPostfix(const string& regex) {
        string explicitRegex = addExplicitConcatenation(regex);
        return regexToPostfix(explicitRegex);
    }

private:

    string addExplicitConcatenation(const string& regex) {
        string result = "";
        for (size_t i = 0; i < regex.length(); i++) {
            result += regex[i];
            if (i + 1 < regex.length() &&
                ((isalnum(regex[i]) || regex[i] == '*' || regex[i] == ')') &&
                    (isalnum(regex[i + 1]) || regex[i + 1] == '('))) {
                result += '.';
            }
        }
        return result;
    }

    // Get precedence of operators
    int precedence(char op) {
        if (op == '*') return 3; 
        if (op == '.') return 2; 
        if (op == '|') return 1; 
        return 0; 
    }


    string regexToPostfix(const string& regex) {
        stack<char> operatorStack;
        string output = "";

        for (char character : regex) {
            if (isalnum(character)) {
                output += character; 
            }
            else if (character == '(') {
                operatorStack.push(character);
            }
            else if (character == ')') {
                while (!operatorStack.empty() && operatorStack.top() != '(') {
                    output += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.pop(); // Remove '('
            }
            else { 
                while (!operatorStack.empty() && precedence(operatorStack.top()) >= precedence(character)) {
                    output += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(character);
            }
        }

        // Pop remaining operators
        while (!operatorStack.empty()) {
            output += operatorStack.top();
            operatorStack.pop();
        }

        return output;
    }

};