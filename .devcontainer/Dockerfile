# Lightweight Ubuntu base with devcontainer niceties
FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04


ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
&& apt-get install -y --no-install-recommends \
ca-certificates \
build-essential \
git \
make \
pkg-config \
usbutils \
udev \
binutils-avr \
gcc-avr \
avr-libc \
avrdude \
clang-format \
&& rm -rf /var/lib/apt/lists/*


# Helpful: show attached USB devices inside the container
CMD ["bash"]