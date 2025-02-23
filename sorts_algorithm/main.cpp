#include <iostream>
#include <compare>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip> 
#include <memory>
#include <functional>
#include "matplotlib-cpp/matplotlibcpp.h"

namespace plt = matplotlibcpp;

class Soldier {
public:
    std::string m_name;
    unsigned int m_rank;
    unsigned int m_companyNumber;
    unsigned int m_age;

    Soldier(std::string& name, std::string& rankStr, std::string& companyNumberStr, std::string& ageStr);
    bool operator<(const Soldier&) const;
    bool operator>(const Soldier&) const;
    bool operator<=(const Soldier&) const;
    bool operator>=(const Soldier&) const;
};

typedef std::vector<Soldier> SoldiersVector;

Soldier::Soldier(std::string& name, std::string& rankStr, std::string& companyNumberStr, std::string& ageStr) 
:   m_name(name), m_rank(std::stoi(rankStr)),
    m_companyNumber(std::stoi(companyNumberStr)),
    m_age(std::stoi(ageStr))
{}

bool Soldier::operator<(const Soldier& soldier) const
{
    if (m_rank != soldier.m_rank) return m_rank < soldier.m_rank;

    if (m_name != soldier.m_name) return m_name < soldier.m_name;

    return m_companyNumber < soldier.m_companyNumber;
}

bool Soldier::operator<=(const Soldier& soldier) const
{
    if (m_rank != soldier.m_rank) return m_rank <= soldier.m_rank;

    if (m_name != soldier.m_name) return m_name <= soldier.m_name;

    return m_companyNumber <= soldier.m_companyNumber;
}

bool Soldier::operator>(const Soldier& soldier) const
{
    if (m_rank != soldier.m_rank) return m_rank > soldier.m_rank;

    if (m_name != soldier.m_name) return m_name > soldier.m_name;

    return m_companyNumber > soldier.m_companyNumber;
}

bool Soldier::operator>=(const Soldier& soldier) const
{
    if (m_rank != soldier.m_rank) return m_rank >= soldier.m_rank;

    if (m_name != soldier.m_name) return m_name >= soldier.m_name;

    return m_companyNumber >= soldier.m_companyNumber;
}

template <typename T>
std::vector<T> SelectSort(std::vector<T> arr)
{   
    size_t size = arr.size();
    for (int i = 0; i < size; ++i) {
        size_t indexMinEl = i;
        for (int j = i + 1; j < size; ++j) {
            if (arr[j] < arr[indexMinEl]) {
                indexMinEl = j;
            }
        }

        if (indexMinEl != i) std::swap(arr[indexMinEl], arr[i]);
    }
    return arr;
}

template <typename T>
std::vector<T> InsertSort(std::vector<T> arr)
{
    size_t size = arr.size();
    for (int i = 1; i < size; ++i) {
        int curElementIndex = i;
        for (int j = i - 1; j >= 0 && arr[j] > arr[curElementIndex]; --j) {
            std::swap(arr[j], arr[curElementIndex]);
            curElementIndex = j;
        }
    }
    return arr;
}

template<typename T>
void Heapify(std::vector<T>& arr, size_t size, long index)
{
    size_t largestIndex = index;
    size_t leftIndex = index * 2 + 1;
    size_t rightIndex = index * 2 + 2;

    if (rightIndex < size && arr[rightIndex] > arr[largestIndex]) {
        largestIndex = rightIndex;
    }
    
    if (leftIndex < size && arr[leftIndex] > arr[largestIndex]) {
        largestIndex = leftIndex;
    }

    if (largestIndex != index ) {
        std::swap(arr[index], arr[largestIndex]);

        Heapify(arr, size, largestIndex);
    }
}

template<typename T>
std::vector<T> HeapSort(std::vector<T> arr)
{
    size_t size = arr.size();
    for (long int i = size / 2 - 1; i >= 0; --i) {
        Heapify(arr, size, i);
    }

    for (int i = size - 1; i > 0; --i) {

        std::swap(arr[0], arr[i]);

        Heapify(arr, i, 0);
    }

    return arr;
}

SoldiersVector ReadSoldiersFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    SoldiersVector soldiers;

    if (!file.is_open()) {
        std::cerr << "File open error: " << filename << std::endl;
        return soldiers;
    }

    std::string line;

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, rankStr, companyNumberStr, ageStr;

        std::getline(ss, name, ',');
        std::getline(ss, rankStr, ',');
        std::getline(ss, companyNumberStr, ',');
        std::getline(ss, ageStr, ',');

        soldiers.emplace_back(name, rankStr, companyNumberStr, ageStr);
    }

    file.close();
    return soldiers;
}


