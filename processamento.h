#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H


typedef struct 
{
	int red;
	int green;
	int blue;
}pixel;

typedef struct 
{
	pixel **px;
	int ncolunas;
	int nlinhas;
}image;

typedef struct
{
	int X;
	int Y;
	int raio;
}MatrixA;

image *new_imagem(int coluna,int linhas);

image *ler_ascii(char *arquivo);

void salva_ascii(image *img);

pixel *ler_pixel(image *img,int coluna,int linha);

void Diagnostico(image *img,MatrixA *m,char *Diag);
#endif
