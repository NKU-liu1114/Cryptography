#include<iostream>
#include<vector>
#include <bitset>
using namespace std;

// bitset:比特流，从右向左索引
bitset<16> getsubkey(bitset<32> key,int start){
    bitset<16> subkey;
    for(int i=15;i>=0;i--){
        subkey[i]=key[i+16-start];
    }
    return subkey;
}

// ming:0010011010110111
// key:00111010100101001101011000111111
int main(){
    vector<int> Sbox={0xE,0x4,0xD,0x1,0x2,0xF,0xB,0x8,0x3,0xA,0x6,0xC,0x5,0x9,0x0,0x7};
    vector<int> Pbox={1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};
    bitset<32> key;
    bitset<16> plaintext,subkey,u,v,w,ciphertext;
    unsigned long temp1;
    bitset<4> temp2;
    cin>>plaintext;
    cin>>key;
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
    cout<<ciphertext;
    return 0;
}