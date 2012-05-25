#ifndef DEVICE_INFO_INCLUDED
#define DEVICE_INFO_INCLUDED


class device_info {
private:
  template<class T> void print_value(ostream& o, cl_device_id id, cl_device_info info_item, const char* message);

public:
  void print(ostream& o);
};

#endif
