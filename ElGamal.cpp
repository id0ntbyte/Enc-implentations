//
////  main.cpp
////  ElGamal Encryption
////
////  Created by Iman Abyaneh  on 11/7/17.
//

#include <iostream>
#include <iomanip>
#include <cstring>
#include <string.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <cryptopp/elgamal.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/aes.h>

#include <cryptopp/secblock.h>
using CryptoPP::SecByteBlock;

#include <cryptopp/elgamal.h>
using CryptoPP::ElGamal;
using CryptoPP::ElGamalKeys;

#include <cryptopp/cryptlib.h>
using CryptoPP::DecodingResult;
#include<cryptopp/integer.h>
using CryptoPP::Integer;

#include<cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;
using namespace std;
using namespace CryptoPP;

Integer squareAndMultiply(Integer base, Integer exponent, Integer mod) //S & M Function
{
    Integer b = 1;
    Integer a = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            b = (b * a) % mod;
        a = ((a*a) % mod);
        exponent = exponent / 2;
    }
    return b % mod;
}

Integer convertToASCII(string letter) //Converts message to ASCII
{
    string temp;
    int num[100];
    for (int i = 0; i < letter.length(); i++)
    {
        char x = letter.at(i);
        num[i] = int(x);
    }
    for (int i=0;i<letter.length();i++){
        if (num[i]<100){
            temp+='0';
        }
        temp+=to_string(num[i]);
    }
    stringstream makeString(temp);
    Integer result;
    makeString >> result;
    cout<<"\nString in ASCII is: "<<result<<endl;
    return result;
}

void asciiToPlain(int num[], int size){ //Converts the message from ASCII to Text
    string ans="";
    for (int i=0;i<(size*3)-3;i+=3){
        int temp =0;
        temp+=num[i]*100;
        temp+=num[i+1]*10;
        temp+=num[i+2];
        ans+=char(temp);
    }
    cout<<"\nPlainText: "<<ans;
    cout<<endl;
}
    

int main()
{

    string plain;
    cout<<"Enter plain text: "<<endl;
    getline(cin, plain);
    Integer m(convertToASCII(plain));
    int keySize;
    AutoSeededRandomPool rng; //Make sure random is working correctly
    cout<<"Enter the desired Key Size: ";
    cin>>keySize;
    PrimeAndGenerator pg; //Find prime and generator
    Integer p, gen;
    cout<<"Generating Prime....... This might take a minute"<<endl;
    pg.Generate(1, rng, keySize, keySize-1);
    p = pg.Prime();
    gen = pg.Generator();
    cout<<"Prime: "<<p<<endl;
    Integer a(rng,Integer(2),p-2);
    cout<<"\nPrivate Key: "<<a<<endl;
    Integer thirdOne = squareAndMultiply(gen, a, p);
    //Encryption
    Integer k(rng,Integer(2),p-2);
    Integer Gama = squareAndMultiply(gen, k, p);
    Integer temp = squareAndMultiply(thirdOne, k, p);
    Integer Delta = a_times_b_mod_c(m,temp,p);
    //Decryption
    Integer exp(p-1-a);
    Integer decypher1(squareAndMultiply(Gama, exp, p));
    Integer foundPlainText(a_times_b_mod_c(decypher1,Delta,p));
    cout<<"\nMessage Retrieved in ASCII: "<<foundPlainText<<endl;
    std::stringstream ss;
    ss << foundPlainText;
    string check = ss.str();
    int myarray[check.size()];
    for(int i=0;i<check.length();i++){ //Convert string to int
        myarray[i] = check[i] - '0';
    }
    asciiToPlain(myarray, (foundPlainText.ByteCount()-1));
 return 0;
 }



