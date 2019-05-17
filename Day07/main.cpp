#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>

const char * AOC_DIR = "/home/patryk/CLionProjects/AdventOfCode/";
const char * DAY = "Day07";

typedef  std::map<char, std::set<char>> DataCollection;

const std::filesystem::path getInputFilePath(const char *filename)
{
	std::filesystem::path p = AOC_DIR;
	return p / DAY / filename;
}

bool getInputAsCollection(const char* filename, DataCollection& output)
{
    const std::filesystem::path input_file = getInputFilePath(filename);

    std::error_code ec;
    if (!std::filesystem::is_regular_file(input_file, ec))
    {
        if (ec)
            std::cerr << "[" << ec.value() << "] " << ec.message() << std::endl;
        return false;
    }

    std::ifstream file(input_file);

    if (!file.is_open())
        return false;

    std::string line;
    //Step C must be finished before step A can begin.
    std::regex re("Step\\s(\\D)\\smust\\sbe\\sfinished\\sbefore\\sstep\\s(\\D)\\scan\\sbegin\\.");

    while (std::getline(file, line)) {
        std::smatch matches;
        std::regex_search(line, matches, re);
        if (!matches.empty()) {
            output.insert(std::make_pair(std::string(matches[1])[0], std::set<char>()));
            output[std::string(matches[2])[0]].insert(std::string(matches[1])[0]);
            continue;
        }
    }

	return true;
}

void firstPart(DataCollection requirementSets)
{
    std::string result;
    auto letterFinder = [](const auto& item) { return item.second.empty(); };

    while (!requirementSets.empty()) {
        auto firstLetterWithNoReqs = std::find_if(requirementSets.begin(), requirementSets.end(), letterFinder);
        result += firstLetterWithNoReqs->first;
        std::for_each(requirementSets.begin(), requirementSets.end(),
                      [&](auto &elem) { elem.second.erase(firstLetterWithNoReqs->first); });
        requirementSets.erase(firstLetterWithNoReqs);
    }

    std::cout << result << std::endl;
}


struct worker
{

    unsigned m_jobEndTime = 0;
};

void secondPart(DataCollection requirementSets)
{
    std::string result;
    unsigned timeCounter = 0;
    std::vector<unsigned> workers(5,0);

    auto letterFinder = [](const auto& item) { return item.second.empty(); };
    auto workerAvailabilityChecker = [=] (const auto x) { return timeCounter == x; };


    while (!requirementSets.empty()) {

        auto freeWorker = std::find_if(workers.begin(), workers.end(), workerAvailabilityChecker);

        while (freeWorker != workers.end())
        {
            auto firstLetterWithNoReqs = std::find_if(requirementSets.begin(), requirementSets.end(), letterFinder);
            auto letter = firstLetterWithNoReqs->first;
            result += letter;
            requirementSets.erase(firstLetterWithNoReqs);

            std::for_each(requirementSets.begin(), requirementSets.end(),
                          [&](auto &elem) { elem.second.erase(firstLetterWithNoReqs->first); });

            *freeWorker += (60 + letter - 'B');
        }

        timeCounter++;
    }

    std::cout << "Result: " << result << ". Seconds: " << timeCounter << std::endl;

}


int main()
{
    DataCollection requirementSets;

	if (!getInputAsCollection("day7test.txt", requirementSets))
	    return -1;

	std::cout << "First part: " << std::endl;
	firstPart(requirementSets);

    std::cout << std::endl <<"Second part: " << std::endl;
    secondPart(requirementSets);

    return 0;
}

