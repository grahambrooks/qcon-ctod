#include <gtest/gtest.h>

typedef struct {
  int line_number;
  long length;
  long size;
  unsigned char digest[16];
  char text[];
} source_line;

extern "C" void hash_line(source_line* sl);

TEST(Algorithm, EmptyString) {
  unsigned char expected_digest[] = {0xE4, 0xC2, 0x37, 0x62, 0xED, 0x28, 0x23, 0xA2, 0x7E, 0x62, 0xA6, 0x4B, 0x95, 0xC0, 0x24, 0xE7};

  source_line l;
  l.length = 0;

  hash_line(&l);
  
  for (auto i = 0; i < sizeof(expected_digest); i++) {
    ASSERT_EQ(expected_digest[i], l.digest[i]);
  }
}


TEST(Algorithm, SingleCharacterString) {
  unsigned char expected_digest[] = { 0x79, 0x3a, 0x9b, 0xc0, 0x7e, 0x20, 0x9b, 0x28, 0x6f, 0xa4, 0x16, 0xd6, 0xee, 0x29, 0xa8, 0x5d};

  source_line* l = (source_line*)malloc(sizeof(source_line) + 1);

  l->length = 1;
  l->text[0] = 'a';

  hash_line(l);


  for (auto i = 0; i < sizeof(expected_digest); i++) {
    ASSERT_EQ(expected_digest[i], l->digest[i]);
  }

  free(l);
}

source_line* hash_string(const char* s) {
  source_line* l = (source_line*)malloc(sizeof(source_line) + strlen(s));

  l->length = strlen(s);
  memcpy(l->text, s, l->length);

  hash_line(l);

  return l;
}

TEST(Algorithm, 3Characters) {
  unsigned char expected_digest[] = {0x79, 0x99, 0xdc, 0x75, 0xe8, 0xda, 0x64, 0x8c, 0x67, 0x27, 0xe1, 0x37, 0xc5, 0xb7, 0x78, 0x03};

  source_line* l = hash_string("abc");

  for (auto i = 0; i < sizeof(expected_digest); i++) {
    ASSERT_EQ(expected_digest[i], l->digest[i]);
  }

  free(l);

}

TEST(Algorithm, Message) {
  unsigned char expected_digest[] = { 0x84, 0x07, 0x93, 0x37, 0x1e, 0xc5, 0x8a, 0x6c, 0xc8, 0x48, 0x96, 0xa5, 0x15, 0x30, 0x95, 0xde};

  source_line* l = hash_string( "message digest");

  for (auto i = 0; i < sizeof(expected_digest); i++) {
    ASSERT_EQ(expected_digest[i], l->digest[i]);
  }

  free(l);

}

TEST(Algorithm, Alphabet) {
  unsigned char expected_digest[] = { 0x98, 0xef, 0x94, 0xf1, 0xf0, 0x1a, 0xc7, 0xb9, 0x19, 0x18, 0xc6, 0x74, 0x7f, 0xde, 0xbd, 0x96};

  source_line* l = hash_string( "abcdefghijklmnopqrstuvwxyz");

  for (auto i = 0; i < sizeof(expected_digest); i++) {
    ASSERT_EQ(expected_digest[i], l->digest[i]);
  }

  free(l);
}

TEST(Algorithm, AlphaNumeric) {
  unsigned char expected_digest[] = { 0xda, 0xbc, 0xd6, 0x37, 0xcd, 0xe4, 0x43, 0x76, 0x4c, 0x4f, 0x8a, 0xa0, 0x99, 0xcf, 0x23, 0xbe};

  source_line* l = hash_string( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

  for (auto i = 0; i < sizeof(expected_digest); i++) {
    ASSERT_EQ(expected_digest[i], l->digest[i]);
  }

  free(l);
}

TEST(Algorithm, Numbers) {
  unsigned char expected_digest[] = { 0xe2, 0x9c, 0x01, 0xa1, 0xe2, 0xa6, 0x63, 0xc2, 0x6b, 0x4a, 0x68, 0xbf, 0x7e, 0xc4, 0x2d, 0xf7};

  source_line* l = hash_string( "12345678901234567890123456789012345678901234567890123456789012345678901234567890");

  for (auto i = 0; i < sizeof(expected_digest); i++) {
    ASSERT_EQ(expected_digest[i], l->digest[i]);
  }

  free(l);
}
