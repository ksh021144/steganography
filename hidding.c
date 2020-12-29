# 2017. 04. 29 작성
#pragma warning(disable:4996)
#include <stdio.h>
#define BMP_HEADER 0x36

FILE *read_original_bmpFile;
FILE *read_jpgFile;
FILE *result_bmpFile;
char buffer_bmpFile[8], buffer_jpgFile, lenghtChar[1024];
const char one_bit = 1;
int length_bmpFile = 0, length_jpgFile = 0, i = 0;

void get_bmpFile_length(); // Original bmp 파일의 길이(크기)를 계산
void get_jpgFile_length(); // 숨기고려고 하는 jpg 파일의 길이(크기)를 계산
void insert_jpgBit_to_bmpBit(); // jpg 파일의 데이터(비트)를 bmp 파일에 복사
void create_result_bmpFile(); // bmp 파일을 생성
void copy_bmpHeader(); // Original bmp 파일의 헤더를 복사



int main(int argc, const char **argv)
{
	get_bmpFile_length();
	get_jpgFile_length();

	// bmp파일에 jpg파일 숨기기
	// bmp 파일에서 54바이트는 헤더 영역이기 때문에 length_bmpFile 크기에서 헤더 54바이트를 감산
	if (length_bmpFile - BMP_HEADER > length_jpgFile) {
		read_original_bmpFile = fopen("MARBLES_ORIGINAL.bmp", "rb");
		if (read_original_bmpFile) {
			printf("open : read_original_bmpFile ,\t target : MARBLES_ORIGINAL.bmp \n");
			
			result_bmpFile = fopen("hide_blueprint.bmp", "wb"); // 바이너리 쓰기 모드로 bmp 파일 쓰기
			if (result_bmpFile) {
				printf("open : result_bmpFile ,\t target : hide_blueprint.bmp \n");
				copy_bmpHeader(); // jpg 파일을 숨기고 있는 새로운 bmp 파일을 생성하기 위해서, 
								  // 원본 bmp 파일의 헤더를 새로운 bmp 파일로 복사함.

				create_result_bmpFile(); // 숨기려고 하는 jpg 파일을 읽고 bmpFile을 생성함.

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
	// 바이너리 읽기 모드로 원본 bmp 파일 읽기
	read_original_bmpFile = fopen("MARBLES_ORIGINAL.bmp", "rb");

	// 원본 bmp 파일 길이를 length_bmpFile에 저장
	if (read_original_bmpFile) {
		printf("open : read_original_bmpFile ,\t target : MARBLES_ORIGINAL.bmp ,\t get length \n");
		while (i = fread(lenghtChar, sizeof(char), sizeof(lenghtChar), read_original_bmpFile))
			length_bmpFile += i; // 1024   256바이트씩 읽어온다.
	}
	fclose(read_original_bmpFile);
	printf("close : read_original_bmpFile ,\t target : MARBLES_ORIGINAL.bmp ,\t get length \n");
}

void get_jpgFile_length() {
	read_jpgFile = fopen("blueprint_ORIGINAL.jpg", "rb");

	// jpg 파일 길이를 length_jpgFile에 저장
	if (read_jpgFile) {
		printf("open : read_jpgFile ,\t target : blueprint_ORIGINAL.jpg ,\t get length \n");
		while (i = fread(lenghtChar, sizeof(char), sizeof(lenghtChar), read_jpgFile))
			length_jpgFile += i;
	}
	fclose(read_jpgFile);
	printf("close : read_jpgFile, \t target : blueprint_ORIGINAL.jpg ,\t get length  \n");
}

void insert_jpgBit_to_bmpBit() {
	// 8바이트의 정보에서 각각의 바이트를 비트 이동 연산 한다.
	// 즉, 1바이트에서 1비트의 정보를 가져오는데 총 8바이트이므로
	// 최종적으로 1바이트(8비트)만큼 bmp파일에 쓰게 된다.

	if (length_jpgFile > 0) {
		fread(&buffer_jpgFile, sizeof(char), sizeof(buffer_jpgFile), read_jpgFile);

		//bmp파일의 변경을 최소화하기 위해
		//250보다 크면 -1
		// 나머지 +1

		// (1 & 0);
		// ((char)1 & (char)0);

		if (buffer_jpgFile != 0) {
			for (i = 7; i >= 0; i--) {
				if ((one_bit & buffer_bmpFile[i]) != (one_bit & buffer_jpgFile)) {
					if (buffer_bmpFile[i] > 125) buffer_bmpFile[i] -= 1;
					else buffer_bmpFile[i] += 1;
				}
				// (4 >> 1); // 최하위 비트(오른쪽 비트)를 없앤다.
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
	// 파일의 끝까지 읽은 경우
	// 나머지 마지막 비트를 1로 바꿔준다.
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
	// 숨기려고 하는 jpg 파일을 읽음
	read_jpgFile = fopen("blueprint_ORIGINAL.jpg", "rb");
	if (read_jpgFile) {
		printf("open : read_jpgFile ,\t target : blueprint_ORIGINAL.jpg \n");
		while (length_bmpFile > 0) {
			fread(&buffer_bmpFile, sizeof(char), sizeof(buffer_bmpFile), read_original_bmpFile);
			insert_jpgBit_to_bmpBit(); // jpg 파일의 비트를 bmp 파일에 삽입
			fwrite(&buffer_bmpFile, sizeof(char), sizeof(buffer_bmpFile), result_bmpFile);
			length_bmpFile -= 8;
		}
		fclose(read_jpgFile);
		printf("close : read_jpgFile ,\t target : blueprint_ORIGINAL.jpg \n");
	}
}

void copy_bmpHeader() {
	// jpg 파일을 숨기고 있는 새로운 bmp 파일을 생성하기 위해서, 원본 bmp 파일의 헤더를 새로운 bmp 파일로 복사함.
	printf("header copy start \n");
	for (i = 0; i < BMP_HEADER; i++) {
		fread(&buffer_jpgFile, sizeof(char), sizeof(buffer_jpgFile), read_original_bmpFile);
		fwrite(&buffer_jpgFile, sizeof(char), sizeof(buffer_jpgFile), result_bmpFile);
		length_bmpFile--;
	}
	printf("header copy end \n");
}

