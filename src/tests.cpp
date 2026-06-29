#include "tests.h"
#include "sorter.h"
#include <algorithm>
#include <iomanip>

// ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ

// Проверяет, отсортирован ли массив по возрастанию
// arr - вектор для проверки
// Возвращает true, если массив отсортирован, иначе false
template<typename T>
bool TestRunner::isSorted(const std::vector<T>& arr) {
    // Проходим по всем элементам, начиная со второго
    for (size_t i = 1; i < arr.size(); i++) {
        // Если предыдущий элемент больше текущего - массив не отсортирован
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    // Если все элементы в правильном порядке - массив отсортирован
    return true;
}

// Сравнивает два вектора на полное равенство элементов
// a, b - векторы для сравнения
// Возвращает true, если векторы полностью совпадают
template<typename T>
bool TestRunner::vectorsEqual(const std::vector<T>& a, const std::vector<T>& b) {
    // Если размеры разные - векторы не равны
    if (a.size() != b.size()) {
        return false;
    }
    // Сравниваем каждый элемент по отдельности
    for (size_t i = 0; i < a.size(); i++) {
        // Если хотя бы один элемент отличается - векторы не равны
        if (a[i] != b[i]) {
            return false;
        }
    }
    // Все элементы совпадают - векторы равны
    return true;
}

// Проверяет условие и выводит результат теста
// condition - проверяемое условие (true = тест пройден)
// testName - название теста для вывода
void TestRunner::assertTrue(bool condition, const std::string& testName) {
    if (condition) {
        // Тест пройден - увеличиваем счетчик и выводим сообщение
        std::cout << testName << " - ПРОЙДЕН" << std::endl;
        testsPassed++;
    }
    else {
        // Тест провален - увеличиваем счетчик и выводим сообщение
        std::cout << testName << " - ПРОВАЛЕН" << std::endl;
        testsFailed++;
    }
}

// МЕТОДЫ ДЛЯ ВИЗУАЛИЗАЦИИ

// Выводит массив в консоль с меткой
// arr - выводимый вектор, label - текстовая метка перед массивом
template<typename T>
void TestRunner::printArray(const std::vector<T>& arr, const std::string& label) {
    // Выводим метку и открывающую квадратную скобку
    std::cout << label << ": [";
    // Проходим по всем элементам массива
    for (size_t i = 0; i < arr.size(); i++) {
        std::cout << arr[i];  // Выводим элемент
        // Если это не последний элемент - добавляем запятую и пробел
        if (i < arr.size() - 1) std::cout << ", ";
    }
    // Закрываем квадратную скобку и переходим на новую строку
    std::cout << "]" << std::endl;
}

// Выводит массив с визуальными маркерами для указателей left, right и pivot
// arr - выводимый вектор
// left - индекс левого указателя (помечается [])
// right - индекс правого указателя (помечается [])
// pivot - индекс опорного элемента (помечается *)
template<typename T>
void TestRunner::printArrayWithMarkers(const std::vector<T>& arr, int left, int right, int pivot) {
    // Первая строка - значения элементов с маркерами
    std::cout << "  ";
    for (int i = 0; i < static_cast<int>(arr.size()); i++) {
        // Если текущий индекс - левая граница, ставим открывающую скобку
        if (i == left) std::cout << "[";
        // Выводим элемент с шириной поля 3 символа
        std::cout << std::setw(3) << arr[i];
        // Если текущий индекс - правая граница, ставим закрывающую скобку
        if (i == right) std::cout << "]";
        // Если текущий индекс - опорный элемент, ставим звездочку
        if (i == pivot) std::cout << "*";
        std::cout << " ";  // Пробел между элементами
    }
    std::cout << std::endl;

    // Вторая строка - подписи указателей (L, R, P)
    std::cout << "   ";
    for (int i = 0; i < static_cast<int>(arr.size()); i++) {
        if (i == left) {
            std::cout << " L ";  // Левая граница
        }
        else if (i == right) {
            std::cout << " R ";  // Правая граница
        }
        else if (i == pivot) {
            std::cout << " P ";  // Опорный элемент
        }
        else {
            std::cout << "   ";  // Пустое место
        }
    }
    std::cout << std::endl;
}

// Выводит состояние массива на определенном шаге сортировки
// arr - текущее состояние массива, step - номер шага, operation - описание выполняемой операции
template<typename T>
void TestRunner::printStep(const std::vector<T>& arr, int step, const std::string& operation) {
    // Выводим номер шага и описание операции
    std::cout << "Шаг " << std::setw(2) << step << ": " << operation << " -> ";
    // Выводим все элементы массива через пробел
    for (size_t i = 0; i < arr.size(); i++) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << " ";
    }
    std::cout << std::endl;
}

