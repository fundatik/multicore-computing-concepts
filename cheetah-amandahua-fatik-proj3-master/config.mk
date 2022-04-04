TAPIR=/project/linuxlab/class/cse539/tapir/build/bin
CC=$(TAPIR)/clang
CXX=$(TAPIR)/clang++
LINK_CC=$(CC)
# If use cheetah
RTS_OPT=-ftapir=cilkr 
RTS_DIR=../runtime
RTS_LIB=libcheetah
RTS_LIB_FLAG=-lcheetah
OPT = -O3
