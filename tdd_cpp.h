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

        if (input.substr(0, 2) == "//") {
            size_t newlinePos = input.find('\n');
            delimiters = parseDelimiters(input.substr(2, newlinePos - 2));
            numbersPart = input.substr(newlinePos + 1);
        }

        return sumNumbers(numbersPart, delimiters);
    }

private:
    static int sumNumbers(const std::string& numbersPart, const std::vector<std::string>& delimiters) {
        std::vector<std::string> tokens = split(numbersPart, delimiters);
        std::vector<int> negatives;
        int sum = 0;

        for (const auto& token : tokens) {
            if (token.empty()) continue;

            int num = std::stoi(token);
            if (num < 0) negatives.push_back(num);
            else if (num <= 1000) sum += num;
        }

        if (!negatives.empty()) {
            throw std::runtime_error(formatNegatives(negatives));
        }

        return sum;
    }

    static std::string formatNegatives(const std::vector<int>& negatives) {
        std::ostringstream oss;
        oss << "negatives not allowed: ";
        for (size_t i = 0; i < negatives.size(); ++i) {
            if (i > 0) oss << ",";
            oss << negatives[i];
        }
        return oss.str();
    }


