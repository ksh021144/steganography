# 2017. 04. 29 작성
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdbool.h>
#define BMP_HEADER 0x36

int buffer;
int sum = 0;
int i = 0;

int main() {
	// 이진 파일 형식으로 읽기
	FILE* fr = fopen("hide_blueprint.bmp", "rb"); // 파일이 숨겨져있는 이미지 경로

	// 이진 파일 형식으로 쓰기
	FILE* fw = fopen("blueprint.jpg", "wb"); // 숨겨진 파일 저장할 경로

	fseek(fr, BMP_HEADER, SEEK_SET); // 54 byte 부터 확인(시작할 파일위치 지시자 설정)

	while (true) {
		for (i = sum = 0; i < 8; i++) {
			buffer = fgetc(fr);
			if (buffer == -1) break;

			// 1비트만 확인하니까 1byte 가져온 값의 홀수인지 짝수인지만 확인하면 된다.
			if ((buffer % 2) == 1) buffer = 1;
			else buffer = 0; // 아니라면 0을 대입시킴.

			// 비트당 승수감소하기 때문에 첫번째 비트는 *128을 해줘야 한다.
			if (buffer) sum += buffer << (7 - i);

			/*
			if (i == 0) {
				sum += (buffer * 128);
			}
			else if (i == 1) {
				sum += (buffer * 64);
			}
			else if (i == 2) {
				sum += (buffer * 32);
			}
			else if (i == 3) {
				sum += (buffer * 16);
			}
			else if (i == 4) {
				sum += (buffer * 8);
			}
			else if (i == 5) {
				sum += (buffer * 4);
			}
			else if (i == 6) {
				sum += (buffer * 2);
			}
			else if (i == 7) {
				sum += (buffer * 1);
			}
			*/
		}

		if (buffer == -1)  break;
		fputc(sum, fw); // 완성된 값을 blueprint.jpg 파일에 쓴다.
	}// while

	fclose(fr);
	fclose(fw);
	return 0;
}