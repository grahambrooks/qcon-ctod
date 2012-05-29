#include <gtest/gtest.h>
#include "../src/source_file_reader.hpp"

TEST(source_file_reader, ProcessesInput) {

  SourceTokenizer st;

  SourceTokens tokens;

  st.parse_file(__FILE__, tokens);

  ASSERT_GT(tokens.size(), __LINE__);
}


TEST(SourceTokenizer, RejectsFilesContainingNull) {
  SourceTokenizer st;

  SourceTokens tokens;

  char buffer[1024] = {'a'};

  buffer[1023] = 0;

  printf("%s", buffer);

  ASSERT_THROW(st.parse(buffer, 1024, tokens), invalid_file_exception);
}
