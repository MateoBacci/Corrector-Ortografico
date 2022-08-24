#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void remove_special_chars(char *word) {
  int len = strlen(word);
  if (word[len-1] < 'A' || word[len-1] > 'Z') {
    word[len - 1] = '\0';
  }
}

void uppercase (char *word) {
  for (; *word != '\0'; ++word)
    if (*word >= 'a' && *word <= 'z')
      *word -= 'a' - 'A';
}

void normalize_word (char *word) {
  uppercase (word);
  remove_special_chars (word);
}