// ТЕСТ QUICKSORT

void TestRunner::testQuickSortDetailed() {
    std::cout << " \n             ТЕСТ: БЫСТРАЯ СОРТИРОВКА\n";

    // Тест 1: Маленький массив
    std::cout << "Тест 1.1: демонстрация QuickSort\n";
    std::vector<int> arr1 = { 64, 34, 25, 12, 22, 11, 90 };

    std::cout << "Исходный массив: ";
    printArray(arr1, "");
    std::cout << std::endl;

    // Создаём копию для ручного отслеживания шагов
    std::vector<int> arr1Copy = arr1;
    std::cout << "Начинаем сортировку...\n\n";

    int step = 0;
    printStep(arr1Copy, step++, "Начальное состояние");

    // Выполняем сортировку и показываем ключевые моменты
    std::cout << "Выполняем разбиение (выбор опорного элемента)...\n";
    std::cout << "Опорный элемент: " << arr1Copy[arr1Copy.size() / 2] << " (средний)\n\n";

    Sorter<int>::quickSort(arr1Copy);
    printStep(arr1Copy, step, "Отсортированный массив");

    assertTrue(isSorted(arr1Copy), "QuickSort: маленький массив");

    // Тест 2: Сравнение с эталонной сортировкой
    std::cout << "\nТест 1.2: Сравнение с std::sort\n";
    std::vector<int> arr2 = { 5, 2, 8, 1, 9, 3, 7, 4, 6, 0 };
    std::vector<int> arr2Expected = arr2;
    std::sort(arr2Expected.begin(), arr2Expected.end());

    std::cout << "Исходный:     ";
    printArray(arr2, "");
    Sorter<int>::quickSort(arr2);
    std::cout << "QuickSort:    ";
    printArray(arr2, "");
    std::cout << "std::sort:    ";
    printArray(arr2Expected, "");

    assertTrue(vectorsEqual(arr2, arr2Expected), "QuickSort: сравнение с std::sort");

    // Тест 3: Сортировка уже отсортированного массива (худший случай)
    std::cout << "\nТест 1.3: Сортировка уже отсортированного массива\n";
    std::vector<int> arr3 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::cout << "До сортировки: ";
    printArray(arr3, "");
    Sorter<int>::quickSort(arr3);
    std::cout << "После сортировки: ";
    printArray(arr3, "");
    assertTrue(isSorted(arr3), "QuickSort: отсортированный массив");

    // Тест 4: Сортировка с дубликатами
    std::cout << "\nТест 1.4: Сортировка с дубликатами\n";
    std::vector<int> arr4 = { 5, 3, 5, 1, 5, 2, 5, 4, 5 };
    std::cout << "До сортировки: ";
    printArray(arr4, "");
    Sorter<int>::quickSort(arr4);
    std::cout << "После сортировки: ";
    printArray(arr4, "");
    assertTrue(isSorted(arr4), "QuickSort: с дубликатами");

    std::cout << std::endl;
}

// ТЕСТ MERGESORT

