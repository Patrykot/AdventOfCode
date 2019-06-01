#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>


const char* AOC_DIR = "/home/patryk/CLionProjects/AdventOfCode/";
const char* DAY = "Day08";

const std::filesystem::path getInputFilePath(const char *filename) {
    std::filesystem::path p = AOC_DIR;
    return p / DAY / filename;
}

bool getInput(const char *filename, std::vector<int> &output) {
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
    std::getline(file, line);

    const auto endstr = line.find_last_not_of(' ') + 1; // End of data excluding trailing spaces

    for (size_t last = 0, strt = 0, fnd = 0; strt < endstr; strt = fnd + last)
        output.push_back(std::stoul(line.substr(fnd = line.find_first_not_of(' ', strt), 2), &last));

    return true;
}

struct node {
    std::vector<node> childs;
    std::vector<int> metadatos;

    unsigned metadatosSum = 0;

    static unsigned METADATOS_SUM;
};

unsigned node::METADATOS_SUM = 0;

static void recurBuildTree(unsigned& index, node& n, const std::vector<int>& input)
{
    if (input[index])
    {
        n.childs = std::vector<node>(input[index]);
    }

    n.metadatos = std::vector<int>(input[++index]);

    for (auto& ch : n.childs)
    {
        recurBuildTree(++index, ch, input);
    }


    for (auto& m : n.metadatos)
    {
        m = input[++index];
    }

    node::METADATOS_SUM = std::accumulate(n.metadatos.begin(), n.metadatos.end(), node::METADATOS_SUM);
}

void firstPart(const std::vector<int>& input)
{
    node root;
    unsigned index = 0;

    recurBuildTree(index, root, input);

    std::cout << "Metadatos sum: " << node::METADATOS_SUM << std::endl;
}

static void recurBuildTree2(unsigned& index, node& n, const std::vector<int>& input)
{
    if (input[index])
    {
        n.childs = std::vector<node>(input[index]);
    }

    n.metadatos = std::vector<int>(input[++index]);

    for (auto& ch : n.childs)
    {
        recurBuildTree2(++index, ch, input);
    }

    for (auto& m : n.metadatos)
    {
        m = input[++index];
    }

    if (n.childs.size())
    {
        for (auto m : n.metadatos)
        {
            if (m != 0 && m <= n.childs.size())
            {
                n.metadatosSum += n.childs[m-1].metadatosSum;
            }
        }
    }
    else
    {
        n.metadatosSum = std::accumulate(n.metadatos.begin(), n.metadatos.end(), 0);
    }

    node::METADATOS_SUM += n.metadatosSum;
}

void secondPart(const std::vector<int>& input)
{
    node root;
    unsigned index = 0;

    recurBuildTree2(index, root, input);

    std::cout << "Metadatos sum: " << root.metadatosSum << std::endl;
}


int main() {
    std::vector<int> input;

    if (!getInput("day08.txt", input))
        return -1;

    std::cout << "First part:\n";
    firstPart(input);

    node::METADATOS_SUM = 0;

    std::cout << std::endl << "Second part:\n";
    secondPart(input);

    return 0;
}

