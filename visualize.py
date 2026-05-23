# -*- coding: utf-8 -*-
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import sys

# Fix encoding for Windows console
if sys.platform == 'win32':
    sys.stdout.reconfigure(encoding='utf-8')

# Настройка стиля
sns.set_style("whitegrid")
plt.rcParams['figure.figsize'] = (14, 8)

def load_all_results():
    """Загружает все CSV файлы с результатами"""
    files = [
        'results_standard_quicksort.csv',
        'results_standard_mergesort.csv',
        'results_string_quicksort.csv',
        'results_string_mergesort.csv',
        'results_msd_radix.csv',
        'results_msd_radix_hybrid.csv'
    ]

    dfs = []
    for file in files:
        if os.path.exists(file):
            df = pd.read_csv(file)
            dfs.append(df)
        else:
            print(f"Warning: {file} not found")

    if dfs:
        return pd.concat(dfs, ignore_index=True)
    return None

def plot_time_comparison(df, data_type):
    """График времени выполнения для конкретного типа данных"""
    plt.figure(figsize=(14, 8))

    subset = df[df['type'] == data_type]

    for algo in subset['algorithm'].unique():
        algo_data = subset[subset['algorithm'] == algo]
        plt.plot(algo_data['size'], algo_data['time_ms'], marker='o', label=algo, linewidth=2)

    plt.xlabel('Размер массива (количество строк)', fontsize=12)
    plt.ylabel('Время выполнения (мс)', fontsize=12)
    plt.title(f'Сравнение времени выполнения алгоритмов сортировки\nТип данных: {data_type}', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(f'time_comparison_{data_type}.png', dpi=300)
    plt.close()

def plot_comparisons(df, data_type):
    """График количества посимвольных сравнений"""
    plt.figure(figsize=(14, 8))

    subset = df[df['type'] == data_type]

    for algo in subset['algorithm'].unique():
        algo_data = subset[subset['algorithm'] == algo]
        plt.plot(algo_data['size'], algo_data['comparisons'], marker='o', label=algo, linewidth=2)

    plt.xlabel('Размер массива (количество строк)', fontsize=12)
    plt.ylabel('Количество посимвольных сравнений', fontsize=12)
    plt.title(f'Сравнение количества посимвольных сравнений\nТип данных: {data_type}', fontsize=14)
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(f'comparisons_{data_type}.png', dpi=300)
    plt.close()

def plot_all_types_time(df):
    """График времени для всех типов данных на одном графике"""
    fig, axes = plt.subplots(1, 3, figsize=(20, 6))

    data_types = ['random', 'reversed', 'nearly_sorted']
    titles = ['Случайные данные', 'Обратно отсортированные', 'Почти отсортированные']

    for ax, dtype, title in zip(axes, data_types, titles):
        subset = df[df['type'] == dtype]

        for algo in subset['algorithm'].unique():
            algo_data = subset[subset['algorithm'] == algo]
            ax.plot(algo_data['size'], algo_data['time_ms'], marker='o', label=algo, linewidth=2)

        ax.set_xlabel('Размер массива', fontsize=11)
        ax.set_ylabel('Время (мс)', fontsize=11)
        ax.set_title(title, fontsize=12)
        ax.legend(fontsize=8)
        ax.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig('time_all_types.png', dpi=300)
    plt.close()

def plot_algorithm_comparison(df):
    """Сравнение стандартных и специализированных алгоритмов"""
    fig, axes = plt.subplots(2, 3, figsize=(20, 12))

    data_types = ['random', 'reversed', 'nearly_sorted']
    metrics = ['time_ms', 'comparisons']
    metric_labels = ['Время выполнения (мс)', 'Количество сравнений']

    for i, metric in enumerate(metrics):
        for j, dtype in enumerate(data_types):
            ax = axes[i, j]
            subset = df[df['type'] == dtype]

            for algo in subset['algorithm'].unique():
                algo_data = subset[subset['algorithm'] == algo]
                ax.plot(algo_data['size'], algo_data[metric], marker='o', label=algo, linewidth=2)

            ax.set_xlabel('Размер массива', fontsize=10)
            ax.set_ylabel(metric_labels[i], fontsize=10)
            ax.set_title(f'{dtype.replace("_", " ").title()}', fontsize=11)
            ax.legend(fontsize=7)
            ax.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig('algorithm_comparison_full.png', dpi=300)
    plt.close()

def plot_speedup_heatmap(df):
    """Тепловая карта ускорения специализированных алгоритмов"""
    standard_algos = ['standard_quicksort', 'standard_mergesort']
    specialized_algos = ['ternary_string_quicksort', 'string_mergesort', 'msd_radix_sort', 'msd_radix_sort_hybrid']

    data_types = df['type'].unique()

    speedup_data = []

    for dtype in data_types:
        for spec_algo in specialized_algos:
            for std_algo in standard_algos:
                subset_std = df[(df['type'] == dtype) & (df['algorithm'] == std_algo)]
                subset_spec = df[(df['type'] == dtype) & (df['algorithm'] == spec_algo)]

                if not subset_std.empty and not subset_spec.empty:
                    avg_time_std = subset_std['time_ms'].mean()
                    avg_time_spec = subset_spec['time_ms'].mean()
                    speedup = avg_time_std / avg_time_spec if avg_time_spec > 0 else 0

                    speedup_data.append({
                        'data_type': dtype,
                        'comparison': f'{spec_algo} vs {std_algo}',
                        'speedup': speedup
                    })

    if speedup_data:
        speedup_df = pd.DataFrame(speedup_data)
        pivot = speedup_df.pivot(index='comparison', columns='data_type', values='speedup')

        plt.figure(figsize=(10, 8))
        sns.heatmap(pivot, annot=True, fmt='.2f', cmap='RdYlGn', center=1.0,
                    cbar_kws={'label': 'Ускорение (>1 = быстрее)'})
        plt.title('Ускорение специализированных алгоритмов относительно стандартных', fontsize=12)
        plt.tight_layout()
        plt.savefig('speedup_heatmap.png', dpi=300)
        plt.close()

def generate_summary_table(df):
    """Генерирует сводную таблицу результатов"""
    summary = df.groupby(['algorithm', 'type']).agg({
        'time_ms': ['mean', 'std'],
        'comparisons': ['mean', 'std']
    }).round(2)

    summary.to_csv('summary_statistics.csv')
    print("\nСводная статистика сохранена в summary_statistics.csv")
    print(summary)

def main():
    print("Загрузка результатов...")
    df = load_all_results()

    if df is None or df.empty:
        print("Ошибка: не найдены файлы с результатами")
        return

    print(f"Загружено {len(df)} записей")
    print(f"Алгоритмы: {df['algorithm'].unique()}")
    print(f"Типы данных: {df['type'].unique()}")

    print("\nГенерация графиков...")

    # Графики для каждого типа данных
    for dtype in df['type'].unique():
        print(f"  - Графики для {dtype}")
        plot_time_comparison(df, dtype)
        plot_comparisons(df, dtype)

    # Сводные графики
    print("  - Сводные графики")
    plot_all_types_time(df)
    plot_algorithm_comparison(df)
    plot_speedup_heatmap(df)

    # Статистика
    print("\nГенерация статистики...")
    generate_summary_table(df)

    print("\n✓ Все графики и статистика сохранены!")
    print("\nСозданные файлы:")
    print("  - time_comparison_*.png (3 файла)")
    print("  - comparisons_*.png (3 файла)")
    print("  - time_all_types.png")
    print("  - algorithm_comparison_full.png")
    print("  - speedup_heatmap.png")
    print("  - summary_statistics.csv")

if __name__ == "__main__":
    main()
