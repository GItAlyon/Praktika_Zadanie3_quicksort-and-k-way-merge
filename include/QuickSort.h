#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <vector>
#include <algorithm>

// Реализация разбиения для быстрой сортировки
// Выбирает средний элемент в качестве опорного
// Возвращает индекс опорного элемента
template<typename T>
int Sorter<T>::partition(std::vector<T>& arr, int left, int right) {
    // Выбираем средний элемент как опорный
    T pivot = arr[left + (right - left) / 2];
    int i = left;   // левый указатель
    int j = right;  // правый указатель

    // Пока указатели не пересеклись
    while (i <= j) {
        // Ищем элемент больше опорного слева
        while (arr[i] < pivot) {
            i++;
        }
        // Ищем элемент меньше опорного справа
        while (arr[j] > pivot) {
            j--;
        }
        // Если указатели встретились - выходим
        if (i >= j) {
            break;
        }
        // Меняем местами найденные элементы
        std::swap(arr[i++], arr[j--]);
    }

    // Возвращаем позицию опорного элемента
    return j;
}

// Рекурсивная реализация быстрой сортировки
template<typename T>
void Sorter<T>::quickSort(std::vector<T>& arr, int left, int right) {
    // Базовый случай: если подмассив пуст или содержит один элемент
    if (left >= right) {
        return;
    }

    // Разбиваем массив и получаем индекс опорного элемента
    int pivotIdx = partition(arr, left, right);

    // Рекурсивно сортируем левую часть
    quickSort(arr, left, pivotIdx);

    // Рекурсивно сортируем правую часть
    quickSort(arr, pivotIdx + 1, right);
}

// Быстрая сортировка всего массива
template<typename T>
void Sorter<T>::quickSort(std::vector<T>& arr) {
    // Проверяем, не пустой ли массив
    if (arr.empty()) {
        return;
    }
    // Запускаем сортировку с границами всего массива
    quickSort(arr, 0, static_cast<int>(arr.size()) - 1);
}

#endif // QUICKSORT_H