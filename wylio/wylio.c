#include <stdio.h>
#include <string.h>
#include <Wyliodrin.h>

int main(int argc, char **argv) {
  if (argc == 2) {
    if (strncmp(argv[1], "--version", strlen("--version")) == 0 ||
        strncmp(argv[1], "-v", strlen("-v")) == 0) {
      printf("v%d.%d\n", get_version_major(), get_version_minor());
    }
  }

  return 0;
}