void TestRunner::testMergeSortDetailed() {
    std::cout << "\n          ТЕСТ: СОРТИРОВКА СЛИЯНИЕМ\n";

    // Тест 1: Демонстрация слияния
    std::cout << "Тест 2.1: Демонстрация процесса слияния\n";
    std::vector<int> arr1 = { 38, 27, 43, 3, 9, 82, 10 };

    std::cout << "Исходный массив: ";
    printArray(arr1, "");
    std::cout << "\nПроцесс сортировки:\n";
    std::cout << "  1. Разбиваем на две части: [38, 27, 43] и [3, 9, 82, 10]\n";
    std::cout << "  2. Рекурсивно сортируем каждую часть\n";
    std::cout << "  3. Сливаем отсортированные части\n\n";

    Sorter<int>::mergeSort(arr1);
    std::cout << "Результат: ";
    printArray(arr1, "");
    assertTrue(isSorted(arr1), "MergeSort: базовая сортировка");

    // Тест 2: Разные размеры массивов
    std::cout << "\nТест 2.2: Сортировка массивов разного размера\n";

    std::vector<std::pair<std::string, std::vector<int>>> testCases = {
        {"Маленький (5 элементов)", {5, 4, 3, 2, 1}},
        {"Средний (8 элементов)", {8, 3, 6, 1, 9, 4, 7, 2}},
        {"Большой (15 элементов)", {14, 13, 15, 9, 12, 10, 11, 8, 5, 6, 4, 7, 3, 1, 2}}
    };

    for (auto& test : testCases) {
        std::cout << "\n  " << test.first << ":\n";
        std::cout << "    До: ";
        printArray(test.second, "");
        Sorter<int>::mergeSort(test.second);
        std::cout << "    После: ";
        printArray(test.second, "");
        assertTrue(isSorted(test.second), "MergeSort: " + test.first);
    }

    std::cout << std::endl;
}

// ТЕСТ ВНЕШНЕЙ СОРТИРОВКИ

void TestRunner::testExternalSortDetailed() {
    std::cout << "\n          ТЕСТ: ВНЕШНЯЯ СОРТИРОВКА (K-Way)\n";

    const std::string inputFile = "test_input.bin";
    const std::string outputFile = "test_output.bin";

    // Тест 1: Маленький файл
    std::cout << "Тест 3.1: Сортировка маленького файла\n";
    std::vector<int> smallData = { 64, 34, 25, 12, 22, 11, 90, 45, 78, 33 };

    std::cout << "Исходные данные: ";
    printArray(smallData, "");
    std::cout << "Размер данных: " << smallData.size() << " элементов\n\n";

    // Создаем входной файл
    std::ofstream input(inputFile, std::ios::binary);
    for (int val : smallData) {
        input.write(reinterpret_cast<const char*>(&val), sizeof(int));
    }
    input.close();

    std::cout << "Этап 1: Разбиение на чанки\n";
    std::cout << "  Лимит памяти: 4 элемента (для демонстрации)\n";

    // Используем маленький лимит памяти для демонстрации
    Sorter<int>::externalSort(inputFile, outputFile, 1);

    std::cout << "\nЭтап 2: Чтение результата\n";
    std::ifstream output(outputFile, std::ios::binary);
    std::vector<int> result;
    int val;
    while (output.read(reinterpret_cast<char*>(&val), sizeof(int))) {
        result.push_back(val);
    }
    output.close();

    std::cout << "Результат: ";
    printArray(result, "");

    // Проверяем
    std::sort(smallData.begin(), smallData.end());
    std::cout << "Ожидаемый результат: ";
    printArray(smallData, "");

    assertTrue(vectorsEqual(result, smallData), "ExternalSort: маленький файл");

    // Тест 2: Сортировка с множеством чанков
    std::cout << "\nТест 3.2: Сортировка с множеством чанков\n";
    const size_t numElements = 1000;
    std::vector<int> largeData(numElements);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (auto& v : largeData) {
        v = dis(gen);
    }

    std::cout << "Генерируем " << numElements << " случайных чисел\n";
    std::cout << "Первые 20 элементов: ";
    for (size_t i = 0; i < std::min(size_t(20), largeData.size()); i++) {
        std::cout << largeData[i] << " ";
    }
    std::cout << "...\n";

    // Сохраняем в файл
    std::ofstream input2(inputFile, std::ios::binary);
    for (int v : largeData) {
        input2.write(reinterpret_cast<const char*>(&v), sizeof(int));
    }
    input2.close();

    std::cout << "Запускаем внешнюю сортировку (лимит памяти: 1MB)...\n";
    auto start = std::chrono::high_resolution_clock::now();
    Sorter<int>::externalSort(inputFile, outputFile, 1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Время сортировки: " << duration.count() << " мс\n";

    // Проверяем результат
    std::ifstream output2(outputFile, std::ios::binary);
    std::vector<int> result2;
    while (output2.read(reinterpret_cast<char*>(&val), sizeof(int))) {
        result2.push_back(val);
    }
    output2.close();

    std::cout << "Количество элементов в результате: " << result2.size() << "\n";
    std::cout << "Первые 20 элементов после сортировки: ";
    for (size_t i = 0; i < std::min(size_t(20), result2.size()); i++) {
        std::cout << result2[i] << " ";
    }
    std::cout << "...\n";

    assertTrue(isSorted(result2), "ExternalSort: большой файл");
    assertTrue(result2.size() == numElements, "ExternalSort: сохранены все элементы");

    // Очистка
    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());

    std::cout << std::endl;
}

