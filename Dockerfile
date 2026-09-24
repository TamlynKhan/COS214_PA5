FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
    && apt-get install -y --no-install-recommends g++ make valgrind gdb \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .
RUN make clean && make

CMD ["./campusguard"]
