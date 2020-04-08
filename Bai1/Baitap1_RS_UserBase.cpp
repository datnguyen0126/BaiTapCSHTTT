#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

FILE *fin, *fout;
int user, item;
int k;
float rating[100][100];
float averUser[100];

struct Sim {
	float cosineSim;
	float personSim;
};

Sim simUser[100][100];
Sim simItem[100][100];

struct Element {
	float value;
	int index;
};

void init(char* link){
	fin = fopen(link, "r");
	fscanf(fin, "%d", &user);
	fscanf(fin, "%d", &item);
	fscanf(fin, "%d", &k);
	for(int i = 0; i < user; i++){
		for(int j = 0; j < item; j++){
			fscanf(fin, "%f", &rating[i][j]);
		}
	}
	for(int i = 0; i < user; i++){
		float sum = 0;
		int count = 0;
		for(int j = 0; j < item; j++){
			if(rating[i][j] != 0){
				sum += rating[i][j];
				count++;
			}
		}
		averUser[i] = count != 0 ? (float) sum / count : 0;
	}
}

void output(){
	fout = fopen("ketqua.txt", "w");
	for(int i = 0; i < user; i++){
		for(int j = 0; j < item; j++){
			fprintf(fout, "%.3f ", rating[i][j]);
		}
		fprintf(fout, "\n");
	}
}

void sort(Element a[], int n){
	for(int i = 0; i < n - 1; i++){
		for(int j = i + 1; j < n; j++){
			if(a[i].value < a[j].value){
				Element temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}
	}
}

float cosineSimUser(){	
	for(int i = 0; i < user; i++){
		for(int j = 0; j < user; j++){
			float u1 = 0;
			float r1 = 0, r2 = 0;
			if(i == j) simUser[i][j].cosineSim = 1.00;
			else{
				for(int m = 0; m < item; m++){
					if(rating[i][m] != 0 && rating[j][m] != 0){
						u1 += rating[i][m] * rating[j][m];
						r1 += rating[i][m] * rating[i][m];
						r2 += rating[j][m] * rating[j][m];						
					}				
				}
				simUser[i][j].cosineSim = u1 / (sqrt(r1) * sqrt(r2));
			}
		}
	}	
}

float PersonSimUser(){	
	for(int i = 0; i < user; i++){
		for(int j = 0; j < user; j++){
			float u1 = 0;
			float r1 = 0, r2 = 0;
			if(i == j) simUser[i][j].personSim = 1.00;
			else{
				for(int m = 0; m < item; m++){
					if(rating[i][m] != 0 && rating[j][m] != 0){
						u1 += (rating[i][m] - averUser[i]) * (rating[j][m] - averUser[j]);
						r1 += (rating[i][m] - averUser[i]) * (rating[i][m] - averUser[i]);
						r2 += (rating[j][m] - averUser[j]) * (rating[j][m] - averUser[j]);						
					}				
				}
				//printf("\nXet user thu %d va %d: \n%f/(sqrt(%f) * sqrt(%f))\n", i, j, u1, r1, r2);
				simUser[i][j].personSim = u1 / (sqrt(r1) * sqrt(r2));
			}
		}
	}	
}

float countRatingUserBase(){	
	for(int i = 0; i < user; i++){
		for(int j = 0; j < item; j++){
			if(rating[i][j] == 0){
				Element temp[user];
				float sum = 0, sum2 = 0;
				
				for(int i1 = 0; i1 < user; i1++){
					if(rating[i1][j] != 0 && simUser[i1][j].personSim != 1.0){
						temp[i1].value = simUser[i1][j].personSim;
						temp[i1].index = i1;				
					}
					else{
						temp[i1].value = 0.0;
						temp[i1].index = 0;						
					}
				}
				sort(temp, user);	
				
				for(int m = 0; m < k; m++){
					sum += temp[m].value * (rating[temp[m].index][j] - averUser[temp[m].index]);
					sum2 += (rating[temp[m].index][j] - averUser[temp[m].index] >= 0 ? rating[temp[m].index][j] - averUser[temp[m].index] : averUser[temp[m].index] - rating[temp[m].index][j]);
				}				
				rating[i][j] = sum / sum2 + averUser[i];				
			}
		}
	}
}

main(){
	char link[16];
	fflush(stdin);
	printf("Nhap file muon test: ");gets(link);
	fin = fopen(link, "r");
	init(link);
	cosineSimUser();
	PersonSimUser();
	countRatingUserBase();
	output();
	
	printf("\nUser base: \n");
	for(int i = 0; i < user; i++){
		for(int j = 0; j < item; j++){
			printf("%.3f ", rating[i][j]);
		}
		printf("\n");
	}
}
