#!/usr/bin/env bash
clear
make clean
make
make qemu QEMU_EXTRA="-append 'param1=hola param2=adios'"
make clean