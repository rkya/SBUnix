#include <string.h>


static char *token_string = NULL;
static int token_string_ptr = 0;

/***
 * Reverses the contents of the string. In this function, the string is reversed in place.
 * No new string is created.
 * @param str whose contents are to be reversed
 * @return 1 if the operation is successful
 */
int strrev(char *str) {
	int i = 0, j = strlen(str);
	while(i < j) {
		char ch = str[i];
		str[i] = str[j];
		str[j] = ch;
		i++;
		j--;
	}
	return 1;
}

/***
 * Compares two strings.
 * @param str1 the first string
 * @param str2 the second string
 * @return 	<li>0 if the strings are equal</li>
 * 					<li>1 if the first string is greater than second string</li>
 * 					<li>-1 if the second string is greater than the first string</li>
 */
int strcmp(const char *str1, const char *str2) {
	int i = 0;
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if(len1 > len2) {
		return 1;
	} else if(len1 < len2) {
		return -1;
	}
	while(str1[i] != '\0' && str1[i] == str2[i]) {
		i++;
	}
	if(str1[i] == '\0') {
		return 0;
	}
	if(str1[i] > str2[i]) {
	  return 1;
	}
	else {
	  return -1;
	}
	//return str1[i] > str2[i];
}

/*int strsubcmp(char *str1, char *str2, int from, int to) {
	int i = 0;
	int j = from;
	int len1 = strlen(str1);
	//int len2 = strlen(str2);
	int len2 = to - from;
	if(len1 > len2) {
		return 1;
	} else if(len1 < len2) {
		return -1;
	}
	while(str1[i] != '\0' && str1[i] == str2[j] && j < to) {
		i++;
		j++;
	}
	if(str1[i] == '\0') {
		return 0;
	}
	if(str1[i] > str2[j]) {
	  return 1;
	}
	else {
	  return -1;
	}
	//return str1[i] > str2[i];
}*/

/***
 * Copy the second string to the first string
 * @param str1 where content is to be copied
 * @param str2 which is to be copied
 * @return 1 when operation is successful
 */
int strcpy(char *str1, char *str2) {
	int i = 0;
	int len = strlen(str2);
	for(i = 0 ; i < len; i++) {
		str1[i] = str2[i];
	}
	str1[i] = '\0';
	return 1;
}

/***
 * Copy substring of a string to another string
 * @param str1 where contents are to be copied
 * @param str2 whose substring is to be copied
 * @param from starting index of str2 inclusive
 * @param to ending index of str2 exclusive
 * @return 1 if the operation is successful
 */
int strsubcpy(char *str1, char *str2, int from, int to) {
	int i;
	for(i = from; i < to; i++) {
		str1[i] = str2[i];
	}
	return 1;
}

/***
 * Concatinate the second string to the first string.
 * @param str1 to which other string is to be concatinated
 * @param str2 which will be concatinated
 * @return 1 if the operation is successful
 */
int strcat(char *str1, char *str2) {
	int len1 = strlen(str1);
	//int len2 = strlen(str2);
	//str1 = realloc(str1, sizeof(char) * (len1 + len2 + 1));
	strcpy(&str1[len1], str2);
	return 1;
}

/***
 * Returns the length of the string
 * @param str whose length is to be calculated
 * @return the length of the string
 */
int strlen(const char *str) {
	int len = 0;
	while(str[len++] != '\0');
	return len - 1;
}

char *strtok(char *string, const char *delimiter) {
	if(string != NULL) {
		token_string = string;
		token_string_ptr = 0;
	}
	if(delimiter == NULL) {
		char *temp = token_string;
		token_string = NULL;
		token_string_ptr = 0;
		return temp;
	}

	int i = token_string_ptr;
	int j = 0;
	while(token_string[i] != '\0') {
		while(token_string[i] != delimiter[j] && delimiter[j] != '\0') {
			//i++;
			j++;
		}
		if(delimiter[j] == '\0') {
			j = 0;
			i++;
		} else {
			char *new_token = (char *) malloc (sizeof(char) * (i - token_string_ptr + 1));
			int l = 0;
			for(int k = token_string_ptr; k < i; k++, l++) {
				new_token[l] = token_string[k];
			}
			new_token[l] = '\0';
			token_string_ptr = i + 1;
			return new_token;
		}
	}
	/*while(token_string[i] != '\0') {
		while(token_string[i] == delimiter[j] && token_string[i] != '\0') {
			i++;
			j++;
		}
		if(token_string[i] != delimiter[j]) {
			j = 0;
			i++;
		} else {
			char *new_token = (char *) malloc (sizeof(char) * (i - token_string_ptr + 1 - strlen(delimiter)));
			int l = 0;
			for(int k = token_string_ptr; k < i - strlen(delimiter); k++, l++) {
				new_token[l] = token_string[k];
			}
			new_token[l] = '\0';
			token_string_ptr = i;
			return new_token;
		}
	}*/
	char *new_token = (char *) malloc (sizeof(char) * (strlen(token_string) - token_string_ptr + 1));
	int l = 0;
	for(int k = token_string_ptr; k < strlen(token_string); k++, l++) {
		new_token[l] = token_string[k];
	}
	new_token[l] = '\0';
	token_string_ptr += strlen(new_token);
	return new_token;
}