#include <stdio.h>
#include <stdlib.h>

int binarySearch(int* arr,int low, int high, int num);

int getFirstK(int* data, int length, int k, int start, int end);
int getLastK(int* data, int length, int k, int start, int end);

int main(int argc, char* argv[])
{

	int* arr;
	int i,n, num;

	scanf("%d %d", &n, &num);
	arr = (int*)malloc(sizeof(int) * n);

	for(i = 0; i < n; i++){
		arr[i] = i;
	}
	
	printf("result:%d\n",binarySearch(arr, 0, n-1, num));

	free(arr);

	return 0;
}

int binarySearch(int* arr, int low, int high, int num){
	int pos;

	while(low <= high){
		pos = (low + high) / 2;
		if(arr[pos] == num){
			break;
		}else if(arr[pos] > num){
			high = pos - 1;
		}else{
			low = pos + 1;
		}
		if(low > high){
			pos = -1;
			break;
		}
	}
	return pos;
}


int getFirstK(int* data, int length, int k, int start, int end)
{
	int middleIndex;
	int middleData;

	if(start > end){
		return -1;
	}

	middleIndex = (start + end) / 2;
	middleData = data[middleIndex];

	if(middleData == k){
		if((middleIndex > 0 && data[middleIndex - 1] != k) || middleIndex == 0){
			return middleIndex;
		}else{
			end = middleIndex - 1;
		}
	}
}
