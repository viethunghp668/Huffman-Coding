#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <stddef.h>
#include "huffman.h"


void alike_check_test()
{
  wchar_t data[3][4] = {{'a', 'b', 'c', 'd'}, {'1', '2', '3', '4'}, {'j', 'h', 'a', 's'}};
  wint_t result[3][7] = {{0, -1, -1, -1}, {-1, -1, -1, 2}, {-1, -1, 0, 1}};
  wchar_t chr[3] = {'a', 's', '4'};
  struct chr_fre *num_input = malloc(5 * sizeof(struct chr_fre));
  for(wint_t i = 0; i <= 2; i++)
  {
    num_input[i].character = chr[i];
    num_input[i].frequency = 1;
  }
  for (wint_t i = 3; i <= 4; i++)
  {
    num_input[i].character = '\0';
    num_input[i].frequency = 0;
  }

  for (wint_t i = 0; i < 3; i++)
  {
    for(int j = 0; j < 4; j++)
    {
      if (alike_check(data[i], j, num_input) != result[i][j])
      {
        wprintf(L"Wrong: %d -> %d\n", alike_check(data[i], j, num_input), result[i][j]);
      }
      else
      {
        wprintf(L"Right\n");
      }
    }
  }
}



void read_text_test()
{
  wchar_t result_str[] = L"Lê Việt Hùng\n";
  wchar_t str[80];
  wint_t current_chr = 0;
  FILE *f = fopen("test.txt", "r");
  read_text(str, f, &current_chr);
  if(wcscmp(result_str, str) != 0)
  {
    wprintf(L"%ls\n%ls\n", result_str, str);
  }
  else
  {
    wprintf(L"Right\n");
  }
}

void get_data_test()
{
  struct chr_fre *num_input = malloc(5 * sizeof(struct chr_fre));
  struct chr_fre* num_input_result = malloc(5 * sizeof(struct chr_fre));
  FILE *text_file = NULL;
  text_file = fopen("test.txt", "r");
  wchar_t str[] = L"â bệ\n";
  int fre[5] = {4, 10, 5, 2, 1};

  for(int i = 0; i < 5; i++)
  {
    num_input_result[i].character = str[i];
    num_input_result[i].frequency = fre[i];
    num_input_result[i].node_address = NULL;
  }
  int x = get_data(num_input, text_file);
  if (x != 5)
  {
    wprintf(L"Wrong: %d -> %d\n", x, 5);
  }
  else
  {
    for (int j = 0; j < 5; j++)
    {
      if (num_input_result[j].character != num_input[j].character || num_input_result[j].frequency != num_input[j].frequency)
      {
        wprintf(L"Wrong: %lc <- %lc\n       %d <- %d\n", num_input_result[j].character, num_input[j].character, num_input_result[j].frequency, num_input[j].frequency);
      }
      else
      {
        wprintf(L"Right\n");
      }
    }
  }
  fclose(text_file);
}

void encode_character_test()
{
  struct chr_fre *num_input_result = malloc(5 * sizeof(struct chr_fre));
  wchar_t str[] = L" b\nệâ";
  int fre[5] = {10, 5, 1, 2, 4};

  for (int i = 0; i < 5; i++)
  {
    num_input_result[i].character = str[i];
    num_input_result[i].frequency = fre[i];
    num_input_result[i].node_address = NULL;
  }
  struct array_of_tree *a = malloc(9 * sizeof(struct array_of_tree));
  struct result *final = malloc (5 * sizeof(struct result));
  char bin_str[5][5] = {"0", "10", "1100", "1101", "111"};
  struct result *truly_final = malloc(5 * sizeof(struct result));
  for (int i = 0; i < 5; i++)
  {
    truly_final[i].str = bin_str[i];
    truly_final[i].ch = str[i];
  }
  build_tree(num_input_result, a, final, 5);
  encode_character(num_input_result[4].node_address, final);
  for (int j = 0; j < 5; j++)
  {
    if (truly_final[j].ch != final[j].ch || strcmp(truly_final[j].str, final[j].str) != 0)
    {
      wprintf(L"Wrong: %lc <- %lc\n       %s <- %s\n", truly_final[j].ch, final[j].ch, truly_final[j].str, final[j].str);
    }
    else
    {
      wprintf(L"Right\n");
    }
  }
}

