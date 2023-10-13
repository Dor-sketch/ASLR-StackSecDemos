
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  printf("Press:\n0 - to quit\n1 - for ASLR off\n2 -  for ASLR on: ");
  int choice;
  int i;
  scanf("%d", &choice);
  while (choice != 0) {
    printf("choice = %d\n", choice);
    if (choice == 1) {
      system(
          "echo \"1\" | sudo -S sh -c 'echo \"0\" > "
          "/proc/sys/kernel/randomize_va_space'");
      printf("\nASLR is OFF\n");
    } else if (choice == 2) {
      printf(
          "echo \"1\" | sudo -S sh -c 'echo \"2\" > "
          "/proc/sys/kernel/randomize_va_space'");
      system(
          "echo \"1\" | sudo -S sh -c 'echo \"2\" > "
          "/proc/sys/kernel/randomize_va_space'");
      printf("\nASLR is ON\n");
    }

    for (i = 0; i < 10; i++) {
      system("./print_mem");
    }

    printf("Press:\n0 - to quit\n1 - for ASLR off\n2 -  for ASLR on: ");
    scanf("%d", &choice);
  }
  return 0;
}
