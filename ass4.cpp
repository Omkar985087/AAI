#include <bits/stdc++.h>
#include <z3++.h>

using namespace std;
using namespace z3;

int main()
{
    int k;
    cin>>k;

    vector<int> prices(k);
    for(int i=0;i<k;i++) cin>>prices[i];

    int n;
    cin>>n;

    vector<int> id(n),a(n),d(n),c(n);

    for(int i=0;i<n;i++)
    {
        cin>>id[i]>>a[i]>>d[i]>>c[i];
    }

    context ctx;
    optimize opt(ctx);

    vector<expr> port,start,duration;

    for(int i=0;i<n;i++)
    {
        port.push_back(ctx.int_const(("Port"+to_string(i)).c_str()));
        start.push_back(ctx.int_const(("start"+to_string(i)).c_str()));
        duration.push_back(ctx.int_const(("duration"+to_string(i)).c_str()));
    }

    for(int i=0;i<n;i++)
    {
        opt.add(port[i]>=0);
        opt.add(port[i]<k);

        opt.add(start[i]>=a[i]);
        opt.add(duration[i]>=1);

        opt.add(duration[i]*(port[i]+1)<=c[i]);
        opt.add(start[i]+duration[i]<=d[i]);
    }

    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            opt.add((port[i]!=port[j]) || (start[i]+duration[i]<=start[j]));
        }
    }

    expr total_cost=ctx.int_val(0);

    for(int i=0;i<n;i++)
    {
        expr cost_i=ctx.int_val(0);

        for(int j=0;j<k;j++)
        {
            cost_i=cost_i+ite(port[i]==j,duration[i]*prices[j],ctx.int_val(0));
        }
        total_cost=total_cost+cost_i;
    }

    opt.minimize(total_cost);

    params p(ctx);
    p.set("timeout",5000u);
    opt.set(p);

    if(opt.check()==sat)
    {
        model m=opt.get_model();

        cout<<"MIni cost"<<m.eval(total_cost)<<endl;

        for(int i=0;i<n;i++)
        {
            cout<<id[i];
            cout<<m.eval(port[i]);
            cout<<m.eval(start[i]);
            cout<<m.eval(duration[i]);
            cout<<endl;
        }
    }
    else
    {
        cout<<"No"<<endl;
    }


}