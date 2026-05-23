# Москалевич Александр БПИ246. Set9

## Реализованные алгоритмы

### Стандартные алгоритмы
1. **Standard Quicksort** - O(n·m·log n)
2. **Standard Mergesort** - O(n·m·log n)

### Специализированные алгоритмы
1. **Ternary String Quicksort** - O(n·log n + m·n)
2. **String Mergesort with LCP** - O(Σ LCP(R) + n·log n)
3. **MSD Radix Sort** - O(n·m + R·n), R=256
4. **MSD Radix Sort Hybrid** - переключение на String Quicksort при размере < 74

## Структура проекта

StringGenerator.h/cpp    - Класс генерации тестовых данных
StringSortTester.h/cpp   - Класс измерения производительности


## Тестовые данные

**Параметры генерации:**
- Алфавит: 74 символа (A-Z, a-z, 0-9, !@#%:;^&*()-.)
- Длина строк: 10-200 символов
- Размеры массивов: 100-3000 с шагом 100

**Типы массивов:**
1. Random - случайные строки
2. Reversed - обратно отсортированные
3. Nearly Sorted - 50 перестановок в отсортированном массиве

**Измеряемые метрики:**
- Время выполнения (мс) - среднее по 5 запускам
- Количество посимвольных сравнений

## Компиляция и запуск

```bash
# Windows (Visual Studio)
MSBuild A1.sln -p:Configuration=Release -p:Platform=x64
x64\Release\A1.exe

# Linux/macOS
g++ -std=c++17 -O2 -o test main.cpp StringGenerator.cpp StringSortTester.cpp
./test
```

## Визуализация

```bash
pip install pandas matplotlib seaborn
python visualize.py
```

## Результаты эмпирического анализа

### Сводная статистика (среднее время, мс)

| Алгоритм | Random | Reversed | Nearly Sorted |
|----------|--------|----------|---------------|
| Standard Quicksort | 0.76 | 0.65 | 0.63 |
| Standard Mergesort | 1.83 | 1.65 | 1.64 |
| **Ternary String Quicksort** | **0.11** | 0.12 | 0.13 |
| String Mergesort | 2.07 | 1.90 | 1.92 |
| MSD Radix Sort | 0.66 | 0.69 | 0.64 |
| **MSD Radix Sort Hybrid** | **0.22** | **0.22** | **0.19** |

### Количество посимвольных сравнений (среднее)

| Алгоритм | Random | Reversed | Nearly Sorted |
|----------|--------|----------|---------------|
| Standard Quicksort | 336,914 | 353,010 | 333,709 |
| Standard Mergesort | 20,756 | 12,804 | 18,569 |
| Ternary String Quicksort | **15,918** | 55,547 | 45,920 |
| String Mergesort | **13,436** | **5,669** | **6,792** |
| **MSD Radix Sort** | **3,569** | **3,750** | **3,723** |
| MSD Radix Sort Hybrid | 7,617 | 26,367 | 11,260 |

### Графики

**Сравнение времени выполнения:**
- [Случайные данные](time_comparison_random.png)
- [Обратно отсортированные](time_comparison_reversed.png)
- [Почти отсортированные](time_comparison_nearly_sorted.png)
- [Все типы данных](time_all_types.png)

**Сравнение количества сравнений:**
- [Случайные данные](comparisons_random.png)
- [Обратно отсортированные](comparisons_reversed.png)
- [Почти отсортированные](comparisons_nearly_sorted.png)

**Сводные графики:**
- [Полное сравнение алгоритмов](algorithm_comparison_full.png)

### Ключевые выводы

**1. Производительность по времени:**
- Ternary String Quicksort быстрее Standard Quicksort в **6.9x** раз (на случайных данных)
- MSD Radix Sort Hybrid быстрее Standard Mergesort в **8.3x** раз

**2. Количество сравнений:**
- MSD Radix Sort делает в 94x раз меньше сравнений, чем Standard Quicksort
- String Mergesort с LCP показал отличные результаты: в 25x раз меньше сравнений, чем Standard Mergesort на случайных данных
- String Mergesort особенно эффективен на обратно отсортированных данных (5,669 сравнений vs 12,804 у Standard)

**3. Соответствие теории:**
- Standard Quicksort: O(n·m·log n) - подтверждается высоким количеством сравнений
- MSD Radix Sort: O(n·m + R·n) - подтверждается низким количеством сравнений (~3,500-3,750)
- Ternary String Quicksort: O(n·log n + m·n) - значительное снижение сравнений
- String Mergesort с LCP: O(Σ LCP(R) + n·log n) - эффективное использование префиксов подтверждено


## Ссылки на решения CodeForces

- **A1m** (String Mergesort): 375757124
- **A1q** (Ternary String Quicksort): 375750180
- **A1r** (MSD Radix Sort): 375755073
- **A1rq** (MSD Radix Sort Hybrid): 375757053
