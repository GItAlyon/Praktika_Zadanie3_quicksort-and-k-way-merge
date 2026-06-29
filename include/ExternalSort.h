#ifndef EXTERNALSORT_H
#define EXTERNALSORT_H

#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <algorithm>
#include <thread>
#include <mutex>

// Оператор сравнения для узла кучи (для min-heap)
template<typename T>
bool Sorter<T>::HeapNode::operator>(const HeapNode& other) const {
    return value > other.value;  // Сравниваем по значению
}

// K-путевое слияние отсортированных чанков
template<typename T>
void Sorter<T>::kWayMerge(const std::vector<std::string>& chunkFiles,
    const std::string& outputFile) {
    // Открываем все чанки для чтения
    std::vector<std::ifstream> chunkStreams;

    // Открываем каждый файл-чанк
    for (const auto& file : chunkFiles) {
        chunkStreams.emplace_back(file, std::ios::binary);
    }

    // Создаем min-heap для K-way merge
    std::priority_queue<HeapNode, std::vector<HeapNode>, std::greater<HeapNode>> heap;

    // Инициализация кучи первыми элементами из каждого чанка
    for (size_t i = 0; i < chunkStreams.size(); i++) {
        T value;
        // Пытаемся прочитать первый элемент из чанка
        if (chunkStreams[i].read(reinterpret_cast<char*>(&value), sizeof(T))) {
            // Если успешно - добавляем в кучу
            heap.push({ value, i, 0 });
        }
    }

    // Открываем выходной файл для записи
    std::ofstream output(outputFile, std::ios::binary);
    if (!output.is_open()) {
        throw std::runtime_error("Cannot open output file");
    }

    // Буфер для записи (1MB)
    std::vector<T> writeBuffer;
    writeBuffer.reserve(1024 * 1024 / sizeof(T));

    // Основной цикл слияния
    while (!heap.empty()) {
        // Извлекаем минимальный элемент из кучи
        HeapNode node = heap.top();
        heap.pop();

        // Добавляем элемент в буфер записи
        writeBuffer.push_back(node.value);

        // Если буфер заполнился - записываем на диск
        if (writeBuffer.size() >= writeBuffer.capacity()) {
            output.write(reinterpret_cast<const char*>(writeBuffer.data()),
                writeBuffer.size() * sizeof(T));
            writeBuffer.clear();  // Очищаем буфер
        }

        // Читаем следующий элемент из того же чанка
        size_t idx = node.chunkIdx;
        T nextValue;
        if (chunkStreams[idx].read(reinterpret_cast<char*>(&nextValue), sizeof(T))) {
            // Если есть еще элементы - добавляем в кучу
            heap.push({ nextValue, idx, node.pos + 1 });
        }
    }

    // Запись оставшихся данных из буфера
    if (!writeBuffer.empty()) {
        output.write(reinterpret_cast<const char*>(writeBuffer.data()),
            writeBuffer.size() * sizeof(T));
    }

    // Закрываем все файлы
    for (auto& stream : chunkStreams) {
        stream.close();
    }
    output.close();
}

// Внешняя сортировка с использованием K-Way Merge
template<typename T>
void Sorter<T>::externalSort(const std::string& inputFile,
    const std::string& outputFile,
    size_t memoryLimitMB) {
    // Вычисляем лимит памяти в количестве элементов
    const size_t memoryLimit = memoryLimitMB * 1024 * 1024 / sizeof(T);

    // Фаза 1: Разбиение на чанки
    std::ifstream input(inputFile, std::ios::binary);
    if (!input.is_open()) {
        throw std::runtime_error("Cannot open input file");
    }

    std::vector<std::string> chunkFiles;  // Список файлов-чанков
    std::vector<T> buffer;                // Буфер для текущего чанка
    buffer.reserve(memoryLimit);

    T value;          // Текущее значение
    size_t chunkCount = 0;  // Счётчик чанков

    // Читаем данные из входного файла
    while (input.read(reinterpret_cast<char*>(&value), sizeof(T))) {
        buffer.push_back(value);

        // Если буфер заполнился - сохраняем чанк
        if (buffer.size() >= memoryLimit) {
            // Сортируем чанк с помощью быстрой сортировки
            quickSort(buffer);

            // Записываем отсортированный чанк на диск
            std::string chunkName = "chunk_" + std::to_string(chunkCount++) + ".bin";
            std::ofstream chunk(chunkName, std::ios::binary);
            chunk.write(reinterpret_cast<const char*>(buffer.data()),
                buffer.size() * sizeof(T));
            chunk.close();

            // Добавляем имя чанка в список
            chunkFiles.push_back(chunkName);
            buffer.clear();  // Очищаем буфер для следующего чанка
        }
    }

    // Обработка последнего чанка (который может быть неполным)
    if (!buffer.empty()) {
        quickSort(buffer);  // Сортируем последний чанк
        std::string chunkName = "chunk_" + std::to_string(chunkCount++) + ".bin";
        std::ofstream chunk(chunkName, std::ios::binary);
        chunk.write(reinterpret_cast<const char*>(buffer.data()),
            buffer.size() * sizeof(T));
        chunk.close();
        chunkFiles.push_back(chunkName);
    }

    input.close();  // Закрываем входной файл

    // Если не было создано ни одного чанка (пустой файл)
    if (chunkFiles.empty()) {
        std::ofstream output(outputFile, std::ios::binary);
        output.close();
        return;
    }

    // Фаза 2: K-путевое слияние отсортированных чанков
    kWayMerge(chunkFiles, outputFile);

    // Очистка временных файлов
    for (const auto& file : chunkFiles) {
        std::remove(file.c_str());
    }
}

#endif // EXTERNALSORT_H