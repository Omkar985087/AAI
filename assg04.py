from z3 import *

with open("input.txt", "r") as f:
    data = f.read().strip().split()

idx = 0

K = int(data[idx]); idx += 1
price = list(map(int, data[idx:idx+K]))
idx += K

n = int(data[idx]); idx += 1

vehicles = []
for _ in range(n):
    vid = int(data[idx]); a = int(data[idx+1])
    d = int(data[idx+2]); c = int(data[idx+3])
    idx += 4
    vehicles.append((vid, a, d, c))

opt = Optimize()

port = [Int(f"port_{i}") for i in range(n)]
start = [Int(f"start_{i}") for i in range(n)]
duration = [Int(f"duration_{i}") for i in range(n)]

for i in range(n):
    vid, a, d, c = vehicles[i]

    opt.add(port[i] >= 0, port[i] < K)
    opt.add(start[i] >= a)
    opt.add(duration[i] >= 1)

    opt.add(duration[i] * (port[i] + 1) >= c)
    opt.add(start[i] + duration[i] <= d)

for i in range(n):
    for j in range(i + 1, n):
        opt.add(
            Or(
                port[i] != port[j],
                start[i] + duration[i] <= start[j],
                start[j] + duration[j] <= start[i]
            )
        )

cost_terms = []
for i in range(n):
    expr = IntVal(0)
    for k in range(K):
        expr = expr + If(port[i] == k, duration[i] * price[k], 0)
    cost_terms.append(expr)

total_cost = Sum(cost_terms)
opt.minimize(total_cost)

if opt.check() == sat:
    model = opt.model()

    print("Minimum Cost:", model.evaluate(total_cost))

    for i in range(n):
        print(
            f"Vehicle {vehicles[i][0]} -> Port {model[port[i]]}, Start {model[start[i]]}"
        )
else:
    print("No feasible schedule")