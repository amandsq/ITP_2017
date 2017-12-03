# Projeto de ITP e PTP-2017.2
Trabalho de conclusão do semestre das disciplinas de ITP e PTP

alunos envolvidos:
------------
- Amanda Freire de Albuquerque

	\* responsavel pela leitura das imagens, transformação em tons de cinza, aplicacao do filtro gaussiano, binarização e diagnostico;
	
- Lucas Lima Marques de Oliveira

	\* responsavel pelo filtro de sobel, transformada de hough, segmentação da pupila e criação das imagens ja com os filtros;
	
O que foi feito:
--------------

	- Leitura da imagem em formato PPM;
	- Transformar a imagem colorida para tons de cinza;
	- Aplicação do filtro gaussiano;
	- Aplicação do filtro de sobel;
	- Binarização;
	- Transformada de Hough;
	- Segmentacao da Pupila;
	- Diagnostico da Catarata;
	
O que nao foi feito
------------------
	Algumas imagens não foipossivel encontrar a pupilae com isso nao se teve o diagnostico preciso da catarata;

O que seria feito diferente
------------------------


Como compilar:
------------
   abra ao gcc no terminal e depois execute os seguintes comandos
   - make
   - ./Catarata -i \<nome da imagem> -f ppm -o Diagnostico.txt
