#pragma warning(disable : 4996)

#include <time.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <Windows.h>
#include "SOR3.h"
#include "pch.h"

#define MIN_RANDOM 0
#define MAX_RANDOM 9999

#define MIN_SIZE 1000
#define MAX_SIZE 10000
#define SIZE_STEP 100
#define REPEAT_COUNT 100

// Функция сравнения двух целый чисел для qsort
int MyCompare(const void* i, const void* j)
{
  return *(int*)i - *(int*)j;
}

// Эталонная сортировка 
void QuickSort(int* arr, int size)
{
  qsort(arr, size, sizeof(int), &MyCompare);
}

// Функция для проверки совпадения массивов
isСoincide(int* arr1, int* arr2, int size) {
  for (int i = 0; i < size; i++)
    if (arr1[i] != arr2[i])
      return 0;
  return 1;
}

// Для адресса функции "GetSortList"
typedef sort_info_t* (*dll_function_t)(int*);

// Заполнение массива случайными числами
void FillRandomArr(int* Arr, int size, int min, int max)
{
  for (int i = 0; i < size; i++)
    Arr[i] = rand() % (max - min) + min;
}

int main(void)
{
  setlocale(0, "");
  LARGE_INTEGER performance_frequency;
  QueryPerformanceFrequency(&performance_frequency);
  // Инициализация библиотеки
  HMODULE handle;
  if ((handle = LoadLibrary(TEXT("SOR3.dll"))) == NULL) {
    printf("Load Library Error");
    return -1;
  }

  // Адресс функции "GetSortList"
  dll_function_t ptrGetSortList;
  ptrGetSortList = (dll_function_t)GetProcAddress(handle, "GetSortList");

  // Информация о функциях сортировки
  int count;
  sort_info_t* sortList = ptrGetSortList(&count);
  sort_func_t sortFunction;

  // Сбор и запись данных в формат .csv
  FILE* pfile;

  // Открытие/Создание файла .csv
  if ((pfile = fopen("result.csv", "w")) == NULL) {
    printf("File Open Error");
    return -2;
  }

  // Шапка таблицы
  fprintf(pfile, "N");
  for (int i = 0; i < count; i++)
    fprintf(pfile, "; %s", sortList[i].name);
  fprintf(pfile, "\n");

  srand((unsigned int)time(NULL));
  LARGE_INTEGER start, end;
  for (int j = MIN_SIZE; j <= MAX_SIZE; j += SIZE_STEP) {
    // Записываем число элементов массива в первую колонку
    fprintf(pfile, "%i", j);
    // Массив, заполняемый случайными значениями
    int* arr = (int*)malloc(j * sizeof(int));
    // Массив, сортируемый. (Копия arr)
    int* cpy = (int*)malloc(j * sizeof(int));
    if (arr == NULL || cpy == NULL) {
      printf("Memory Alloc Error");
      return -3;
    }
    // Заполнение случайными числами
    FillRandomArr(arr, j, MIN_RANDOM, MAX_RANDOM);
    for (int i = 0; i < count; i++) {
      // Функция Сортировки
      sortFunction = sortList[i].sort;
      // Начинаем замерять время
      QueryPerformanceCounter(&start);
      for (int k = 0; k < REPEAT_COUNT; k++) {
        // Копируем массив
        memcpy(cpy, arr, j * sizeof(int));
        // Сортируем
        sortFunction(cpy, j);
      }
      QueryPerformanceCounter(&end);
      // Информация о ходе процесса номер выполненой функции
      printf("%i ", i);
      // Записываем время (мс) в соответствующую колонку
      fprintf(pfile, "; %f", ((end.QuadPart - start.QuadPart) * 1000.0) / ((double)REPEAT_COUNT * performance_frequency.QuadPart));
      // Проверяем отсортирован ли массив (только в Debug)
#if defined(_DEBUG) || !defined(NDEBUG)
      int* etalon = (int*)malloc(j * sizeof(int));
      if (etalon == NULL) {
        printf("Memory Alloc Error");
        return -3;
      }
      memcpy(etalon, arr, j * sizeof(int));
      QuickSort(etalon, j);

      if (!isСoincide(cpy, etalon, j)) {
        printf("Func #%i : An array of size %i was not sorted", i, j);
        return -4;
      }
      free(etalon);
#endif
    }
    // Информация о ходе процесса размер сортированного массива
    printf("%i\n", j);
    fprintf(pfile, "\n");
    free(arr);
    free(cpy);
  }

  fclose(pfile);
  FreeLibrary(handle);
  return 0;
}