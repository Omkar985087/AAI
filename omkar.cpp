#include <z3++.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace z3;
using namespace std;

int main() {
    ifstream fin("input.txt");

    int K;
    fin >> K;

    vector<int> price(K);
    for(int i = 0; i < K; i++)
        fin >> price[i];

    int N;
    fin >> N;

    vector<int> id(N), arrival(N), departure(N), charge(N);
    for(int i = 0; i < N; i++) {
        fin >> id[i] >> arrival[i] >> departure[i] >> charge[i];
    }

    context c;
    optimize opt(c);

    vector<expr> port;
    vector<expr> start;

    for(int i = 0; i < N; i++) {
        port.push_back(c.int_const(("port_" + to_string(i)).c_str()));
        start.push_back(c.int_const(("start_" + to_string(i)).c_str()));
    }

    for(int i = 0; i < N; i++) {
        opt.add(port[i] >= 0 && port[i] < K);

        expr charge_time = c.int_val(0);
        for(int k = 0; k < K; k++) {
            int ct = (charge[i] + k) / (k + 1);
            charge_time = ite(port[i] == k, c.int_val(ct), charge_time);
        }

        opt.add(start[i] >= arrival[i]);
        opt.add(start[i] + charge_time <= departure[i]);
    }

    for(int i = 0; i < N; i++) {
        for(int j = i + 1; j < N; j++) {

            expr charge_i = c.int_val(0);
            expr charge_j = c.int_val(0);

            for(int k = 0; k < K; k++) {
                int ct_i = (charge[i] + k) / (k + 1);
                int ct_j = (charge[j] + k) / (k + 1);

                charge_i = ite(port[i] == k, c.int_val(ct_i), charge_i);
                charge_j = ite(port[j] == k, c.int_val(ct_j), charge_j);
            }

            opt.add(implies(port[i] == port[j],
                (start[i] + charge_i <= start[j]) ||
                (start[j] + charge_j <= start[i])
            ));
        }
    }

    expr total_cost = c.int_val(0);

    for(int i = 0; i < N; i++) {
        expr cost = c.int_val(0);

        for(int k = 0; k < K; k++) {
            int ct = (charge[i] + k) / (k + 1);
            cost = ite(port[i] == k, c.int_val(ct * price[k]), cost);
        }

        total_cost = total_cost + cost;
    }

    opt.minimize(total_cost);

    if(opt.check() == sat) {
        model m = opt.get_model();
        cout << "Optimal Schedule\n";
        for(int i = 0; i < N; i++) {
            cout << "Vehicle " << id[i]
                 << " -> Port: " << m.eval(port[i])
                 << " Start: " << m.eval(start[i])
                 << endl;
        }
        cout << "Total Cost: " << m.eval(total_cost) << endl;
    } else {
        cout << "No feasible schedule found\n";
    }

    return 0;
}