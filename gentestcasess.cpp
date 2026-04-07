#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(0));

    ofstream fout("input.txt");

    int K = 1 + rand() % 10;
    fout << K << endl;

    int current_price = 5 + rand() % 10;
    for(int i = 0; i < K; i++) {
        current_price += rand() % 10 + 1;
        fout << current_price << " ";
    }
    fout << endl;

    int N = 1 + rand() % 20;
    fout << N << endl;

    for(int i = 0; i < N; i++) {
        int arrival = rand() % 50;
        int departure = arrival + 1 + rand() % 50;
        int charge = 1 + rand() % 30;

        fout << i+1 << " "
             << arrival << " "
             << departure << " "
             << charge << endl;
    }

    fout.close();

    cout << "input.txt generated successfully.\n";

    return 0;
}