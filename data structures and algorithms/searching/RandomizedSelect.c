#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRLEN 64

void swap(int* a, int* b);
int randomInRange(int a, int b);

int partition(int* arr, int start, int end);
int randomizedPartition(int* arr, int start, int end);
void randomizedQuickSort(int* arr, int start, int end);
int randomizedSelect(int* arr, int p, int r, int i);

int main(int argc, char* argv[])
{
	int arr[ARRLEN];
	int i, j;

	srand(time(NULL));
	for(i = 0; i < ARRLEN; i++){
		arr[i] = randomInRange(1, ARRLEN);
	}

	for(i = 0; i < 64; i++ ){
		//for(j = 0; j < 8; j++){
			printf("%d\t", arr[i]);
		//}
		//printf("\n");
	}
	printf("\n");

	printf("the k value:%d\n", randomizedSelect(arr, 0, ARRLEN-1, 6));

	randomizedQuickSort(arr, 0, ARRLEN -1);


	for(i = 0; i < 64; i++ ){
		//for(j = 0; j < 8; j++){
			printf("%d\t", arr[i]);
		//}
		//printf("\n");
	}
	printf("\n");

	return 0;
}

void swap(int* a, int* b)
{
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

/*
作用：产生一个取值范围在[a, b]的int值
参数：a,b —— 随机数范围
返回值：取值范围在[a, b]的随机数
注意：在调用该函数前必须先使用srand函数初始化随机种子
*/
int randomInRange(int a, int b)
{
	int result = a + rand() % (b - a);
	return result;
}


int partition(int* arr, int start, int end)
{
	int x, i, j;

	x = arr[end];
	i = start - 1;
	for(j = start; j < end; j++){
		if(arr[j] <= x){
			i = i + 1;
			swap(arr+i, arr+j);
		}
	}
	swap(arr+i+1, arr+end);

	return i+1;
}

int randomizedPartition(int* arr, int start, int end)
{
	int i;
	
	srand(time(NULL));
	i = randomInRange(start, end);
	swap(arr+end, arr+i);
	return partition(arr, start, end);
}

void randomizedQuickSort(int* arr, int start, int end)
{
	if(start < end){
		int q; 
		q = randomizedPartition(arr, start, end);
		randomizedQuickSort(arr, start, q-1);
		randomizedQuickSort(arr, q+1, end);
	}else{
		return;
	}
}


int randomizedSelect(int* arr, int p, int r, int i)
{
	int k,q;

	if(p == r){
		return arr[p];
	}
	
	q = randomizedPartition(arr, p, r);
	k = q - p + 1;
	if(i == k){
		return arr[q];
	}else if(i < k){
		return randomizedSelect(arr, p, q-1, i);
	}else{
		return randomizedSelect(arr, q+1, r, i-k);
	}
}

