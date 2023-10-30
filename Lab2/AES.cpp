#include<iostream>
#include <cstdint>
#include<string.h>
using namespace std;
const uint8_t Nr=10;
const uint8_t ShiftTable[16]={0,5,10,15,4,9,14,3,8,13,2,7,12,1,6,11};
const uint32_t RCon[10]={
0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000,0x1B000000,0X36000000
};
const uint8_t Sbox[16][16] =
{
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

uint8_t SboxFunc(uint8_t k){
    int x,y;
    x=k>>4;
    y=k&0xF;
    return Sbox[x][y];
}

void String2uint8_t(string a,uint8_t* res){// 将32个十六进制数转换位16个uint8_t类型
    for(int i=0;i<16;i++){
        res[i]=0;
    }
    if(a.size()!=32){
        cout<<"ERROR!"<<endl;
    }
    for(int i=0;i<16;i++){
        for(int j=0;j<=1;j++){
            if(a[i*2+j]>='a'&&a[i*2+j]<='f'){
                res[i]+=(a[i*2+j]-'a'+10);
            }
            else if(a[i*2+j]>='0'&&a[i*2+j]<='9'){
                res[i]+=a[i*2+j]-'0';
            }
            else{
                cout<<"ERROR!"<<endl;
            }
            if(j!=1){
                res[i]*=16;
            }
        }
    }
}

uint32_t RotWord(uint32_t x){
    uint32_t res=0;
    res=x>>24;
    res|=(x<<8);
    return res;
}

uint32_t SubWord(uint32_t x){
    uint32_t res=0; 
    uint8_t t[4]={0};
    for(int i=0;i<4;i++){
        t[i]=(x<<(8*i))>>24;
        t[i]=int(SboxFunc(t[i]));
        res|=t[i]<<((3-i)*8);
    }
    return res;
}

void  KeyExpansion(string key,uint32_t Roundkey[4*(Nr+1)]){// 11个轮秘钥，每个密钥大小为16字节，4个uint32_t
    // string 转uint32_t
    // Roundkey[0][1][2][3]并联组成第一轮轮秘钥
    for(int i=0;i<4*(Nr+1);i++){
        Roundkey[i]=0;
    }
    for(int i=0;i<=3;i++){
        for(int j=0;j<=7;j++){
            if(key[i*8+j]>='a'&&key[i*8+j]<='f'){
                Roundkey[i]+=key[i*8+j]-'a'+10;
            }
            else if(key[i*8+j]>='0'&&key[i*8+j]<='9'){
                Roundkey[i]+=key[i*8+j]-'0';
            }
            else{
                cout<<"ERROR!"<<endl;
            }
            if(j!=7){
                Roundkey[i]*=16;
            }
        }
    }

    // 迭代生成后面九轮的密钥
    for(int i=4;i<4*(Nr+1);i++){
        uint32_t temp=Roundkey[i-1];
        if(i%4==0){
            temp=(SubWord(RotWord(temp)))^RCon[(i-1)/4];
        }
        Roundkey[i]=Roundkey[i-4]^temp;
    }
} 

//八位为一组，把32位分为4组，返回第index组 index:1-4
uint8_t Uint32_to_Uint8(uint32_t x,int index){
    uint8_t res=0;
    uint32_t temp =x<<((index-1)*8);
    res|=temp>>24;
    return res;
} 

// 将16个字节与Roundkey[curr]开始的4个32位数相异或
void AddRoundKey(uint8_t* state,uint32_t* RoundKey, int curr){
    for(int i=0;i<=3;i++){
        for(int j=0;j<=3;j++){
            state[4*i+j]=state[4*i+j]^int(Uint32_to_Uint8(RoundKey[curr+i],j+1));
        }
    }
}

// 计算 x or x+1 乘以b表示的多项式后模x^4+x^3+x+1的结果
uint8_t Poly_Mul(uint8_t a,uint8_t b){
    uint16_t temp=b;
    uint8_t res=0;
    if(a!=2&&a!=3){
        cout<<"error"<<endl;
        return 0;
    }
    if(a==2){
        temp<<=1;
    }
    else{
        temp=temp^(temp<<1);
    }
    res=temp&0xFF;
    if((temp>>8)==1){
        res^=0x1B;
    }
    return res; 
}

void ShiftRows(uint8_t* state){
    uint8_t* temp=new uint8_t[16];
    for(int j=0;j<16;j++){
        temp[j]=state[j];
    }
    for(int j=0;j<16;j++){
        state[j]=temp[ShiftTable[j]];
    }
    delete []temp;
}

void MixColumn(uint8_t* state){
    uint8_t x=2,y=3;
    for(int i=0;i<4;i++){
        uint8_t temp1=state[4*i+0];
        uint8_t temp2=state[4*i+1];
        uint8_t temp3=state[4*i+2];
        uint8_t temp4=state[4*i+3];
        state[4*i]=Poly_Mul(x,temp1)^Poly_Mul(y,temp2)^temp3^temp4;
        state[4*i+1]=temp1^Poly_Mul(x,temp2)^Poly_Mul(y,temp3)^temp4;
        state[4*i+2]=temp1^temp2^Poly_Mul(x,temp3)^Poly_Mul(y,temp4);
        state[4*i+3]=Poly_Mul(y,temp1)^temp2^temp3^Poly_Mul(x,temp4);
    }
}

string AES(string plaintext ,string key){
    uint8_t* state=new uint8_t[16];
    // 明文的string串转成16个字节的state
    String2uint8_t(plaintext,state);
    uint32_t* RoundKey=new uint32_t[4*(Nr+1)];
    KeyExpansion(key,RoundKey);
    AddRoundKey(state,RoundKey,0);


    for(int curr=0;curr<Nr;curr++){    

        // SubBytes
        for(int i=0;i<16;i++){
            state[i]=SboxFunc(state[i]);
        }
    
        // ShiftRows
        ShiftRows(state);

        // MixColumn
        if(curr!=Nr-1){
            MixColumn(state);
        }

        // AddRoundKey
        AddRoundKey(state,RoundKey,(curr+1)*4);
    }
    string ciphertext="";
    int x,y;
    for(int i=0;i<16;i++){
        x=state[i]>>4;
        y=state[i]&0xF;
        if(x>=0&&x<=9){
            ciphertext+=x+'0';
        }
        else{
            ciphertext+=x+'A'-10;
        }
        if(y>=0&&y<=9){
            ciphertext+=y+'0';
        }
        else{
            ciphertext+=y+'A'-10;
        }
    }
    return ciphertext;
}


int main(){
    
    // sample1:
    // string key="2b7e151628aed2a6abf7158809cf4f3c";
    // string plaintext="3243f6a8885a308d313198a2e0370734";
    // string ciphertext="3925841D02DC09FBDC118597196A0B32";

    // sample2:
    // string key="000102030405060708090a0b0c0d0e0f";
    // string plaintext="00112233445566778899aabbccddeeff";
    // stirng ciphertext="69C4E0D86A7B0430D8CDB78070B4C55A";

    string key,plaintext;
    cin>>key>>plaintext;
    string ciphertext=AES(plaintext,key);
    cout<<ciphertext;
    return 0;
}