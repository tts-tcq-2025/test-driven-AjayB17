#ifndef TDD_CPP_H_
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

class StringCalculator {
 public:
    static int Add(const std::string& input) {
        if (input.empty()) return 0;  // 1 decision

        std::string numbersPart = input;
        std::vector<std::string> delimiters = {",", "\n"};

        if (input.substr(0, 2) == "//") {  // 2nd decision
            size_t newlinePos = input.find('\n');
            std::string delimiterPart = input.substr(2, newlinePos - 2);
            numbersPart = input.substr(newlinePos + 1);

            delimiters = parseDelimiters(delimiterPart);
        }

        return calculateSum(numbersPart, delimiters);
    }

 private:
    static int calculateSum(const std::string& numbersPart, 
        const std::vector<std::string>& delimiters) {
        std::vector<std::string> tokens = split(numbersPart, delimiters);
        std::vector<int> negatives;
        int sum = 0;

        for (const auto& token : tokens) {
            if (token.empty()) continue;  // 1 decision

            int num = std::stoi(token);
            processNumber(num, sum, negatives);  // extract logic to reduce complexity
        }

        checkNegatives(negatives);  // extract exception throwing

        return sum;
    }

    static void processNumber(int num, int& sum, std::vector<int>& negatives) {
        if (num < 0) {  // 1 decision
            negatives.push_back(num);
        } else if (num <= 1000) {  // 2nd decision
            sum += num;
        }
    }

    static void checkNegatives(const std::vector<int>& negatives) {
        if (!negatives.empty()) {  // 1 decision
            std::ostringstream oss;
            oss << "negatives not allowed: ";
            for (size_t i = 0; i < negatives.size(); ++i) {
                if (i > 0) oss << ",";
                oss << negatives[i];
            }
            throw std::runtime_error(oss.str());
        }
    }

    static std::vector<std::string> parseDelimiters(const std::string& delimiterPart) {
        if (delimiterPart.empty()) {
            return {","};  // 1 decision
        }

        if (delimiterPart[0] == '[') {  // 2nd decision
            return parseMultipleDelimiters(delimiterPart);
        }

        return {delimiterPart};  // no decision here
    }

    static std::vector<std::string> parseMultipleDelimiters(const std::string& delimiterPart) {
        std::vector<std::string> result;
        size_t pos = 0;

        while (pos < delimiterPart.length()) {
            size_t start = delimiterPart.find('[', pos);
            size_t end = delimiterPart.find(']', start);
            if (start == std::string::npos || end == std::string::npos) break;

            result.push_back(delimiterPart.substr(start + 1, end - start - 1));
            pos = end + 1;
        }

        return result;
    }

    static std::vector<std::string> split(const std::string& str, 
        const std::vector<std::string>& delimiters) {
        std::vector<std::string> tokens;
        size_t start = 0;

        while (start < str.length()) {
            size_t closestPos = std::string::npos;
            std::string closestDelim;

            for (const auto& delim : delimiters) {
                size_t delimPos = str.find(delim, start);
                if (delimPos != std::string::npos && 
                    (closestPos == std::string::npos || delimPos < closestPos)) {
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
