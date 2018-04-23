#include <math.h>


/***
 * Converts octal string to integer
 * @param str string of the octal number
 * @param size of the string
 * @return integer value of the octal string
 */
int octal_to_binary(unsigned char *str, int size)
{
  int num = 0;
  unsigned char *ptr = str;
  while (size-- > 0) {
    num *= 8;
    num += *ptr - '0';
    ptr++;
  }
  return num;
}

/***
 * Returns x^y value.
 * @param base x
 * @param pow y
 * @return x^y
 */
uint64_t power(uint64_t base, uint64_t pow) {
	if(pow == 0)
		return 1;
	uint64_t answer = base;
	while(--pow) {
		answer *= base;
	}
	return answer;
}

int stoi(char *str) {
  int negative = 0;
  int number = 0;
  int index = 0;

  if(str[index] == '-') {
    negative = 1;
    index++;
  }

  while(str[index] >= '0' && str[index] <= '9') {
    number = number * 10 + str[index] - '0';
    index++;
  }

  return negative ? number * -1 : number;
}
