#include <bits/stdc++.h>
using namespace std;

int N;
map<int,int> cost;
map<int, vector<int>> deps;
vector<int> tasks;
int TOTAL;

bool DELAY = false;

bool canFinish(int K, int maxd)
{
    set<int> done;
    vector<int> order = tasks;

    for (int day = 0; day < maxd; day++)
    {
        vector<int> cap(N, K);
        set<int> todayDone;

        bool progress = true;
        while (progress)
        {
            progress = false;

            for (int t : order)
            {
                if (done.count(t) || todayDone.count(t)) continue;

                bool ok = true;
                for (int d : deps[t])
                    if (!done.count(d) && !todayDone.count(d))
                        ok = false;

                if (!ok) continue;

                for (int s = 0; s < N; s++)
                {
                    if (cap[s] >= cost[t])
                    {
                        cap[s] -= cost[t];
                        todayDone.insert(t);
                        progress = true;
                        break;
                    }
                }
            }
        }

        for (int t : todayDone)
            done.insert(t);

        if ((int)done.size() == TOTAL)
            return true;
    }

    return (int)done.size() == TOTAL;
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cout << "Usage:\n";
        cout << "./assg02 input.txt N K\n";
        cout << "./assg02 input.txt N -1 DAYS\n";
        cout << "Optional: --delay\n";
        return 0;
    }

    string filename = argv[1];
    N = stoi(argv[2]);
    int K = stoi(argv[3]);

    if (argc == 5 && string(argv[4]) == "--delay")
        DELAY = true;

    ifstream fin(filename);
    if (!fin)
    {
        cout << "Cannot open input file\n";
        return 0;
    }

    string line;
    while (getline(fin, line))
    {
        if (line.empty() || line[0] == '%') continue;

        stringstream ss(line);
        string t;
        ss >> t;

        if (t == "A")
        {
            int id, p;
            ss >> id >> p;
            cost[id] = p;
            tasks.push_back(id);

            int d;
            while (ss >> d && d != 0)
                deps[id].push_back(d);
        }
    }

    sort(tasks.begin(), tasks.end());
    TOTAL = tasks.size();

    if (K != -1)
    {
        int lo = 1, hi = 10000, ans = -1;

        while (lo <= hi)
        {
            int mid = (lo + hi) / 2;
            if (canFinish(K, mid))
            {
                ans = mid;
                hi = mid - 1;
            }
            else lo = mid + 1;
        }

        cout << "Minimum days needed = " << ans << "\n";
    }
    else
    {
        int DAYS = stoi(argv[3]);

        int lo = 1, hi = 100000, ans = -1;
        while (lo <= hi)
        {
            int mid = (lo + hi) / 2;
            if (canFinish(mid, DAYS))
            {
                ans = mid;
                hi = mid - 1;
            }
            else lo = mid + 1;
        }

        cout << "Minimum K needed = " << ans << "\n";
    }

    return 0;
}
