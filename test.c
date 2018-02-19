#include <stdio.h> 

int chartest(char n)
{
	printf("%c",n);
	return chartest;
}
int main()
{
	int loop1;
	char p[30];
	for(loop1 = 0 ; loop1<=10 ; loop1++)
	{
	printf("Insert Char :");
	scanf("%c",&p[loop1]);
	chartest(p[loop1]);
	}	
}

