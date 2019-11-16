#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <stddef.h>
#include "huffman.h"

int main()
{
  setlocale(LC_CTYPE, "vi_VN.UTF8");
  FILE *f = NULL;
  f = fopen("result.dat", "rb");
  int str_len = 0, amount_0 = 0, p = 0;
  unsigned char *str = NULL;
  struct array_of_tree *a = NULL;


  if(f != NULL)
  {
    fread(&str_len, sizeof(int), 1, f);
    str = malloc(str_len * sizeof(unsigned char));
    for (int i = 0; i < str_len; i++)
    {
      fread(&str[i], sizeof(unsigned char), 1, f);
    }
    fread(&amount_0, sizeof(int), 1, f);
    fread(&p, sizeof(int), 1, f);
    a = malloc(p * sizeof(struct array_of_tree));
    int i = 0;
    while(!feof(f))
    {
      fread(&a[i], sizeof(struct array_of_tree), 1, f);
      i++;
    }

    fclose(f);
  }

  decompress_huffman(a, str, str_len, amount_0);

  return 0;
}
