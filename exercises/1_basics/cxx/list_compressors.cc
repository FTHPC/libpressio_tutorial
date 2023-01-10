#include <libpressio_ext/cpp/libpressio.h>
#include <iostream>

int main(int argc, char *argv[])
{
    pressio library;

    std::cout << library.supported_compressors() << std::endl;
    
    return 0;
}
