#include<iostream>
#include<random>
using namespace std;
#define TestRound 20

/*
Square-Multi on Field Zn
return a^m mod n
*/
long long Square_Multi(long long a, long long m, long long n) {
    long long result = 1, current = a;
    while (m) {
        if (m & 1) {
            result = (result * current) % n;
        }
        current = (current * current) % n;
        m >>= 1;
    }
    return result;
}

/* 
Decompose n:n-1=(2^k)*m
*/
void Decompose(long long n,long long& k,long long& m){
    k=0;n--;
    while(!(n & 1)){
        k++;
        n>>=1;
    }
    m=n;
}

/*
Miller-Rabin Algorithm
*/
bool Miller_Rabin(long long n){
    if(n==2||n==3){
        return true;
    }
    if(n%2==0||n<=1){
        return false;
    }
    long long k=0,m=0,a,b;
    Decompose(n,k,m);
    for(int Nr=0;Nr<TestRound;Nr++){
        // get the random number
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<long long> dis(1, n - 1);
        a=dis(gen);
        // b = a^m mod n
        b=Square_Multi(a,m,n);
        if(b%n){
            continue;
        }
        for(long long i=0;i<=k-1;i++){
            if(b==n-1){
                break;
            }
            else b=(b*b)%n;
        }
        if(b==n-1){
            continue;
        }
        return false;
    }
    return true;
}
int main(){
    // input the number tested
    long long n;
    cin>>n;
    if(Miller_Rabin(n)){
        cout<<"Yes";
    }
    else cout<<"No";
    return 0;
}