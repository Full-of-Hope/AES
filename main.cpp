#include<iostream>
#include"AES.h"
using namespace std;

void drawMain();//������
void drawEncrypt();//���ܽ���
void drawDecrypt();//���ܽ���
void handleMouse(int x, int y);//��괦��
vector<byte> stringToBytes(const string& s);// �ַ���ת�ֽ�����
vector<byte> hexToBytes(const string& hex);// ʮ�������ַ���ת�ֽ�����
string bytesToHex(const vector<byte>& bytes);// �ֽ�����ת�ַ���
vector<byte> pkcs7Pad(const vector<byte>& data);// PKCS7���
vector<byte> pkcs7Unpad(const vector<byte>& data);// PKCS7ȥ���
void handleEncryption();//�������
void handleDecryption();//�������
//����ö��״̬
enum State { MAIN, ENCRYPT, DECRYPT };//�����桢���ܡ�����
State c_state = MAIN;
//����״̬ö��
enum ActiveInput { TEXT_INPUT, KEY_INPUT };
ActiveInput activeInput = TEXT_INPUT;
//ȫ�ֱ���
vector<vector<int>> key_arr;//��Կ��
string inputtext;//�����ı�
string resulttext;//����ı�
string encryptKey;//������Կ
string decryptKey;//������Կ
string encryptionKey;//�Ƚ϶���
string encryptedResult; // �������һ�μ��ܵĽ��
//��ť
const int BTN_WIDTH = 100;
const int BTN_HEIGHT = 40;
RECT mainEncryptBtn = { 220, 150, 220 + BTN_WIDTH, 150 + BTN_HEIGHT };
RECT mainDecryptBtn = { 220, 250, 220 + BTN_WIDTH, 250 + BTN_HEIGHT };
//����
RECT e_Input = { 50, 50, 590, 80 };
RECT e_KeyInput = { 50, 130, 590, 160 };
RECT e_Result = { 50, 210, 590, 240 };
//����
RECT d_Input = { 50, 50, 590, 80 };
RECT d_KeyInput = { 50, 130, 590, 160 };
RECT d_Result = { 50, 210, 590, 240 };
//����
RECT backBtn = { 50, 300, 150, 340 };
RECT actionBtn = { 450, 300, 550, 340 };

int main() {
	initgraph(640, 480);
	ExMessage message;//�����Ϣ

	while (true) {
		if (peekmessage(&message)) {//�¼�
			if (message.message == WM_LBUTTONDOWN) {//���
				handleMouse(message.x, message.y);
			}
			else if (message.message == WM_CHAR) {//����
				if (message.ch == 8) {
					if (c_state == ENCRYPT) {
						if (activeInput == TEXT_INPUT && !inputtext.empty())
							inputtext.pop_back();
						else if (activeInput == KEY_INPUT && !encryptKey.empty())
							encryptKey.pop_back();
					}
					else if (c_state == DECRYPT) {
						if (activeInput == TEXT_INPUT && !inputtext.empty())
							inputtext.pop_back();
						else if (activeInput == KEY_INPUT && !decryptKey.empty())
							decryptKey.pop_back();
					}
				}
				else if (message.ch >= 32 && message.ch <= 126) {
					if (c_state == ENCRYPT) {
						if (activeInput == TEXT_INPUT)
							inputtext += char(message.ch);
						else if (activeInput == KEY_INPUT && encryptKey.size() < 16)
							encryptKey += char(message.ch);
					}
					else if (c_state == DECRYPT) {
						if (activeInput == TEXT_INPUT) {
							char c = toupper(message.ch);
							if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))
								inputtext += c;
						}
						else if (activeInput == KEY_INPUT && decryptKey.size() < 16)
							decryptKey += char(message.ch);
					}
				}
			}
		}
		//������ͼ
		BeginBatchDraw();
		cleardevice();
		switch (c_state) {//״̬
		case MAIN:
			drawMain();
			break;
		case ENCRYPT:
			drawEncrypt();
			break;
		case DECRYPT:
			drawDecrypt();
			break;
		}
		EndBatchDraw();
		Sleep(10);
	}
	closegraph();
	return 0;
}

