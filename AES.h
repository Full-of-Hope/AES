#include<iostream>
#include<vector>
#include<bitset>
#include<graphics.h>
#include<string>
#include<algorithm>
using namespace std;

typedef unsigned char byte;//�ֽ�
struct word    //��
{
	byte wordKey[4];
};

class AES
{
public:
	AES() {
		initRcon();  //	//��ʼ���ֳ���
	};
	// ~AES();
	void setKey(byte key[]);//���ü�����Կ
	void setPlainText(byte plain[]);//��������
	void setCipherText(byte cipher[]);//��������
	void keyExpansion(byte key[], word w[]);//��Կ��չ
	word rotWord(word w);//ѭ��������
	word subWord(word w);//���滻
	word wordXOR(word w1, word w2);//�����
	
	void encryption();//����
	void addRoundKey(word in[], int round)const;//����Կ��
	void subByte(word in[]);//�ֽ��滻
	void shiftRows(word in[]);//����λ
	void mixColumn(word in[]);//�л��
	byte GFMultiplyByte(byte L, byte R);//������˷�

	void decryption();//����
	void invShiftRows(word in[]);//���ֽ��滻
	void invSubByte(word in[]);//������λ
	void invMixColumn(word in[]);//���л��
	void initRcon();//��ʼ���ֳ���
	word cipherText[4];//����
	word deCipherText[4];//���ܽ��
private:
	byte Key[16];//������Կ
	word plainText[4];//����
	static const int Nb = 4, Nk = 4, Nr = 10;// ���С4�֣���Կ����4�֣�����10��
	word Rcon[11];//�ֳ�����
	word wordKey[44];//����Կ
	static const byte SBox[16][16];//S��
	static const byte invSBox[16][16];//��S��
	static const byte mixCol[4][4];//�л��
	static const byte invmixCol[4][4];//���л��
};
