#include <mutex>
#include <map>
#include <string>

class Solution {
private:
	std::mutex answerMutex;
	std::map<std::pair<int,int>, std::string> answers;

public:
	Solution() {};
	~Solution() {};

	std::map<std::pair<int, int>, std::string>* GetAnswers();
	void FindMask(const std::string line, const std::string& mask, const int lineNumber);

};