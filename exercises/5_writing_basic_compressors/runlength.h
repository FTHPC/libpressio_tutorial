#include <vector>
#include <string>
#include <sstream>

#define RUNLEGTH_MAJOR_VERSION 1
#define RUNLEGTH_MINOR_VERSION 0
#define RUNLEGTH_PATCH_VERSION 0

template <class T>
std::string compress(T const* data, size_t len, T value_to_encode = static_cast<T>(0)) {
  std::stringstream ss;

  size_t zeros_in_run=0;
  for (size_t i = 0; i < len; ++i) {
    if(zeros_in_run > 0) {
      if(data[i] == value_to_encode) {
        zeros_in_run++;
      } else {
        ss.write(reinterpret_cast<const char*>(&zeros_in_run), sizeof(size_t));
        ss.write(reinterpret_cast<const char*>(data+i), sizeof(T));
        zeros_in_run = 0;
      }
    } else {
      if(data[i] == value_to_encode) {
        zeros_in_run = 1;
      }
      ss.write(reinterpret_cast<const char*>(data+i), sizeof(T));
    }
  }
  if(zeros_in_run > 0) {
    ss.write(reinterpret_cast<const char*>(&zeros_in_run), sizeof(size_t));
  }

  return ss.str();
}

template <class T>
std::vector<T> decompress(std::string&& s, T value_to_encode = static_cast<T>(0)) {
  std::vector<T> dec;
  std::stringstream ss(s);

  T value = 0;
  size_t n_zeros = 0;
  ss.read(reinterpret_cast<char*>(&value), sizeof(T));
  while(ss.good()) {
    if(value != value_to_encode) {
        dec.emplace_back(value);
    } else {
        ss.read(reinterpret_cast<char*>(&n_zeros), sizeof(size_t));
        for (size_t i = 0; i < n_zeros; ++i) {
          dec.emplace_back(value);
        }
    }
    ss.read(reinterpret_cast<char*>(&value), sizeof(T));
  }
  return dec;
}
