#include <iostream>
#include <stdio.h>

using namespace std;

void dump(const char* str){
    for (int p = 0; p<1024; ++p){
        printf("%u ", str[p]);
    }
    printf("\n");
}


int main(){
	char str[2048]="\x99\x88\x77\x66\x55\x44\x33\x22";
	int* ptr = (int*)str;
	*ptr = 1024;
	
	cout<<*ptr<<endl<<str<<endl;
	dump(str);
	return 0;
}
