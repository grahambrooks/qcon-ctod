#include <gtest/gtest.h>
#include "../src/source_file_reader.hpp"

TEST(source_file_reader, ProcessesInput) {

  SourceTokenizer st;

  SourceTokens tokens;

  st.parse_file(__FILE__, tokens);

  ASSERT_GT(tokens.size(), __LINE__);
}