void WriteSoldiersToFormattedFile(const SoldiersVector& soldiers, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "File open error: " << filename << std::endl;
        return;
    }

    file << std::setw(20) << "Name" << std::setw(10) << "Rank" << std::setw(15) << "CompanyNumber" << std::setw(10) << "Age\n";

    for (const auto& soldier : soldiers) {
        file << std::setw(20) << soldier.m_name
             << std::setw(10) << soldier.m_rank
             << std::setw(15) << soldier.m_companyNumber
             << std::setw(10) << soldier.m_age << "\n";
    }

    file.close();
}

void RecordSortTimeAndWriteInFile(
    auto&& sort, 
    SoldiersVector& data,
    std::vector<double>& arrSortTime, 
    const std::string& out)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto sortedVec = sort(data);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    arrSortTime.emplace_back(elapsed.count());
    WriteSoldiersToFormattedFile(sortedVec, out);
}

void OutTime(std::vector<double> vec)
{
    for (auto& i : vec) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

int main() {
    int rangeStep = (100'000 - 100) / 10;

    std::vector<double> selectSortTime;
    std::vector<double> insertSortTime;
    std::vector<double> heapSortTime;
    std::vector<double> stdSortTime;
    // 100 10090 20080 30070 40060 50050 60040 70030 80020 90010 100000 
    std::vector<int> countRecords{100};//, 10090, 20080, 30070, 40060, 50050, 60040, 70030, 80020, 90010, 100000 };
    for (auto&count : countRecords) {
        const std::string fileName("data_" + std::to_string(count));
        const std::string outputDir("../output/");
        const std::string dataDir("../data/");
        SoldiersVector data = ReadSoldiersFromCSV(dataDir + fileName + ".csv");
        std::unique_ptr<SoldiersVector> pSortedVec = std::make_unique<SoldiersVector>();

        // auto start = std::chrono::high_resolution_clock::now();
        // *pSortedVec = SelectSort(data);
        // auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> elapsed = end - start;
        // selectSortTime.emplace_back(elapsed.count());
        // WriteSoldiersToFormattedFile(*pSortedVec, outputDir + "select_" + fileName + ".txt");
        RecordSortTimeAndWriteInFile(SelectSort<Soldier>, data, selectSortTime, outputDir + "select_" + fileName + ".txt");
        
        // auto start = std::chrono::high_resolution_clock::now();
        // *pSortedVec = InsertSort(data);
        // auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> elapsed = end - start;
        // insertSortTime.emplace_back(elapsed.count());
        // WriteSoldiersToFormattedFile(*pSortedVec, outputDir + "insert_" + fileName + ".txt");
        RecordSortTimeAndWriteInFile(InsertSort<Soldier>, data, insertSortTime, outputDir + "insert_" + fileName + ".txt");
        
        // auto start = std::chrono::high_resolution_clock::now();
        // *pSortedVec = HeapSort(data);
        // auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> elapsed = end - start;
        // heapSortTime.emplace_back(elapsed.count());
        // WriteSoldiersToFormattedFile(*pSortedVec, outputDir + "heap_" + fileName + ".txt");
        RecordSortTimeAndWriteInFile(HeapSort<Soldier>, data, heapSortTime, outputDir + "heap_" + fileName + ".txt");
        
        // start = std::chrono::high_resolution_clock::now();
        // std::sort(data.begin(), data.end());
        // end = std::chrono::high_resolution_clock::now();
        // elapsed = end - start;
        // heapSortTime.emplace_back(elapsed.count());
        // WriteSoldiersToFormattedFile(data, outputDir + "std_" + fileName + ".txt");
        auto stdSort = [](SoldiersVector vec) -> SoldiersVector {
            std::sort(vec.begin(), vec.end());
            return vec;
        };
        RecordSortTimeAndWriteInFile(std::move(stdSort), data, stdSortTime, outputDir + "std_" + fileName + ".txt");

    }
    OutTime(selectSortTime);
    OutTime(insertSortTime);
    OutTime(heapSortTime);
    OutTime(stdSortTime);
    std::vector<double> x = {1, 2, 3, 4};
    std::vector<double> y = {1, 4, 9, 16};

    // Построение графика
    plt::plot(x, y);
    // plt::plot(selectSortTime, insertSortTime);
    plt::title("Пример графика");
    plt::xlabel("Ось X");
    plt::ylabel("Ось Y");
    plt::show();

    return 0;
}