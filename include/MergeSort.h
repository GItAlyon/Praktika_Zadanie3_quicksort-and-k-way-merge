#ifndef MERGESORT_H
#define MERGESORT_H

#include <vector>

// Слияние двух отсортированных частей массива
// left - начало левой части, mid - конец левой части и начало правой, right - конец правой части
template<typename T>
void Sorter<T>::merge(std::vector<T>& arr, int left, int mid, int right) {
    // Временный массив для хранения результата слияния
    std::vector<T> temp(right - left + 1);
    int i = left;      // указатель для левой части
    int j = mid + 1;   // указатель для правой части
    int k = 0;         // указатель для временного массива

    // Слияние двух отсортированных половин
    while (i <= mid && j <= right) {
        // Сравниваем элементы и выбираем меньший
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }

    // Копирование оставшихся элементов из левой половины
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // Копирование оставшихся элементов из правой половины
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    // Копирование отсортированных элементов обратно в исходный массив
    for (int idx = 0; idx < k; idx++) {
        arr[left + idx] = temp[idx];
    }
}

// Рекурсивная реализация сортировки слиянием
template<typename T>
void Sorter<T>::mergeSort(std::vector<T>& arr, int left, int right) {
    // Базовый случай: если подмассив пуст или содержит один элемент
    if (left >= right) {
        return;
    }

    // Находим середину массива
    int mid = left + (right - left) / 2;

    // Рекурсивно сортируем левую половину
    mergeSort(arr, left, mid);

    // Рекурсивно сортируем правую половину
    mergeSort(arr, mid + 1, right);

    // Сливаем отсортированные половины
    merge(arr, left, mid, right);
}

// Сортировка слиянием всего массива
template<typename T>
void Sorter<T>::mergeSort(std::vector<T>& arr) {
    // Проверяем, не пустой ли массив
    if (arr.empty()) {
        return;
    }
    // Запускаем сортировку с границами всего массива
    mergeSort(arr, 0, static_cast<int>(arr.size()) - 1);
}

#endif // MERGESORT_H