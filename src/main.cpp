#include "TArithmeticExpression.h"
#include <iostream>
#include <string>
#include <map>
#include <iomanip> 
#include <locale>

int main() {
    std::setlocale(LC_ALL, "C");
    std::locale::global(std::locale("C"));

    std::cout << "Enter expression" << std::endl;
    std::cout << "\nType 'exit' to exit" << std::endl << std::endl;

    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        try {
            TArithmeticExpression expr(input);
            std::cout << "Reverse polish notation " << expr.GetPostfix() << std::endl;

            auto operands = expr.GetOperands();

            if (!operands.empty()) {
                std::cout << "\nVariables: ";
                for (const auto& op : operands) {
                    std::cout << op << " ";
                }
                std::cout << std::endl;

                std::map<std::string, double> values;
                std::cout << "\nEnter variable values:" << std::endl;

                for (const auto& op : operands) {
                    bool valid = false;
                    while (!valid) {
                        std::cout << op << " = ";
                        std::string valStr;
                        std::getline(std::cin, valStr);

                        try {
                            double val = std::stod(valStr);
                            values[op] = val;
                            valid = true;
                        }
                        catch (...) {
                            std::cout << "Incorrect input. Please enter a number." << std::endl;
                        }
                    }
                }

                double result = expr.Calculate(values);
                std::cout << "\nResult: " << std::fixed << std::setprecision(6) << result << std::endl;
            }
            else {
                double result = expr.Calculate();
                std::cout << "\nResult: " << std::fixed << std::setprecision(6) << result << std::endl;
            }

        }
        catch (const std::exception& e) {
            std::cerr << "error: " << e.what() << std::endl;
        }

        std::cout << std::endl;
    }

    std::cout << "Program completed." << std::endl;
    return 0;
}