void build_tree_test()
{
  struct chr_fre *num_input_result = malloc(5 * sizeof(struct chr_fre));
  wchar_t str[] = L" b\nệâ";
  int fre[5] = {10, 5, 1, 2, 4};

  for (int i = 0; i < 5; i++)
  {
    num_input_result[i].character = str[i];
    num_input_result[i].frequency = fre[i];
    num_input_result[i].node_address = NULL;
  }
  struct array_of_tree *a = malloc(9 * sizeof(struct array_of_tree));
  struct result *final = malloc(5 * sizeof(struct result));
  struct array_of_tree *true_a = malloc(9 * sizeof(struct array_of_tree));
  int left[9] = {1, 0, 3, 0, 5, 6, 0, 0, 0};
  int right[9] = {2, 0, 4, 0, 8, 7, 0, 0, 0};
  int value[9] = {22, 10, 12, 5, 7, 3, 1, 2, 4};
  wchar_t chr[] = L"\0 \0b\0\0\nệâ";
  for(int i = 0; i < 9; i++)
  {
    true_a[i].ch = chr[i];
    true_a[i].value = value[i];
    true_a[i].left = left[i];
    true_a[i].right = right[i];
  }
  build_tree(num_input_result, a, final, 5);
  for (int j = 0; j < 9; j++)
  {
    if (true_a[j].ch != a[j].ch || true_a[j].value != a[j].value || true_a[j].left != a[j].left || true_a[j].right != a[j].right)
    {
      wprintf(L"Wrong: %lc <- %lc\n       %d <- %d\n", true_a[j].ch, a[j].ch, true_a[j].value, a[j].value);
    }
    else
    {
      wprintf(L"Right\n");
    }
  }
}

