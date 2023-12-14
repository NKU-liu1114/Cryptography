#include<iostream>
#include<stack>
using namespace std;

/*
represent a point in Elliptic Curve
*/
struct Point{
    long long x,y;
};

/*
judge if P is an infinite point
*/
bool Is_Infinity(Point P){
    if(P.x==-1 && P.y==-1){
        return true;
    }
    else return false;
}

/*
return a^-1 mod p
*/
long long Inverse(long long  a,long long  p){
    long long r1=p,r2;
    r2=(a%p+p)%p;
    long long r,s,t;
    long long q;
    long long s1=1,s2=0;
    long long t1=0,t2=1;
    while(r2!=0){
        q=r1/r2;
        r=r1%r2;
        r1=r2;
        r2=r;
        s=s1-s2*q;
        s1=s2;
        s2=s;
        t=t1-t2*q;
        t1=t2;
        t2=t;
    }
    return t1=(t1%p+p)%p;
}

/*
return  Point m + Point n in  Elliptic Curve
*/
Point Add(Point m,Point n,long long p,long long a){
    Point res;
    if(Is_Infinity(m)){
        res=n;
        return res;
    }
    if(Is_Infinity(n)){
        res=m;
        return res;
    }
    long long k=0,x1=m.x,y1=m.y,x2=n.x,y2=n.y;
    if(x1!=x2){
        k=((y1-y2)%p*Inverse((x1-x2)%p,p)%p+p)%p;
    }
    else{
        if((y1+y2)%p==0){// x1=x2 , y1=-y2
            // stand for point O
            res.x=-1;
            res.y=-1;
            return res;
        }
        else k=((3*x1%p*x1%p+a)%p*Inverse(2*y1%p,p)%p+p)%p;
    }
    res.x=((k*k%p-x1-x2)%p+p)%p;
    res.y=((k*(x1-res.x)-y1)%p+p)%p;
    return res;
}

/*
return k*P mod p
*/
Point PointMul(long long k,Point P,long long p,long long a){
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
    long long a,b,p,x,y,k;
    // point P=(x,y)
    // y^2=x^3+ax+b
    // output:kP
    cin>>a>>b>>p>>x>>y>>k;
    Point P,res;
    P.x=x;P.y=y;
    if(Is_Infinity(P)||k==0){
        cout<<"-1 -1";
        return 0;
    }
    res=PointMul(k,P,p,a);
    cout<<res.x<<" "<<res.y;
    return 0;
}