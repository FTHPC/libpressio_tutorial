#include <libpressio.h>
#include <libpressio_ext/io/posix.h>
#include <cuda_runtime.h>
#include <zfp.h>
#include <stdio.h>
#include <stdlib.h>

void lp_cuda_free(void* ptr, void* /*metadata*/) {
  cudaFree(ptr);
}

void lp_check_cuda(cudaError_t err) {
  if(err != cudaSuccess) {
    fprintf(stderr, "%s\n", cudaGetErrorString(err));
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  struct pressio* library = pressio_instance();
  struct pressio_compressor* comp = pressio_get_compressor(library, "zfp");

  printf("%s\n", DATADIR "CLOUDf48.bin.f32");

  //read in the dataset
  size_t dims[] = {500, 500, 100}; //fortran order
  size_t ndims = sizeof(dims)/sizeof(dims[0]);
  size_t buf_size = sizeof(float);
  for (size_t i = 0; i < ndims; ++i) {
    buf_size *= dims[i];
  }
  float* d_input;
  cudaMallocManaged((void**)&d_input, buf_size, 0);
  struct pressio_data* metadata = pressio_data_new_move(pressio_float_dtype, d_input, ndims, dims, lp_cuda_free, NULL);
  struct pressio_data* input_data_shared = pressio_io_data_path_read(metadata, DATADIR "/CLOUDf48.bin.f32");

  float* d_output;
  cudaMallocManaged((void**)&d_output, buf_size, 0);
  struct pressio_data* output_shared = pressio_data_new_move(pressio_float_dtype, d_output, ndims, dims, lp_cuda_free, NULL);

  //allocate the compressed buffer
  //for now you need to get max_size directly from ZFP, libpressio is going to add an API for this
  //but it doesn't exist yet
  double rate = 4.0;
  zfp_stream* zfp = zfp_stream_open(NULL);
  zfp_stream_set_rate(zfp, rate, zfp_type_float, /*dims*/3, /*random_access?*/0);
  zfp_field* field = zfp_field_3d(NULL, zfp_type_float, dims[0], dims[1], dims[2]);
  size_t comp_buf_size = zfp_stream_maximum_size(zfp, field);
  zfp_stream_close(zfp);
  zfp_field_free(field);

  float* d_compressed;
  cudaMallocManaged((void**)&d_compressed, comp_buf_size, 0);
  struct pressio_data* comp_shared = pressio_data_new_move(pressio_byte_dtype, d_compressed, 1, &comp_buf_size, lp_cuda_free, NULL);

  // it is also possible to use gpu direct storage here, but my experience is it's buggy
  // and requires every part of the stack to support it
  // libpressio provides a helper io module called "cufile"

  //put the memory on the device
  int device = -1;
  cudaGetDevice(&device);
  cudaMemPrefetchAsync(d_input, buf_size, device, NULL);

  struct pressio_options* options = pressio_options_new();
  pressio_options_set_double(options, "zfp:rate", rate);
  pressio_options_set_integer(options, "zfp:wra", 0);
  pressio_options_set_uinteger(options, "zfp:type", zfp_type_float);
  pressio_options_set_uinteger(options, "zfp:dims", 3);
  pressio_options_set_string(options, "zfp:execution_name", "cuda");
  pressio_options_set_string(options, "pressio:metric", "composite");
  const char* plugins[] ={"time","size"};
  size_t n_plugins = sizeof(plugins)/sizeof(plugins[0]);
  pressio_options_set_strings(options, "composite:plugins", n_plugins, plugins);
  pressio_compressor_set_options(comp, options);
  
  if(pressio_compressor_compress(comp, input_data_shared, comp_shared)) {
    fprintf(stderr, "%s\n", pressio_compressor_error_msg(comp));
    exit(1);
  }
  if(pressio_compressor_decompress(comp, comp_shared, output_shared)) {
    fprintf(stderr, "%s\n", pressio_compressor_error_msg(comp));
    exit(1);
  }

  struct pressio_options* metrics_results = pressio_compressor_get_metrics_results(comp);
  char* metrics_results_str = pressio_options_to_string(metrics_results);
  printf("%s\n", metrics_results_str);
  free(metrics_results_str);

  pressio_data_free(comp_shared);
  pressio_data_free(output_shared);
  pressio_data_free(input_data_shared);
  pressio_options_free(options);
  pressio_compressor_release(comp);
  pressio_release(library);

  return 0;
}
