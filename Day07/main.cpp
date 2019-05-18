#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <string>

#include <boost/range/adaptors.hpp>

const char *AOC_DIR = "/home/patryk/CLionProjects/AdventOfCode/";
const char *DAY = "Day07";

typedef std::map<char, std::set<char>> DataCollection;

const std::filesystem::path getInputFilePath(const char *filename) {
    std::filesystem::path p = AOC_DIR;
    return p / DAY / filename;
}

bool getInputAsCollection(const char *filename, DataCollection &output) {
    const std::filesystem::path input_file = getInputFilePath(filename);

    std::error_code ec;
    if (!std::filesystem::is_regular_file(input_file, ec)) {
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
            output[matches.str(1)[0]];
            output[matches.str(2)[0]].insert(matches.str(1)[0]);
            continue;
        }
    }

    return true;
}

void firstPart(DataCollection requirementSets) {
    std::string result;
    auto letterFinder = [](const auto &item) { return item.second.empty(); };

    while (!requirementSets.empty()) {
        auto firstLetterWithNoReqs = std::find_if(requirementSets.begin(), requirementSets.end(), letterFinder);
        result += firstLetterWithNoReqs->first;
        std::for_each(requirementSets.begin(), requirementSets.end(),
                      [&](auto &elem) { elem.second.erase(firstLetterWithNoReqs->first); });
        requirementSets.erase(firstLetterWithNoReqs);
    }

    std::cout << result << std::endl;
}

struct worker {
    char letterWorkingOn;
    unsigned jobDoneAt;
};

void secondPart(DataCollection requirementSets) {

    std::string result;
    unsigned timeCounter = 0;
    std::vector<worker> workers(5);

    auto letterFinder = [](const auto &item) { return item.second.empty(); };
    auto workerAvailabilityChecker = [&timeCounter] (const auto& w) { return timeCounter >= w.jobDoneAt; };

    while (!requirementSets.empty()) {

        auto availableWorkers = workers | boost::adaptors::filtered(workerAvailabilityChecker);

        if (availableWorkers.empty()) {
            timeCounter++;
            continue;
        }

        for (auto &w : availableWorkers) {
            if (w.letterWorkingOn) {
                result += w.letterWorkingOn;
                std::for_each(requirementSets.begin(), requirementSets.end(),
                              [&](auto &elem) { elem.second.erase(w.letterWorkingOn); });
                w.letterWorkingOn = 0;
            }
        }

        auto availableLetters = requirementSets | boost::adaptors::filtered(letterFinder);
        auto availLettersIt = availableLetters.begin();

        for (auto &w : availableWorkers) {
            if (availLettersIt != availableLetters.end()) {
                w.letterWorkingOn = availLettersIt->first;
                w.jobDoneAt =  timeCounter + w.letterWorkingOn - ('A'-1) + 60;
                ++availLettersIt;
                requirementSets.erase(w.letterWorkingOn);
            }
            else {
                break;
            }
        }

        timeCounter++;
    }

    auto lastWorker = std::max_element(workers.begin(), workers.end(), [] (const auto& a, const auto& b) { return a.jobDoneAt < b.jobDoneAt;});

    std::cout << result << ". Seconds: " << lastWorker->jobDoneAt << std::endl;
}


int main() {
    DataCollection requirementSets;

    if (!getInputAsCollection("day7.txt", requirementSets))
        return -1;

    std::cout << "First part: " << std::endl;
    firstPart(requirementSets);

    std::cout << std::endl << "Second part: " << std::endl;
    secondPart(requirementSets);

    return 0;
}

