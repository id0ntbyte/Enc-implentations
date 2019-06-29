//
//  main.cpp
//  DES Encryption Implementation with CBC
//
//  Created by IMI  on 10/17/17.
//  Copyright © 2017 Iman Abyaneh. All rights reserved.
//

#include<string.h>
#include<iostream>
#include<bitset>
#include<sstream>

using namespace std;

string hexToBinHelper(char userInput){
    switch(char(userInput))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
    return "Couldnt get it";
}
string hexToBin(string userInput){
    string bin;
    for(unsigned i = 0; i != userInput.length(); ++i)
        bin += hexToBinHelper(userInput[i]);
    return bin;
}
string inputToBinary(string text){
    string fixed="";
        for (int i=0;i<text.length();i++){
            bitset<8>ans(text.c_str()[i]);
            fixed += ans.to_string<char,std::string::traits_type,std::string::allocator_type>();
        }
    return fixed;
}
int prexpansionPareText(string text, string (&arr)[100]){
    int count=0;
    for (unsigned i = 0; i < text.length(); i += 8) {
        arr[count]=text.substr(i, 8);
        count++;
    }
    if (arr[count-1].length() < 8){
        while(arr[count-1].length() < 8)
            arr[count-1]+=" ";
    }
    for(int i=0;i<count;i++){
        string temp = inputToBinary(arr[i]);
        arr[i]=temp;
    }
    return count;
}


