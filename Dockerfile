#Build stage with Spack pre-installed and ready to be used
FROM spack/centos7:latest as builder

RUN git clone https://github.com/robertu94/spack_packages /opt/robertu94_spack

# What we want to install and how we want to install it
# is specified in a manifest file (spack.yaml)
RUN mkdir /opt/spack-environment \
&&  (echo "spack:" \
&&   echo "  specs:" \
&&   echo "  - libpressio+json+hdf5+mgard+mpi+python+remote+sz+zfp target=x86_64 ^libstdcompat+boost" \
&&   echo "  view: /opt/view" \
&&   echo "  repos:" \
&&   echo "  - /opt/robertu94_spack" \
&&   echo "  concretization: together" \
&&   echo "  config:" \
&&   echo "    install_tree: /opt/software") > /opt/spack-environment/spack.yaml

# Install the software, remove unnecessary deps
RUN cd /opt/spack-environment && spack env activate . && spack install --fail-fast && spack gc -y

# Strip all the binaries
RUN find -L /opt/view/* -type f -exec readlink -f '{}' \; | \
    xargs file -i | \
    grep 'charset=binary' | \
    grep 'x-executable\|x-archive\|x-sharedlib' | \
    awk -F: '{print $1}' | xargs strip -s

# Modifications to the environment that are necessary to run
RUN cd /opt/spack-environment && \
    spack env activate --sh -d . >> /etc/profile.d/z10_spack_environment.sh


# Bare OS image to run the installed executables
FROM centos:7

COPY --from=builder /opt/spack-environment /opt/spack-environment
COPY --from=builder /opt/software /opt/software
COPY --from=builder /opt/view /opt/view
COPY --from=builder /etc/profile.d/z10_spack_environment.sh /etc/profile.d/z10_spack_environment.sh

RUN yum update -y && yum install -y epel-release && yum update -y \
 && yum install -y libgomp libgfortran openssh-clients pkgconfig gcc-c++ gcc make gdb \
 && rm -rf /var/cache/yum  && yum clean all


RUN useradd tutorial
COPY --chown=tutorial:tutorial ./exercises /exercises
USER tutorial
WORKDIR /exercises
ENTRYPOINT ["/bin/bash", "--rcfile", "/etc/profile", "-l"]
