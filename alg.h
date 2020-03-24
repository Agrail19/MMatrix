#include <algorithm>
#include <utility>
#include <QVector>
#include <iostream>
#include <QDebug>

using namespace std;


namespace KRS {
    QVector<QVector<int>> kruskal(int N, QVector<QVector<int>> const &matrix)
    {
        int M = 0;
        QVector<QVector<int>> res(N, QVector<int>(N, 0));
        QVector<pair<int, pair<int, int>>> g;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j <= i; j++)
            {
                if (i == j) {
                    if (matrix[i][j] != 0) {
                        qDebug() << "Error 1!";
                        QVector<QVector<int>> neres;
                        return neres;
                    }
                }
                else
                {
                    if ((matrix[i][j] != 1 && matrix[j][i] != 1) && (matrix[i][j] != 0 && matrix[j][i] != 0)) {
                        qDebug() << "Error 2!";
                        QVector<QVector<int>> neres;
                        return neres;
                    }
                    else
                    {
                        if (matrix[i][j] != 0)
                        {
                            g.push_back(make_pair(matrix[i][j], make_pair(i, j)));
                            M++;
                        }
                    }
                }
            }
        }

        QVector<int> colors(N);
        for (int i = 0; i < N; i++)
            colors[i] = i;
        for (int i = 0; i < M; i++)
        {
            int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
            if (colors[a] != colors[b])
            {
                res[a][b] = l;
                res[b][a] = l;
                int old_c = colors[b], new_c = colors[a];
                for (int j = 0; j < N; j++)
                {
                    if (colors[j] == old_c)
                        colors[j] = new_c;
                }
            }

        }

        bool check = false;
        for (int i = 0; i < N - 1; i++) {
            if (colors[i] != colors[i + 1])
                check = true;
        }
        if (check) {
            qDebug() << "Error 3!";
            QVector<QVector<int>> neres;
            return neres;
        }

        return res;
    }
}
