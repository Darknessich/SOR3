#pragma once

#ifndef SOR_3_H_INCLUDED__
#define SOR_3_H_INCLUDED__

typedef void (*sort_func_t)(int array[], int size);

// Семейства сортировок
typedef enum
{
  // (Не определено)
  SORT_NA = -1,
  // Квадратичные
  SORT_SELECTION, SORT_INSERTION, SORT_BUBBLE,
  // Субквадратичные
  SORT_SHELL,
  // Квазилинейные
  SORT_COMB, SORT_MERGE, SORT_QUICK, SORT_HEAP,
  // Линейные
  SORT_COUNT, SORT_RADIX, SORT_BUCKET,
} sort_family_t;

// Вычислительная сложность
typedef enum
{
  SORT_QUADRATIC,
  SORT_SUBQUADRATIC,
  SORT_QUASILINEAR,
  SORT_LINEAR
} sort_complexity_t;

// Описание одной вариации сортировки
typedef struct
{
  sort_func_t sort; // Функция сортировки
  sort_family_t family; // Базовое семейство
  sort_complexity_t complexity; // Вычислительная сложность
  char const* name; // Имя, описывающее вариацию
} sort_info_t;

__declspec(dllexport) sort_info_t* GetSortList(int* count);

#endif // !SOR_3_H_INCLUDED__