#include <stdio.h>

extern long long asmpow(long long num, long long power);

int main() {
  long long result = asmpow(10, 2);

  printf("result of asmpow when called from C: %lld\n", result);
  return 0;
}
