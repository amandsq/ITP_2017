#ifndef FILTROS_H
#define FILTROS_H

#include "processamento.h"

void cinquenta_tons_de_cinza(image *img);

image *img_filtro(image *img);

image *Soma_sobel(image *img,int matrix[3][3]);

image *sobel(image *img);

image *Binarizacao(image *img,int lim);

//MatrixA *hough(image *img,int rmin,int rmin_i,int rmax);

MatrixA *hough2(image *img,int rm,int rx);

image *pupila(image *img,MatrixA *m);

void maca_pupila(image *img,MatrixA *m);

#endif
