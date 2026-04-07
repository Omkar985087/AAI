#include <bits/stdc++.h>
using namespace std;

int main() {
    srand(time(0));

    ofstream fout("input.txt");

    int K = rand() % 4 + 3;
    fout << K << "\n";

    vector<int> price(K);
    int base = rand() % 5 + 3;

    for (int i = 0; i < K; i++) {
        price[i] = base + i * (rand() % 5 + 2);
        fout << price[i] << " ";
    }
    fout << "\n";

    int n = rand() % 4 + 4;
    fout << n << "\n";

    for (int i = 1; i <= n; i++) {
        int arrival = rand() % 30;

        int departure = arrival + (rand() % 25 + 5);

        int charge = rand() % 20 + 5;

        fout << i << " " << arrival << " "
             << departure << " " << charge << "\n";
    }

    fout.close();

    return 0;
}