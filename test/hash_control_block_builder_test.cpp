#include <gtest/gtest.h>

struct hash_control_block {
  uint source_line_number;
  uint text_length;
  char text[];
};

class hash_control_block_builder {
public:
  hash_control_block* build(const char* text) {
    auto line_number = 1;
    auto i = 0;
    auto line_start = 0;
    while (text[i] != 0) {
      
      if (text[i] == '\n') {
	hash_control_block* result = (hash_control_block*)malloc(sizeof(hash_control_block) + i - line_start + 1);
	result->source_line_number = line_number;
	result->text_length = i - line_start;
	memcpy(result->text, text, i - line_start);
	return result;
      }
      i++;
    }
    
    return NULL;
  }
};


TEST(hash_control_block_builder, generates_line_number) {
  hash_control_block_builder builder;

  hash_control_block* blocks = builder.build("This is a line of text\n");

  ASSERT_EQ(blocks->source_line_number, 1) << "First line has a line number of 1";
}

TEST(hash_control_block_builder, generates_line_length) {
  hash_control_block_builder builder;
  auto text = "This is a line of text\n";

  auto blocks = builder.build(text);

  ASSERT_EQ(blocks->text_length, strlen(text)-1) << "Captures line length";
}

TEST(hash_control_block_builder, contains_source_test) {
  hash_control_block_builder builder;
  auto text = "This is a line of text\n";

  auto blocks = builder.build(text);

  ASSERT_EQ(blocks->text[0], 'T') << "Block contains source text";
  ASSERT_EQ(blocks->text[21], 't') << "Block contains source text";
}
