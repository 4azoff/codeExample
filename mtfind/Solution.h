#include <mutex>
#include <vector>
#include <iostream>

class Solution {
private:
	std::mutex answerMutex;
	std::vector<std::pair<int, std::string>> answers;

public:
	Solution() {};
	~Solution() {};

	std::vector<std::pair<int, std::string>>* GetAnswers();
	void FindMask(const std::string line, const std::string& mask, const int lineNumber);

};