#include <../gtest/gtest.h>
#include "TArithmeticExpression.h"
#include <cmath>
#include <map>

TEST(TArithmeticExpressionTest, TestParsing) {
    std::string s = "3.14";
    double d = std::stod(s);
    std::cout << "DEBUG: String '" << s << "' parsed as " << d << std::endl;
    EXPECT_NEAR(d, 3.14, 0.0001);
}

TEST(TArithmeticExpressionTest, ConstructorAndBasicGetters) {
    TArithmeticExpression expr("2+3");
    EXPECT_EQ(expr.GetInfix(), "2+3");
    EXPECT_EQ(expr.GetPostfix(), "2 3 +");

    EXPECT_EQ(expr.GetOperands().size(), 0);
}

TEST(TArithmeticExpressionTest, SimpleArithmetic) {
    TArithmeticExpression expr1("2+3");
    EXPECT_NEAR(expr1.Calculate(), 5.0, 0.0001);

    TArithmeticExpression expr2("10-4");
    EXPECT_NEAR(expr2.Calculate(), 6.0, 0.0001);

    TArithmeticExpression expr3("3*4");
    EXPECT_NEAR(expr3.Calculate(), 12.0, 0.0001);

    TArithmeticExpression expr4("15/3");
    EXPECT_NEAR(expr4.Calculate(), 5.0, 0.0001);
}

TEST(TArithmeticExpressionTest, FloatingPointNumbers) {
    TArithmeticExpression expr1("3.14+2.86");
    EXPECT_NEAR(expr1.Calculate(), 6.0, 0.0001);

    TArithmeticExpression expr2("5.5*2");
    EXPECT_NEAR(expr2.Calculate(), 11.0, 0.0001);

    TArithmeticExpression expr3("10.0/4.0");
    EXPECT_NEAR(expr3.Calculate(), 2.5, 0.0001);
}

TEST(TArithmeticExpressionTest, ComplexExpressions) {
    TArithmeticExpression expr1("2+3*4");
    EXPECT_NEAR(expr1.Calculate(), 14.0, 0.0001);

    TArithmeticExpression expr2("(2+3)*4");
    EXPECT_NEAR(expr2.Calculate(), 20.0, 0.0001);

    TArithmeticExpression expr3("(3.5+4.5)*2/4-1");
    EXPECT_NEAR(expr3.Calculate(), 3.0, 0.0001);
}

TEST(TArithmeticExpressionTest, Variables) {
    TArithmeticExpression expr1("a+b");
    EXPECT_EQ(expr1.GetOperands().size(), 2);
    EXPECT_EQ(expr1.GetPostfix(), "a b +");

    TArithmeticExpression expr2("a+b*c");
    EXPECT_EQ(expr2.GetPostfix(), "a b c * +");

    TArithmeticExpression expr3("(a+b)*c");
    EXPECT_EQ(expr3.GetPostfix(), "a b + c *");
}

TEST(TArithmeticExpressionTest, CalculateWithVariables) {
    TArithmeticExpression expr1("a+b");
    std::map<std::string, double> values1 = { {"a", 2.5}, {"b", 3.5} };
    EXPECT_NEAR(expr1.Calculate(values1), 6.0, 0.0001);

    TArithmeticExpression expr2("a+b*c");
    std::map<std::string, double> values2 = { {"a", 2}, {"b", 3}, {"c", 4} };
    EXPECT_NEAR(expr2.Calculate(values2), 14.0, 0.0001);

    TArithmeticExpression expr3("(a+b)*c");
    std::map<std::string, double> values3 = { {"a", 2}, {"b", 3}, {"c", 4} };
    EXPECT_NEAR(expr3.Calculate(values3), 20.0, 0.0001);

    TArithmeticExpression expr4("2*x+3*y");
    std::map<std::string, double> values4 = { {"x", 1.5}, {"y", 2.5} };
    EXPECT_NEAR(expr4.Calculate(values4), 10.5, 0.0001);
}

TEST(TArithmeticExpressionTest, DivisionByZero) {
    TArithmeticExpression expr1("5/0");
    EXPECT_THROW(expr1.Calculate(), std::runtime_error);

    TArithmeticExpression expr2("a/b");
    std::map<std::string, double> values = { {"a", 10}, {"b", 0} };
    EXPECT_THROW(expr2.Calculate(values), std::runtime_error);
}

TEST(TArithmeticExpressionTest, InvalidExpressions) {
    EXPECT_THROW(TArithmeticExpression("2$3"), std::invalid_argument);

    EXPECT_THROW(TArithmeticExpression("2..5+3"), std::invalid_argument);

    EXPECT_THROW(TArithmeticExpression("(2+3"), std::runtime_error);
    EXPECT_THROW(TArithmeticExpression("2+3)"), std::runtime_error);
}

TEST(TArithmeticExpressionTest, MultipleVariableUsage) {
    TArithmeticExpression expr("x*x + 2*x + 1");
    EXPECT_EQ(expr.GetOperands().size(), 1);

    std::map<std::string, double> values = { {"x", 3} };
    EXPECT_NEAR(expr.Calculate(values), 16.0, 0.0001);
}

TEST(TArithmeticExpressionTest, WhitespaceHandling) {
    TArithmeticExpression expr(" 2 + 3 * 4 ");
    EXPECT_EQ(expr.GetInfix(), " 2 + 3 * 4 ");
    EXPECT_EQ(expr.GetPostfix(), "2 3 4 * +");
    EXPECT_NEAR(expr.Calculate(), 14.0, 0.0001);

    TArithmeticExpression expr2("\t(2\t+\t3)\t*\t4\n");
    EXPECT_NEAR(expr2.Calculate(), 20.0, 0.0001);
}

