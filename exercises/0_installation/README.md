# 0. Installing LibPressio

The easiest way to get LibPressio installed is via [spack](https://github.com/spack/spack).

You'll need the following things to get started:

+ A linux development environment, other environments may be supported for some compressors
+ `git`, `curl`, `tar`, `python`, `openssl` with development headers (`libssl-dev` on ubuntu and `openssl-devel` on fedora) and either `clang` or `gcc`

```bash
#download the source code
git clone https://github.com/spack/spack
git clone https://github.com/robertu94/spack_packages robertu94_packages

#configure spack 
source ./spack/share/spack/setup-env.sh
spack compiler find
spack external find openssl
spack repo add robertu94_packages

# install libpressio with spack, this step takes a while
# if your c++ compiler is pre-c++17, you might need to add "^libstdcompat+boost" to the end of this command
spack install libpressio+python+sz+zfp+mgard+remote+json+mpi

#once you've built libpressio, you'll need to load it
spack load libpressio
```

You can also use the Dockerfile provided in the root of this repo to build a container with everything you need:

```bash
# or build it yourself
cd ../..
docker build -t robertu94/libpressio_tutorial .


#then run it!
docker run -it --rm robertu94/libpressio_tutorial
```