int main()
{
    string plainText,left,right,ap="",ke,passKey,rightTerm;
    string plainTextBlock[100];
    string cipherTextBlock[100];
    string decryptedTextBlock[100];
    int key1[64];
    cout<<"Enter your plain text"<<endl;
    getline(cin, plainText);
    int len = prexpansionPareText(plainText, plainTextBlock);
pass:;
    cout<<"Enter the Key"<<endl;
    cin>>passKey;
    if (passKey.length()!=8){
        cout<<"Key must be 8 characters (64 bits)"<<endl;
        goto pass;
    }
    passKey = inputToBinary(passKey);
    system( "read -n 1 -s -p 'Press any key to continue...'; echo" );
    
    //KEY GENERATION PHASE
    for(int i=0;i<64;i++)
        key1[i]=passKey[i]-'0';
    int keyp[56]={57  , 49   , 41   ,33 ,   25   , 17 ,   9,
        1 ,  58    ,50  , 42 ,   34  ,  26  , 18,
        10 ,   2   , 59 ,  51  ,  43  ,  35  , 27,
        19  , 11   ,  3  , 60  ,  52  ,  44  , 36,
        63  , 55  ,  47  , 39  ,  31   , 23  , 15,
        7  , 62   , 54  , 46  ,  38  ,  30  , 22,
        14  ,  6   , 61  , 53  ,  45   , 37  , 29,
        21  , 13  ,   5  , 28   , 20  ,  12  ,  4};
    int key2[48]={14,    17,   11,    24,     1,    5,
        3   , 28 ,  15    , 6  ,  21  , 10 ,
        23    ,19  , 12   ,  4 ,  26  ,  8,
        16     ,7  , 27  ,  20  ,  13  ,  2,
        41   , 52 ,  31  ,  37  ,  47  , 55,
        30   , 40  , 51  ,  45  ,  33 , 48,
        44    ,49 ,  39  ,  56  ,  34 ,  53,
        46    ,42 ,  50  ,  36 ,   29 ,  32};
    int key[16][48],keyl[28],keyr[28],nshift,temp1,temp2,pkey[56];
    int i,t=1,j,row,col,temp;
    j=0;
    for(i=0;i<56;i++)
        pkey[i]=key1[keyp[i]-1];
    for(i=0;i<28;i++){
        keyl[i]=pkey[i];}
    for(i=0;i<28;i++){
        keyr[i]=pkey[i+28];}
    //round key generation
    for(i=0;i<16;i++)
    {
        if(i==0||i==1||i==8||i==15)
            nshift=1;
        else
            nshift=2;
        while(nshift--)
        {
            temp1=keyl[0];
            temp2=keyr[0];
            for(j=0;j<27;j++)
            {
                keyl[j]=keyl[j+1];
                keyr[j]=keyr[j+1];
            }
            keyl[27]=temp1;
            keyr[27]=temp2;
        }
        for(j=0;j<24;j++)
            key[i][j]=keyl[key2[j]-1];
        for(j=24;j<48;j++)
            key[i][j]=keyr[key2[j]-1-28];
    }
    int per[32]={16 ,  7 , 20  ,21,
        29 , 12 , 28 , 17,
        1 , 15,  23,  26,
        5 , 18 , 31  ,10,
        2  , 8 , 24 , 14,
        32  ,27,   3 ,  9,
        19  ,13,  30,   6,
        22 , 11  , 4 , 25};
    int s[8][4][16]=
    {{
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
    },
        {
            15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
            3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
            0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
            13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
        },


        {
            10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
            13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
            13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
            1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
        },
        {
            7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
            13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
            10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
            3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
        },
        {
            2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
            14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
            4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
            11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
        },
        {
            12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
            10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
            9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
            4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
        },
        {
            4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
            13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
            1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
            6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
        },
        {
            13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
            1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
            7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
            2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
        }};
    string newPlainText="";
    string cipher="";
    string result="";
    string text="";
    string bs= "1000100010001000100010001000100010001000100010001000100010001001";
    for (int k=0;k<len;k++){
        cout<<"piece #"<<k+1<<endl; //FOR diagnosis
            string newPlainText="";
            text = plainTextBlock[k];
            if (k == 0){
                for(i=0;i<64;i++){
                    newPlainText += ((text[i]-'0') ^ (bs[i]-'0')) + '0';
                }
                left=newPlainText.substr(0,32);
                right=newPlainText.substr(32,32);
            }
            else if (k>0){
                string tmp = hexToBin(result);
                for(i=0;i<64;i++){
                    newPlainText += ((text[i]-'0') ^ (tmp[i]-'0')) + '0';
                }
                left=newPlainText.substr(0,32);
                right=newPlainText.substr(32,32);
            }
        
        
    //ENCRYPTION PHASE
    int round=16;
    while(round--)
    {
        rightTerm=right;
        t=1;
        string expansionP="",xorOperation="",sout="",sBoxOutput;
        expansionP+=right[31];
        for(i=0;i<32;i++)
        {
            if((t+1)%6==0)
            {
                expansionP+=right[4*((t+1)/6)];
                t++;
            }
            if(t%6==0&&i!=0)
            {
                expansionP+=right[4*(t/6)-1];
                t++;
            }
            expansionP=expansionP+right[i];
            t++;
        }
        expansionP+=right[0];
        //XOR
        for(i=0;i<48;i++)
            xorOperation+=char(((int(expansionP[i])-48)^key[16-round-1][i])+48);
        //sbox compression
        for(i=0;i<48;i+=6)
        {
            row=(int(xorOperation[i+5])-48)+(int(xorOperation[i])-48)*2;
            col= (int(xorOperation[i+1])-48)*8+(int(xorOperation[i+2])-48)*4+(int(xorOperation[i+3])-48)*2+(int(xorOperation[i+4])-48);
            temp=s[i/6][row][col];
            sBoxOutput="";
            while(temp>0)
            {
                sBoxOutput+=char(temp%2+48);
                temp/=2;
            }
            while(sBoxOutput.length()!=4)
                sBoxOutput+='0';
            for(j=sBoxOutput.length()-1;j>=0;j--)
                sout+=sBoxOutput[j];
        }
        char pc[32];
        for(i=0;i<32;i++)
            pc[i]=sout[per[i]-1];
        right="";
        for(i=0;i<32;i++)
            right+=char(((int(pc[i])-48)^(int(left[i])-48))+48);
        left=rightTerm;
        cout<<"Output after Round"<<16-round<<endl; //FOR diagnosis
        string cipher="";
        for(i=0;i<32;i+=4)
        {
            int te;
            te=(int(left[i])-48)*8+(int(left[i+1])-48)*4+(int(left[i+2])-48)*2+(int(left[i+3])-48);
            if(te<10)
                cipher+=char(te+48);
            else
                cipher+=char(te+55);
        }
        for(i=0;i<32;i+=4)
        {
            int te;
            te=(int(right[i])-48)*8+(int(right[i+1])-48)*4+(int(right[i+2])-48)*2+(int(right[i+3])-48);
            if(te<10)
                cipher+=char(te+48);
            else
                cipher+=char(te+55);
        }
        result = cipher;
        cipherTextBlock[k] = result;
        cout<<cipher<<endl; //FOR diagnosis
    }
        cout<<"The encrypted message is: ";
        for(i=0;i<len;i++){
            cout<<cipherTextBlock[i];
        }
    }
    cout<<endl;
    system( "read -n 1 -s -p 'Press any key to Decrypt...'; echo" );
for (int k=0;k<len;k++){
    left=hexToBin(cipherTextBlock[k]).substr(0,32);
    right=hexToBin(cipherTextBlock[k]).substr(32,32);
    cout<<"Decrypt piece #"<<k+1<<endl;  //For Diagnosis

    //DECRYPTION PHASE
    int round=16;
    string leftTerm;
    while(round--)
    {
        leftTerm=left;
        t=1;
        string expansionP="",xorOperation="",sout="",sBoxOutput;
        //the expansion P box
        expansionP+=leftTerm[31];
        for(i=0;i<32;i++)
        {
            if((t+1)%6==0)
            {
                expansionP+=leftTerm[4*((t+1)/6)];
                t++;
            }
            if(t%6==0&&i!=0)
            {
                expansionP+=left[4*(t/6)-1];
                t++;
            }
            expansionP=expansionP+left[i];
            t++;
        }
        expansionP+=left[0];
        //XOR
        for(i=0;i<48;i++)
            xorOperation+=char(((int(expansionP[i])-48)^key[round][i])+48);
        for(i=0;i<48;i+=6)
        {
            row=(int(xorOperation[i+5])-48)+(int(xorOperation[i])-48)*2;
            col= (int(xorOperation[i+1])-48)*8+(int(xorOperation[i+2])-48)*4+(int(xorOperation[i+3])-48)*2+(int(xorOperation[i+4])-48);
            temp=s[i/6][row][col];
            sBoxOutput="";
            while(temp>0)
            {
                sBoxOutput+=char(temp%2+48);
                temp/=2;
            }
            while(sBoxOutput.length()!=4)
                sBoxOutput+='0';
            for(j=sBoxOutput.length()-1;j>=0;j--)
                sout+=sBoxOutput[j];
        }
        char pc[32];
        for(i=0;i<32;i++)
            pc[i]=sout[per[i]-1];
        left="";
        for(i=0;i<32;i++)
            left+=char(((int(pc[i])-48)^(int(right[i])-48))+48);
        right=leftTerm;
        cout<<"Decrypted Output after Round"<<16-round<<endl; //FOR diagnosis
        string cipher="";
        for(i=0;i<32;i+=4)
        {
            int te;
            te=(int(left[i])-48)*8+(int(left[i+1])-48)*4+(int(left[i+2])-48)*2+(int(left[i+3])-48);
            if(te<10)
                cipher+=char(te+48);
            else
                cipher+=char(te+55);
        }
        for(i=0;i<32;i+=4)
        {
            int te;
            te=(int(right[i])-48)*8+(int(right[i+1])-48)*4+(int(right[i+2])-48)*2+(int(right[i+3])-48);
            if(te<10)
                cipher+=char(te+48);
            else
                cipher+=char(te+55);
        }
        result = cipher;
        cout<<cipher<<endl; //FOR diagnosis
    }
    string newPlainText="";
        if(k==0){
            text = hexToBin(result);
            for(i=0;i<64;i++){
                newPlainText += ((text[i]-'0') ^ (bs[i]-'0')) + '0';
            }
            decryptedTextBlock[k] = newPlainText;
        }
        else if (k>0){
            string tmp = hexToBin(result);
            text=hexToBin(cipherTextBlock[k-1]);
            for(i=0;i<64;i++){
                newPlainText += ((tmp[i]-'0') ^ (text[i]-'0')) + '0';
            }
            decryptedTextBlock[k] = newPlainText;
        }
    }
    //Binary to ASCII
    string output;
    for(int i=0;i<len;i++){
        cout<<decryptedTextBlock[i]<<endl;
        stringstream sstream(decryptedTextBlock[i]);
        while(sstream.good())
        {
            bitset<8> bits;
            sstream >> bits;
            char c = char(bits.to_ulong());
            output += c;
        }
    }
    cout<<"\n\nMessage was: "<<output<<"\n"<<endl;
}
