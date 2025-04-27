cc -g -D WIDTH=$1 siniristilippu.c utils.c && valgrind -q --leak-check=full ./a.out
