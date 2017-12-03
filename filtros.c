#include "filtros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//funcao para transformar a foto em tons de cinza, ela recebe como paramentro a imagem original
void cinquenta_tons_de_cinza(image *img){
	int i,j;

	//funcao para maipular os pixels
	for(i=0;i< img->nlinhas;i++){
		for(j=0;j<img->ncolunas;j++){
			img->px[i][j].red=(int)((0.3*img->px[i][j].red)+(0.59*img->px[i][j].green)+(0.11*img->px[i][j].blue));// pega o primeiro pixel e soma com os outros pixels como o arquivo do professor mandou, pra mais informacoes olhe la
			img->px[i][j].green=img->px[i][j].red;// adiciona a cor verde a mesma soma feito na cor vermelha
			img->px[i][j].blue=img->px[i][j].red;// a diciona na cor blue a mesma soma feita na cor vermelha

			if(img->px[i][j].red>255){// ver se red nao ficou maior q 255 q é o valor maximo do pixel, se sim, colocar todas as corres como 255
				img->px[i][j].red=255;
				img->px[i][j].green=255;
				img->px[i][j].blue=255;
			}
		}
	}
}

//funcao para aplicar o filtro gaussiano na imagem, ele recebe como paramentro a imagem com tons de cinza
image *img_filtro(image *img){
	int i,j,linha,coluna,npx;
	pixel *Px;
	int som,div;

	//matriz de kernel
	int kernel[5][5]={{2,4,5,4,2},{4,9,12,9,4},{5,12,15,12,5},{4,9,12,9,4},{2,4,5,4,2}};

	//printf("Abre filtro\n");
	//criar uma nova imagem para receber o filtro
	image *nimg=new_imagem(img->ncolunas,img->nlinhas);

	for(linha=0;linha<img->nlinhas;linha++){
		for(coluna=0;coluna<img->ncolunas;coluna++){

			som=0;
			div=0;

			for(i=0;i<5;i++){
				for(j=0;j<5;j++){
					Px=ler_pixel(img,coluna+(j-2),linha+(i-2));
					//printf("px red %d\n",Px->red);
					som+=(Px->red*kernel[i][j]);
					//printf("som %d\n",som );
					div+=kernel[i][j];

				}
			}

			//divide o pixel por 159
			npx=som/div;

			nimg->px[linha][coluna].red=npx;
			nimg->px[linha][coluna].green=npx;
			nimg->px[linha][coluna].blue=npx;
		}
	}

	return nimg;
}

//funcao para fazer os gradientes do pixel, recebe como entrada a imagem e a matrix do gradiente
image *Soma_sobel(image *img,int matrix[3][3]){
	int i,j,coluna,linha,soma;

	pixel *Px;

	image *nimg=new_imagem(img->ncolunas,img->nlinhas);

	for(linha=0;linha<img->nlinhas;linha++){
		for(coluna=0;coluna<img->ncolunas;coluna++){

			soma=0;

			for(i=0;i<3;i++){
				for(j=0;j<3;j++){

					Px=ler_pixel(img,coluna+(j-2),linha+(i-2));

					soma+=(Px->red*matrix[i][j]);
				}
			}

			nimg->px[linha][coluna].red=soma;
			nimg->px[linha][coluna].green=soma;
			nimg->px[linha][coluna].blue=soma;
		}	
	}


	return nimg;
}
//funcao q aplica o filtro de sobel na imagem ja com o filtro gausiano
image *sobel(image *img){

	int i,j;
	//matriz do graidente vertical
	int Gx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
	//matriz do gradienteo  horizontal
	int Gy[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};

	image *nimg1=Soma_sobel(img,Gx);

	image *nimg2=Soma_sobel(img,Gy);

	image *novaImg=new_imagem(img->ncolunas,img->nlinhas);


	for(i=0;i<img->nlinhas;i++){
		for(j=0;j<img->ncolunas;j++){
			novaImg->px[i][j].red=sqrt((nimg1->px[i][j].red*nimg1->px[i][j].red)+(nimg2->px[i][j].red*nimg2->px[i][j].red));
			novaImg->px[i][j].green=novaImg->px[i][j].red;
			novaImg->px[i][j].blue=novaImg->px[i][j].red;

			//printf("%d\n",novaImg->px[i][j].red);
		}
	}

		return novaImg;
}

//procedimento de binarização que irá transformar essa imagem em uma imagem binária

