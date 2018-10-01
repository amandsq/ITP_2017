#include "../include/processamento.h"
//funcao criar uma nova imagem com os pixels com o valor 0 0 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void Destroi(image *img){

}

image *new_imagem(int coluna,int linhas){
	int i;


	image *img=(image*)calloc(1,sizeof(image));
	img->px=(pixel**)calloc(linhas,sizeof(pixel*));

	for(i=0;i<linhas;i++){
		img->px[i]=(pixel*)calloc(coluna,sizeof(pixel));
	}



	img->ncolunas=coluna;
	img->nlinhas=linhas;


	return img;
}
//ler uma imagem dada pelo usuario
image *ler_ascii(char *arquivo){
	int i=0,j;//inteiros para usar no for
	FILE *foto;//Declarando um ponteiro do tipo FILE q é responsavel pela manipulacao de arquivos
	image *image;

	int linha,coluna,max;
	char code[4],ch;

	//char nome_arq[50];//char que pede o nome do arquivo do usuario
	//printf("digite o nome do arquivo\n");
	//scanf("%s",nome_arq);

	foto=fopen(arquivo,"r");//abre o arquvo no modo leitura, por isso o "r"  

	fscanf(foto,"%s",code);
	fgetc(foto);

	while(fgetc(foto)!='\n'){
		i++;
	}

	fscanf(foto, "%d",&coluna);// pega o numero de colunas
	fscanf(foto, "%d",&linha);//pega o numero de linhas
	fscanf(foto, "%d",&max);// pega o numero maximo de cada pixel

	image =new_imagem(coluna,linha);

	//For usado para pegar cada pixel da imagem, repare q uso a struct pixel q declarei no inicio da funcao
	for(i=0;i<linha;i++){
		for(j=0;j<coluna;j++){
			fscanf(foto, "%d",&image->px[i][j].red);
			fscanf(foto, "%d",&image->px[i][j].green);
			fscanf(foto, "%d",&image->px[i][j].blue);
		}
	}

	
	fclose(foto);// fecho o arquivo passado pelo usuario

	return image;
}
//salva a imagem no fim do programa
void salva_ascii(image *img){
	int i,j;

	FILE *ft;
	char nome_arq[50];
	printf("Digite o nome do arquivo destino\n");
	scanf("%s",nome_arq);

	strcat(nome_arq,".ppm");// coloca o ".ppm" ao fim do nome do arquivo dado pelo usuario
	ft=fopen(nome_arq,"w");// //isso é responsavel por criar o arquivo com o nome dado pelo usuario, por isso o "w"

	/*for(i=0;i<8;i++){
		fprintf(ft, "%s\n",test[i]);
	}*/

	/*for(i=0;i<8;i++){
		fprintf(ft, "%s\n",test[i]);
	}*/
	fprintf(ft, "P3\n");//passa o codigo do tipo ppm para o arquivo
	fprintf(ft, "%d\n",img->ncolunas);//passa o numero de colunas para o arquivo
	fprintf(ft, "%d\n",img->nlinhas);//passa o numero de linhas para o arquivo 
	fprintf(ft, "%d\n",255);//passa o numero maximo de cada pixel 
	
	// passa a matrix com os pixel do arquivo
	for(i=0;i<img->nlinhas;i++){
		for(j=0;j<img->ncolunas;j++){
			fprintf(ft, "%d ",img->px[i][j].red);
			fprintf(ft, "%d ",img->px[i][j].green);
			fprintf(ft, "%d\n",img->px[i][j].blue);
		}
	}

	fclose(ft);// fecha o arquvio
}
//ler o pixel pra nao ficar com o valor negativo
pixel *ler_pixel(image *img,int coluna,int linha){
	if(coluna>= img->ncolunas){
		coluna=img->ncolunas-1;
	}
	if(linha>=img->nlinhas){
		linha=img->nlinhas-1;
	}
	if(coluna<0){
		coluna=0;
	}
	if(linha<0){
		linha=0;
	}

	return &img->px[linha][coluna];
}

void Diagnostico(image *img,MatrixA *m,char *Diag){
	
	FILE *dg;

	dg=fopen(Diag,"w"); //Abrir um arquivo texto para gravação

	int threshold=90; //Definindo limiar pra saber se o pixel é ou nao catarata
	int i,j;
	double medPixel;
	int contPixel=0,pxCatarata=0;

	for(i=0;i<img->nlinhas;i++){
		for(j=0;j<img->ncolunas;j++){
			int d=(int)sqrt(pow(i-m->X,2)+pow(j-m->Y,2));

			if(d<=m->raio-5){
				medPixel=(img->px[i][j].red+img->px[i][j].green+img->px[i][j].blue)/3; //calcula a media das cores (vermelho, verde e azul) de cada pixel

				contPixel++;
				if(medPixel>threshold){ //compara a media com o limiar e se for maior eh considerado catarata
					pxCatarata++;
				}
			}
		}
	}

	double porcentagem=pxCatarata*100/contPixel; //calculo da porcentagem da catarata

	if(porcentagem<60){
		fprintf(dg, "Diaginostico final: Sem catarata\n");
	}else{
		fprintf(dg, "Diaginostico final: Com catarata\n");
	}

	fprintf(dg, "porcentagem de comprometimento: %.2lf%c\n",porcentagem,'%');

	fclose(dg);

}
