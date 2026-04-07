#include <bits/stdc++.h>
using namespace std;

struct Assignment {
    int id;
    int prompts;
    vector<int> deps;
};

struct State {
    set<int> completed;
    int day;
    int g; 
    int h; 
};

vector<Assignment> assignments;
map<int,int> idIndex;

int c1, c2; 
int Pchat, Pgem; 
int totalAssignments;

int nodesDFS=0, nodesDFBB=0, nodesAstar=0;

bool isChatGPT(int id){
    return id % 2 == 0;
}

bool dependenciesMet(int id, const set<int>& done){
    for(int d : assignments[idIndex[id]].deps){
        if(done.find(d) == done.end())
            return false;
    }
    return true;
}

int remainingPrompts(const set<int>& done){
    int sum = 0;
    for(auto &a : assignments){
        if(done.find(a.id) == done.end())
            sum += a.prompts;
    }
    return sum;
}

int heuristic(const set<int>& done){
    int rem = remainingPrompts(done);
    int capacity = Pchat + Pgem;
    if(capacity == 0) return INT_MAX/2;
    return (rem + capacity - 1)/capacity;
}

bool allDone(const set<int>& done){
    return done.size() == totalAssignments;
}

int bestDFS = INT_MAX;

void DFS(State s){
    nodesDFS++;
    if(allDone(s.completed)){
        bestDFS = min(bestDFS, s.day);
        return;
    }
    if(s.day >= bestDFS) return;

    vector<int> available;
    for(auto &a : assignments){
        if(s.completed.count(a.id)==0 && dependenciesMet(a.id, s.completed))
            available.push_back(a.id);
    }

    for(int id : available){
        State next = s;
        next.completed.insert(id);
        next.day = s.day + 1;
        DFS(next);
    }
}

int bestDFBB = INT_MAX;

void DFBB(State s){
    nodesDFBB++;

    if(allDone(s.completed)){
        bestDFBB = min(bestDFBB, s.day);
        return;
    }

    s.h = heuristic(s.completed);
    if(s.day + s.h >= bestDFBB)
        return;

    vector<int> available;
    for(auto &a : assignments){
        if(s.completed.count(a.id)==0 && dependenciesMet(a.id, s.completed))
            available.push_back(a.id);
    }

    for(int id : available){
        State next = s;
        next.completed.insert(id);
        next.day = s.day + 1;
        DFBB(next);
    }
}

struct Compare {
    bool operator()(State const& a, State const& b){
        return (a.g + a.h) > (b.g + b.h);
    }
};

int Astar(){
    priority_queue<State, vector<State>, Compare> pq;

    State start;
    start.day = 0;
    start.g = 0;
    start.completed.clear();
    start.h = heuristic(start.completed);

    pq.push(start);

    while(!pq.empty()){
        State cur = pq.top(); pq.pop();
        nodesAstar++;

        if(allDone(cur.completed))
            return cur.day;

        vector<int> available;
        for(auto &a : assignments){
            if(cur.completed.count(a.id)==0 &&
               dependenciesMet(a.id, cur.completed))
                available.push_back(a.id);
        }

        for(int id : available){
            State next = cur;
            next.completed.insert(id);
            next.day = cur.day + 1;
            next.g = next.day;
            next.h = heuristic(next.completed);
            pq.push(next);
        }
    }

    return -1;
}

void readInput(string filename){
    ifstream fin(filename);
    string line;

    while(getline(fin,line)){
        if(line.empty() || line[0]=='%') continue;

        stringstream ss(line);
        char type;
        ss >> type;

        if(type == 'A'){
            Assignment a;
            ss >> a.id;
            ss >> a.prompts;
            int dep;
            while(ss >> dep){
                if(dep == 0) break;
                a.deps.push_back(dep);
            }
            assignments.push_back(a);
        }
    }

    for(int i=0;i<assignments.size();i++)
        idIndex[assignments[i].id] = i;

    totalAssignments = assignments.size();
}

int main(int argc, char* argv[]){

    if(argc < 5){
        cout << "Usage:\n";
        cout << "./assg03 input.txt caseA|caseB finish|mincost ...\n";
        return 0;
    }

    string filename = argv[1];
    string scenario = argv[2];
    string objective = argv[3];

    readInput(filename);

    if(objective == "finish"){
        c1 = stoi(argv[4]);
        c2 = stoi(argv[5]);
        Pchat = stoi(argv[6]);
        Pgem  = stoi(argv[7]);

        State start;
        start.day = 0;
        start.completed.clear();

        DFS(start);
        DFBB(start);
        int astarRes = Astar();

        cout << "Depth fisrt search days: " << bestDFS << "\n";
        cout << "Depth first breadth and bound days: " << bestDFBB << "\n";
        cout << "A* days: " << astarRes << "\n";

        cout << "Nodes Depth fisrt search: " << nodesDFS << "\n";
        cout << "Nodes Depth first breadth and bound: " << nodesDFBB << "\n";
        cout << "Nodes A*: " << nodesAstar << "\n";
    }

    else if(objective == "mincost"){
        int m = stoi(argv[4]);
        c1 = stoi(argv[5]);
        c2 = stoi(argv[6]);

        int bestCost = INT_MAX;

        for(int pc=1; pc<=20; pc++){
            for(int pg=1; pg<=20; pg++){
                Pchat = pc;
                Pgem  = pg;

                bestDFBB = INT_MAX;
                State start;
                start.day = 0;
                start.completed.clear();
                DFBB(start);

                if(bestDFBB <= m){
                    int cost = pc*c1 + pg*c2;
                    bestCost = min(bestCost, cost);
                }
            }
        }

        if(bestCost == INT_MAX)
            cout << "Infeasible\n";
        else
            cout << "Minimum daily subscription cost: " << bestCost << "\n";
    }

    return 0;
}
