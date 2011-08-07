################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BTree.cpp \
../BufferPool.cpp \
../BufferPool_FIFO.cpp \
../BufferPool_RANK.cpp \
../DataEntry.cpp \
../InternalNode.cpp \
../LeafNode.cpp \
../Main.cpp \
../Node.cpp 

OBJS += \
./BTree.o \
./BufferPool.o \
./BufferPool_FIFO.o \
./BufferPool_RANK.o \
./DataEntry.o \
./InternalNode.o \
./LeafNode.o \
./Main.o \
./Node.o 

CPP_DEPS += \
./BTree.d \
./BufferPool.d \
./BufferPool_FIFO.d \
./BufferPool_RANK.d \
./DataEntry.d \
./InternalNode.d \
./LeafNode.d \
./Main.d \
./Node.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


