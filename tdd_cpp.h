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

        std::vector<std::string> tokens = split(numbersPart, delimiters);
        return calculateSum(tokens);
    }

private:
    static int calculateSum(const std::vector<std::string>& tokens) {
        std::vector<int> negatives;
        int sum = 0;

        for (const auto& token : tokens) {
            if (token.empty()) continue;

            int num = std::stoi(token);
            if (num < 0) {
                negatives.push_back(num);
            } else if (num <= 1000) {
                sum += num;
            }
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

    // Your original parseDelimiters and split unchanged:

    static std::vector<std::string> parseDelimiters(const std::string& delimiterPart) {
        std::vector<std::string> result;

        if (delimiterPart.empty()) {
            return {","};
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
            result.push_back(delimiterPart);
        }

        return result;
    }

    static std::vector<std::string> split(const std::string& str, const std::vector<std::string>& delimiters) {
        std::vector<std::string> tokens;
        size_t start = 0;

        while (start < str.length()) {
            size_t closestPos = std::string::npos;
            std::string closestDelim;

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
