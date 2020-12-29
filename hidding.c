# 2017. 04. 29 �ۼ�
#pragma warning(disable:4996)
#include <stdio.h>
#define BMP_HEADER 0x36

FILE *read_original_bmpFile;
FILE *read_jpgFile;
FILE *result_bmpFile;
char buffer_bmpFile[8], buffer_jpgFile, lenghtChar[1024];
const char one_bit = 1;
int length_bmpFile = 0, length_jpgFile = 0, i = 0;

void get_bmpFile_length(); // Original bmp ������ ����(ũ��)�� ���
void get_jpgFile_length(); // �������� �ϴ� jpg ������ ����(ũ��)�� ���
void insert_jpgBit_to_bmpBit(); // jpg ������ ������(��Ʈ)�� bmp ���Ͽ� ����
void create_result_bmpFile(); // bmp ������ ����
void copy_bmpHeader(); // Original bmp ������ ����� ����



int main(int argc, const char **argv)
{
	get_bmpFile_length();
	get_jpgFile_length();

	// bmp���Ͽ� jpg���� �����
	// bmp ���Ͽ��� 54����Ʈ�� ��� �����̱� ������ length_bmpFile ũ�⿡�� ��� 54����Ʈ�� ����
	if (length_bmpFile - BMP_HEADER > length_jpgFile) {
		read_original_bmpFile = fopen("MARBLES_ORIGINAL.bmp", "rb");
		if (read_original_bmpFile) {
			printf("open : read_original_bmpFile ,\t target : MARBLES_ORIGINAL.bmp \n");
			
			result_bmpFile = fopen("hide_blueprint.bmp", "wb"); // ���̳ʸ� ���� ���� bmp ���� ����
			if (result_bmpFile) {
				printf("open : result_bmpFile ,\t target : hide_blueprint.bmp \n");
				copy_bmpHeader(); // jpg ������ ����� �ִ� ���ο� bmp ������ �����ϱ� ���ؼ�, 
								  // ���� bmp ������ ����� ���ο� bmp ���Ϸ� ������.

				create_result_bmpFile(); // ������� �ϴ� jpg ������ �а� bmpFile�� ������.

				fclose(result_bmpFile);
				printf("close : result_bmpFile ,\t target : hide_blueprint.bmp \n");
			}
			fclose(read_original_bmpFile);
			printf("close : read_original_bmpFile ,\t target : MARBLES_ORIGINAL.bmp \n");
		}
	}

	return 0;
}



void get_bmpFile_length() {
	// ���̳ʸ� �б� ���� ���� bmp ���� �б�
	read_original_bmpFile = fopen("MARBLES_ORIGINAL.bmp", "rb");

	// ���� bmp ���� ���̸� length_bmpFile�� ����
	if (read_original_bmpFile) {
		printf("open : read_original_bmpFile ,\t target : MARBLES_ORIGINAL.bmp ,\t get length \n");
		while (i = fread(lenghtChar, sizeof(char), sizeof(lenghtChar), read_original_bmpFile))
			length_bmpFile += i; // 1024   256����Ʈ�� �о�´�.
	}
	fclose(read_original_bmpFile);
	printf("close : read_original_bmpFile ,\t target : MARBLES_ORIGINAL.bmp ,\t get length \n");
}

void get_jpgFile_length() {
	read_jpgFile = fopen("blueprint_ORIGINAL.jpg", "rb");

	// jpg ���� ���̸� length_jpgFile�� ����
	if (read_jpgFile) {
		printf("open : read_jpgFile ,\t target : blueprint_ORIGINAL.jpg ,\t get length \n");
		while (i = fread(lenghtChar, sizeof(char), sizeof(lenghtChar), read_jpgFile))
			length_jpgFile += i;
	}
	fclose(read_jpgFile);
	printf("close : read_jpgFile, \t target : blueprint_ORIGINAL.jpg ,\t get length  \n");
}

void insert_jpgBit_to_bmpBit() {
	// 8����Ʈ�� �������� ������ ����Ʈ�� ��Ʈ �̵� ���� �Ѵ�.
	// ��, 1����Ʈ���� 1��Ʈ�� ������ �������µ� �� 8����Ʈ�̹Ƿ�
	// ���������� 1����Ʈ(8��Ʈ)��ŭ bmp���Ͽ� ���� �ȴ�.

	if (length_jpgFile > 0) {
		fread(&buffer_jpgFile, sizeof(char), sizeof(buffer_jpgFile), read_jpgFile);

		//bmp������ ������ �ּ�ȭ�ϱ� ����
		//250���� ũ�� -1
		// ������ +1

		// (1 & 0);
		// ((char)1 & (char)0);

		if (buffer_jpgFile != 0) {
			for (i = 7; i >= 0; i--) {
				if ((one_bit & buffer_bmpFile[i]) != (one_bit & buffer_jpgFile)) {
					if (buffer_bmpFile[i] > 125) buffer_bmpFile[i] -= 1;
					else buffer_bmpFile[i] += 1;
				}
				// (4 >> 1); // ������ ��Ʈ(������ ��Ʈ)�� ���ش�.
				buffer_jpgFile = (buffer_jpgFile >> 1);
			}
		}
		else {
			for (i = 7; i >= 0; i--) {
				if ((one_bit & buffer_bmpFile[i]) != 0) {
					if (buffer_bmpFile[i] > 125) buffer_bmpFile[i] -= 1;
					else buffer_bmpFile[i] += 1;
				}
			}
		}

		length_jpgFile--;
	}
	// ������ ������ ���� ���
	// ������ ������ ��Ʈ�� 1�� �ٲ��ش�.
	else
	{
		for (i = 7; i >= 0; i--) {
			if ((one_bit & buffer_bmpFile[i]) == 0) {
				if (buffer_bmpFile[i] > 125) buffer_bmpFile[i] -= 1;
				else buffer_bmpFile[i] += 1;
			}
		}
	}
}

void create_result_bmpFile() {
	// ������� �ϴ� jpg ������ ����
	read_jpgFile = fopen("blueprint_ORIGINAL.jpg", "rb");
	if (read_jpgFile) {
		printf("open : read_jpgFile ,\t target : blueprint_ORIGINAL.jpg \n");
		while (length_bmpFile > 0) {
			fread(&buffer_bmpFile, sizeof(char), sizeof(buffer_bmpFile), read_original_bmpFile);
			insert_jpgBit_to_bmpBit(); // jpg ������ ��Ʈ�� bmp ���Ͽ� ����
			fwrite(&buffer_bmpFile, sizeof(char), sizeof(buffer_bmpFile), result_bmpFile);
			length_bmpFile -= 8;
		}
		fclose(read_jpgFile);
		printf("close : read_jpgFile ,\t target : blueprint_ORIGINAL.jpg \n");
	}
}

void copy_bmpHeader() {
	// jpg ������ ����� �ִ� ���ο� bmp ������ �����ϱ� ���ؼ�, ���� bmp ������ ����� ���ο� bmp ���Ϸ� ������.
	printf("header copy start \n");
	for (i = 0; i < BMP_HEADER; i++) {
		fread(&buffer_jpgFile, sizeof(char), sizeof(buffer_jpgFile), read_original_bmpFile);
		fwrite(&buffer_jpgFile, sizeof(char), sizeof(buffer_jpgFile), result_bmpFile);
		length_bmpFile--;
	}
	printf("header copy end \n");
}

