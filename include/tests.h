#ifndef TESTS_H
#define TESTS_H

#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <chrono>
#include <cassert>
#include <fstream>
#include <iomanip>

// Класс для тестирования алгоритмов сортировки
class TestRunner {
private:
    int testsPassed = 0;
    int testsFailed = 0;

    // Вспомогательные методы
    template<typename T>
    bool isSorted(const std::vector<T>& arr);

    template<typename T>
    bool vectorsEqual(const std::vector<T>& a, const std::vector<T>& b);

    void assertTrue(bool condition, const std::string& testName);

    // Методы для вывода
    template<typename T>
    void printArray(const std::vector<T>& arr, const std::string& label);

    template<typename T>
    void printArrayWithMarkers(const std::vector<T>& arr, int left, int right, int pivot);

    template<typename T>
    void printStep(const std::vector<T>& arr, int step, const std::string& operation);

    // Тесты
    void testQuickSortDetailed();
    void testMergeSortDetailed();
    void testExternalSortDetailed();
    void testParallelSortDetailed();
    void testEdgeCasesDetailed();
    void testPerformanceDetailed();

public:
    void runAllTests();
    void printResults() const;
};

#endif // TESTS_H