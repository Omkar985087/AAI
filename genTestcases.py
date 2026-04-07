import random

K = random.randint(3, 6)

price = []
base = random.randint(3, 8)
for i in range(K):
    price.append(base + i * random.randint(2, 5))

n = random.randint(5, 10)

vehicles = []
for i in range(1, n + 1):
    arrival = random.randint(0, 30)
    departure = arrival + random.randint(5, 25)
    charge = random.randint(5, 20)

    vehicles.append((i, arrival, departure, charge))

with open("input.txt", "w") as f:
    f.write(str(K) + "\n")
    f.write(" ".join(map(str, price)) + "\n")
    f.write(str(n) + "\n")

    for v in vehicles:
        f.write(f"{v[0]} {v[1]} {v[2]} {v[3]}\n")
