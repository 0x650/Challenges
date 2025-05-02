#include <stdio.h>

char base[] = "9\x04\x0e\x03\x05?";

char func(int x) {
	return (char)((x * x) - (12 * x) + (101));
}

int main(void) {
	printf("Can you tell me the minima?\n");
	char output[16] = {0};
	int x = 0;
	scanf("%d", &x);
	for (int i = 0; i < 15; i++) {
		output[i] = base[i] ^  func(x);
	}
	printf("Heres your message: %s Have a nice day!\n", output);
	return 0;
}