//������
void drawMain() {
	cleardevice();
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	//����
	outtextxy(224, 70, _T("AES�ӽ���ϵͳ"));
	//����
	rectangle(mainEncryptBtn.left, mainEncryptBtn.top, mainEncryptBtn.right, mainEncryptBtn.bottom);
	outtextxy(mainEncryptBtn.left + 34, mainEncryptBtn.top + 12, _T("����"));
	//����
	rectangle(mainDecryptBtn.left, mainDecryptBtn.top, mainDecryptBtn.right, mainDecryptBtn.bottom);
	outtextxy(mainDecryptBtn.left + 34, mainDecryptBtn.top + 12, _T("����"));
}
//���ܽ���
void drawEncrypt() {
	cleardevice();
	settextcolor(WHITE);
	//����
	outtextxy(e_Input.left, e_Input.top - 20, _T("���ģ����ⳤ�ȣ��Զ���䣩:"));
	rectangle(e_Input.left, e_Input.top, e_Input.right, e_Input.bottom);
	outtextxy(e_Input.left + 10, e_Input.top + 10, inputtext.c_str());
	//��Կ
	outtextxy(e_KeyInput.left, e_KeyInput.top - 20, _T("��Կ��16�ַ���:"));
	rectangle(e_KeyInput.left, e_KeyInput.top, e_KeyInput.right, e_KeyInput.bottom);
	outtextxy(e_KeyInput.left + 10, e_KeyInput.top + 10, encryptKey.c_str());
	//����
	outtextxy(e_Result.left, e_Result.top - 20, _T("���ܽ��:"));
	rectangle(e_Result.left, e_Result.top, e_Result.right, e_Result.bottom);
	outtextxy(e_Result.left + 10, e_Result.top + 10, resulttext.c_str());
	//����
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("����"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("����"));
}
//���ܽ���
void drawDecrypt() {
	cleardevice();
	settextcolor(WHITE);
	//����
	outtextxy(d_Input.left, d_Input.top - 20, _T("���ģ�16���ƣ�32�������ȣ�:"));
	rectangle(d_Input.left, d_Input.top, d_Input.right, d_Input.bottom);
	outtextxy(d_Input.left + 10, d_Input.top + 10, inputtext.c_str());
	//��Կ
	outtextxy(d_KeyInput.left, d_KeyInput.top - 20, _T("��Կ��16�ַ���:"));
	rectangle(d_KeyInput.left, d_KeyInput.top, d_KeyInput.right, d_KeyInput.bottom);
	outtextxy(d_KeyInput.left + 10, d_KeyInput.top + 10, decryptKey.c_str());
	//����
	outtextxy(d_Result.left, d_Result.top - 20, _T("���ܽ��:"));
	rectangle(d_Result.left, d_Result.top, d_Result.right, d_Result.bottom);
	outtextxy(d_Result.left + 10, d_Result.top + 10, resulttext.c_str());
	//��ť
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("����"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("����"));
}
//��������¼�
void handleMouse(int x, int y) {
	switch (c_state) {
	case MAIN:
		if (x > mainEncryptBtn.left && x < mainEncryptBtn.right &&
			y > mainEncryptBtn.top && y < mainEncryptBtn.bottom) {
			c_state = ENCRYPT;
			inputtext.clear();
			encryptKey.clear();
			resulttext.clear();
			activeInput = TEXT_INPUT;
		}
		else if (x > mainDecryptBtn.left && x < mainDecryptBtn.right &&
			y > mainDecryptBtn.top && y < mainDecryptBtn.bottom) {
			c_state = DECRYPT;
			//inputtext.clear();
			inputtext = encryptedResult; // �Զ������ܽ�������������
			decryptKey.clear();
			resulttext.clear();
			activeInput = TEXT_INPUT;
		}
		break;

	case ENCRYPT:
		if (x > backBtn.left && x < backBtn.right &&
			y > backBtn.top && y < backBtn.bottom) {
			c_state = MAIN;
			inputtext.clear();
			encryptKey.clear();
			resulttext.clear();
		}
		else if (x > actionBtn.left && x < actionBtn.right &&
			y > actionBtn.top && y < actionBtn.bottom) {
			handleEncryption();//����
		}
		else if (x >= e_Input.left && x <= e_Input.right &&
			y >= e_Input.top && y <= e_Input.bottom) {
			activeInput = TEXT_INPUT;
		}
		else if (x >= e_KeyInput.left && x <= e_KeyInput.right &&
			y >= e_KeyInput.top && y <= e_KeyInput.bottom) {
			activeInput = KEY_INPUT;
		}
		break;

	case DECRYPT:
		if (x > backBtn.left && x < backBtn.right &&
			y > backBtn.top && y < backBtn.bottom) {
			c_state = MAIN;
			inputtext.clear();
			decryptKey.clear();
			resulttext.clear();
		}
		else if (x > actionBtn.left && x < actionBtn.right &&
			y > actionBtn.top && y < actionBtn.bottom) {
			handleDecryption();//����

		}
		else if (x >= d_Input.left && x <= d_Input.right &&
			y >= d_Input.top && y <= d_Input.bottom) {
			activeInput = TEXT_INPUT;
		}
		else if (x >= d_KeyInput.left && x <= d_KeyInput.right &&
			y >= d_KeyInput.top && y <= d_KeyInput.bottom) {
			activeInput = KEY_INPUT;
		}
		break;
	}
}
// �ַ���ת�ֽ�����
vector<byte> stringToBytes(const string& s) {
	vector<byte> bytes;
	for (char c : s) {
		bytes.push_back(static_cast<byte>(c));
	}
	return bytes;
}