image *Binarizacao(image *img,int lim){
	int i,j;
	//printf("entrada\n");


	int nw=lim;
	image *novo=new_imagem(img->ncolunas,img->nlinhas);
	for(i=1;i<img->nlinhas-1;i++){
		for(j=1;j<img->ncolunas-1;j++){

			if(img->px[i][j].red>nw){
				novo->px[i][j].red=255;
				novo->px[i][j].green=255;
				novo->px[i][j].blue=255;
			}else{
				novo->px[i][j].red=0;
				novo->px[i][j].green=0;
				novo->px[i][j].blue=0;
			}
		}
	}

	return novo;

}
	
// detectar circulo

MatrixA *hough2(image *img,int rm,int rx){
	int r,rmin,rmax;
	int x,y,i,j,t;
	int a,b;

	int rad=3.1415926/180.0;
	int linha=img->nlinhas;
	int coluna=img->ncolunas;

int Min=fmin(linha,coluna);
	rmin=rm;
	rmax=rx;

	int max=0;

	double rho;
	//int hough_h=(int)sqrt(linha*linha+coluna*coluna)+1;

	int theta=360;

	int ***A=(int***)calloc(linha,sizeof(int**));

	for(i=0;i<linha;i++){
		A[i]=(int**)calloc(coluna,sizeof(int*));
		for(j=0;j<coluna;j++){
			A[i][j]=(int*)calloc(rmax-rmin+1,sizeof(int));
		}
	}



	for(i=0;i<linha;i++){
		for(j=0;j<coluna;j++){
			for(r=rmin;r<rmax;r++){
				A[i][j][r-rmin]=0;
			}
		}
	}

	r=rmax;

	int tlinha=(int)linha/4;
	int tcoluna=(int)coluna/3;

//	printf("%d %d %d %d\n",linha,coluna,tlinha,tcoluna );

	MatrixA *m=malloc(sizeof(MatrixA));


	for(x=tlinha;x<linha-rmax;x++){
		for(y=tcoluna;y<coluna-rmax;y++){
			if(img->px[x][y].red==255){
				for(r=rmin;r<rmax;r++){

					for(t=0;t<theta;t++){
						
						a=(int)x-r*cos(t*rad);
						b=(int)y-r*sin(t*rad);

						//if(a>0 && b>0 && a<linha && b<coluna){
							A[a][b][r-rmin]+=1;	

						//	printf("%d %d %d\n",a,b,r-rmin );	
						//}
					}
				}
			}
		}
	}


	int valorI=0;

	for(i=tlinha;i<linha-rmax;i++){
		for(j=tcoluna;j<coluna-rmax;j++){
			for(r=rmin;r<rmax;r++){
				if(A[i][j][r-rmin]>valorI){
					valorI=A[i][j][r-rmin];
					m->X=i;
					m->Y=j;
					m->raio=r;
				}
			}
		}
	}

	int ic=0,jc=0,c=0,rt=0;
	for(i=tlinha;i<linha-rmax;i++){
		for(j=tcoluna;j<coluna-rmax;j++){
			for(r=rmin;r<rmax;r++){
				if(A[i][j][r-rmin] == valorI){
					c++;
					ic+=i;
					jc+=j;
					rt+=r;
				}
			}
		}
	}

	unsigned int imax, jmax,rtmax;

	imax=(unsigned int)ic/c;
	jmax=(unsigned int)jc/c;
	rtmax=(unsigned int)rt/c;

	
	m->X=(int)imax;
	m->Y=(int)jmax;


	m->raio=(int)rtmax;

	//printf("%d %d %d %d\n",m->X,m->Y,m->raio,valorI);

	return m;
}
// cria uma imagem som com a pupila
image *pupila(image *img,MatrixA *m){
	image *pimg=new_imagem(img->ncolunas,img->nlinhas);

	int i,j;


	for(i=0;i<img->nlinhas;i++){
		for(j=0;j<img->ncolunas;j++){
			int d=(int)sqrt(pow(i-m->X,2)+pow(j-m->Y,2));
			//caso nao esteja no raio da pupila, a imagem fica preta
			if(d<= m->raio-5){
				pimg->px[i][j].red=img->px[i][j].red;
				pimg->px[i][j].green=img->px[i][j].green;
				pimg->px[i][j].blue=img->px[i][j].blue;
			}else{
				pimg->px[i][j].red=0;
				pimg->px[i][j].green=0;
				pimg->px[i][j].blue=0;
			}
		}
	}

	return pimg;
} 
// coloca marcacao na pupila
void maca_pupila(image *img,MatrixA *m){
	unsigned int t;
	int xp,yp;


	for(t=0;t<360;t++){
		yp=m->raio*cos(t*(3.14/180.0));
		xp=m->raio*sin(t*(3.14/180.0));

		img->px[m->X+xp][m->Y+yp].red=0;
		img->px[m->X+xp][m->Y+yp].green=255;
		img->px[m->X+xp][m->Y+yp].blue=0;
	}
}
