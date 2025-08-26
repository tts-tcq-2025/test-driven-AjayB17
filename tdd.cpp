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

        std::string numbersPart;
        std::vector<std::string> delimiters;

        if (hasCustomDelimiter(input)) {
            parseCustomDelimiter(input, numbersPart, delimiters);
        } else {
            numbersPart = input;
            delimiters = {",", "\n"};
        }

        std::vector<std::string> tokens = splitNumbers(numbersPart, delimiters);
        return calculateSum(tokens);
    }

private:
    static bool hasCustomDelimiter(const std::string& input) {
        return input.substr(0, 2) == "//";
    }

    static void parseCustomDelimiter(const std::string& input, std::string& numbersPart, std::vector<std::string>& delimiters) {
        size_t newlinePos = input.find('\n');
        std::string delimiterPart = input.substr(2, newlinePos - 2);
        numbersPart = input.substr(newlinePos + 1);
        delimiters = parseDelimiters(delimiterPart);
    }

static std::vector<std::string> parseDelimiters(const std::string& delimiterPart) {
    if (delimiterPart.empty()) return {","};

    std::vector<std::string> result;

    if (delimiterPart[0] == '[') {
        size_t pos = 0;
        while (true) {
            size_t start = delimiterPart.find('[', pos);
            size_t end = delimiterPart.find(']', start);
            if (start == std::string::npos || end == std::string::npos) break;

            result.push_back(delimiterPart.substr(start + 1, end - start - 1));
            pos = end + 1;
        }
    } else {
        // Single char delimiter, no conditional branch needed here
        result.push_back(delimiterPart);
    }
    return result;
}

    // Split by multiple delimiters one by one
    static std::vector<std::string> splitNumbers(const std::string& str, const std::vector<std::string>& delimiters) {
        std::vector<std::string> tokens = {str};
        for (const auto& delim : delimiters) {
            tokens = splitByDelimiter(tokens, delim);
        }
        return tokens;
    }

    // Split all strings in tokens vector by one delimiter
    static std::vector<std::string> splitByDelimiter(const std::vector<std::string>& tokens, const std::string& delim) {
        std::vector<std::string> result;
        for (const auto& token : tokens) {
            size_t start = 0;
            size_t pos;
            while ((pos = token.find(delim, start)) != std::string::npos) {
                result.push_back(token.substr(start, pos - start));
                start = pos + delim.size();
            }
            result.push_back(token.substr(start));
        }
        return result;
    }

    static int calculateSum(const std::vector<std::string>& tokens) {
        std::vector<int> negatives;
        int sum = 0;

        for (const auto& token : tokens) {
            if (token.empty()) continue;
            int num = std::stoi(token);
            if (num < 0) negatives.push_back(num);
            else if (num <= 1000) sum += num;
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
};

// Tests omitted for brevity, same as before.

int main() {
    assert(StringCalculator::Add("") == 0);
    assert(StringCalculator::Add("1") == 1);
    assert(StringCalculator::Add("1,2") == 3);
    assert(StringCalculator::Add("1\n2,3") == 6);
    assert(StringCalculator::Add("//;\n1;2") == 3);
    assert(StringCalculator::Add("//[***]\n1***2***3") == 6);
    assert(StringCalculator::Add("//[*][%]\n1*2%3") == 6);

    try {
        StringCalculator::Add("1,-2,3,-4");
        assert(false);
    } catch (const std::runtime_error& e) {
        std::string msg = e.what();
        assert(msg.find("negatives not allowed") != std::string::npos);
        assert(msg.find("-2") != std::string::npos);
        assert(msg.find("-4") != std::string::npos);
    }

    assert(StringCalculator::Add("2,1001") == 2);
    assert(StringCalculator::Add("1000,1001,3") == 1003);

    std::cout << "All tests passed!\n";
    return 0;
}
