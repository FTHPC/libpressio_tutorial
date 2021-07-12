# Writing Basic Compressor Plugins

The goal of this exercise is to write a compressor plugin for a simple compressor called `runlength` that implements a form of runlength encoding in order to become familiar with the various functions that can be provided for compressor plugins.

In this directory, you should have several files:

+ `runlength.h` which implements a simple compressor
+ `test_runlength.cc` which provides some example usage of runlength
+ `pressio_runlength_base.cc` where you will implement a libpresiso plugin
+ `test_pressio_base.cc` which provides a series of tests for your plugin
+ `pressio_runlength_solution.cc` which provides a possible solution

## Getting familiar with `runlength`

Before we write a compressor plugin, we need to become familiar with how runlength works.  Look at the `test_runlength.cc` code as well as `runlength.h`.  You should see the output `passed!` when you run `test_runlength`.  There are two major functions `compress` and `decompress` that we would like to wrap.  

`compress` takes a pointer and `size_t` pair to the data to be encoded, and optionally a value to use for the encoding.  If one is not provided, it uses 0 as the default.  It produces a `std::string` as output.

`decompress` takes a rvalue reference to a std::string data to be decoded, and optionally a value to use for the decoding.  If one is not provided, it uses 0 as the default.

The implementation of these functions is relatively straight forward.  One notable aspect is that these functions are lossless, infalible, and pure.  This makes them very easy to wrap.  It is recommended to look at the compressor plugins provided with LibPressio to see how to handle lossy, fallible, and impure compressor plugins.

For lossy plugins, not much changes except it is harder to verify that compression generates correct results.  Often the best way to do this is to check a few known inputs for expected behavior.

For fallible plugins, you need to return warning or errors messages. To do this call the class method `set_error` which takes an error code and error message as a string.

For impure plugins, you need to be more careful about regulating state and reporting thread-safety correctly.

## Options for `runlength`

Based on what we read, there is only one option that seems to exist for `runlength` what value to encode.  We'll give this the name `runlength:value_to_encode` and store a variable at class scope to hold its value.

We use the `set` method to set a value in a options structure.
We use the `get` method to get a value from a options structure.

Unsurprisingly, we configure options in the `protected` functions `get_options_impl` and `set_options_impl`.  The `_impl` refers to the actual implementation of these functions.  The base class provides a definition which users can actually call which handles some error handling other common tasks like invoking metrics functions.

## Meta Data about `runlength`

LibPressio requires several pieces of metadata for each plugin:

+ version information for the compressor plugin in the form of a version string, major, minor, and patch version number.  These are referenced by the `version` functions in the base class.
+ the `prefix` a unique string that defines options for this plugin
+ `pressio:description` a human readable of what plugin does.  It is stored in the documentation implementation.
+ `pressio:thread_safe` an indication if the plugin is thread safe.  It can be multiple -- thread safe as long as different handles are used -- serialized -- thread safe as long as different handles are only accessed one at a time -- or single -- not thread safe.  It is stored in the configuration implementation.
+ `pressio:stability` an indication of how stable the plugin is.  Before it passes all tests, and has been used for a while, it should be `experimental`.  It is stored in the configuration implementation.
+ lastly, a description is needed for each configuration and option.  These are stored in the documentation implementation with the same name as the option or configuration being mentioned.


These pieces of metadata help users reason about and use compressors.

## Compress and Decompress

Now to the actual compress and decompress functions.
We need to pass the type of the data being passed in to the compress and decompress routines.
How can we get to it?
We could use a large switch case function based on the `pressio_data::dtype` method.
However, we don't actually have to write this.
We can use `pressio_data_for_each` which is specifically created for this use case.

Since LibPressio proper is limited to C++11, we can solve this with templated function objects, later versions of C++ could use generic lambdas and save quite a bit of code.

## Finishing up

To finish up I'll briefly mention some other methods.

`clone` is responsible for creating a deep copy of a `pressio_compressor`.  If you use RAII based classes as data members, this can often be written as `compat::make_unqiue<T>(*this)` where T is the plugin class.

There are also version of compress and decompress that operate on sets of buffers at once.  These are called `compress_many_impl` and `decompress_many_impl`.

Some compressors have internal performance metrics that they capture.  You can use `get_metrics_results_impl` to provide these to users.

## Exercises

1. Write a compressor plugin for `runlength`.  There is a solution in `pressio_runlength_solution.cc` if you get stuck.  You can verify that everything is correct by running `./test_pressio_base` and getting the message `passed!`
2. Write a compressor plugin for a compressor that you have developed.
