#include <stdio.h>
#include <unistd.h>
int main(){
	int i = 0;
	printf("Demo is running\n");
	while(i<2){
		i=i+1;
		sleep(1);
		printf("Demo has running %d seconds.\n", i);
		fflush(stdout);
	}
	printf("Demo is ending.\n");
	
}
