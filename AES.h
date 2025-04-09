#include<iostream>
#include<vector>
#include<bitset>
#include<graphics.h>
#include<string>
#include<algorithm>
using namespace std;

typedef unsigned char byte;//字节
struct word    //字
{
	byte wordKey[4];
};

class AES
{
public:
	AES() {
		initRcon();  //	//初始化轮常量
	};
	// ~AES();
	void setKey(byte key[]);//设置加密密钥
	void setPlainText(byte plain[]);//设置明文
	void setCipherText(byte cipher[]);//设置密文
	void keyExpansion(byte key[], word w[]);//密钥扩展
	word rotWord(word w);//循环左移字
	word subWord(word w);//字替换
	word wordXOR(word w1, word w2);//字异或
	
	void encryption();//加密
	void addRoundKey(word in[], int round)const;//轮密钥加
	void subByte(word in[]);//字节替换
	void shiftRows(word in[]);//行移位
	void mixColumn(word in[]);//列混合
	byte GFMultiplyByte(byte L, byte R);//有限域乘法

	void decryption();//解密
	void invShiftRows(word in[]);//逆字节替换
	void invSubByte(word in[]);//逆行移位
	void invMixColumn(word in[]);//逆列混合
	void initRcon();//初始化轮常量
	word cipherText[4];//密文
	word deCipherText[4];//解密结果
private:
	byte Key[16];//种子密钥
	word plainText[4];//明文
	static const int Nb = 4, Nk = 4, Nr = 10;// 块大小4字，密钥长度4字，轮数10轮
	word Rcon[11];//轮常量表
	word wordKey[44];//轮密钥
	static const byte SBox[16][16];//S盒
	static const byte invSBox[16][16];//逆S盒
	static const byte mixCol[4][4];//列混合
	static const byte invmixCol[4][4];//逆列混合
};
