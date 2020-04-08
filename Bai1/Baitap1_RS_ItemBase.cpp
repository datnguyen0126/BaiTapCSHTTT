#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

FILE *fin, *fout;
int user, item;
int k;
float rating[100][100];
float averItem[100];

struct Sim {
	float cosineSim;
	float personSim;
};

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
	for(int i = 0; i < item; i++){
		float sum = 0;
		int count = 0;
		for(int j = 0; j < user; j++){
			if(rating[j][i] != 0){
				sum += rating[j][i];
				count++;
			}
		}
		averItem[i] = count != 0 ? (float) sum / count : 0;
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

float cosineSimItem(){	
	for(int i = 0; i < item; i++){
		for(int j = 0; j < item; j++){
			float u1 = 0;
			float r1 = 0, r2 = 0;
			if(i == j) simItem[i][j].cosineSim = 1.00;
			else{
				for(int m = 0; m < user; m++){
					if(rating[m][i] != 0 && rating[m][j] != 0){
						u1 += rating[m][i] * rating[m][j];
						r1 += rating[m][i] * rating[m][i];
						r2 += rating[m][j] * rating[m][j];						
					}				
				}
				//printf("\nXet user thu %d va %d: \n%f/(sqrt(%f) * sqrt(%f))\n", i, j, u1, r1, r2);
				simItem[i][j].cosineSim = u1 / (sqrt(r1) * sqrt(r2));
			}
		}
	}	
}

float personSimItem(){	
	for(int i = 0; i < item; i++){
		for(int j = 0; j < item; j++){
			float u1 = 0;
			float r1 = 0, r2 = 0;
			if(i == j) simItem[i][j].cosineSim = 1.00;
			else{
				for(int m = 0; m < user; m++){
					if(rating[m][i] != 0 && rating[m][j] != 0){
						u1 += (rating[m][i] - averItem[i]) * (rating[m][j] - averItem[j]);
						r1 += (rating[m][i] - averItem[i]) * (rating[m][i] - averItem[i]);
						r2 += (rating[m][j] - averItem[j]) * (rating[m][j] - averItem[j]);						
					}				
				}
				//printf("\nXet user thu %d va %d: \n%f/(sqrt(%f) * sqrt(%f))\n", i, j, u1, r1, r2);
				simItem[i][j].personSim = (r1 != 0 && r2 != 0) ? u1 / (sqrt(r1) * sqrt(r2)) : 0;
			}
		}
	}	
}

float countRatingItemBase(){	
	for(int i = 0; i < item; i++){
		for(int j = 0; j < user; j++){
			if(rating[j][i] == 0){
				Element temp[item];
				float sum = 0, sum2 = 0;
				
				for(int i1 = 0; i1 < item; i1++){
					if(rating[j][i1] != 0 && simItem[j][i1].personSim != 1.0){
						temp[i1].value = simItem[i1][i].personSim;
						temp[i1].index = i1;				
					}
					else{
						temp[i1].value = 0.0;
						temp[i1].index = 0;						
					}
				}
				sort(temp, item);	
				
				for(int m = 0; m < k; m++){
					sum += temp[m].value * (rating[temp[m].index][i] - averItem[temp[m].index]);
					sum2 += (rating[temp[m].index][i] - averItem[temp[m].index] >= 0 ? rating[temp[m].index][i] - averItem[temp[m].index] : averItem[temp[m].index] - rating[temp[m].index][i]);
				}				
				rating[j][i] = sum / sum2 + averItem[j];				
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
	cosineSimItem();
	personSimItem();
	countRatingItemBase();
	output();
	printf("\nItem base: \n");
	for(int i = 0; i < user; i++){
		for(int j = 0; j < item; j++){
			printf("%.3f ", rating[i][j]);
		}
		printf("\n");
	}
}
