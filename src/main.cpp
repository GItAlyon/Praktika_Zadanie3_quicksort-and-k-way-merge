#include "sorter.h"
#include "tests.h"
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>

// Вспомогательная функция для проверки сортировки
template<typename T>
bool isSorted(const std::vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

// Демонстрация примеров использования
void demonstrateUsage() {
    std::cout << "\n                      ПРИМЕРЫ ПРИМЕНЕНИЯ\n";

    // Пример 1: Сортировка вектора
    std::cout << "Пример 1: Сортировка вектора целых чисел\n";
    std::vector<int> data = { 5, 2, 8, 1, 9, 3, 7, 4, 6 };

    std::cout << "\nИсходный массив: [";
    for (size_t i = 0; i < data.size(); i++) {
        std::cout << data[i];
        if (i < data.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n\n";

    // Сортируем с демонстрацией
    std::cout << "Применяем быструю сортировку...\n";
    std::cout << "  - Выбираем опорный элемент (средний)\n";
    std::cout << "  - Разбиваем массив на две части\n";
    std::cout << "  - Рекурсивно сортируем каждую часть\n\n";

    Sorter<int>::quickSort(data);

    std::cout << "Результат: [";
    for (size_t i = 0; i < data.size(); i++) {
        std::cout << data[i];
        if (i < data.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n\n";

    // Пример 2: Сортировка чисел с плавающей точкой
    std::cout << "Пример 2: Сортировка чисел с плавающей точкой (MergeSort)\n";
    std::vector<double> doubles = { 3.14, 1.41, 2.71, 0.57, 1.73, 2.18, 0.99 };

    std::cout << "Исходный: [";
    for (size_t i = 0; i < doubles.size(); i++) {
        std::cout << std::fixed << std::setprecision(2) << doubles[i];
        if (i < doubles.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";

    Sorter<double>::mergeSort(doubles);

    std::cout << "Результат: [";
    for (size_t i = 0; i < doubles.size(); i++) {
        std::cout << std::fixed << std::setprecision(2) << doubles[i];
        if (i < doubles.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n\n";

    // Пример 3: Сравнение производительности на большом массиве
    std::cout << "Пример 3: Сравнение производительности на 100000 элементах\n";
    std::vector<int> largeData(100000);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    for (auto& val : largeData) {
        val = dis(gen);
    }

    std::cout << "Сгенерировано " << largeData.size() << " случайных чисел\n";
    std::cout << "Первые 10 элементов: ";
    for (int i = 0; i < 10; i++) {
        std::cout << largeData[i] << " ";
    }
    std::cout << "...\n\n";

    // Обычная сортировка
    std::vector<int> seqData = largeData;
    auto start = std::chrono::high_resolution_clock::now();
    Sorter<int>::quickSort(seqData);
    auto end = std::chrono::high_resolution_clock::now();
    auto seqTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Параллельная сортировка
    std::vector<int> parData = largeData;
    start = std::chrono::high_resolution_clock::now();
    Sorter<int>::parallelQuickSort(parData, 4);
    end = std::chrono::high_resolution_clock::now();
    auto parTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Результаты:\n";
    std::cout << "  Обычная сортировка:     " << seqTime.count() << " мс\n";
    std::cout << "  Параллельная сортировка: " << parTime.count() << " мс\n";
    std::cout << "  Ускорение:              " << std::fixed << std::setprecision(2)
        << (double)seqTime.count() / parTime.count() << "x\n";
    std::cout << "  Корректность:           " << (isSorted(parData) ? "ДА" : "НЕТ") << "\n\n";

    // Пример 4: Внешняя сортировка
    std::cout << "Пример 4: Внешняя сортировка (K-Way Merge)\n";
    const std::string inputFile = "example_input.bin";
    const std::string outputFile = "example_output.bin";

    std::cout << "Создание файла с 100000 случайными числами...\n";
    std::ofstream input(inputFile, std::ios::binary);
    for (int i = 0; i < 100000; i++) {
        int val = dis(gen);
        input.write(reinterpret_cast<const char*>(&val), sizeof(int));
    }
    input.close();

    std::cout << "Запуск внешней сортировки (лимит памяти: 1MB)...\n";
    start = std::chrono::high_resolution_clock::now();
    Sorter<int>::externalSort(inputFile, outputFile, 1);
    end = std::chrono::high_resolution_clock::now();
    auto extTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Время сортировки: " << extTime.count() << " мс\n";

    // Проверка результата
    std::ifstream output(outputFile, std::ios::binary);
    std::vector<int> result;
    int val;
    while (output.read(reinterpret_cast<char*>(&val), sizeof(int))) {
        result.push_back(val);
    }
    output.close();

    std::cout << "Размер результата: " << result.size() << " элементов\n";
    std::cout << "Первые 20 элементов после сортировки: ";
    for (int i = 0; i < std::min(20, (int)result.size()); i++) {
        std::cout << result[i] << " ";
    }
    std::cout << "...\n";
    std::cout << "Корректность: " << (isSorted(result) ? "ДА" : "НЕТ") << "\n";

    // Очистка
    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());

    std::cout << "\n";
}

// Главная функция
int main() {
    setlocale(LC_ALL, "rus");

    // Запуск всех тестов
    TestRunner runner;
    runner.runAllTests();
    runner.printResults();

    // Демонстрация примеров использования
    demonstrateUsage();

    return 0;
}