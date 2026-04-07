#include <bits/stdc++.h>
#include <z3++.h>

using namespace std;

struct Course {
    int id, s, d, t;
};

void parse_input(string file, int &M, int &N, vector<Course> &courses) {

    ifstream f(file);
    string line;

    while(getline(f,line)) {

        if(line.empty() || line[0]=='%') continue;

        stringstream ss(line);
        string type;
        ss >> type;

        if(type=="M")
            ss >> M;

        else if(type=="N")
            ss >> N;

        else if(type=="C") {
            Course c;
            ss >> c.id >> c.s >> c.d >> c.t;
            courses.push_back(c);
        }
    }
}

int main(int argc,char* argv[]) {

    if(argc!=2){
        cout<<"Usage: assg05 input.txt\n";
        return 0;
    }

    int M=0,N=0;
    vector<Course> courses;

    parse_input(argv[1],M,N,courses);

    z3::context c;
    z3::solver s(c);

    std::map<std::tuple<int,int,int>, z3::expr> z;

    for(auto &course:courses)
        for(int j=1;j<=M;j++)
            for(int t=course.s;t<=course.d-course.t+1;t++) {

                string name =
                "z_"+to_string(course.id)+"_"+to_string(j)+"_"+to_string(t);

                z.insert({make_tuple(course.id,j,t),
                         c.bool_const(name.c_str())});
            }

    for(auto &course:courses){

        z3::expr_vector vars(c);

        for(int j=1;j<=M;j++)
            for(int t=course.s;t<=course.d-course.t+1;t++)
                vars.push_back(
                    z.at(make_tuple(course.id,j,t))
                );

        s.add(z3::mk_or(vars));

        for(unsigned i=0;i<vars.size();i++)
            for(unsigned j=i+1;j<vars.size();j++)
                s.add(!vars[i] || !vars[j]);
    }

    for(int i=0;i<courses.size();i++)
    for(int k=i+1;k<courses.size();k++) {

        auto &c1=courses[i];
        auto &c2=courses[k];

        for(int j=1;j<=M;j++)
        for(int t1=c1.s;t1<=c1.d-c1.t+1;t1++)
        for(int t2=c2.s;t2<=c2.d-c2.t+1;t2++) {

            int end1 = t1 + c1.t;
            int end2 = t2 + c2.t;

            bool overlap = !(end1<=t2 || end2<=t1);

            if(overlap){

                s.add(
                    !z.at(make_tuple(c1.id,j,t1)) ||
                    !z.at(make_tuple(c2.id,j,t2))
                );
            }
        }
    }

    if(s.check()==z3::sat){

        cout<<"SAT\n";

        z3::model m = s.get_model();

        for(auto &p:z){

            if(m.eval(p.second).bool_value()==Z3_L_TRUE){

                auto key = p.first;

                cout<<"Course "
                    <<get<0>(key)
                    <<" Room "
                    <<get<1>(key)
                    <<" StartDay "
                    <<get<2>(key)
                    <<endl;
            }
        }

    } else {

        cout<<"UNSAT\n";
    }
}