#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

class StringCalculator {
public:
    static int Add(const std::string& input) {
        if (input.empty()) return 0;

        std::string numbersPart = input;
        std::vector<std::string> delimiters = {",", "\n"};

        // Check for custom delimiter
        if (input.substr(0, 2) == "//") {
            size_t newlinePos = input.find('\n');
            std::string delimiterPart = input.substr(2, newlinePos - 2);
            numbersPart = input.substr(newlinePos + 1);

            delimiters = parseDelimiters(delimiterPart);
        }

        // Split numbers by delimiters
        std::vector<std::string> tokens = split(numbersPart, delimiters);

        // Convert tokens to ints, check negatives, ignore > 1000
        std::vector<int> negatives;
        int sum = 0;
        for (const auto& token : tokens) {
            if (token.empty()) continue;  // ignore empty tokens (robustness)

            int num = std::stoi(token);
            if (num < 0) {
                negatives.push_back(num);
            } else if (num <= 1000) {
                sum += num;
            }
        }

        if (!negatives.empty()) {
            std::ostringstream oss;
            oss << "negatives not allowed: ";
            for (size_t i = 0; i < negatives.size(); ++i) {
                if (i > 0) oss << ",";
                oss << negatives[i];
            }
            throw std::runtime_error(oss.str());
        }

        return sum;
    }

private:
    // Parse delimiter string like ";" or "[***]" or "[*][%]"
    static std::vector<std::string> parseDelimiters(const std::string& delimiterPart) {
        std::vector<std::string> result;

        if (delimiterPart.empty()) {
            return {","};  // fallback
        }

        if (delimiterPart[0] == '[') {
            size_t pos = 0;
            while (pos < delimiterPart.length()) {
                size_t start = delimiterPart.find('[', pos);
                size_t end = delimiterPart.find(']', start);
                if (start == std::string::npos || end == std::string::npos) break;

                std::string delim = delimiterPart.substr(start + 1, end - start - 1);
                result.push_back(delim);
                pos = end + 1;
            }
        } else {
            // single char delimiter, e.g. ";"
            result.push_back(delimiterPart);
        }

        return result;
    }

    // Split a string by multiple delimiters
    static std::vector<std::string> split(const std::string& str, const std::vector<std::string>& delimiters) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t pos = std::string::npos;

        size_t i = 0;
        while (start < str.length()) {
            size_t closestPos = std::string::npos;
            std::string closestDelim;

            // Find closest delimiter position
            for (const auto& delim : delimiters) {
                size_t delimPos = str.find(delim, start);
                if (delimPos != std::string::npos && (closestPos == std::string::npos || delimPos < closestPos)) {
                    closestPos = delimPos;
                    closestDelim = delim;
                }
            }

            if (closestPos == std::string::npos) {
                tokens.push_back(str.substr(start));
                break;
            } else {
                tokens.push_back(str.substr(start, closestPos - start));
                start = closestPos + closestDelim.length();
            }
        }

        return tokens;
    }
};

// TESTS

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
