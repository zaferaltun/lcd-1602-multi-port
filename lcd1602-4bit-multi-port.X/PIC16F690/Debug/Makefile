###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = PIC16F690
COMPILER = "MPLAB XC8"
TARGET = Debug
SHELL = C:\WINDOWS\system32\cmd.exe

## Tools general options
CCFLAGS=--pass1 --errformat="Error at file %%f line %%l column %%c: (%%n) %%s" --warnformat="Warning at file %%f line %%l column %%c: (%%n) %%s" --msgformat="Message at file %%f line %%l column %%c: (%%n) %%s" -G --chip=16F690
LDFLAGS=--output=mcof -G --chip=16F690

## Processing Tools
CC=xc8.exe
ASM=xc8.exe
LD=xc8.exe
LIB=xc8.exe
#  E:/git/pic/___new-works___/lcd1602/lcd1602-4bit-multi-port.X/PIC16F690/Debug/Debug.cof: TOOL=LD
#  Post: rm startup.as
#  Post: rm Debug.as
#     E:/git/pic/___new-works___/lcd1602/lcd1602-4bit-multi-port.X/PIC16F690/Debug/main.p1: TOOL=CC
#        E:/git/pic/___new-works___/lcd1602/lcd1602-4bit-multi-port.X/main.c

# Build tree:
all:	Debug

Debug:	Debug.cof

Debug.cof:	main.p1 
	$(LD) $(LDFLAGS)  -O"Debug.cof" "main.p1"
	rm startup.as
	rm Debug.as

main.p1:	../../main.c 
	$(CC) $(CCFLAGS)  -O"main.p1" "../../main.c"

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf Debug.hxl
	rm -rf Debug.lst
	rm -rf Debug.sym
	rm -rf Debug.obj
	rm -rf Debug.sdb
	rm -rf Debug.as
	rm -rf ./startup.obj
	rm -rf ./startup.rlf
	rm -rf ./startup.as
	rm -rf ./startup.lst
	rm -rf ./funclist.
	rm -rf Debug.hex
	rm -rf main.pre
	rm -rf main.lst

# cleanup - delete all generated files
clean:	tidy
	rm -rf Debug.cof
	rm -rf main.p1
