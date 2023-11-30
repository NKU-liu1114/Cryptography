#include<iostream>
#include<random>
using namespace std;
#define TestRound 10
typedef unsigned long long ull; 

/*
Modular Multiplication on Feild Zn
*/
ull ModMul(ull a, ull b, ull n){
    ull result = 0;
    a = a % n;
    while(b){
        if(b & 1){
            result= (result+a) % n;
        }
        a=(a << 1) % n;
        b>>=1;
    }
    return  result;
}

/*
Square-Multi on Field Zn
return a^m mod n
*/
ull Square_Multi(ull a, ull m, ull n) {
    ull result = 1, current = a;
    while (m) {
        if (m & 1) {
            result = ModMul(result,current,n);
        }
        current = ModMul(current,current,n);
        m >>= 1;
    }
    return result;
}

/* 
Decompose n:n-1=(2^k)*m
*/
void Decompose(ull n,ull& k,ull& m){
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
bool Miller_Rabin(ull n){

    ull k=0,m=0,a,b;
    Decompose(n,k,m);
    // get the random number
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<ull> dis(1, n - 1);
    a=dis(gen);
    // b = a^m mod n
    b=Square_Multi(a,m,n);
    if(b%n==1){
        return true;
    }
    for(ull i =0;i<k;i++){
        if(b%n==n-1){
            return true;
        }
        else b=ModMul(b,b,n);
    }
    return false;
}
/*
take TestRound  Miller_Rabin Test
*/
bool IsPrime(ull n){
    if(n<=1||n==4){
        return false;
    }
    if(n<=3){
        return true;
    }
    for(int j=0;j<TestRound;j++){
        if(!Miller_Rabin(n)){
            return false;
        }
    }
    return true;
}

int main(){
    // input the number tested
    ull n;
    cin>>n;
    if(IsPrime(n)){
        cout<<"Yes";
    }
    else cout<<"No";
    return 0;
}
