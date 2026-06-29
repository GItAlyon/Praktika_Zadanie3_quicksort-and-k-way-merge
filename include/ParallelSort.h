#ifndef PARALLELSORT_H
#define PARALLELSORT_H

#include <vector>
#include <thread>
#include <algorithm>

// Параллельная быстрая сортировка с использованием потоков
template<typename T>
void Sorter<T>::parallelQuickSort(std::vector<T>& arr, int numThreads) {
    // Для маленьких массивов используем обычную сортировку
    if (arr.size() < 10000) {
        quickSort(arr);
        return;
    }

    // Находим середину массива
    int mid = static_cast<int>(arr.size() / 2);

    // Разделяем массив на две части
    std::vector<T> left(arr.begin(), arr.begin() + mid);
    std::vector<T> right(arr.begin() + mid, arr.end());

    // Создаем потоки для параллельной сортировки
    std::vector<std::thread> threads;
    threads.emplace_back([&]() { quickSort(left); });   // Сортируем левую часть
    threads.emplace_back([&]() { quickSort(right); });  // Сортируем правую часть

    // Ждем завершения обоих потоков
    for (auto& t : threads) {
        t.join();
    }

    // Слияние двух отсортированных частей
    std::merge(left.begin(), left.end(),
        right.begin(), right.end(),
        arr.begin());
}

#endif // PARALLELSORT_H