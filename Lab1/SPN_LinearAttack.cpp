#include <iostream>
#include <random>
#include <bitset>
#include<vector>
using namespace std;
struct ptext_ctext{
    bitset<16> plaintext;
    bitset<16> ciphertext;
};

bitset<16> GetPlaintext(){// 获得随机的16位明文
    // 使用random_device来为mt19937引擎种子
    random_device rd;
    mt19937 engine(rd());
    // 使用均匀分布来生成0或1的随机比特
    uniform_int_distribution<int> dist(0, 1);
    bitset<16> Plaintext;
    for (int i = 0; i < 16; ++i) {
        Plaintext[i] = dist(engine);
    }
    return Plaintext;
}
bitset<4> GetSubBitset(bitset<16> x,int index){//返回16位比特流的第几个子串 index:1-4
    bitset<4> res;
    for(int i=3;i>=0;i--){
            res[i]=x[16-index*4+i];
    }
    return res;
}
bitset<32> GetKey(){// 获得随机的32位比特流作为密钥
    // 使用random_device来为mt19937引擎种子
    random_device rd;
    mt19937 engine(rd());
    // 使用均匀分布来生成0或1的随机比特
    uniform_int_distribution<int> dist(0, 1);
    bitset<32> key;
    for (int i = 0; i < 32; ++i) {
        key[i] = dist(engine);
    }
    return key;
}
bitset<16> getsubkey(bitset<32> key,int start){//获得子密钥
    bitset<16> subkey;
    for(int i=15;i>=0;i--){
        subkey[i]=key[i+16-start];
    }
    return subkey;
}
bitset<16> SPN(bitset<16> plaintext,bitset<32> key){//SPN，最后返回密文
    vector<int> Sbox={0xE,0x4,0xD,0x1,0x2,0xF,0xB,0x8,0x3,0xA,0x6,0xC,0x5,0x9,0x0,0x7};
    vector<int> Pbox={1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};
    bitset<16> subkey,u,v,w,ciphertext;
    unsigned long temp1;
    bitset<4> temp2;
    w=plaintext; //  第一轮的初始值即为明文
    for(int r=1;r<=5;r++){
        subkey=getsubkey(key,4*r-4);
        u = w^subkey;  //  异或操作：将子密钥Kr与第r轮的初始值异或
        if(r<=4){      //  第五轮只需将结果与subkey异或即可，无需代换和置换  
            for(int j=3;j>=0;j--){//代换步骤
                bitset<4> subu;
                for(int q=3;q>=0;q--){
                    subu[q]=u[j*4+q];
                }// 得到以四个比特为一组的子串
                temp1= subu.to_ulong();// 将子串转换为整数
                temp2 = Sbox[temp1];// 查询Sbox来得到应该代换结果
                for(int q=3;q>=0;q--){
                    v[q+4*j]=temp2[q]; 
                }
            }
        }
        else{
            w=u;//跳过代换
        }
        if(r<=3){//置换步骤，最后一轮无需置换
            for(int m=0;m<16;m++ ){
                w[16-Pbox[m]]=v[15-m];//置换
            }
        }
        if(r==4){
            w=v;//跳过置换步骤
        }
    }
    ciphertext=w;
    return ciphertext;
}
void SPN_LinearAttack(int T ,ptext_ctext* t,bitset<4>& res1,bitset<4>& res2){
    int reverseSbox[16]={0xE,0x3,0x4,0x8,0x1,0xC,0xA,0xF,0x7,0xD,0x9,0x6,0xB,0x2,0x0,0x5};
    int count[16][16];
    for(int L1=0;L1<16;L1++){
        for(int L2=0;L2<16;L2++){
            count[L1][L2]=0;
        }
    }
    for(int i=0;i<T;i++){
        bitset<16> x=t[i].plaintext;
        bitset<16> y=t[i].ciphertext;
        bitset<4> v4_2,v4_4,u4_2,u4_4;
        // 分别表示第4轮第2子串的代换值，第4轮第4子串的代换值
        // 第4轮第2子串的异或值，第4轮第4子串的异或值
        for(int L1=0;L1<16;L1++){
            for(int L2=0;L2<16;L2++){
                bitset<4> l1=L1;//把整数转为比特流
                bitset<4> l2=L2;
                v4_2=l1^GetSubBitset(y,2);
                v4_4=l2^GetSubBitset(y,4);
                u4_2=reverseSbox[v4_2.to_ulong()];
                u4_4=reverseSbox[v4_4.to_ulong()];
                bitset<1> z= x[11]^x[9]^x[8]^u4_2[0]^u4_2[2]^u4_4[0]^u4_4[2];
                if(z[0]==0){
                    count[L1][L2]++;
                }
            }
        }
    }
    int max=-1;
    int maxL1=0,maxL2=0;
    for(int L1=0;L1<16;L1++){
        for(int L2=0;L2<16;L2++){
            count[L1][L2]=abs(count[L1][L2]-T/2);
            if (count[L1][L2]>max){
                max=count[L1][L2];
                maxL1=L1;
                maxL2=L2;
            }
        }
    }
    res1=maxL1;
    res2=maxL2;
}
int main() {
    int T=8000;
    ptext_ctext* t=new ptext_ctext[T];
    bitset<32> key = GetKey();
    for(int i=0;i<T;i++){
        t[i].plaintext=GetPlaintext();
        t[i].ciphertext=SPN(t[i].plaintext,key);
    }//生成T个明密文对
    bitset<4> r1,r2;
    SPN_LinearAttack(T,t,r1,r2);
    cout<<"The 5th round subkey2 is:"<<r1<<endl;
    cout<<"The 5th round subkey4 is:"<<r2<<endl;
    cout<<"------------------------------------------"<<endl;
    cout<<"The 5th Key is:"<<getsubkey(key,16)<<endl;
    return 0;
}

