#include <sys/stat.h>
#include <list>

struct source_line {
  int line_number;
  size_t length;
  char text[];
};

class SourceTokens {
  std::list<source_line*> lines;
public:
  ~SourceTokens() {
    for (auto token : lines) {
      free((void*)(token));
    }
  }
  
  void add(source_line* line) {
    lines.insert(lines.end(), line);
  }

  size_t size() {
    return lines.size();
  }
};


class SourceTokenizer {
  long long file_size(const char* path) {
    struct stat filestatus;
    stat(__FILE__, &filestatus );
    
    return filestatus.st_size;
  }
public:
  void parse_file(const char* filepath, SourceTokens& tokens) {
    long long fs = file_size(filepath);
    
    char* buffer = (char*) malloc(fs + 1);
    memset(buffer, 0, fs + 1);
  
    FILE* f = fopen(__FILE__, "r");
    fread(buffer, 1, fs, f);
    fclose(f);

    parse(buffer, tokens);

    free(buffer);
  }

  void parse(const char*src, SourceTokens& tokens) {
    source_line* current = (source_line*)malloc(sizeof(source_line) + 2000);
    tokens.add(current);

    current->line_number = 1;
    current->length = 0;
    
    while (*src != 0) {
      if (*src == '\n') {
	source_line* next = (source_line*)malloc(sizeof(source_line) + 2000);
	next->line_number = current->line_number + 1;
	next->length = 0;
	current = next;
	
	tokens.add(current);
      } else {
	current->text[current->length++] = *src;
      }
      src++;
    }
  }
};
