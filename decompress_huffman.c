#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <wchar.h>
#include <stddef.h>
#include "huffman.h"

void dec_to_bin(unsigned char c, char binary_num[])
{

  int i = 7;
  unsigned char value = 0;
  while (i >= 0)
  {
    value = c % 2;

    if (value == 0)
    {
      binary_num[i] = '0';
    }
    else
    {
      binary_num[i] = '1';
    }

    c = (c - value) / 2;
    i--;
  }
  binary_num[8] = '\0';
}

void decompress_huffman(struct array_of_tree a[], unsigned char str[], int str_len, int amount_0)
{
  FILE *f = NULL;
  f = fopen("result_after_decode.txt", "wb");
  char *bin_string = malloc(8000 * sizeof(char));
  char *root_bin = malloc((8000) * sizeof(char));
  char *binary_num = malloc(9 * sizeof(char));
  wchar_t *result = malloc(1000 * sizeof(wchar_t));
  int z = 0, k = 0;
  unsigned char *multibyte_str = malloc(4000 * sizeof(unsigned char));
  struct array_of_tree *x = malloc(sizeof(struct array_of_tree));
  x->ch = a[0].ch;
  x->value = a[0].value;
  x->left = a[0].left;
  x->right = a[0].right;
  for (int j = 0; j < str_len; j++)
  {
    int m = 0;
    dec_to_bin(str[j], binary_num);
    while (binary_num[m] != '\0')
    {
      bin_string[k] = binary_num[m];
      k++;
      m++;
      if (k == 1000)
      {
        bin_string[k] = '\0';
        decode_huffman(bin_string, a, result, &z, x);
        result[z] = '\0';
        z = 0;
        size_t len = wcstombs(multibyte_str, result, 4000 * sizeof(unsigned char));
        for (size_t i = 0; i < len; i++)
        {
          fwrite(&multibyte_str[i], sizeof(unsigned char), 1, f);
        }
        k = 0;
      }
    }
  }
  bin_string[k] = '\0';
  int len_of_bin_str = strlen(bin_string);
  for (int j = 0; j < (len_of_bin_str - amount_0); j++)
  {
    root_bin[j] = bin_string[j];
  }
  root_bin[len_of_bin_str - amount_0] = '\0';
  decode_huffman(root_bin, a, result, &z, x);
  result[z] = '\0';
  z = 0;
  size_t len = wcstombs(multibyte_str, result, 4000 * sizeof(unsigned char));
  for (wint_t i = 0; i < len; i++)
  {
    fwrite(&multibyte_str[i], sizeof(unsigned char), 1, f);
  }

  fclose(f);
}

void decode_huffman(char root_bin[], struct array_of_tree a[], wchar_t result[], int *k, struct array_of_tree *x)
{
  int m = strlen(root_bin);
  for (int i = 0; i < m; i++)
  {
    if (root_bin[i] == '0')
    {
      int l = x->left;
      x->left = a[l].left;
      x->ch = a[l].ch;
      x->value = a[l].value;
      x->right = a[l].right;
    }
    else if (root_bin[i] == '1')
    {
      int r = x->right;
      x->left = a[r].left;
      x->ch = a[r].ch;
      x->value = a[r].value;
      x->right = a[r].right;
    }
    if (x->ch != '\0')
    {
      result[*k] = x->ch;
      x->ch = a[0].ch;
      x->value = a[0].value;
      x->left = a[0].left;
      x->right = a[0].right;
      *k += 1;
    }
  }
}

wint_t hash_code(wchar_t c)
{
  wint_t value = c % 300;

  return value;
}

void insert(struct result hash_array[], struct result final[], int number)
{
  for (int i = 0; i < number; i++)
  {
    wint_t hash_index = hash_code(final[i].ch);

    hash_array[hash_index].str = final[i].str;
    hash_array[hash_index].ch = final[i].ch;
  }
}

wchar_t utf8_decode(unsigned char result[], int temp_i)
{
  wchar_t x = 0;
  if (result[temp_i] >= 192 && result[temp_i] <= 223)
  {
    x = (result[temp_i] - 192) * pow_num(6) + result[temp_i + 1] - pow_num(7);
  }
  if (result[temp_i] <= 239 && result[temp_i] >= 224)
  {
    x = (result[temp_i] - 224) * pow_num(12) + ((result[temp_i + 1] - 128) * pow_num(6)) + result[temp_i + 2] - pow_num(7);
  }
  return x;
}
