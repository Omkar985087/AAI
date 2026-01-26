#include <bits/stdc++.h>
using namespace std;

int N, K, maxd;
map<int,int> cost;
map<int, vector<int>> deps;
vector<int> tasks;

set<int> done;
int TOTAL;

long long answer = 0;


vector<vector<vector<int>>> schedule;


bool ready(int t) {
    for (int d : deps[t])
        if (!done.count(d)) return false;
    return true;
}


void printSchedule() {
    cout << "========== Schedule " << answer << " ==========\n";
    for (int d = 0; d < (int)schedule.size(); d++) {
        cout << "Day " << d + 1 << ":\n";
        for (int s = 0; s < N; s++) {
            cout << "  Student " << s + 1 << ": ";
            if (schedule[d][s].empty()) cout << "[]";
            else {
                cout << "[ ";
                for (int x : schedule[d][s]) cout << x << " ";
                cout << "]";
            }
            cout << "\n";
        }
    }
    cout << "\n";
}

void dfs(int day) {
    if ((int)done.size() == TOTAL) {
        answer++;
        printSchedule();
        return;
    }

    if (day == maxd) return;


    vector<int> avail;
    for (int t : tasks)
        if (!done.count(t) && ready(t))
            avail.push_back(t);

    if (avail.empty()) return;

    vector<int> cap(N, K);
    vector<vector<int>> today(N);

    function<void(int)> assign = [&](int idx) {

        bool used = false;
        for (int s = 0; s < N; s++)
            if (!today[s].empty()) used = true;

        if (used) {
            vector<int> added;
            for (int s = 0; s < N; s++)
                for (int x : today[s]) {
                    added.push_back(x);
                    done.insert(x);
                }

            schedule.push_back(today);
            dfs(day + 1);
            schedule.pop_back();

            for (int x : added) done.erase(x);
        }

        for (int i = idx; i < (int)avail.size(); i++) {
            int t = avail[i];
            if (done.count(t)) continue;

            for (int s = 0; s < N; s++) {

                if (s > 0 && today[s-1].empty()) break;

                if (cap[s] >= cost[t]) {
                    cap[s] -= cost[t];
                    today[s].push_back(t);
                    done.insert(t);

                    assign(i + 1);

                    done.erase(t);
                    today[s].pop_back();
                    cap[s] += cost[t];
                }
            }
        }
    };

    assign(0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: main1 <inputfile> <days>\n";
        return 0;
    }

    string filename = argv[1];
    maxd = stoi(argv[2]);

    ifstream fin(filename);
    if (!fin) {
        cout << "Cannot open input file\n";
        return 0;
    }

    string line;
    while (getline(fin, line)) {
        if (line.empty() || line[0] == '%') continue;

        stringstream ss(line);
        string t;
        ss >> t;

        if (t == "N") ss >> N;
        else if (t == "K") ss >> K;
        else if (t == "A") {
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

    dfs(0);

    cout << "Total valid schedules = " << answer << "\n";
    return 0;
}