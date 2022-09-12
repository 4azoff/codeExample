#include "Solution.h"
#include <string>
#include <utility>

std::vector<std::pair<int, std::string>>* Solution::GetAnswers() 
{
    return &answers;
}

void Solution::FindMask(const std::string line, const std::string& mask, const int lineNumber)
{
    std::string ans;
    for (size_t i = 0; i < line.size(); i++) {
        auto j = i;
        size_t h = 0;
        while (h < mask.size() && j < line.size() &&
            (line[j] == mask[h] || mask[h] == '?')) {
            ans += line[j];
            j++, h++;
        }
        if (ans.size() == mask.size()) {
            std::lock_guard<std::mutex> lockit(answerMutex);
            answers.push_back(std::make_pair(lineNumber, std::to_string(lineNumber) + " " + std::to_string(i + 1) +
                " " + ans + "\n"));
            ans.clear();
            i = j - 1;
        }
        else {
            ans.clear();
        }
    }
}
