# 2017. 04. 29 �ۼ�
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdbool.h>
#define BMP_HEADER 0x36

int buffer;
int sum = 0;
int i = 0;

int main() {
	// ���� ���� �������� �б�
	FILE* fr = fopen("hide_blueprint.bmp", "rb"); // ������ �������ִ� �̹��� ���

	// ���� ���� �������� ����
	FILE* fw = fopen("blueprint.jpg", "wb"); // ������ ���� ������ ���

	fseek(fr, BMP_HEADER, SEEK_SET); // 54 byte ���� Ȯ��(������ ������ġ ������ ����)

	while (true) {
		for (i = sum = 0; i < 8; i++) {
			buffer = fgetc(fr);
			if (buffer == -1) break;

			// 1��Ʈ�� Ȯ���ϴϱ� 1byte ������ ���� Ȧ������ ¦�������� Ȯ���ϸ� �ȴ�.
			if ((buffer % 2) == 1) buffer = 1;
			else buffer = 0; // �ƴ϶�� 0�� ���Խ�Ŵ.

			// ��Ʈ�� �¼������ϱ� ������ ù��° ��Ʈ�� *128�� ����� �Ѵ�.
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
		fputc(sum, fw); // �ϼ��� ���� blueprint.jpg ���Ͽ� ����.
	}// while

	fclose(fr);
	fclose(fw);
	return 0;
}