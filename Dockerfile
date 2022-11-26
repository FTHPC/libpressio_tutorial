FROM ghcr.io/robertu94/libpressio
RUN . /etc/profile && \
    spack add py-matplotlib && \
    spack add gdb && \
    spack install
ADD --chown=demo:demo exercises/ /exercises
ENV LD_LIBRARY_PATH=/app/.spack-env/view/lib
WORKDIR /exercises
