FROM gcc:latest

# Install dependencies for GMP
RUN apt-get update && apt-get install -y \
    build-essential \
    m4 \
    && apt-get clean

# Download and install GMP
RUN wget https://gmplib.org/download/gmp/gmp-6.2.1.tar.xz && \
    tar -xf gmp-6.2.1.tar.xz && \
    cd gmp-6.2.1 && \
    ./configure && \
    make && \
    make check && \
    make install && \
    ldconfig

# Clean up
RUN rm -rf gmp-6.2.1 gmp-6.2.1.tar.xz
