#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <z3++.h>

using namespace std;
using namespace z3;

struct Course {
    int id;
    int s;
    int d;
    int t;
};

bool solveSchedule(int M, vector<Course> &courses)
{
    context c;
    solver s(c);

    map<tuple<int,int,int>, expr> z;

    // Create variables
    for(auto &course : courses)
    {
        for(int j = 1; j <= M; j++)
        {
            for(int t = course.s; t <= course.d - course.t + 1; t++)
            {
                string name = "z_" +
                              to_string(course.id) + "_" +
                              to_string(j) + "_" +
                              to_string(t);

                z.emplace(
                    make_tuple(course.id,j,t),
                    c.bool_const(name.c_str())
                );
            }
        }
    }

    // Each course starts exactly once
    for(auto &course : courses)
    {
        expr_vector vars(c);

        for(int j = 1; j <= M; j++)
        {
            for(int t = course.s; t <= course.d - course.t + 1; t++)
            {
                vars.push_back(
                    z.at(make_tuple(course.id,j,t))
                );
            }
        }

        s.add(mk_or(vars));

        // At most one
        for(unsigned i=0;i<vars.size();i++)
        {
            for(unsigned j=i+1;j<vars.size();j++)
            {
                s.add(!vars[i] || !vars[j]);
            }
        }
    }

    // Room conflict constraint
    for(int i=0;i<courses.size();i++)
    {
        for(int k=i+1;k<courses.size();k++)
        {
            Course c1 = courses[i];
            Course c2 = courses[k];

            for(int j=1;j<=M;j++)
            {
                for(int t1=c1.s;t1<=c1.d-c1.t+1;t1++)
                {
                    for(int t2=c2.s;t2<=c2.d-c2.t+1;t2++)
                    {
                        int end1 = t1 + c1.t;
                        int end2 = t2 + c2.t;

                        bool overlap =
                        !(end1 <= t2 || end2 <= t1);

                        if(overlap)
                        {
                            s.add(
                                !z.at(make_tuple(c1.id,j,t1)) ||
                                !z.at(make_tuple(c2.id,j,t2))
                            );
                        }
                    }
                }
            }
        }
    }

    if(s.check() == sat)
    {
        cout << "SAT\n";
        model m = s.get_model();
        cout << m << endl;
        return true;
    }
    else
    {
        cout << "UNSAT\n";
        return false;
    }
}

int main()
{
    int M,N;

    cout<<"Enter number of rooms: ";
    cin>>M;

    cout<<"Enter number of courses: ";
    cin>>N;

    vector<Course> courses;

    for(int i=0;i<N;i++)
    {
        Course c;

        cout<<"Course "<<i+1<<" (s d t): ";
        cin>>c.s>>c.d>>c.t;

        c.id = i+1;

        courses.push_back(c);
    }

    solveSchedule(M,courses);

    return 0;
}