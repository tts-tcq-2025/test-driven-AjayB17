#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "test-driven-AjayB17/tdd.h"

void testEmptyStringReturnsZero() {
    assert(StringCalculator::Add("") == 0);
    std::cout << "Test passed: Empty string returns 0\n";
}

void testSingleNumber() {
    assert(StringCalculator::Add("1") == 1);
    std::cout << "Test passed: Single number returns itself\n";
}

void testTwoNumbers() {
    assert(StringCalculator::Add("1,2") == 3);
    std::cout << "Test passed: Two numbers summed\n";
}

void testUnknownAmountOfNumbers() {
    assert(StringCalculator::Add("1,2,3,4,5") == 15);
    std::cout << "Test passed: Unknown amount of numbers summed\n";
}

void testNewlinesAsDelimiters() {
    assert(StringCalculator::Add("1\n2,3") == 6);
    std::cout << "Test passed: Newlines supported as delimiters\n";
}

void testCustomSingleCharDelimiter() {
    assert(StringCalculator::Add("//;\n1;2") == 3);
    std::cout << "Test passed: Custom single char delimiter\n";
}

void testCustomMultiCharDelimiter() {
    assert(StringCalculator::Add("//[***]\n1***2***3") == 6);
    std::cout << "Test passed: Custom multi-char delimiter\n";
}

void testMultipleCustomDelimiters() {
    assert(StringCalculator::Add("//[*][%]\n1*2%3") == 6);
    std::cout << "Test passed: Multiple custom delimiters\n";
}

void testNegativeNumbersThrow() {
    try {
        StringCalculator::Add("1,-2,3,-4");
        assert(false);  // should not reach here
    } catch (const std::runtime_error& e) {
        std::string msg = e.what();
        assert(msg.find("negatives not allowed") != std::string::npos);
        assert(msg.find("-2") != std::string::npos);
        assert(msg.find("-4") != std::string::npos);
        std::cout << "Test passed: Negative numbers throw with message\n";
    }
}

void testNumbersOver1000Ignored() {
    assert(StringCalculator::Add("2,1001") == 2);
    assert(StringCalculator::Add("1000,1001,1002,3") == 1003);
    std::cout << "Test passed: Numbers >1000 ignored\n";
}

int main() {
    testEmptyStringReturnsZero();
    testSingleNumber();
    testTwoNumbers();
    testUnknownAmountOfNumbers();
    testNewlinesAsDelimiters();
    testCustomSingleCharDelimiter();
    testCustomMultiCharDelimiter();
    testMultipleCustomDelimiters();
    testNegativeNumbersThrow();
    testNumbersOver1000Ignored();
    return 0;
}
