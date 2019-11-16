#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <stddef.h>
#include "huffman.h"

int main()
{
  setlocale(LC_CTYPE, "vi_VN.UTF8");
  clock_t start_over = clock();

  FILE *text_file = NULL;
  text_file = fopen("news1.txt", "r");
  struct chr_fre *temp_character_frequency_list = malloc(500 * sizeof(struct chr_fre));

  for (int p = 0; p < 500; p++)
  {
    temp_character_frequency_list[p].node_address = NULL;
    temp_character_frequency_list[p].frequency = 0;
    temp_character_frequency_list[p].character = '\0';
  }
  int number = get_data(temp_character_frequency_list, text_file);
  fclose(text_file);

  struct chr_fre *character_frequency_list = malloc(number * sizeof(struct chr_fre));
  struct result *final = malloc(number * sizeof(struct result));

  for (int p = 0; p < number; p++)
  {
    character_frequency_list[p].node_address = NULL;
    character_frequency_list[p].frequency = temp_character_frequency_list[p].frequency;
    character_frequency_list[p].character = temp_character_frequency_list[p].character;
  }
  struct array_of_tree *tree_array = malloc((2 * number - 1) * sizeof(struct array_of_tree));
  for (int p = 0; p < (2 * number - 1); p++)
  {
    tree_array[p].ch = '\0';
    tree_array[p].value = 0;
    tree_array[p].left = 0;
    tree_array[p].right = 0;
  }
  build_tree(character_frequency_list, tree_array, final, number);
  encode_character(character_frequency_list[number -1].node_address, final);

  FILE *wchar_f = NULL, *f = NULL;
  f = fopen("result.dat", "wb");
  wchar_f = fopen("news1.txt", "r");
  encrypt_root_file_and_save_result(final, number, tree_array, f, wchar_f);
  fclose(f);
  fclose(wchar_f);

  clock_t finish_over = clock();

  double times = (double)(finish_over - start_over) / CLOCKS_PER_SEC;

  printf("Tong thoi gian: %.2lf\n", times);

  return 0;
}
