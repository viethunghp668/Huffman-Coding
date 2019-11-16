#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <wchar.h>
#include <stddef.h>
#include "huffman.h"

void read_text(wchar_t str[], FILE *text_file, wint_t *current_char)
{
  wint_t n = 0;

  while (n != 1000000)
  {
    if (*current_char != WEOF)
    {
      *current_char = fgetwc(text_file);
      str[n] = *current_char;
      n++;
    }
    else
    {
      str[n - 1] = '\0';
      break;
    }
  }
  str[n] = '\0';
}

wint_t alike_check(wchar_t *str, int i, struct chr_fre num_input[])
{
  int j = 0;
  int fre = num_input[j].frequency;

  while (fre != 0)
  {
    fre = num_input[j].frequency;
    if (str[i] == num_input[j].character)
    {
      return j;
    }
    j++;
  }
  return -1;
}

int get_data(struct chr_fre *num_input, FILE *text_file)
{
  int number = 0;
  wint_t j = 0, current_char = 0;
  wchar_t *str = malloc(1000000 * sizeof(wchar_t));
  while(current_char != WEOF)
  {
    read_text(str, text_file, &current_char);
    wint_t k = wcslen(str);

    for (wint_t i = 0; i < k; i++)
    {
      if (alike_check(str, i, num_input) == -1)
      {
        num_input[j].character = str[i];
        num_input[j].frequency = 1;
        num_input[j].node_address = NULL;
        j += 1;
      }
      else
      {
        wint_t pos = alike_check(str, i, num_input);
        num_input[pos].frequency += 1;
      }
    }
  }
  while (num_input[number].frequency != 0)
  {
    number++;
  }
  return number;
}

void bubble(struct chr_fre *number_input, int counter)
{
  for(int i = 0; i < counter - 1; i++)
  {
    for(int j = i + 1; j < counter; j++)
    {
      if(number_input[i].frequency > number_input[j].frequency)
      {
        struct chr_fre *temp = malloc(sizeof(struct chr_fre));
        temp->character = number_input[i].character;
        temp->frequency = number_input[i].frequency;
        temp->node_address = number_input[i].node_address;
        number_input[i].character = number_input[j].character;
        number_input[i].frequency = number_input[j].frequency;
        number_input[i].node_address = number_input[j].node_address;
        number_input[j].character = temp->character;
        number_input[j].frequency = temp->frequency;
        number_input[j].node_address = temp->node_address;

        free(temp);
      }
    }
  }
}

struct tree *input_tree(struct chr_fre num_input1, struct chr_fre num_input2)
{
  struct tree *left_node;
  struct tree *right_node;
  struct tree *root = malloc(sizeof(struct tree));

  if (num_input1.node_address == NULL)
  {
    left_node = malloc(sizeof(struct tree));
    left_node->value = num_input1.frequency;
    left_node->ch = num_input1.character;
    left_node->left = NULL;
    left_node->right = NULL;
  }
  else
  {
    left_node = num_input1.node_address;
  }

  if (num_input2.node_address == NULL)
  {
    right_node = malloc(sizeof(struct tree));
    right_node->ch = num_input2.character;
    right_node->value = num_input2.frequency;
    right_node->left = NULL;
    right_node->right = NULL;
  }
  else
  {
    right_node = num_input2.node_address;
  }

  root->value = num_input1.frequency + num_input2.frequency;
  root->ch = '\0';
  root->left = left_node;
  root->right = right_node;

  return root;
}

void pre_order_traversal(struct tree *root, struct array_of_tree a[], int *i, int *p)
{

  if (root != NULL)
  {
    a[*i].ch = root->ch;
    a[*i].value = root->value;
    a[*i].left = *i + 1;

    if (root->left == NULL)
    {
      a[*i].left = 0;
    }

    *i += 1;
    *p = *i;

    pre_order_traversal(root->left, a, i, p);
    pre_order_traversal(root->right, a, i, p);
    do
    {
      *p -= 1;
    } while (a[*p - 1].right != 0);
    a[*p - 1].right = *i;
  }
}

void build_tree(struct chr_fre *num_input, struct array_of_tree a[], struct result *final, int number)
{
  int i = 0, p = 0, node = 0, k = 0;
  char temp_bit[100];
  for (int j = 0; j < number - 1; j++)
  {
    bubble(num_input, number);

    num_input[j + 1].node_address = input_tree(num_input[j], num_input[j + 1]);
    num_input[j + 1].frequency += num_input[j].frequency;
    num_input[j + 1].character = '\0';
  }

  pre_order_traversal(num_input[number - 1].node_address, a, &i, &p);

  for (int j = 0; j < (2 * number - 1); j++)
  {
    if (a[j].left == 0)
    {
      a[j].right = 0;
    }
  }
}

void post_order(struct tree *root, int node, char *temp_bit, struct result *final, int *k)
{
  node++;
  if (root != NULL)
  {
    temp_bit[node - 1] = '0';
    post_order(root->left, node, temp_bit, final, k);
    temp_bit[node - 1] = '\0';

    temp_bit[node - 1] = '1';
    post_order(root->right, node, temp_bit, final, k);
    temp_bit[node - 1] = '\0';

    if (root->ch != '\0')
    {
      final[*k].ch = root->ch;
      int n = strlen(temp_bit);
      final[*k].str = malloc(n * sizeof(char));
      strcpy(final[*k].str, temp_bit);
      *k += 1;
    }
  }
  node--;
}

