#include "TArithmeticExpression.h"
#include "TDynamicStack.h"
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

TArithmeticExpression::TArithmeticExpression(string infx)
    : infix(infx) {
    priority = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2} };
    Parse();
    ToPostfix();
}

void TArithmeticExpression::Parse() {
    lexems.clear();

    for (size_t i = 0; i < infix.length(); i++) {
        char c = infix[i];
        if (isspace(static_cast<unsigned char>(c))) {
            continue;
        }

        if (isalpha(static_cast<unsigned char>(c))) {
            string identifier;
            while (i < infix.length() && isalpha(static_cast<unsigned char>(infix[i]))) {
                identifier += infix[i];
                i++;
            }
            i--; 

            if (identifier == "sin") {
                lexems.push_back(Token(Token::FUNCTION_SIN, "sin"));
            }
            else if (identifier == "cos") {
                lexems.push_back(Token(Token::FUNCTION_COS, "cos"));
            }
            else if (identifier == "pi") {
                Token piToken(Token::NUMBER, "pi");
                piToken.numValue = 3.14159265358979323846;
                lexems.push_back(piToken);
            }
            else if (identifier.length() == 1) {
                lexems.push_back(Token(Token::OPERAND, identifier));
                operands[identifier] = 0.0;
            }
            else {
                throw invalid_argument("Unknown function or variable: " + identifier);
            }
        }
        else if (isdigit(static_cast<unsigned char>(c)) || c == '.') {
            string number;
            bool hasDecimal = (c == '.');

            while (i < infix.length() &&
                (isdigit(static_cast<unsigned char>(infix[i])) || infix[i] == '.')) {
                if (infix[i] == '.') {
                    if (hasDecimal) {
                        throw invalid_argument("Invalid number: multiple decimal points");
                    }
                    hasDecimal = true;
                }
                number += infix[i];
                i++;
            }
            i--;

            Token numToken(Token::NUMBER, number);
            try {
                numToken.numValue = std::stod(number);
            }
            catch (...) {
                throw invalid_argument("Invalid number format: " + number);
            }
            lexems.push_back(numToken);
        }
        else if (c == '(') {
            lexems.push_back(Token(Token::LEFT_PAREN, "("));
        }
        else if (c == ')') {
            lexems.push_back(Token(Token::RIGHT_PAREN, ")"));
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            lexems.push_back(Token(Token::OPERATOR, string(1, c)));
        }
        else {
            throw invalid_argument("Invalid character in expression: " + string(1, c));
        }
    }
}

void TArithmeticExpression::ToPostfix() {
    TDynamicStack<Token> st(100);
    postfix = "";

    for (const Token& token : lexems) {
        switch (token.type) {
        case Token::OPERAND:
        case Token::NUMBER:
            postfix += token.value + " ";
            break;

        case Token::LEFT_PAREN:
            st.Push(token);
            break;

        case Token::RIGHT_PAREN: {
            while (!st.IsEmpty() && st.Top().type != Token::LEFT_PAREN) {
                postfix += st.Pop().value + " ";
            }
            if (st.IsEmpty()) {
                throw runtime_error("Mismatched parentheses");
            }
            st.Pop();

            if (!st.IsEmpty() &&
                (st.Top().type == Token::FUNCTION_SIN ||
                    st.Top().type == Token::FUNCTION_COS)) {
                postfix += st.Pop().value + " ";
            }
            break;
        }

        case Token::FUNCTION_SIN:
        case Token::FUNCTION_COS:
            st.Push(token);
            break;

        case Token::OPERATOR:
            while (!st.IsEmpty() &&
                st.Top().type == Token::OPERATOR &&
                priority[token.value[0]] <= priority[st.Top().value[0]]) {
                postfix += st.Pop().value + " ";
            }
            st.Push(token);
            break;
        }
    }

    while (!st.IsEmpty()) {
        if (st.Top().type == Token::LEFT_PAREN) {
            throw runtime_error("Mismatched parentheses");
        }
        postfix += st.Pop().value + " ";
    }

    if (!postfix.empty() && postfix.back() == ' ') {
        postfix.pop_back();
    }
}

vector<string> TArithmeticExpression::GetOperands() const {
    vector<string> op;
    for (const auto& item : operands) {
        op.push_back(item.first);
    }
    return op;
}

double TArithmeticExpression::Calculate(const map<string, double>& values) {
    for (const auto& val : values) {
        operands[val.first] = val.second;
    }

    return Calculate();
}

double TArithmeticExpression::Calculate() {
    TDynamicStack<double> st(100);

    istringstream iss(postfix);
    string token;

    while (iss >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (st.size() < 2) {
                throw runtime_error("Invalid expression: not enough operands");
            }

            double right = st.Pop();
            double left = st.Pop();

            if (token == "+") {
                st.Push(left + right);
            }
            else if (token == "-") {
                st.Push(left - right);
            }
            else if (token == "*") {
                st.Push(left * right);
            }
            else if (token == "/") {
                if (right == 0.0) {
                    throw runtime_error("Division by zero");
                }
                st.Push(left / right);
            }
        }
        else if (token == "sin") {
            if (st.IsEmpty()) {
                throw runtime_error("Invalid expression: no argument for sin");
            }
            double arg = st.Pop();
            st.Push(sin(arg));
        }
        else if (token == "cos") {
            if (st.IsEmpty()) {
                throw runtime_error("Invalid expression: no argument for cos");
            }
            double arg = st.Pop();
            st.Push(cos(arg));
        }
        else if (isdigit(token[0]) || (token[0] == '.' && token.length() > 1) ||
            (token[0] == '-' && token.length() > 1 && isdigit(token[1]))) {
            try {
                double num = stod(token);
                st.Push(num);
            }
            catch (...) {
                throw runtime_error("Invalid number format: " + token);
            }
        }
        else if (token == "pi") {
            st.Push(3.14159265358979323846);
        }
        else if (token.length() == 1 && isalpha(token[0])) {
            auto it = operands.find(token);
            if (it != operands.end()) {
                st.Push(it->second);
            }
            else {
                throw runtime_error("Undefined variable: " + token);
            }
        }
        else {
            throw runtime_error("Invalid token in expression: " + token);
        }
    }

    if (st.size() != 1) {
        throw runtime_error("Invalid expression");
    }

    return st.Pop();
}