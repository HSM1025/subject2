/**
	바이너리 형태로 파일 입출력을 할 경우 컴퓨터만 알아 들을 수 있는 텍스트 파일이 만들어집니다.
	코드 상으론 오류가 나진 않지만 메모장 등과 같은 것으로 사람들이 볼 때는 글씨가 깨져서 출력됩니다.
*/

#define _CRT_SECURE_NO_WARNINGS	// 보안문제를 무시 (scanf, fopen 등 보안이슈 없이 사용가능)

#include <stdio.h>
#include <stdlib.h>

typedef struct Student {		// Student 구조체를 선언
	char name[80];			// 이름
	char id[20];			// 학번
	float s1;			// 점수1
	float s2;			// 점수2
}student;	// Student 구조체로 student 타입을 생성

int main() {
	student data[3] = { {"H.S.M", "20240001", 80, 60},		// {학생의 이름, 학번, 점수1, 점수2} 순으로 구조체 배열 data 초기화
						{"K.M.J", "20240002", 60, 60},
						{"O.D.H", "20240003", 100, 60} };
	student read_data[3];		// 파일을 읽었다는 것을 확인하기 위한 구조체 배열 read_data 선언
	float s1_sum = 0, s2_sum = 0;	// 과목별 점수의 합
	FILE* fp = fopen("sj.txt", "wb");	// 바이너리 형태로 파일 쓰기
	fwrite(data, sizeof(data[0]), 3, fp);	// fwrite(데이터, 데이터 크기, 반복 횟수, FILE*)
											// FILE*가 가르키는 파일의 데이터를 데이터 크기만큼 잘라서 쓰는 것을 반복 횟수 만큼 반복
											// 즉, 현 코드는 data[0] 쓰고 data[1] 쓰고 data[2]를 씀
	fclose(fp);	// 파일 닫기
	fp = fopen("sj.txt", "rb");			// 바이너리 형태로 파일 읽기
	fread(read_data, sizeof(data[0]), 3, fp);	//fread(데이터, 데이터 크기, 반복 횟수, FILE*)
												// FILE*가 가르키는 파일의 데이터에서 데이터 크기만큼 잘라서 읽는 것을 반복 횟수 만큼 반복
	for (int i = 0; i < 3; i++) {
		printf("%s의 평균점수 : %.2f\n", read_data[i].name, (read_data[i].s1 + read_data[i].s2) / 2);	// 학생의 평균 점수 출력
		s1_sum += read_data[i].s1;	// 점수1의 합계를 구함
		s2_sum += read_data[i].s2;	// 점수2의 합계를 구함
	}
	fclose(fp);	// 파일 닫기
	printf("s1과목 평균점수 : %.2f\n", s1_sum / 3);	// 점수1의 전체 평균 출력
	printf("s2과목 평균점수 : %.2f\n", s2_sum / 3);	// 점수2의 전체 평균 출력
	return 0;
}
