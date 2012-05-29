#include <gtest/gtest.h>

#include <sys/stat.h>
#include <list>
struct source_line {
  int line_number;
  size_t length;
  char text[];
};

struct source_file {
  int line_count;
  source_line lines[];
};

class SourceTokenizer {
public:
  std::list<source_line*> parse(const char*src) {
    std::list<source_line*> result;
    
    printf("Allocating first buffer\n");
    source_line* current = (source_line*)malloc(sizeof(source_line) + 2000);
    result.insert(result.end(), current);
    current->line_number = 1;
    current->length = 0;
    
    printf("Processing text\n");
    while (*src != 0) {
      if (*src == '\n') {
	source_line* next = (source_line*)malloc(sizeof(source_line) + 2000);
	next->line_number = current->line_number + 1;
	next->length = 0;
	current = next;
	result.insert(result.end(), current);
      } else {
	current->text[current->length++] = *src;
      }
      src++;
    }
    return result;
  }
};


TEST(source_file_reader, ProcessesInput) {

  struct stat filestatus;
  stat(__FILE__, &filestatus );

  printf("Reading file %s of size %ld bytes\n", __FILE__, filestatus.st_size); 
  
  char* buffer = (char*) malloc(filestatus.st_size+1);
  memset(buffer, 0, filestatus.st_size+1);
  
  FILE* f = fopen(__FILE__, "r");
  fread(buffer, 1, filestatus.st_size, f);
  fclose(f);

  SourceTokenizer st;

  std::list<source_line*> lines = st.parse(buffer);

  ASSERT_EQ(1, (*lines.begin())->line_number);

  printf("First line %s", (*lines.begin())->text);
  // ASSERT_GT(0, psf->line_count);

  free(buffer);
}
