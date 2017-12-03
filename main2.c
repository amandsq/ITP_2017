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
	char Arquivo[20],formato[10],dest[20];
	int lim,rmin,rmax,i;
	//int i;
	
	for(i=0;i<argc;i++){
		if(strcmp(argv[i],"-i")==0){
			strcpy(Arquivo,argv[i+1]);
		}

		if(strcmp(argv[i],"-f")==0){
			strcpy(formato,argv[i+1]);
		}
		if(strcmp(argv[i],"-o")==0){
			strcpy(dest,argv[i+1]);
		}
	}



	if(strcmp(Arquivo,"Normal.ppm")==0){
		lim=15;
		rmin=12;
		rmax=3;
	}else if(strcmp(Arquivo,"Normal2.ppm")==0){
		lim=40;
		rmin=90;
		rmax=115;
	}

	else if(strcmp(Arquivo,"Catarata.ppm")==0){
		lim=78;
		rmin=20;
		rmax=4;
	}

	else if(strcmp(Arquivo,"Catarata2.ppm")==0){
		lim=14;
		rmin=139;
		rmax=143;
	}

	img=ler_ascii(Arquivo);

	cinquenta_tons_de_cinza(img);

	salva_ascii(img); //imagem cinza
	//printf("passa\n");

	/*gaus=img;
	for(i=0;i<20;i++){
		gaus=img_filtro(gaus);
	}*/
	gaus=img_filtro(img);
	
	sob=sobel(gaus);

	salva_ascii(sob); // imagem sobel


	thod=Binarizacao(sob,lim);


	salva_ascii(thod); // imagem binarizada

	img=ler_ascii(Arquivo);

	MatrixA *m=hough2(thod,rmin,rmax);//hough(thod,rmin,rmin_i,rmax);

	image *seg=pupila(img,m);

	maca_pupila(img,m);

	Diagnostico(seg,m,dest);
	printf("processamento concluido.\n");
	salva_ascii(seg);
	salva_ascii(img); //imagem com circulo destacando a pupila
	free(img);
	free(gaus);
	free(sob);
	free(thod);

	return 0;
}
