#ifndef DEF_HUFFMAN
#define DEF_HUFFMAN

struct array_of_tree
{
  wchar_t ch;
  int value;
  int left;
  int right;
};

struct result
{
  char *str;
  wchar_t ch;
};

struct tree
{
  int value;
  wchar_t ch;
  struct tree *left;
  struct tree *right;
};

struct chr_fre
{
  int frequency;
  wchar_t character;
  struct tree *node_address;
};

void read_text(wchar_t str[], FILE *text_file, wint_t *current_char);
wint_t alike_check(wchar_t *str, int i, struct chr_fre num_input[]);
int get_data(struct chr_fre *num_input, FILE *text_file);
void bubble(struct chr_fre *number_input, int counter);
struct tree *input_tree(struct chr_fre num_input1, struct chr_fre num_input2);
void pre_order_traversal(struct tree *root, struct array_of_tree a[], int *i, int *p);
void build_tree(struct chr_fre *num_input, struct array_of_tree a[], struct result *final, int number);
void post_order(struct tree *root, int node, char *temp_bit, struct result *final, int *k);
void encode_character(struct tree *root, struct result *final);
void padding_0(char s[], char temp[], int n);
void split_string(char s[], char k[], int a, int b);
unsigned char pow_num(int x);
unsigned char read_per_8bit(char *a);
void read_binary(char *s, char *temp, unsigned char * result_of_huffman);
void binary_code(char binary_string[], wchar_t *string, struct result final[], unsigned char result_of_huffman[], int *length, int *j, FILE *f);
int amount_of_0_added(int n);
void save_others(struct array_of_tree a[], int p, FILE *f, int amount_0);
void encrypt_root_file_and_save_result(struct result final[], int number, struct array_of_tree a[], FILE *f, FILE *wchar_f);

void dec_to_bin(unsigned char c, char binary_num[]);
void decode_huffman(char root_bin[], struct array_of_tree a[], wchar_t result[], int *k, struct array_of_tree *x);
void decompress_huffman(struct array_of_tree a[], unsigned char str[], int str_len, int amount_0);
wint_t hash_code(wchar_t c);
void insert(struct result hash_array[], struct result final[], int number);
wchar_t utf8_decode(unsigned char result[], int temp_i);


void character_count_test();
void alike_check_test();
void count_test();
void read_text_test();
void get_data_test();
void bubble_test();
void input_tree_test();
void post_order_test();
void read_per_8bit_test();
void encode_character_test();
void build_tree_test();
void encrypt_root_file_and_save_result_test();

#endif
