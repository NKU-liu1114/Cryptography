#include<iostream>
#include<stack>
#include<math.h>
using namespace std;
struct Point{
    int x,y;
};
int Hash(int x){return pow(2,x);}
bool Is_Infinity(Point P){return P.x==0 && P.y==0 ? true:false;}
// 打表求逆元，eg: 2^-1 mod 11 ，由 Inv_p[2-1]=6 求得
const int Inv_p[10]={1,6,4,3,9,2,8,7,5,10};
const int Inv_q[12]={1,7,9,10,8,11,2,5,3,4,6,12};

Point Add(Point m,Point n,int p=11,int a=1){
    if(Is_Infinity(m)){ return n; }
    if(Is_Infinity(n)){ return m; }
    Point res;
    int k=0,x1=m.x,y1=m.y,x2=n.x,y2=n.y;
    if(x1!=x2){
        k=((y1-y2)*Inv_p[((x1-x2)%p+p)%p-1]%p+p)%p;
    }
    else{
        if((y1+y2)%p==0){// 相加为无穷远点
            res.x=0;
            res.y=0;
            return res;
        }
        else k=((3*x1*x1+a)*Inv_p[2*y1%p-1]%p+p)%p;
    }
    res.x=((k*k-x1-x2)%p+p)%p;
    res.y=((k*(x1-res.x)-y1)%p+p)%p;
    return res;
}
Point PointMul(int k,Point P,int p=11,int a=1){
    Point res=P;
    stack<int> s;
    while(k){
        s.push(k&1);
        k>>=1;
    }
    s.pop();
    while(!s.empty()){
        res=Add(res,res,p,a);
        if(s.top())
            res=Add(res,P,p,a);
        s.pop();
    }
    return res;
}
int main(){
    // E:y^2=x^3+x+6
    // #E:q=13
    int x0,y0,m,x,k,q=13,r,s;
    cin>>x0>>y0>>m>>x>>k;
    Point A;
    x=(x%(q-1)+q-1)%(q-1);
    A.x=x0,A.y=y0;
    r=PointMul(k,A).x;
    s=Inv_q[k-1]*(Hash(x)+m*r)%q;
    cout<<r<<" "<<s;
    return 0;
}