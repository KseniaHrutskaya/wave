
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

// 112-Хруцкая. Матрично-волновой алгоритм

struct Point {
    double x, y;
};

// Расстояние между точками
double dist(Point a, Point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

int main() {
    vector<Point> p;
    ifstream file("points.dat");
    Point t;

    // Чтение точек из файла
    while (file >> t.x >> t.y)
        p.push_back(t);
    file.close();

    int n = p.size();
    if (n == 0) {
        cout << "Error: no points!" << endl;
        return 1;
    }

    double R = 0.8;

    // Матрица смежности
    vector<vector<int>> A(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i == j || dist(p[i], p[j]) <= R)
                A[i][j] = 1;

    // Начальная матрица волны
    vector<vector<int>> W = A;
    bool changed = true;
    int steps = 0;

    // Распространение волны до стабилизации
    while (changed) {
        changed = false;
        vector<vector<int>> next = W;
        for (int i = 0; i < n; i++)
            for (int k = 0; k < n; k++)
                if (W[i][k])
                    for (int j = 0; j < n; j++)
                        if (A[k][j] && !next[i][j]) {
                            next[i][j] = 1;
                            changed = true;
                        }
        W = next;
        steps++;
    }

    // Определение кластеров
    vector<int> cluster(n, -1);
    int m = 0;
    for (int i = 0; i < n; i++) {
        if (cluster[i] != -1) continue;
        for (int j = 0; j < n; j++)
            if (W[i][j]) cluster[j] = m;
        m++;
    }

    // Вычисление центров кластеров
    vector<double> sx(m, 0), sy(m, 0);
    vector<int> count(m, 0);
    for (int i = 0; i < n; i++) {
        int c = cluster[i];
        sx[c] += p[i].x;
        sy[c] += p[i].y;
        count[c]++;
    }

    // Сохранение результатов
    ofstream out("clusters.dat");
    ofstream labels("points-clusters.dat");
    ofstream matrix("wave-matrix.txt");

    cout << "Points: " << n << endl;
    cout << "Wave steps: " << steps << endl;
    cout << "Clusters: " << m << endl;

    for (int c = 0; c < m; c++) {
        double x = sx[c] / count[c], y = sy[c] / count[c];
        cout << c + 1 << ": " << x << " " << y << endl;
        out << x << " " << y << " " << count[c] << endl;
    }

    for (int i = 0; i < n; i++) {
        labels << p[i].x << " " << p[i].y
               << " " << cluster[i] + 1 << endl;
        for (int j = 0; j < n; j++)
            matrix << W[i][j] << " ";
        matrix << endl;
    }
    return 0;
}
