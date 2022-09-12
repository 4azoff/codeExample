#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <vector>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <mutex>
#include "Solution.h"

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
    auto solution = new Solution();

    boost::asio::thread_pool pool(std::thread::hardware_concurrency());   

    if (in.is_open())
    {
        while (getline(in, line))
        {
            lineNumber++;
            boost::asio::post(pool, boost::bind(&Solution::FindMask, 
                solution, line, std::ref(mask), lineNumber));
        }   
    }

    pool.join();

    auto answers = solution->GetAnswers();    
    std::cout << answers->size() << "\n";
    //std::sort(answers.begin(), answers.end());
    for (auto el : *answers) {
        std::cout << el.second;
    }
    delete solution;

    return 0;
}