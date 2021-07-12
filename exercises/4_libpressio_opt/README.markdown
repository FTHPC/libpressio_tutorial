# LibPressio-OPT

The goal of this part of the tutorial is to be introduced to the basics of automated configuration of compressors using LibPressio-OPT.  This part of the tutorial is implemented in C.

In this directory, you should have a couple of files:

1. `opt_zfp_perf` which first optimizes ZFP performance using LibPressio-OPT, and then runs several evaluations.

Now, run `./opt_zfp_perf` and see what output you get.  I got this output


```console

rank={} iter={0} input={7,257,} output={146,} objective={146}
rank={} iter={1} input={4,391,} output={191,} objective={191}
rank={} iter={2} input={3,145,} output={236,} objective={236}
rank={} iter={3} input={6,70,} output={153,} objective={153}
rank={} iter={4} input={12,511,} output={147,} objective={147}
rank={} iter={5} input={10,1,} output={278,} objective={278}
rank={} iter={6} input={9,512,} output={162,} objective={162}
rank={} iter={7} input={7,510,} output={139,} objective={139}
rank={} iter={8} input={1,511,} output={474,} objective={474}
rank={} iter={9} input={11,339,} output={151,} objective={151}
rank={} iter={10} input={8,376,} output={174,} objective={174}
rank={} iter={11} input={12,213,} output={147,} objective={147}
final_iter={12} inputs={7,510,} output={139,}
bound=1.0e-06
composite:compression_rate <double> = <empty>
composite:compression_rate_many <double> = <empty>
composite:decompression_rate <double> = <empty>
composite:decompression_rate_many <double> = <empty>
error_stat:average_difference <double> = 2.45035e-11
error_stat:average_error <double> = 2.34941e-08
error_stat:difference_range <double> = 7.76723e-07
error_stat:error_range <double> = 3.91621e-07
error_stat:max_error <double> = 3.91621e-07
error_stat:max_rel_error <double> = 0.000191226
error_stat:min_error <double> = 0
error_stat:min_rel_error <double> = 0
error_stat:mse <double> = 1.25201e-15
error_stat:n <uint64> = 25000000
error_stat:psnr <double> = 95.2503
error_stat:rmse <double> = 3.53837e-08
error_stat:value_max <double> = 0.00204795
error_stat:value_mean <double> = 8.59691e-06
error_stat:value_min <double> = 0
error_stat:value_range <double> = 0.00204795
error_stat:value_std <double> = 5.25875e-05
opt:input <data> = data{ type=double dims={2, } has_data=[7, 510, ]}
opt:msg <char*> = ""
opt:output <data> = data{ type=double dims={1, } has_data=[139, ]}
opt:status <int32> = 0
size:bit_rate <double> = 7.0306
size:compressed_size <uint64> = 21970616
size:compression_ratio <double> = 4.55153
size:decompressed_size <uint64> = 100000000
size:uncompressed_size <uint64> = 100000000
time:check_options <uint32> = 0
time:compress <uint32> = 2603
time:compress_many <uint32> = <empty>
time:decompress <uint32> = 290
time:decompress_many <uint32> = <empty>
time:get_options <uint32> = 0
time:set_options <uint32> = 0

bound=1.0e-05
composite:compression_rate <double> = <empty>
composite:compression_rate_many <double> = <empty>
composite:decompression_rate <double> = <empty>
composite:decompression_rate_many <double> = <empty>
error_stat:average_difference <double> = 9.56625e-10
error_stat:average_error <double> = 1.71337e-07
error_stat:difference_range <double> = 5.96203e-06
error_stat:error_range <double> = 3.06748e-06
error_stat:max_error <double> = 3.06748e-06
error_stat:max_rel_error <double> = 0.00149783
error_stat:min_error <double> = 0
error_stat:min_rel_error <double> = 0
error_stat:mse <double> = 7.12646e-14
error_stat:n <uint64> = 25000000
error_stat:psnr <double> = 77.6977
error_stat:rmse <double> = 2.66954e-07
error_stat:value_max <double> = 0.00204795
error_stat:value_mean <double> = 8.59691e-06
error_stat:value_min <double> = 0
error_stat:value_range <double> = 0.00204795
error_stat:value_std <double> = 5.25875e-05
opt:input <data> = data{ type=double dims={2, } has_data=[7, 510, ]}
opt:msg <char*> = ""
opt:output <data> = data{ type=double dims={1, } has_data=[131, ]}
opt:status <int32> = 0
size:bit_rate <double> = 4.81796
size:compressed_size <uint64> = 15056112
size:compression_ratio <double> = 6.64182
size:decompressed_size <uint64> = 100000000
size:uncompressed_size <uint64> = 100000000
time:check_options <uint32> = 0
time:compress <uint32> = 302
time:compress_many <uint32> = <empty>
time:decompress <uint32> = 241
time:decompress_many <uint32> = <empty>
time:get_options <uint32> = 0
time:set_options <uint32> = 0
... <snip>
```
Now open up the `opt_zfp_perf` code an notice the following:

1. Opt works by invoking a compressor and a metric in an optimization loop.  We used the time metric here to search for the min compression time.
2. There are many options for configuring LibPressio-OPT used for describing configuration.  Here are some of the common ones and what they mean (there are a few others that are performance optimizations in the example):
 + `opt:inputs <char*[]>` list of input settings
 + `opt:output <char*[]>` list of output settings
 + `opt:lower_bound <data>` lower bounds on inputs.
 + `opt:upper_bound <data>` upper bounds on inputs.
 + `opt:objective_mode_name <char*>` the name for the optimization method
 + `opt:do_decompress <int32>` preform decompression while tuning
3. Sometimes it is beneficial to switch stragegies for different iterations to maximize performance like done in this example.

## Exercises

1. Consider trying automated tuning of a different compressor like SZ.  What other parameters might you want to consider?
2. How might you use an approach like this to choose between different compressors in an automated fashion?  Hint: think about the `select` meta-compressor.
