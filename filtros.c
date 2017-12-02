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
			img->px[i][j].red=(int)((0.3*img->px[i][j].red)+(0.59*img->px[i][j].green)+(0.11*img->px[i][j].blue));// pega o primeiro pixel e soma com os outros pixels
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
	printf("entrada\n");

	
	int nw=lim;
	image *novo=new_imagem(img->ncolunas,img->nlinhas);
	for(i=1;i<img->nlinhas-1;i++){
		for(j=1;j<img->ncolunas-1;j++){
			
			// caso o valor seja superior a 255 ou inferior a 0, sera atribuido os valores 255 e 0 a eles.
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
	

/*MatrixA *hough(image *img,int rm,int rm_i,int rx){
	int r,rmin,rmin_i,rmax;
	int x,y,i,j,t;
	int a,b;
	

	int linha=img->nlinhas;
	int coluna=img->ncolunas;

	int Min=fmin(linha,coluna);

	rmin=Min/rm;
	rmin_i=Min/rm_i;
	rmax=Min/rx;


	printf("%d %d %d\n",rmin,rmin_i,rmax );
	double conv=3.141592/180.0;

	int *A=calloc(linha*coluna*(rmax-rmin+1),sizeof(int));

	//printf("ccccc\n");

//cria os circulos usando a transformada de hough
	int dim=linha*coluna;

	for(x=rmax;x<linha-rmax;x++){
		for(y=rmax;y<coluna-rmax;y++){
			if(img->px[x][y].red==255){
				for(r=rmin;r<=rmax;r++){
					for(t=0;t<360;t++){
						a=x-r*sin(t*conv);
						b=y-r*cos(t*conv);

						A[(r-rmin)*dim+(a*coluna)+b]++;
						
					}
				}
			}
		}
	}
	//printf("safada\n");

	unsigned int xmax,ymax;
	double max=0;
	long int ic=0,jc=0,c=0;

//identifica o valor max de hough com mais pontos
	for(i=rmin;i<linha-rmax;i++){
		for(j=rmin;j<coluna-rmax;j++){
			for(r=rmin_i;r<=rmax;r++){
				max=fmax(A[dim*(r-rmin)+(i*coluna)+j],max);
			}
		}
	}

	//acha o centro

	for(i=rmin;i<linha-rmax;i++){
		for(j=rmin;j<coluna-rmax;j++){
			for(r=rmin_i;r<=rmax;r++){
				if(A[dim*(r-rmin)+(i*coluna)+j]>=0.875*max){
					c++;
					ic+=i;
					jc+=j;
				}
			}
		}
	}

	ymax=(unsigned int)ic/c;
	xmax=(unsigned int)jc/c;

	MatrixA *m=malloc(sizeof(MatrixA));

	//cordnadas
	m->x=(int)xmax;
	m->y=(int)ymax;
	m->raio=0;

	max=0;

	int rai[rmax-rmin+1];
	int r_max=0,contR=0;
//acha o raio da pupila
	for(r=rmin;r<=rmax;r++){
	//	printf("e aquu\n");
		if((A[dim*(r-rmin)+(ymax*coluna)+xmax] == 0 && max!=0) || (r == rmax+1)){
		//	printf("ou aqui\n");
			max=0;
			rai[contR++]=r_max;
			continue;
		}
		if(A[dim*(r-rmin)+(ymax*coluna)+xmax]>max){
		//	printf("ou sera aqui\n");
			max=A[dim*(r-rmin)+(ymax*coluna)+xmax];
			r_max=r;
		}
	}
//	printf("merda\n");
	for(i=0;i<contR;i++){
		if(i==contR-1){
			m->raio=rai[i];
		}else if(rai[i]>(double)(rai[i+1])/3){
			m->raio=rai[i];
			if(i+1 == contR-1){
				break;
			}
		}
	}

	return m;
}*/

MatrixA *hough2(image *img,int rm,int ri,int rx){
	int r,rmin,rmin_i,rmax;
	int x,y,i,j,t;
	int a,b;

	int rad=3.1415926/180.0;
	int linha=img->nlinhas;
	int coluna=img->ncolunas;

int Min=fmin(linha,coluna);
	rmin=100;
	rmin_i=Min/10;
	rmax=220;


scanf("%d %d",&rmin,&rmax);
	int max=0;

	double rho;
	//int hough_h=(int)sqrt(linha*linha+coluna*coluna)+1;

	int theta=360;
printf("aquivau\n");
	int ***A=(int***)calloc(linha,sizeof(int**));

	for(i=0;i<linha;i++){
		A[i]=(int**)calloc(coluna,sizeof(int*));
		for(j=0;j<coluna;j++){
			A[i][j]=(int*)calloc(rmax-rmin+1,sizeof(int));
		}
	}


	printf("affffffff\n");
	for(i=0;i<linha;i++){
		for(j=0;j<coluna;j++){
			for(r=rmin;r<rmax;r++){
				A[i][j][r-rmin]=0;
			}
		}
	}

	MatrixA *m=malloc(sizeof(MatrixA));

printf("aqhi\n");
	for(x=rmin;x<linha-rmax;x++){
		for(y=rmin;y<coluna-rmax;y++){
			if(img->px[x][y].red==255){
				for(r=rmin;r<rmax;r++){

					for(t=0;t<theta;t++){
						
						a=(int)x-r*cos(t*rad);
						b=(int)y-r*sin(t*rad);

						if(a>0 && b>0 && a<linha && b<coluna){
							A[a][b][r-rmin]+=1;	

						//	printf("%d %d %d\n",a,b,r-rmin );	
						}
					}
				}
			}
		}
	}
	printf("aqu\n");

	int valorI=0;

	for(i=rmin;i<linha-rmax;i++){
		for(j=rmin;j<coluna-rmax;j++){
			for(r=rmin;r<rmax;r++){
				if(A[i][j][r-rmin]>valorI){
					valorI=A[i][j][r-rmin];
				}
			}
		}
	}

int ic=0,jc=0,c=0,rt=0;
	for(i=rmin;i<linha-rmax;i++){
		for(j=rmin;j<coluna-rmax;j++){
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

	printf("%d %d %d %d\n",m->X,m->Y,m->raio,valorI);

/*

printf("%d %d %d %d\n",m->X,m->Y,m->raio,max);


	unsigned int imax,jmax;

	int ic=0, jc=0,c=0;

	for(i=rmax;i<linha-rmax;i++){
		for(j=rmax;j<coluna-rmax;j++){
			for(r=rmin_i;r<=rmax;r++){
				max=fmax(A[i][j][r],max);
			}
		}
	}
	printf("ap\n");

	for(i=rmin;i<linha-rmax;i++){
		for(j=rmin;j<coluna-rmax;j++){
			for(r=rmin_i;r<=rmax;r++){
				if(A[i][j][r]>=0.875*max){
					c++;
					ic+=i;
					jc+=j;
				}
			}
		}
	}
	printf("ap2\n");

	imax=(unsigned int)ic/c;
	jmax=(unsigned int)jc/c;


	m->X=(int)jmax;
	m->Y=(int)imax;
	m->raio=0;

	max=0;

	int rai[rmax-rmin+1];

	int r_max;
	int cont_r=0;

printf("p2 depois %d %d\n",imax,jmax);
	for(r=rmin;r<=rmax;r++){
		if((A[imax][jmax][r]==0 && max!=0)||(r==rmax+1)){
			max=0;
			rai[cont_r++]=r_max;

			printf("b1\n");
			continue;
		}
		if(A[imax][jmax][r]>max){
			max=A[imax][jmax][r];
			r_max=r;
			printf("b2\n");
		}
	}

printf("ap3\n");
	for(i=0;i<cont_r;i++){
		if(i==cont_r-1){
			m->raio=rai[i];
		}else if(rai[i]>(double)(rai[i+1])/3){
			m->raio=rai[i];
			printf("buceta\n");
			if(i+1==cont_r-1){
				break;
			}
		}
	}
	printf("ap4 %d\n",m->raio);*/

	return m;
}

image *pupila(image *img,MatrixA *m){
	image *pimg=new_imagem(img->ncolunas,img->nlinhas);

	int i,j;
	printf("aqui\n");

	for(i=0;i<img->nlinhas;i++){
		for(j=0;j<img->ncolunas;j++){
			int d=(int)sqrt(pow(i-m->X,2)+pow(j-m->Y,2));

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

void maca_pupila(image *img,MatrixA *m){
	unsigned int t;
	int xp,yp;
	printf("ops\n");

	for(t=0;t<360;t++){
		yp=m->raio*cos(t*(3.14/180.0));
		xp=m->raio*sin(t*(3.14/180.0));

		img->px[m->X+xp][m->Y+yp].red=255;
		img->px[m->X+xp][m->Y+yp].green=0;
		img->px[m->X+xp][m->Y+yp].blue=0;
	}
}

