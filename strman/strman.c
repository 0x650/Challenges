#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*manip_func_t)(char *buffer);

void handle_upper(char *buffer) {
	for (int i = 0; buffer[i] != '\0'; i++) {
		if (buffer[i] >= 'a' && buffer[i] <= 'z') {
			buffer[i] -= 32;
		}
	}
}

void handle_lower(char *buffer) {
	for (int i = 0; buffer[i] != '\0'; i++) {
                if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
                        buffer[i] += 32;
                }
        }
}

void handle_mocking(char *buffer) {
	handle_lower(buffer);
	for (int i = 0; buffer[i] != '\0'; i++) {
                if (i & 1) {
                        buffer[i] -= 32;
                }
        }
}

void handle_string_manip(char op) {
	manip_func_t func = NULL;
	char buffer[128] = {0};
	switch (op) {
		case 'U':
			func = handle_upper;
			break;
		case 'L':
			func = handle_lower;
			break;
		case 'M':
			func = handle_mocking;
			break;
		default:
			printf("Invalid operation\n");
			exit(-1);
	}
	gets(buffer);
	func(buffer);
	puts(buffer);
}

int main(void) {
	system("echo strman running on $(uname -a)");
	char op[3] = {0};
	printf("Enter operation\nU For upper-case\nL for lower-case\nM for mocking text\n");
	fgets(op, 3, stdin);
	printf("Enter your text\n");
	handle_string_manip(op[0]);
	return 0;
}

