#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <sciplot/sciplot.hpp>

struct BenchmarkData {
    std::vector<double> sizes;
    std::vector<double> elements;
    std::vector<double> naive;
    std::vector<double> optimized;
};

bool ReadBenchmarkData(const std::string& filename, BenchmarkData& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << "\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        double size, elements, naive, optimized;
        if (ss >> size >> elements >> naive >> optimized) {
            data.sizes.push_back(size);
            data.elements.push_back(elements);
            data.naive.push_back(naive);
            data.optimized.push_back(optimized);
        }
    }

    file.close();
    return !data.sizes.empty();
}

int main(int argc, char* argv[]) {
    std::string filename = "benchmark_results.txt";

    if (argc > 1) {
        filename = argv[1];
    }

    std::cout << "Reading data from: " << filename << "\n";

    BenchmarkData data;
    if (!ReadBenchmarkData(filename, data)) {
        std::cerr << "No data found in file\n";
        return 1;
    }

    std::cout << "Loaded " << data.sizes.size() << " data points\n";

    sciplot::Plot2D plot1;
    plot1.drawCurve(data.sizes, data.naive).label("Naive").lineColor("red").lineWidth(2);
    plot1.drawCurve(data.sizes, data.optimized).label("Optimized").lineColor("blue").lineWidth(2);
    plot1.xlabel("Matrix Size (N x N)");
    plot1.ylabel("Time (ms)");
    plot1.grid();
    plot1.legend().hide();

    sciplot::Figure fig1 = {{plot1}};
    fig1.title("Time vs Matrix Size (Red = Naive, Blue = Optimized)");

    sciplot::Plot2D plot2;
    plot2.drawCurve(data.elements, data.naive).label("Naive").lineColor("red").lineWidth(2);
    plot2.drawCurve(data.elements, data.optimized).label("Optimized").lineColor("blue").lineWidth(2);
    plot2.xlabel("Number of Elements (N^2)");
    plot2.ylabel("Time (ms)");
    plot2.grid();
    plot2.legend().hide();

    sciplot::Figure fig2 = {{plot2}};
    fig2.title("Time vs Elements (Red = Naive, Blue = Optimized)");

    sciplot::Canvas canvas = {{fig1}, {fig2}};
    canvas.show();

    return 0;
}
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
~          
