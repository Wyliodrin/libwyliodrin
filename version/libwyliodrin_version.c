#include <stdio.h>
#include <Wyliodrin.h>

int main() {
  printf("v%d.%d\n", get_version_major(), get_version_minor());
  return 0;
}