#include "runlength.h"
#include <assert.h>
#include <iostream>

int main()
{
  std::vector<float> f {1.2f, 2.3f, 0.f, 0.f, 0.f, 2.1f};
  std::string compressed = compress(f.data(), f.size());
  assert(compressed.size() == (4*4 + 8));
  std::vector<float> decompressed = decompress<float>(std::move(compressed));

  assert(f.size() == decompressed.size());
  for (size_t i = 0; i < f.size(); ++i) {
    assert(f[i] == decompressed[i]);
  }

  std::cout << "passed!" << std::endl;
}
