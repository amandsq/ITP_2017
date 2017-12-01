#include "processamento.h"
#include "filtros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	image *img;
	image *gaus;

	image *sob;
	image *thod;

	int lim,rmin,rmin_i,rmax;
	//int i;

	if(strcmp(argv[1],"Normal.ppm")==0){
		lim=15;
		rmin=12;
		rmin_i=9;
		rmax=3;
	}else if(strcmp(argv[1],"Normal2.ppm")==0){
		lim=78;
		rmin=20;
		rmin_i=12;
		rmax=4;
	}

	else if(strcmp(argv[1],"Catarata.ppm")==0){
		lim=78;
		rmin=20;
		rmin_i=10;
		rmax=4;
	}

	else if(strcmp(argv[1],"Catarata2.ppm")==0){
		lim=22;
		rmin=7;
		rmin_i=5;
		rmax=3;
	}

	img=ler_ascii(argv[1]);

	cinquenta_tons_de_cinza(img);

	//salva_ascii(img);
	printf("passa\n");

	/*gaus=img;
	for(i=0;i<20;i++){
		gaus=img_filtro(gaus);
	}*/
	int i;
	gaus=img_filtro(img);
	
	sob=sobel(gaus);

	//salva_ascii(sob);

	printf("aff\n");
	scanf("%d",&lim);
	thod=Binarizacao(sob,lim);
	printf("cu\n");

	salva_ascii(thod);

	img=ler_ascii(argv[1]);

	MatrixA *m=hough2(thod,rmin,rmin_i,rmax);//hough(thod,rmin,rmin_i,rmax);

	image *seg=pupila(img,m);

	maca_pupila(img,m);


	salva_ascii(seg);
	salva_ascii(img);
	free(img);
	free(gaus);
	free(sob);
	free(thod);

	return 0;
}