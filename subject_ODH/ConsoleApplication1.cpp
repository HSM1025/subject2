#define _CRT_SECURE_NO_WARNINGS	// 보안문제를 무시 (scanf, fopen 등 보안이슈 없이 사용가능)

#include<stdio.h>
typedef struct Student {
	char name[80];
	int id;
	int score1;
	int score2;
} S;

int main() {

	S info[3] = {
		{"ohdohyun", 20050412, 68, 10},
		{"dohyunoh", 20051204, 92, 21},
		{"hyunohdo", 20052041, 100, 30}
	};

	FILE* fp1 = fopen("sj.txt", "w");
	for (int i = 0; i < 3; i++) {
		fprintf(fp1, "%s %d %d %d\n", info[i].name, info[i].id, info[i].score1, info[i].score2);
	}
	fclose(fp1);

	FILE* fp2 = fopen("sj.txt", "r");
	for (int i = 0; i < 3; i++) {
		fscanf(fp2, "%s %d %d %d\n", info[i].name, &info[i].id, &info[i].score1, &info[i].score2);
	}
	fclose(fp2);

	float avg1 = 0, avg2 = 0, indiAvg = 0;

	for (int i = 0; i < 3; i++) {
		avg1 += info[i].score1;
		avg2 += info[i].score2;
	}
	printf("Subject 1 Avg : %.1f\n", avg1 / 3.0);
	printf("Subject 2 Avg : %.1f\n", avg2 / 3.0);

	for (int i = 0; i < 3; i++) {
		indiAvg = (info[i].score1 + info[i].score2) / 2.0;
		printf("%d_%s's Avg : %.1f\n", info[i].id, info[i].name, indiAvg);
	}
	return 0;
}
