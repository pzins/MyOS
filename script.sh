make loader.o
make kernel.o
make mykernel.bin

scp -r ../OS 192.168.2.11:/home/pierre
ssh pierre@192.168.2.11