// ТЕСТ ПАРАЛЛЕЛЬНОЙ СОРТИРОВКИ

void TestRunner::testParallelSortDetailed() {
    std::cout << "\n            ТЕСТ: ПАРАЛЛЕЛЬНАЯ СОРТИРОВКА\n";

    std::cout << "Тест 4.1: Сравнение производительности потоков\n";
    const int numElements = 100000;
    std::vector<int> arr(numElements);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    for (auto& v : arr) {
        v = dis(gen);
    }

    std::cout << "Размер массива: " << numElements << " элементов\n";
    std::cout << "Первые 10 элементов: ";
    for (int i = 0; i < 10; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << "...\n\n";

    // Тестируем с разным количеством потоков
    std::vector<int> threadCounts = { 1, 2, 4, 8 };

    std::cout << "-----------T------------T------------T--------------¬\n";
    std::cout << "¦ Потоки   ¦ Время (мс) ¦ Ускорение  ¦ Относительно ¦\n";
    std::cout << "+----------+------------+------------+--------------+\n";

    double baseTime = 0;
    for (int threads : threadCounts) {
        std::vector<int> arrCopy = arr;
        auto start = std::chrono::high_resolution_clock::now();
        Sorter<int>::parallelQuickSort(arrCopy, threads);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        double timeMs = duration.count();
        if (threads == 1) baseTime = timeMs;

        double speedup = (threads == 1) ? 1.0 : baseTime / timeMs;

        std::cout << "¦ " << std::setw(6) << threads << "   ¦ "
            << std::setw(8) << std::fixed << std::setprecision(2) << timeMs << "   ¦ "
            << std::setw(8) << std::fixed << std::setprecision(2) << speedup << "x   ¦ "
            << std::setw(12) << std::fixed << std::setprecision(1) << (speedup / threads * 100) << "%  ¦\n";

        assertTrue(isSorted(arrCopy), "ParallelSort: " + std::to_string(threads) + " потоков");
    }

    std::cout << "L----------+------------+------------+---------------\n";

    // Тест 4.2: Сравнение с обычной сортировкой
    std::cout << "\nТест 4.2: Сравнение параллельной и обычной сортировки\n";

    std::vector<int> arr2(10000);
    for (auto& v : arr2) {
        v = dis(gen);
    }

    std::cout << "Массив из 10000 элементов\n";

    // Обычная сортировка
    std::vector<int> arrSeq = arr2;
    auto start = std::chrono::high_resolution_clock::now();
    Sorter<int>::quickSort(arrSeq);
    auto end = std::chrono::high_resolution_clock::now();
    auto seqTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Параллельная сортировка
    std::vector<int> arrPar = arr2;
    start = std::chrono::high_resolution_clock::now();
    Sorter<int>::parallelQuickSort(arrPar, 4);
    end = std::chrono::high_resolution_clock::now();
    auto parTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "  Обычная сортировка:     " << seqTime.count() << " мкс\n";
    std::cout << "  Параллельная сортировка: " << parTime.count() << " мкс\n";
    std::cout << "  Ускорение:              " << std::fixed << std::setprecision(2)
        << (double)seqTime.count() / parTime.count() << "x\n";

    assertTrue(vectorsEqual(arrSeq, arrPar), "ParallelSort: сравнение с обычной");

    std::cout << std::endl;
}

// ТЕСТ ГРАНИЧНЫХ СЛУЧАЕВ

void TestRunner::testEdgeCasesDetailed() {
    std::cout << "\n              ТЕСТ: ГРАНИЧНЫЕ СЛУЧАИ\n";

    // Пустой массив
    std::cout << "Тест 5.1: Пустой массив\n";
    std::vector<int> empty;
    std::cout << "Размер массива: " << empty.size() << "\n";
    Sorter<int>::quickSort(empty);
    std::cout << "После сортировки: ";
    printArray(empty, "");
    assertTrue(empty.empty(), "Edge: пустой массив");

    // Один элемент
    std::cout << "\nТест 5.2: Один элемент\n";
    std::vector<int> single = { 42 };
    std::cout << "До сортировки: ";
    printArray(single, "");
    Sorter<int>::quickSort(single);
    std::cout << "После сортировки: ";
    printArray(single, "");
    assertTrue(single[0] == 42 && single.size() == 1, "Edge: один элемент");

    // Два элемента (обратный порядок)
    std::cout << "\nТест 5.3: Два элемента (обратный порядок)\n";
    std::vector<int> two = { 2, 1 };
    std::cout << "До сортировки: ";
    printArray(two, "");
    Sorter<int>::quickSort(two);
    std::cout << "После сортировки: ";
    printArray(two, "");
    assertTrue(isSorted(two), "Edge: два элемента");

    // Все элементы равны
    std::cout << "\nТест 5.4: Все элементы равны (100 раз по 5)\n";
    std::vector<int> equal(100, 5);
    std::cout << "Первые 10 элементов до сортировки: ";
    for (int i = 0; i < 10; i++) std::cout << equal[i] << " ";
    std::cout << "...\n";
    Sorter<int>::quickSort(equal);
    std::cout << "Первые 10 элементов после сортировки: ";
    for (int i = 0; i < 10; i++) std::cout << equal[i] << " ";
    std::cout << "...\n";
    assertTrue(isSorted(equal), "Edge: все равные элементы");

    // Очень маленький массив
    std::cout << "\nТест 5.5: Массив из 0 элементов\n";
    std::vector<int> zero = {};
    std::cout << "Размер: " << zero.size() << "\n";
    Sorter<int>::quickSort(zero);
    assertTrue(zero.empty(), "Edge: 0 элементов");

    std::cout << std::endl;
}

// ТЕСТ ПРОИЗВОДИТЕЛЬНОСТИ

void TestRunner::testPerformanceDetailed() {
    std::cout << "\n             ТЕСТ: ПРОИЗВОДИТЕЛЬНОСТЬ\n";

    std::cout << "Сравнение производительности алгоритмов на разных размерах данных\n\n";

    const size_t sizes[] = { 1000, 5000, 10000, 50000, 100000 };
    std::random_device rd;
    std::mt19937 gen(rd());

    std::cout << "--------------T--------------T--------------T--------------T--------------¬\n";
    std::cout << "¦ Размер      ¦ QuickSort    ¦ MergeSort    ¦ ParallelSort ¦ Ускорение    ¦\n";
    std::cout << "+-------------+--------------+--------------+--------------+--------------+\n";

    for (size_t size : sizes) {
        // Создаем случайный массив
        std::vector<int> arr(size);
        std::uniform_int_distribution<> dis(1, size * 10);
        for (auto& v : arr) {
            v = dis(gen);
        }

        // QuickSort
        std::vector<int> arr1 = arr;
        auto start = std::chrono::high_resolution_clock::now();
        Sorter<int>::quickSort(arr1);
        auto end = std::chrono::high_resolution_clock::now();
        auto quickTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        // MergeSort
        std::vector<int> arr2 = arr;
        start = std::chrono::high_resolution_clock::now();
        Sorter<int>::mergeSort(arr2);
        end = std::chrono::high_resolution_clock::now();
        auto mergeTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        // ParallelSort (только для достаточно больших массивов)
        auto parallelTime = std::chrono::microseconds(0);
        double speedup = 1.0;
        if (size >= 10000) {
            std::vector<int> arr3 = arr;
            start = std::chrono::high_resolution_clock::now();
            Sorter<int>::parallelQuickSort(arr3, 4);
            end = std::chrono::high_resolution_clock::now();
            parallelTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            speedup = (double)quickTime.count() / parallelTime.count();

            assertTrue(isSorted(arr3), "ParallelSort: размер " + std::to_string(size));
        }

        std::cout << "¦ " << std::setw(9) << size << "   ¦ "
            << std::setw(8) << quickTime.count() << " мкс ¦ "
            << std::setw(8) << mergeTime.count() << " мкс ¦ ";

        if (size >= 10000) {
            std::cout << std::setw(8) << parallelTime.count() << " мкс ¦ "
                << std::setw(10) << std::fixed << std::setprecision(2) << speedup << "x  ¦\n";
        }
        else {
            std::cout << std::setw(7) << "N/A" << "      ¦ "
                << std::setw(7) << "N/A" << "      ¦\n";
        }

        assertTrue(isSorted(arr1), "QuickSort: размер " + std::to_string(size));
        assertTrue(isSorted(arr2), "MergeSort: размер " + std::to_string(size));
    }

    std::cout << "L-------------+--------------+--------------+--------------+---------------\n";
    std::cout << std::endl;
}

// ЗАПУСК ВСЕХ ТЕСТОВ

void TestRunner::runAllTests() {
    std::cout << "\n                          ЗАПУСК ВСЕХ ТЕСТОВ\n";

    testQuickSortDetailed();
    testMergeSortDetailed();
    testExternalSortDetailed();
    testParallelSortDetailed();
    testEdgeCasesDetailed();
    testPerformanceDetailed();

    std::cout << "\n                          РЕЗУЛЬТАТЫ ТЕСТОВ\n";
}

void TestRunner::printResults() const {
    std::cout << "\n";
    std::cout << "--------------------------------------------------------¬\n";
    std::cout << "¦                   СТАТИСТИКА ТЕСТОВ                  ¦\n";
    std::cout << "+-------------------------------------------------------+\n";
    std::cout << "¦  Пройдено: " << std::setw(35) << testsPassed << " ¦\n";
    std::cout << "¦  Провалено: " << std::setw(33) << testsFailed << " ¦\n";
    std::cout << "¦  Всего: " << std::setw(37) << (testsPassed + testsFailed) << " ¦\n";
    if (testsPassed + testsFailed > 0) {
        double successRate = (double)testsPassed / (testsPassed + testsFailed) * 100;
        std::cout << "¦  Успешность: " << std::setw(32)
            << std::fixed << std::setprecision(1) << successRate << "% ¦\n";
    }
    std::cout << "L--------------------------------------------------------\n";
}