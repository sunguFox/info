#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <locale.h>

using namespace std;

//���������� �������� ����� ����� (����� �������� Mode); ��������� ������ ���������� � ������, ���� ���� � ��������� ������ �� ������
ifstream OpenFile(string FileName, ios_base::openmode Mode = ios::in)
{
	ifstream Input(FileName, Mode);
	if (!Input.is_open())			//���� ���� �� ������
	{
		cout << "���� �� ������" << endl;
		system("pause");
		exit(0);
	}
	return Input;
}

//���������� ������ ������� A ������� Size, �������� �������� ����� Value (������������ ��� ������������)
int FindIndex(int A[], int Size, int Value)
{
	for (int i = 0; i < Size; i++)
		if (A[i] == Value)
			return i;
	return -1;
}

//������� ������� � ����� ������� (������������ ��� ������������)
void DelSpace(vector <char> & Array)
{
	while (Array.back() == ' ')
		Array.pop_back();	//������� ��������� �������
}

//���������� ���������� �������� � ������
int LengthFile(string FileName)
{
	ifstream file(FileName, ios::binary | ios::ate);
	int Length = file.tellg();
	file.close();
	return Length;
}

//����������/������������ ������ ����� InputFile ������ Key ������� Size. ��������� ���������� �������� � ���� OutputFile.
//���� Mod = "Encrypt" - ����������� ����������, 
//���� Mod = "Decrypt" - ����������� ������������
void CodingFile(string InputFile, string OutputFile, int Key[], int Size, string Mode)
{
	ifstream Input = OpenFile(InputFile);					//����� �� �������� ����
	ofstream Output(OutputFile, ios::trunc);				//��������� ���� ��� ������ � ���� �������������� ������, ios::trunc ������� ���������� �����, ���� �� �� ����

	//���������� ������ ����� � ������
	int Length = LengthFile(InputFile);
	int NSpaces = Size - Length % Size;						//����� ��������, ����������� � ������ ��� ������������ �����
	Length += NSpaces;
	vector <char> Text(Length, ' ');

	//���� �� ��������� ����� �����, ��������� ������� � ������ Text � ������������ �������� �� �� �����
	for (int i = 0; Input.get(Text[i]); i++);
		//cout << Text[i];

	if (Mode == "Encrypt")								//���������� ������
		for (int i = 0; i < Length / Size; i++)
			for (int j = 0; j < Size; j++)
				Output << Text[Size * i + Key[j] - 1];

	if (Mode == "Decrypt")								//������������ ������
	{
		vector <char> Decrypt(Length);
		for (int i = 0; i < Length / Size; i++)
			for (int j = 0; j < Size; j++)
				Decrypt[Size * i + j] = Text[Size * i + FindIndex(Key, Size, j + 1)];
		DelSpace(Decrypt);		//������� ������� � ����� �������
		for (int i = 0; i < Decrypt.size(); i++)
			Output << Decrypt[i];
	}

	Input.close();										//������� ������
	Output.close();
}

//��������� ��������� ������ �� �����������
bool CompareFiles(string FileName1, string FileName2)
{
	ifstream File1 = OpenFile(FileName1, ios::binary);
	ifstream File2 = OpenFile(FileName2, ios::binary);

	char ch1, ch2;
	bool Result = true;
	while (File1.get(ch1) && File2.get(ch2))
	{
		if (ch1 != ch2)
		{
			Result = false;
			break;
		}
	}

	File1.close();
	File2.close();
	return Result;
}

/*
	  Input.txt - �������� ��������� ����
	Encrypt.txt - ����������� ��������� ����
	Decrypt.txt - �������������� ��������� ����
*/

void main()
{
	setlocale(LC_ALL,"Rus");
	const int N = 10;												//����� �������� � ��������������� �����
	int Key[N] = { 1, 2, 10, 4, 8, 7, 3, 6, 9, 5 };					//��������������� ����

	//cout << "Original text file:" << endl;
	CodingFile("Input.txt", "Encrypt.txt", Key, N, "Encrypt");		//���������� ������

	//cout << endl << endl << "Encrypted text file:" << endl;
	CodingFile("Encrypt.txt", "Decrypt.txt", Key, N, "Decrypt");	//������������� ������

	bool Compare = CompareFiles("Input.txt", "Decrypt.txt");		//�������� ��������� �������� � ��������������� ������
	if (Compare) cout  << endl << "�������� � ������������� ������ ���������" << endl;
	else		 cout  << endl << "�������� � ������������� ������ �� ���������" << endl;

	system("pause");
}