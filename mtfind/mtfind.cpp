#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <mutex>
#include "Solution.h"

bool initMaskAndFile(int& argc, char* argv[], std::ifstream& in, std::string& mask) {
    
    auto result = true;
    if (argc != 3) {
        std::cout << "Wrong number of arguments, need: \n1. Path to the file \n2. Search mask\n";
        std::cout << "Example : mtfind input.txt ""?ad""\n";
        return false;
    }

    in.open(argv[1]);
    if (!in.good())
    {
        std::cout << "Cannot open file '" << argv[1] <<
            "' invalid path or filename \n";
        result = false;
    }

    mask = argv[2];
    if (mask.empty())
    {
        std::cout << "Invalid mask \n";
        result = false;
    }    
    return result;
}

int main(int argc, char* argv[])
{
    std::ifstream in;
    std::string mask;
    if (!initMaskAndFile(argc, argv, in, mask)) {
        return 0;
    }    

    std::string line;
    int lineNumber = 0;     
    auto solution = new Solution();

    //TODO: оптимизировать количество создаваемых потоков 
    boost::asio::thread_pool pool(std::thread::hardware_concurrency());   
    
    while (getline(in, line))
    {
        lineNumber++;
        boost::asio::post(pool, boost::bind(&Solution::FindMask,
            solution, line, std::ref(mask), lineNumber));
    }
    pool.join();

    auto answers = solution->GetAnswers();    
    std::cout << answers->size() << "\n";
    for (auto el : *answers) {
        std::cout << el.second;
    }
    delete solution;

    return 0;
}