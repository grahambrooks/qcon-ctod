
class app {
public:
  int run(int argc, const char* argv[]);
private:
  void print_platform_info();
  void find_duplicates(const std::list<std::string>& paths);
};
