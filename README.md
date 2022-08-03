# c_bitmap_blur
An algorithm in C for applying a moving average filter in bitmap images.

---
# How to compile

In your console, type:

``gcc main.c -o main -lm``

---
# How program works

The program uses 3 arguments: original_bmp file.bmp, exported_bmp_file.bmp and the number for average filter.

Example:

``./main imagem_alternativa.png imagem_filtro.bmp 9``

Original image:

![imagem_alternativa.bmp](imagem_alternativa.bmp)

Image with filter:

![imagem_filtro.bmp](imagem_filtro.bmp)
