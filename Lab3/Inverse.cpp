#include<iostream>
using namespace std;

int Inverse(int a,int p){// return a^-1 mod p
    int r1=p;
    int r2;
    if(a<0){
        r2=(a+p)%p;
    }
    else{
        r2=a;
    }
    int r,s,t;
    int q;
    int s1=1,s2=0;
    int t1=0,t2=1;
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
    if(t1<0){
        t1=(t1+p)%p;
    }
    return t1;
}

int main(){
    int a,p,b;
    // a>0
    cin>>a>>p;
    if(a==0){
        cout<<"Error!"<<endl;
    }
    // return b, s.t a*b = 1 mod Z_p 
    b=Inverse(a,p);
    cout<<b;
    return 0;
}