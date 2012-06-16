#ifndef SOURCE_FILE_READER_INCLUDED
#define SOURCE_FILE_READER_INCLUDED

#include <list>
#include <exception>

class invalid_file_exception : public std::exception {};


typedef struct {
  int line_number;
  long length;
  long size;
  unsigned char digest[16];
  char text[];
} source_line;

class SourceTokens {
  std::list<source_line*> lines;
public:
  ~SourceTokens() {
    for (auto token : lines) {
      free((void*)(token));
    }
  }
  
  void add(source_line* line) {
    lines.push_back(line);
  }
  
  void replace_current(source_line* replacement) {
    lines.pop_back();;
    lines.push_back(replacement);
  }
  
  size_t size() {
    return lines.size();
  }
  
  const std::list<source_line*> source_lines() {
    return lines;
  }
};


class SourceTokenizer {
  long long file_size(FILE* f) {
    fseek(f , 0 , SEEK_END);
    auto fs = ftell(f);
    rewind(f);
    return fs;
  }
  
  source_line* alloc_source_line() {
    source_line* sl = (source_line*)malloc(sizeof(source_line) + 2000);
    sl->line_number = 1;
    sl->length = 0;
    sl->size = 2000;
    return sl;
  }
  source_line* alloc_source_line(size_t size) {
    source_line* sl = (source_line*)malloc(sizeof(source_line) + size);
    sl->line_number = 1;
    sl->length = 0;
    sl->size = size;
    return sl;
  }
  
  bool is_binary(FILE* f) {
    char test_buffer[1024];
    
    auto bytes = fread(test_buffer, 1, 1024, f);
    
    for (auto i = 0; i < bytes; i++) {
      if (test_buffer[i] == 0)
        return true;
    }
    rewind(f);
    return false;
  }
public:
  size_t parse_file(const char* filepath, SourceTokens& tokens) {
    
    FILE* f = fopen(filepath, "r");
    
    auto fs = file_size(f);
    
    if (fs > 1000000 && is_binary(f))
      throw invalid_file_exception();
    
    if (f == NULL) {
      printf("File not found %s\n", filepath);
      return 0;
    }
    
    char* buffer = (char*) malloc(fs + 1);
    
    auto bytes_read = fread(buffer, 1, fs, f);
    if (bytes_read != fs) {
      printf("Failed to read all the file data\n");
      throw std::exception();
    }
    buffer[fs] = 0;
    
    fclose(f);
    
    parse(buffer, bytes_read, tokens);
    
    free(buffer);
    
    return fs;
  }
  
  void parse(const char*src, size_t size, SourceTokens& tokens) {
    
    source_line* current = alloc_source_line();
    tokens.add(current);
    
    auto end = &src[size];
    
    while (src != end) {
      if (current->length >= current->size) {
        
        source_line* replacement = alloc_source_line(current->size * 2);
        replacement->length = current->length;
        replacement->line_number = current->line_number;
        memcpy(replacement->text, current->text, current->size);
        
        tokens.replace_current(replacement);
        free(current);
        current = replacement;
      }
      
      if (*src == 0) {
        throw invalid_file_exception();
      }
      
      if (*src == '\n') {
        source_line* next = alloc_source_line();
        next->line_number = current->line_number + 1;
        
        current = next;
        tokens.add(current);
      } else {
        current->text[current->length++] = *src;
      }
      src++;
    }
  }
};

#endif
