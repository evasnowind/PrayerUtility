#include <stdio.h>

void getStringPermutation(char* str);
void getPermutation(char* str, char* strBegin);

int main(int argc, char* argv[])
{
	getStringPermutation(argv[1]);	
	return 0;
}

void getStringPermutation(char* str)
{
	if(str == NULL){
		return;
	}

	return getPermutation(str, str);
}

void getPermutation(char* str, char* strBegin)
{
	char* itr;
	char tmp;

	if(*strBegin == '\0'){
		printf("%s\n", str);
	}else{
		for(itr = strBegin; *itr != '\0'; itr++){
			tmp = *itr;
			*itr = *strBegin;
			*strBegin = tmp;

			getPermutation(str, strBegin+1);

			tmp = *itr;
			*itr = *strBegin;
			*strBegin = tmp;
		}
	}
}
