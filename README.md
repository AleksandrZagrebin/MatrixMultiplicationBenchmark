# Matrix Multiplication Benchmark

## Описание

Бенчмарк для сравнения производительности двух алгоритмов умножения квадратных матриц:

- **Наивный алгоритм** (O(n³))
- **Оптимизированный алгоритм** (с транспонированием). В C++ матрицы хранятся в строчно‑ориентированном порядке (row‑major). При наивном умножении доступ к элементам второй матрицы идёт по столбцам — это вызывает частые *cache misses* и замедляет работу.  


Измеряется зависимость времени выполнения от **количества элементов** в матрице.
Для квадратных матриц есть визулизация.
---

## Результаты

График показывает время выполнения двух алгоритмов в зависимости от количества элементов в матрице:

<img width="1439" height="759" alt="image" src="https://github.com/user-attachments/assets/10487e7e-a111-4651-ad74-aad2659c94dd" />

*Красная линия — наивный алгоритм, синяя — оптимизированный*

---

## Сборка и запуск

```bash
sudo apt install gnuplot
git clone https://github.com/sciplot/sciplot.git
cp -r sciplot/sciplot /путь/к/проекту/external/
git clone https://github.com/AleksandrZagrebin/matrix-benchmark.git
cd matrix-benchmark
mkdir build && cd build
cmake ..
make
./squarebench   # запуск бенчмарка
./plotter     # построение графика
./bench #для не квадратных матриц
