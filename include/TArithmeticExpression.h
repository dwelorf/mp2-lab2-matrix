#ifndef TARITHMETICEXPRESSION_H
#define TARITHMETICEXPRESSION_H

#include <string>
#include <vector>
#include <map>
#include "TDynamicStack.h"

using namespace std;

struct Token {
    enum Type { OPERAND, OPERATOR, LEFT_PAREN, RIGHT_PAREN, NUMBER, FUNCTION_SIN, FUNCTION_COS };
    Type type;
    string value;
    double numValue;    

    Token() : type(NUMBER), value(""), numValue(0) {}

    Token(Type t, const string& v = "") : type(t), value(v), numValue(0) {}
};

class TArithmeticExpression
{
    string infix;
    string postfix;
    vector<Token> lexems; 
    map<char, int> priority;
    map<string, double> operands; 

    void Parse();
    void ToPostfix();

public:
    TArithmeticExpression(string infx);

    string GetInfix() const
    {
        return infix;
    }

    string GetPostfix() const
    {
        return postfix;
    }

    vector<string> GetOperands() const;
    double Calculate(const map<string, double>& values);
    double Calculate();
};

#endif