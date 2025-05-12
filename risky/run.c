#include <unistd.h>


int main(void) {
	char *args[] = {"qemu-riscv64", "risky", NULL};
	execv("./qemu-riscv64", args);
	return 0;
}