void encrypt_root_file_and_save_result_test()
{
  FILE *wchar_f = NULL, *f = NULL;
  f = fopen("result_test.dat", "wb");
  wchar_f = fopen("test.txt", "r");
  wchar_t str[] = L" b\nệâ";
  char bin_str[5][5] = {"0", "10", "1100", "1101", "111"};
  struct result *truly_final = malloc(5 * sizeof(struct result));
  for (int i = 0; i < 5; i++)
  {
    truly_final[i].str = bin_str[i];
    truly_final[i].ch = str[i];
  }
  struct array_of_tree *true_a = malloc(9 * sizeof(struct array_of_tree));
  int left[9] = {1, 0, 3, 0, 5, 6, 0, 0, 0};
  int right[9] = {2, 0, 4, 0, 8, 7, 0, 0, 0};
  int value[9] = {22, 10, 12, 5, 7, 3, 1, 2, 4};
  wchar_t chr[] = L"\0 \0b\0\0\nệâ";
  for (int i = 0; i < 9; i++)
  {
    true_a[i].ch = chr[i];
    true_a[i].value = value[i];
    true_a[i].left = left[i];
    true_a[i].right = right[i];
  }
  encrypt_root_file_and_save_result(truly_final, 5, true_a, f, wchar_f);
  fclose(f);
  fclose(wchar_f);
  f = fopen("result_test.dat", "rb");
  int str_len = 0, amount_0 = 0, p = 0;
  unsigned char *string = NULL;
  struct array_of_tree *a = NULL;

    fread(&str_len, sizeof(int), 1, f);
    string = malloc(str_len * sizeof(unsigned char));
    for (int i = 0; i < str_len; i++)
    {
      fread(&string[i], sizeof(unsigned char), 1, f);
    }
    fread(&amount_0, sizeof(int), 1, f);
    fread(&p, sizeof(int), 1, f);
    a = malloc(p * sizeof(struct array_of_tree));
    int i = 0;
    while (!feof(f))
    {
      fread(&a[i], sizeof(struct array_of_tree), 1, f);
      i++;
    }

    fclose(f);
    if(str_len != 6 || amount_0 != 4 || p != 9)
    {
      wprintf(L"Wrong: str_len: %d => 6\n amount_0: %d => 4\n p: %d => 9\n", str_len, amount_0, p);
    }
    else
    {
      wprintf(L"Right\n");
    }

    for(int i = 0; i < 9; i++)
    {
      if (true_a[i].ch != a[i].ch || true_a[i].value != a[i].value || true_a[i].left != a[i].left || true_a[i].right != a[i].right)
      {
        wprintf(L"Wrong: ch: %lc => lc\n value: %d => %d\n left: %d => %d\n right: %d => %d\n", a[i].ch, true_a[i].ch, a[i].value, true_a[i].value, a[i].left, true_a[i].left, a[i].right, true_a[i].right);
      }
      else
      {
        wprintf(L"Right\n");
      }
    }

    unsigned char true_str[6] = {238, 233, 39, 77, 77, 192};
    for(int i = 0; i < 6; i++)
    {
      if(true_str[i] != string[i])
      {
        wprintf(L"Wrong: %d => %d\n", string[i], true_str[i]);
      }
      else
      {
        wprintf(L"Right\n");
      }
    }
}

    // void bubble_test()
    // {
    //   char character_data[3] = {'a', 'b', 'c'};
    //   int frequency_data[3] = {5, 3, 2};
    //   char character_result[3] = {'c', 'b', 'a'};
    //   int frequency_result[3] = {2, 3, 5};
    //   struct input* num_input = malloc(3 * sizeof(struct input));
    //   for(int i = 0; i < 3; i++)
    //   {
    //     num_input[i].character = character_data[i];
    //     num_input[i].frequency = frequency_data[i];
    //   }
    //   bubble(num_input, 3);
    //   for(int j = 0; j < 3; j++)
    //   {
    //     if (num_input[j].character != character_result[j] || num_input[j].frequency != frequency_result[j])
    //     {
    //       printf("Wrong %c -> %c\n       %d -> %d\n", num_input[j].character, character_result[j], num_input[j].frequency, frequency_result[j]);

    //     }
    //     else
    //     {
    //       printf("Right\n");
    //     }
    //   }
    // }

    // void input_tree_test()
    // {
    //   struct input num_input1 = {2, 'a', NULL};
    //   struct input num_input2 = {4, 'x', NULL};
    //   struct tree* root_result = malloc(sizeof(struct tree));
    //   root_result -> value = 6;
    //   root_result -> ch = '\0';
    //   root_result -> left = NULL;
    //   root_result -> right = NULL;
    //   struct tree* root_test = input_tree(num_input1, num_input2);
    //   if (root_result->value != root_test->value || root_result->ch != root_test->ch)
    //   {
    //     printf("Wrong: %c -> %c\n", root_test->ch, root_result->ch);
    //   }
    //   else
    //   {
    //     printf("Right\n");
    //   }
    // }

    // void post_order_test()
    // {
    //   struct tree *left_node = malloc(sizeof(struct tree));
    //   struct tree *right_node = malloc(sizeof(struct tree));
    //   struct tree *root = malloc(sizeof(struct tree));
    //   struct result *final = malloc(2 * sizeof(struct result));
    //     left_node->value = 2;
    //     left_node->ch = 'b';
    //     left_node->left = NULL;
    //     left_node->right = NULL;

    //     right_node->ch = 'a';
    //     right_node->value = 5;
    //     right_node->left = NULL;
    //     right_node->right = NULL;

    //   root->value = 7;
    //   root->ch = '\0';
    //   root->left = left_node;
    //   root->right = right_node;

    //   char temp[] = "";

    //   char *temp_bit = malloc(8 * 7 * sizeof(char));
    //   char *result = NULL;
    //   int node = 0;
    //   int i = 0;

    //   post_order(root, node, temp_bit, final, &i);
    //   for (int i = 0; i < 2; i++)
    //   {
    //     strcat(temp, (&final[i])->str);
    //   }
    //   printf("%s \n", temp);
    // }

    // void read_per_8bit_test()
    // {
    //   char data[2][8] = {"00110111", "00110011"};
    //   unsigned char result[2] = {55, 51};

    //   for(int i = 0; i < 2; i++)
    //   {
    //     if (read_per_8bit(data[i]) == result[i])
    //     {
    //       printf("%c\n", result[i]);
    //     }
    //     else
    //     {
    //       printf("Wrong\n");
    //     }
    //   }
    // }
