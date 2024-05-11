#define _CRT_SECURE_NO_WARNINGS	// 보안문제를 무시 (scanf, fopen 등 보안이슈 없이 사용가능)

#include <stdio.h>

typedef struct Student { //구조체 선언
    char name[20];         //구조체 멤버들
    int id;
    double kor;
    double eng;
} student;

int main(void) {
    student std[3]; //구조체 변수 std선언, 학생 3명
    //구조체 변수 std_data 선언 후 내용 초기화
    //구조체 멤버 선언한 순서로 정보 입력{"이름", 학번, 국어, 영어}
    student std_data[3] = { {"Kim", 20101245, 40, 60},
                                  {"Lee", 20101246, 80, 60},
                                  {"Park", 20101247, 40, 80} };
    FILE* fp;
    fp = fopen("sj.txt", "wb"); //바이너리 쓰기 모드
    fwrite(std_data, sizeof(std_data[0]), 3, fp); // std_data에서 3개의 정보를 파일에 쓰되, 각 정보의 크기는 바이트임을 선언
    fclose(fp);

    fp = fopen("sj.txt", "rb"); //바이너리 읽기 모드
    fread(std, sizeof(std_data[0]), 3, fp); // sj.txt를 읽어서 std에 3개의 정보를 저장
    fclose(fp);

    printf("학생별 평균:\n"); //각 학생별 국어점수와 영어점수의 합 / 2(과목수)
    for (int i = 0; i < 3; i++) {
        printf("%s(%d): %.1lf\n", std[i].name, std[i].id,
            (std[i].kor + std[i].eng) / 2);
    }

    printf("\n과목별 평균:\n");
    printf("kor: %.1lf\n", (std[0].kor + std[1].kor + std[2].kor) / 3);
    // 3명의 국어점수의 합계 / 3(인원수)
    printf("eng: %.1lf\n", (std[0].eng + std[1].eng + std[2].eng) / 3);
    // 3명의 영어점수의 합계 / 3
    return 0;
}
