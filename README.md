# Steganography

>  C언어와 BMP 이미지 파일을 이용한 Steganograph

코드 작성 : **2017. 04. 29**



## Hidding

`A.bmp`, `B.jpg` 파일이 있을 때, `B.jpg`파일을 `hide.bmp`파일에 숨길 수 있습니다.

jpg 파일의 1바이트 정보를 저장하기 위해 bmp 파일의 8바이트 각각의 1비트(LSB)를 이용합니다.

이 과정을 통해 `B.jpg` 파일을 숨길 수 있습니다.



`/steganography` 디렉토리는 `MARBLES_ORIGINAL.bmp`이라는 이미지를 포함하고 있는데, 해당 이미지는 임의의 jpg 파일을 숨기기 위해 사용하는 예제 파일입니다.

숨기려고 하는 파일의 크기가 숨겨지는 파일(*.bmp)의 크기와 비슷하거나 더 큰 경우에는 정상적으로 작동하지 않습니다.

![MARBLES_ORIGINAL](MARBLES_ORIGINAL.bmp)





## Restore

BMP 파일에서 1바이트씩 8번 읽으면서 LSB 영역이 1인지 0인지 체크하고, MSB에서 LSB순서로 시프트 연산의 누적합을 구하여 새로운 파일에 작성합니다.

이 과정이 끝나면 `hide.bmp`에 숨겨져 있었던 `B.jpg`를 얻을 수 있습니다.



### How to restore image

`restore.c` 복원하려는 파일 이름으로 수정하여 사용하면 됩니다. 이때의 파일 이름은 `hidding.c` 코드를 실행하고 얻는 결과 파일 이름과 동일합니다.