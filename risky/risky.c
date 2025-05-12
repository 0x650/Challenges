#include <stdio.h>
#include <stdlib.h>

char *gets(char *str);

void echo(void) {
	char buffer[128] = {0};
	gets(buffer);
	printf(buffer);
}

int main(void) {
	system("echo Welcome to Risky Business");
	for (;;) {
		echo();
		putchar('\n');
		fflush(stdout);
	}
}
