#include <bits/stdc++.h>
#include <z3++.h>
using namespace std;
using namespace z3;

int main() {
    int K;
    cin >> K;

    vector<int> price(K);
    for (int i = 0; i < K; i++) cin >> price[i];

    int n;
    cin >> n;

    vector<int> id(n), a(n), d(n), c(n);
    for (int i = 0; i < n; i++) {
        cin >> id[i] >> a[i] >> d[i] >> c[i];
    }

    context ctx;
    optimize opt(ctx);

    vector<expr> port, start, duration;

    for (int i = 0; i < n; i++) {
        port.push_back(ctx.int_const(("port_" + to_string(i)).c_str()));
        start.push_back(ctx.int_const(("start_" + to_string(i)).c_str()));
        duration.push_back(ctx.int_const(("dur_" + to_string(i)).c_str()));
    }

    for (int i = 0; i < n; i++) {
        opt.add(port[i] >= 0);
        opt.add(port[i] < K);

        opt.add(start[i] >= a[i]);
        opt.add(duration[i] >= 1);

        opt.add(duration[i] * (port[i] + 1) >= c[i]);

        opt.add(start[i] + duration[i] <= d[i]);
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            opt.add(
                (port[i] != port[j]) ||
                (start[i] + duration[i] <= start[j]) ||
                (start[j] + duration[j] <= start[i])
            );
        }
    }

    expr total_cost = ctx.int_val(0);

    for (int i = 0; i < n; i++) {
        expr cost_i = ctx.int_val(0);

        for (int k = 0; k < K; k++) {
            cost_i = cost_i +
                ite(port[i] == k,
                    duration[i] * price[k],
                    ctx.int_val(0)
                );
        }

        total_cost = total_cost + cost_i;
    }

    opt.minimize(total_cost);

    params p(ctx);
    p.set("timeout", 5000u);  
    opt.set(p);

    if (opt.check() == sat) {
        model m = opt.get_model();

        cout << "Minimum Cost: " << m.eval(total_cost) << "\n";

        for (int i = 0; i < n; i++) {
            cout << "Vehicle " << id[i]
                 << " -> Port " << m.eval(port[i])
                 << ", Start " << m.eval(start[i])
                 << ", Duration " << m.eval(duration[i])
                 << "\n";
        }
    } else {
        cout << "No feasible schedule\n";
    }
 
    return 0;
}

