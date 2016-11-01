#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <map>
#include <set>
#include <vector>

#include <chrono>
#include <thread>
#include <utility>


#include "EdmondsMatching.hpp"

std::string TESTS_PATH = "tests/";
std::vector<std::pair<std::string, int>> tests = {
        {"test-6-1.in", 7},
        {"test-8-1.in", 21},
        {"test-10-1.in", 19},
        {"test-10-2.in", -1},
        {"test-14-1.in", 24},
        {"test-x-1.in", 10},
        {"test-x-2.in", 10},
        {"test-x-3.in", 9},
        {"test-edmt2.in", 158},
        {"test-edmt3.in", 29},
        {"test-edmt4.in", 55},
        {"test-edmt5.in", 27},
        {"test-edmt6.in", 40},
        {"test-edmt7.in", 6924},
        {"test-edmt8.in", 859},
        {"test-edmt9.in", 726},
        {"test-edmt10.in", 948},
        {"test-edmt11.in", 1027},
        {"test-edmt12.in", 1290},
        {"test-edmt13.in", 800},
        {"test-edmt14.in", 902},
        {"test-edmt15.in", 723},
        {"test-edmt20.in", 21},
        {"test-edmt21.in", 18},
        {"1000.in", 82612},
};

const std::string RESET         = "\x1b[0m";
const std::string BLINK         = "\x1b[5m";
const std::string GREEN         = "\x1b[92m";
const std::string GREEN_BACK    = "\x1b[42m\x1b[1m";
const std::string RED           = "\x1b[91m\x1b[1m";
const std::string RED_BACK      = "\x1b[101m";
const std::string CYAN          = "\x1b[96m";


int _main(int argc, char *argv[])
{
    std::cout << "Starting tests ..." << std::endl;
    int pass = 0;

    for (int i = 0; i < tests.size(); i++)
    {
        std::cout << "[" << tests[i].first << "]";
        std::cout << std::string(20-tests[i].first.length(), ' ');
        std::cout << BLINK << CYAN << "RUNNING";
        std::cout.flush();

        std::ifstream input(TESTS_PATH + tests[i].first, std::ios::in);
        int n, m;
        input >> n >> m;
        EdmondsMatching matching(n);
        std::map<std::pair<int, int>, int> edges;
        for (int j = 0; j < m; j++)
        {
            int v1, v2, w;
            input >> v1 >> v2 >> w;
            // Zero based indexing
            v1 -= 1;
            v2 -= 1;

            if (v1 > v2)
                std::swap(v1, v2);

            edges[std::make_pair(v1, v2)] = w;
            matching.addEdge(v1, v2, w);
        }

        auto start = std::chrono::high_resolution_clock::now();
        bool success = matching.findMinimumWeightMatching();
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "\r" << RESET << std::string(80, ' ');
        std::cout << "\r[" << tests[i].first << "]";
        std::cout << std::string(20-tests[i].first.length(), ' ');
        std::cout << CYAN << "DONE" << RESET << "\t";

        int got = (success) ? matching.getMatchingWeight() : -1;
        std::cout << "Expected: " << std::setfill(' ') << std::setw(5)
                  << tests[i].second << " Got: "
                  << std::setfill(' ') << std::setw(5) << got << "\t";

        bool invalid = false;

        if (got != tests[i].second)
        {
            invalid = true;
            std::cout << RED_BACK << "MISMATCH\t\t";
        }
        else
            std::cout << GREEN_BACK << "  MATCH ";
        std::cout << RESET << "\t";

        if (tests[i].second == -1)
            std::cout << "\t\t";
        else if (got == tests[i].second)
        {
            int weight = 0;
            std::set<int> vertices;
            for (auto edge: matching.getMatchedEdges())
            {
                int v1 = edge.first;
                int v2 = edge.second;

                if (v1 == v2 || vertices.count(v1) > 0 || vertices.count(v2) > 0)
                {
                    invalid = true;
                    break;
                }

                vertices.insert(v1);
                vertices.insert(v2);

                if (v1 > v2)
                    std::swap(v1, v2);

                if (edges.count(std::make_pair(v1, v2)) != 1)
                {
                    invalid = true;
                    break;
                }

                weight += edges[std::make_pair(v1, v2)];
            }
            if (weight != got || vertices.size() != n)
                invalid = true;

            if (invalid)
                std::cout << RED_BACK << "INVALID";
            else
                std::cout << GREEN_BACK << "  VALID";
            std::cout << RESET << "\t\t";
        }

        int ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::cout << "Time: " << ms << "ms" << std::endl;

        pass += !invalid;
    }

    if (pass == tests.size())
        std::cout << GREEN;
    else
        std::cout << RED;
    std::cout << "Finished all tests. "
              << "Passed: " << pass << "/" << tests.size();
    std::cout << RESET << std::endl;
}