void encode_character(struct tree *root, struct result *final)
{
  char temp_bit[100];
  int k = 0, node = 0;
  post_order(root, node, temp_bit, final, &k);
}

void padding_0(char s[], char temp[], int n)
{
  if (n % 8 == 0)
  {
    strcpy(temp, s);
  }
  else
  {
    for (int k = 0; k < n; k++)
    {
      temp[k] = s[k];
    }
    for (int k = n; k < n + (8 - (n % 8)); k++)
    {
      temp[k] = '0';
    }

    temp[n + (8 - (n % 8))] = '\0';
  }
}

void split_string(char s[], char k[], int a, int b)
{
  int count = 0;

  for (int i = a; i <= b; i++)
  {
    k[count] = s[i];
    count++;
  }

  k[count] = '\0';
}

unsigned char pow_num(int x)
{
  unsigned char result = 1;
  if (x == 0)
  {
    result = 1;
  }
  else
  {
    for (int i = 1; i <= x; i++)
    {
      result *= 2;
    }
  }
  return result;
}

unsigned char read_per_8bit(char *a)
{
  unsigned char value = 0;
  for (int i = 0; i < 8; i++)
  {
    if (a[i] == '1')
    {
      value += pow_num((7 - i));
    }
  }
  return value;
}

void read_binary(char *s, char *temp, unsigned char *result_of_huffman)
{
  int n = strlen(s);
  char blank[9];

  padding_0(s, temp, n);

  int m = strlen(temp);
  int loop = (m / 8);
  int a = 0, b = 7;

  for (int i = 0; i < loop; i++)
  {
    split_string(temp, blank, a, b);
    result_of_huffman[i] = read_per_8bit(blank);
    a += 8;
    b += 8;
  }
  result_of_huffman[loop] = '\0';
}

void binary_code(char binary_string[], wchar_t *string, struct result final[], unsigned char result_of_huffman[], int *length, int *j, FILE *f)
{
  char *temp_bin = malloc(8000 * sizeof(char));
  wint_t i = 0, n = 0;
  n = wcslen(string);
  while (i < n)
  {
    wint_t k = 0, m = 0;
    while (final[m].ch != string[i])
    {
      m++;
    }
    while (final[m].str[k] != '\0')
    {
      binary_string[*j] = final[m].str[k];
      *j += 1;
      k += 1;
      if (*j == 8000)
      {
        binary_string[*j] = '\0';
        read_binary(binary_string, temp_bin, result_of_huffman);
        *length += 1000;
        rewind(f);
        fwrite(length, sizeof(int), 1, f);
        fseek(f, 0, SEEK_END);
        for (int i = 0; i < 1000; i++)
        {
          fwrite(&result_of_huffman[i], sizeof(unsigned char), 1, f);
        }
        *j = 0;
      }
    }
    i += 1;
  }
}

int amount_of_0_added(int n)
{
  if (n % 8 == 0)
  {
    return 0;
  }
  else
  {
    int i = 0;
    i = 8 - (n % 8);
    return i;
  }
}

void save_others(struct array_of_tree a[], int p, FILE *f, int amount_0)
{
  if (f != NULL)
  {
    fseek(f, 0, SEEK_END);
    fwrite(&amount_0, sizeof(int), 1, f);
    fwrite(&p, sizeof(int), 1, f);
    for (int i = 0; i < p; i++)
    {
      fwrite(&a[i], sizeof(struct array_of_tree), 1, f);
    }
  }
}

void encrypt_root_file_and_save_result(struct result final[], int number, struct array_of_tree a[], FILE *f, FILE *wchar_f)
{
  char *binary_string = malloc(8000 * sizeof(char));
  char *cursor = binary_string;

  wchar_t *str = malloc(1000000 * sizeof(wchar_t));
  int length = 0, amount_0 = 0, j = 0;
  unsigned char *result_of_huffman = malloc(1000 * sizeof(unsigned char));
  char *temp_bin = malloc(8000 * sizeof(char));

  wint_t current_char = 0;

  while (current_char != WEOF)
  {
    read_text(str, wchar_f, &current_char);
    binary_code(binary_string, str, final, result_of_huffman, &length, &j, f);
  }
  binary_string[j] = '\0';
  amount_0 = amount_of_0_added(strlen(binary_string));
  length += ((strlen(binary_string) + amount_0) / 8);
  rewind(f);
  fwrite(&length, sizeof(int), 1, f);
  fseek(f, 0, SEEK_END);
  read_binary(binary_string, temp_bin, result_of_huffman);
  for (int i = 0; i < ((strlen(binary_string) + amount_0) / 8); i++)
  {
    fwrite(&result_of_huffman[i], sizeof(unsigned char), 1, f);
  }

  save_others(a, ((2 * number) - 1), f, amount_0);
}