// ʮ�������ַ���ת�ֽ�����
vector<byte> hexToBytes(const string& hex) {
	vector<byte> bytes;
	for (size_t i = 0; i < hex.length(); i += 2) {
		string byteString = hex.substr(i, 2);
		byte b = static_cast<byte>(strtol(byteString.c_str(), NULL, 16));
		bytes.push_back(b);
	}
	return bytes;
}

// �ֽ�����תʮ�������ַ���
string bytesToHex(const vector<byte>& bytes) {
	string hex;
	const char* hexChars = "0123456789ABCDEF";
	for (byte b : bytes) {
		hex.push_back(hexChars[(b >> 4) & 0x0F]);
		hex.push_back(hexChars[b & 0x0F]);
	}
	return hex;
}

// PKCS7���
vector<byte> pkcs7Pad(const vector<byte>& data) {
	size_t padLen = 16 - (data.size() % 16);
	vector<byte> padded = data;
	for (size_t i = 0; i < padLen; ++i) {
		padded.push_back(static_cast<byte>(padLen));
	}
	return padded;
}
// PKCS7ȥ���
vector<byte> pkcs7Unpad(const vector<byte>& data) {
	if (data.empty()) return data;
	byte padLen = data.back();
	if (padLen > 16) return data;
	return vector<byte>(data.begin(), data.end() - padLen);
}

// ���ܴ����߼�
void handleEncryption() {
	if (encryptKey.size() != 16) {
		resulttext = "��Կ����Ϊ16�ַ�!";
		return;
	}
	encryptionKey = encryptKey;
	// ׼��AESʵ��
	AES aes;
	byte key[16];
	copy(encryptKey.begin(), encryptKey.end(), key);
	aes.setKey(key);

	// �������Ĳ����
	vector<byte> plainData = stringToBytes(inputtext);
	plainData = pkcs7Pad(plainData);

	// �ֿ����
	vector<byte> cipherData;
	for (size_t i = 0; i < plainData.size(); i += 16) {
		byte block[16];
		copy_n(&plainData[i], 16, block);
		aes.setPlainText(block);
		aes.encryption();

		// ��ȡ���ܽ��
		word cipherBlock[4];
		copy_n(aes.cipherText, 4, cipherBlock);
		for (int w = 0; w < 4; ++w) {
			for (int b = 0; b < 4; ++b) {
				cipherData.push_back(cipherBlock[w].wordKey[b]);
			}
		}
	}

	resulttext = bytesToHex(cipherData);
	encryptedResult = resulttext; // ������ܽ��

}

// �޸ĺ�Ľ��ܴ����߼�
void handleDecryption() {
	if (decryptKey.size() != 16) {
		resulttext = "��Կ����Ϊ16�ַ�!";
		return;
	}
	if (decryptKey != encryptionKey) {
		resulttext = "��Կ��ƥ��!";
		return;
	}
	// ��֤�����Ƿ�Ϊ��Чʮ������
	if (inputtext.empty() || inputtext.size() % 32 != 0) {
		resulttext = "��Ч���ĳ���!";
		return;
	}

	// ׼��AESʵ��
	AES aes;
	byte key[16];
	copy(decryptKey.begin(), decryptKey.end(), key);
	aes.setKey(key);

	// ת��ʮ����������
	vector<byte> cipherData = hexToBytes(inputtext);

	// �ֿ����
	vector<byte> plainData;
	for (size_t i = 0; i < cipherData.size(); i += 16) {
		byte block[16];
		copy_n(&cipherData[i], 16, block);
		aes.setCipherText(block);
		aes.decryption();

		// ��ȡ���ܽ��
		word plainBlock[4];
		copy_n(aes.deCipherText, 4, plainBlock);
		for (int w = 0; w < 4; ++w) {
			for (int b = 0; b < 4; ++b) {
				plainData.push_back(plainBlock[w].wordKey[b]);
			}
		}
	}

	// ȥ�����
	plainData = pkcs7Unpad(plainData);
	resulttext = string(plainData.begin(), plainData.end());
}