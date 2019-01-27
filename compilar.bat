nasm -f win32 -o enmascarar_asm.obj enmascarar_asm.asm
gcc enmascarar_asm.obj main.c -o main.exe