TEST(TArithmeticExpressionTest, GetOperands) {
    TArithmeticExpression expr1("2+3");
    auto ops1 = expr1.GetOperands();
    EXPECT_TRUE(ops1.empty());

    TArithmeticExpression expr2("x+5");
    auto ops2 = expr2.GetOperands();
    EXPECT_EQ(ops2.size(), 1);
    EXPECT_EQ(ops2[0], "x");

    TArithmeticExpression expr3("a+b*c-d/e");
    auto ops3 = expr3.GetOperands();
    EXPECT_EQ(ops3.size(), 5);

    std::vector<std::string> expected = { "a", "b", "c", "d", "e" };
    for (const auto& op : expected) {
        EXPECT_NE(std::find(ops3.begin(), ops3.end(), op), ops3.end());
    }
}

TEST(TArithmeticExpressionTest, MultipleCalculateCalls) {
    TArithmeticExpression expr("a+b");

    std::map<std::string, double> values1 = { {"a", 1}, {"b", 2} };
    EXPECT_NEAR(expr.Calculate(values1), 3.0, 0.0001);

    std::map<std::string, double> values2 = { {"a", 5}, {"b", 7} };
    EXPECT_NEAR(expr.Calculate(values2), 12.0, 0.0001);

    EXPECT_NEAR(expr.Calculate(), 12.0, 0.0001);
}

TEST(TArithmeticExpressionTest, TrigonometricFunctions) {
    // Простые функции
    TArithmeticExpression expr1("sin(0)");
    EXPECT_NEAR(expr1.Calculate(), 0.0, 0.0001);

    TArithmeticExpression expr2("cos(0)");
    EXPECT_NEAR(expr2.Calculate(), 1.0, 0.0001);

    // Использование константы pi
    TArithmeticExpression expr3("sin(pi/2)");
    EXPECT_NEAR(expr3.Calculate(), 1.0, 0.0001);

    TArithmeticExpression expr4("cos(pi)");
    EXPECT_NEAR(expr4.Calculate(), -1.0, 0.0001);
}

TEST(TArithmeticExpressionTest, TrigonometricWithVariables) {
    TArithmeticExpression expr1("sin(x)");
    std::map<std::string, double> values1 = { {"x", 0} };
    EXPECT_NEAR(expr1.Calculate(values1), 0.0, 0.0001);

    TArithmeticExpression expr2("cos(y)");
    std::map<std::string, double> values2 = { {"y", 0} };
    EXPECT_NEAR(expr2.Calculate(values2), 1.0, 0.0001);
}

TEST(TArithmeticExpressionTest, ComplexTrigonometricExpressions) {
    // Комбинация функций и операций
    TArithmeticExpression expr1("sin(0)+cos(0)");
    EXPECT_NEAR(expr1.Calculate(), 1.0, 0.0001);

    TArithmeticExpression expr2("2*sin(0)");
    EXPECT_NEAR(expr2.Calculate(), 0.0, 0.0001);

    TArithmeticExpression expr3("sin(x)*cos(x)");
    std::map<std::string, double> values3 = { {"x", 0.5} };
    double expected3 = sin(0.5) * cos(0.5);
    EXPECT_NEAR(expr3.Calculate(values3), expected3, 0.0001);

    TArithmeticExpression expr4("(sin(a)+cos(b))*2");
    std::map<std::string, double> values4 = { {"a", 0.3}, {"b", 0.7} };
    double expected4 = (sin(0.3) + cos(0.7)) * 2;
    EXPECT_NEAR(expr4.Calculate(values4), expected4, 0.0001);
}

TEST(TArithmeticExpressionTest, NestedTrigonometricFunctions) {
    TArithmeticExpression expr1("sin(cos(0))");
    EXPECT_NEAR(expr1.Calculate(), sin(cos(0)), 0.0001);

    TArithmeticExpression expr2("cos(sin(0.5))");
    EXPECT_NEAR(expr2.Calculate(), cos(sin(0.5)), 0.0001);
}

TEST(TArithmeticExpressionTest, InvalidTrigonometricUsage) {
    // "sin" - функция без аргументов - ошибка при вычислении
    TArithmeticExpression expr1("sin");
    EXPECT_THROW(expr1.Calculate(), std::runtime_error);

    // "sin()" - функция с пустыми скобками - ошибка при вычислении  
    TArithmeticExpression expr2("sin()");
    EXPECT_THROW(expr2.Calculate(), std::runtime_error);

    // "sin+1" - функция без скобок - ошибка при парсинге
    // (Это зависит от реализации парсера)
    try {
        TArithmeticExpression expr3("sin+1");
        // Если создался, то при вычислении должна быть ошибка
        EXPECT_THROW(expr3.Calculate(), std::runtime_error);
    }
    catch (const std::invalid_argument&) {
        // Или ошибка при создании
        SUCCEED();
    }
    catch (...) {
        FAIL() << "Unexpected exception type";
    }
}

TEST(TArithmeticExpressionTest, PiConstant) {
    TArithmeticExpression expr1("pi");
    EXPECT_NEAR(expr1.Calculate(), 3.141592653589793, 0.0001);

    TArithmeticExpression expr2("2*pi");
    EXPECT_NEAR(expr2.Calculate(), 2 * 3.141592653589793, 0.0001);

    TArithmeticExpression expr3("sin(pi/2)");
    EXPECT_NEAR(expr3.Calculate(), 1.0, 0.0001);

    TArithmeticExpression expr4("cos(pi)");
    EXPECT_NEAR(expr4.Calculate(), -1.0, 0.0001);
}