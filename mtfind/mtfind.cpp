#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <vector>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>
#include <mutex>

std::mutex answersMutex;

void FindMask(const std::string line, const std::string& mask, const int lineNumber,
    std::vector<std::pair<int, std::string>>& answers){
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
            std::lock_guard<std::mutex> lockit(answersMutex);
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

bool checkFlags(char* argv[]) {
    
    auto result = true;
    
    if (!argv[1]) {
        std::cout << "Specifay a file \n";
        result = false;
    }
    else
    {
        std::ifstream in(argv[1]);
        if (!in)
        {
            std::cout << "Cannot open file '" << argv[1] <<
                "' invalid path or filename \n";
            result = false;
        }
        in.close();
    }    

    if (!argv[2])
    {
        std::cout << "Invalid mask \n";
        result = false;
    }
    else {
        std::string mask(argv[2]);

        if (mask.empty())
        {
            std::cout << "Invalid mask \n";
            result = false;
        }
    }
    
    return result;
}

int main(int argc, char* argv[])
{
    if (!checkFlags(argv)) {
        return 0;
    }
    std::ifstream in(argv[1]);
    std::string mask(argv[2]);

    std::string line;
    int lineNumber = 0;     
    std::vector<std::pair<int, std::string>> answers;

    boost::asio::thread_pool pool(std::thread::hardware_concurrency());   

    if (in.is_open())
    {
        while (getline(in, line))
        {
            lineNumber++;
            boost::asio::post(pool, std::bind(FindMask, line, 
                std::ref(mask), lineNumber, std::ref(answers)));           
        }
    }
    in.close();     
    pool.join();

    std::cout << answers.size() << "\n";
    std::sort(answers.begin(), answers.end());
    for (auto el : answers) {
        std::cout << el.second;
    }

    return 0;
}