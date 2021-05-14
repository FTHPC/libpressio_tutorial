#!/usr/bin/env python

import libpressio

print("compressors", libpressio.supported_compressors())

print("metrics", libpressio.supported_metrics())

print("io", libpressio.supported_io())

