#ifndef SORTER_H
#define SORTER_H

#include <vector>
#include <string>
#include <functional>

/* Главный класс для сортировки
   Предоставляет различные алгоритмы сортировки:
     - QuickSort
     - MergeSort
     - External Sort (K-Way Merge)
     - Parallel Sort */
template<typename T>
class Sorter {
public:
    // QUICKSORT

    // Быстрая сортировка массива
    // arr - вектор для сортировки, left - левая граница, right - правая граница
    static void quickSort(std::vector<T>& arr, int left, int right);

    // Быстрая сортировка всего массива
    // arr - вектор для сортировки
    static void quickSort(std::vector<T>& arr);

    // MERGE SORT

    // Сортировка слиянием
    // arr - вектор для сортировки, left - левая граница, right - правая граница
    static void mergeSort(std::vector<T>& arr, int left, int right);

    // Сортировка слиянием всего массива
    // arr - вектор для сортировки
    static void mergeSort(std::vector<T>& arr);

    // EXTERNAL SORT

    // Внешняя сортировка с K-Way Merge
    // inputFile - входной файл, outputFile - выходной файл, memoryLimitMB - лимит памяти в МБ
    static void externalSort(const std::string& inputFile, const std::string& outputFile, size_t memoryLimitMB = 100);

    // PARALLEL SORT

    // Параллельная быстрая сортировка
    // arr - вектор для сортировки, numThreads - количество потоков
    static void parallelQuickSort(std::vector<T>& arr, int numThreads = 4);

private:
    // Вспомогательные методы
    static int partition(std::vector<T>& arr, int left, int right);
    static void merge(std::vector<T>& arr, int left, int mid, int right);

    // Структура для K-Way Merge
    struct HeapNode {
        T value;          // значение элемента
        size_t chunkIdx;  // индекс чанка
        size_t pos;       // позиция в чанке

        // Оператор сравнения для min-heap
        bool operator>(const HeapNode& other) const;
    };

    // K-путевое слияние
    static void kWayMerge(const std::vector<std::string>& chunkFiles,
        const std::string& outputFile);
};

// Включение реализации шаблонов
#include "quicksort.h"
#include "mergesort.h"
#include "externalsort.h"
#include "parallelsort.h"

#endif // SORTER_H