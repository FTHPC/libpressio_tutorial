#include <libpressio.h>
#include <libpressio_ext/io/pressio_io.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
  printf("compressors [%s]\n", pressio_supported_compressors());
  printf("metrics [%s]\n", pressio_supported_metrics());
  printf("io [%s]\n", pressio_supported_io_modules());
  return 0;
}
