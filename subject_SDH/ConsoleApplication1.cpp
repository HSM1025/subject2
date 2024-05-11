#define _CRT_SECURE_NO_WARNINGS	// 보안문제를 무시 (scanf, fopen 등 보안이슈 없이 사용가능)

#include <stdio.h>
typedef struct { // 구조체 정의
    char name[20]; //학생이름(최대 20자)
    int student_id;
    float eng;
    float math;
} Student; // 이름, 학번 , 과목 성적 저장 및 typedef를 사용하여 이 구조체를 student 라는 이름으로 별칭
int main() {
    FILE* fp = fopen("sj.txt", "w"); // 파일에 쓰기 모드로 열기

    Student students[3] = { {"son", 20240101, 85.5, 90.3},  // 학생 정보 입력 및 파일에 저장
                           {"kendrick", 20240102, 92.1, 88.7},
                           {"Aubrey", 20240103, 78.3, 85.2} };
    for (int i = 0; i < 3; i++) {
        fprintf(fp, "%s %d %.3f %.3f\n", students[i].name, students[i].student_id,
            students[i].eng, students[i].math);
    }
    fclose(fp); // 파일 닫기
    fp = fopen("sj.txt", "r");  // 파일에서 읽기
    float avg_eng = 0, avg_math = 0; // 과목 평균 계산을 위한 변수 선언
    printf("personal avg \n");
    for (int i = 0; i < 3; i++) {
        fscanf(fp, "%s %d %f %f", students[i].name, &students[i].student_id,
            &students[i].eng, &students[i].math);

        float personal_avg = (students[i].eng + students[i].math) / 2.0;
        printf("%s personal avg: %.3f\n", students[i].name, personal_avg);

        avg_eng += students[i].eng; // 과목 평균 합산
        avg_math += students[i].math; // 과목 평균 합산
    }

    fclose(fp); // 파일 닫기
    avg_eng /= 3;  // 과목 평균 계산
    avg_math /= 3; // 과목 평균 계산

    printf("subject avg  \n"); // 과목 평균 출력
    printf("eng avg: %.3f\n", avg_eng);// 과목 평균 출력
    printf("math avg: %.3f\n", avg_math);// 과목 평균 출력
    return 0;
}
