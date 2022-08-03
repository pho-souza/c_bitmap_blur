#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#pragma pack(push,1)
//*#pragma pack(2)
typedef struct tagBITMAPFILEHEADER{
//cabeçalho de arquivo
	unsigned short BfType;
	unsigned int BfSize;
	unsigned short BfReserva1;
	unsigned short BfReserva2;
	unsigned int BfOffSetBits; //deslocamento, em bytes, para ínicio da area de dados

}BITMAPFILEHEADER;
#pragma pack(pop)
typedef struct tagBITMAPINFOHEADER{
//mapa de bits
	unsigned int BiSize;//tamanho do cabeçalho
	unsigned int BiWidth;//largura da imagem/////////////////
	unsigned int BiHeight;//altura da imagem/////////////////
	unsigned short BiPlanes;
	unsigned short BiBitCount;//bits p/ pixel
	unsigned  int BiCompress;
	unsigned  int BiSizeImag;
	unsigned  int BiXPPMeter;//resolução horizontal
	unsigned  int BiYPPMeter;//resolução vertical
	unsigned  int BiClrUsed;//n de cores usadas
	unsigned  int BiClrImpor;//cores realmente usadas
	/////
}BITMAPINFOHEADER;

typedef struct tagCORES{
	unsigned char B;
	unsigned char G;
	unsigned char R;
}CORES;

typedef struct tagNULOS{
	unsigned char a;
}NULOS;

int main(int argc, char* argv[])
{

	int x=3, y=3, j=0, i=0, k, l, tamanho, *tamn;
	int *px, *py;
	px=&x;
	py=&y;

	BITMAPFILEHEADER header, *ponteiroHead;
	BITMAPINFOHEADER info, *ponteiroInfo;
	CORES *ponteiroCor, **filtro, **coresfinais;
	NULOS *anulados, anula;

	anulados = &anula;
	(anulados)->a=0;
	tamn = &tamanho;

	ponteiroHead = &header;
	ponteiroInfo = &info;

	ponteiroHead = (BITMAPFILEHEADER *) malloc(sizeof(BITMAPFILEHEADER));
	ponteiroInfo =(BITMAPINFOHEADER *) malloc(sizeof(BITMAPINFOHEADER));

	if(argc !=4){
		printf("Nome do arquivo com extensão seguido da intensidade do filtro\n");
		printf("Dois argumentos necessários.\n");
		exit(0);
	}

	*tamn = atoi(argv[3]);
	//*tamn = atoi(tamn);
	printf("Tamanho do filtro: %d\n\n", tamanho);


	FILE *AbreImagem;
	FILE *FechaImagem;

	if(tamanho<1){
		printf("Erro: Tamanho errado no filtro.\n");
		printf("Apenas valor positivo\n");
		exit(1);
	}

	ponteiroInfo = &info;

	printf("\n\nArgumentos\n\n");
	printf("Arg1: %s - Arg2: %s - Arg3: %s \n\n",argv[1],argv[2],argv[3]);


	AbreImagem = fopen(argv[1], "rb");
	
	//*fclose(FechaImagem);

	if(AbreImagem == NULL)
	{
		printf("Erro: Nome de arquivo errado.\n");
		exit(0);
	}


	fread(ponteiroHead, sizeof(unsigned char), 14, AbreImagem);
	fread(ponteiroInfo, sizeof(unsigned char), 40, AbreImagem);

	printf("BfType: %d\n", ponteiroHead->BfType);
	printf("BfSize: %d\n", ponteiroHead->BfSize);
	printf("Reserva: %d\n", ponteiroHead->BfReserva1);
	printf("Reserva: %d\n", ponteiroHead->BfReserva2);
	printf("Offset: %d\n\n", ponteiroHead->BfOffSetBits);

	/////
	printf("Size: %d\n", ponteiroInfo->BiSize);
	printf("X: %d\n", ponteiroInfo->BiWidth);
	printf("Y: %d\n", ponteiroInfo->BiHeight);
	printf("Planos: %d\n", ponteiroInfo->BiPlanes);
	printf("Compressao: %d\n", ponteiroInfo->BiCompress);
	printf("QuantidadeDeBits: %d\n", ponteiroInfo->BiBitCount);
	printf("Tamanho da imagem: %d\n", ponteiroInfo->BiSizeImag);
	printf("Cores usadas: %d\n", ponteiroInfo->BiClrUsed);
	printf("Cores realmente usadas: %d\n", ponteiroInfo->BiClrImpor);
	printf("Cores usadas: %d\n", ponteiroInfo->BiClrUsed);
	printf("Cores realmente usadas: %d\n\n\n", ponteiroInfo->BiXPPMeter);

	*px=ponteiroInfo->BiWidth;
	*py=ponteiroInfo->BiHeight;

	ponteiroCor = calloc((x)*(y), sizeof(CORES));
	filtro = calloc((y+tamanho)*(x+tamanho),sizeof(CORES));

	for(i=0;i<(y+tamanho);i++)
		filtro[i] = calloc(x+tamanho, sizeof(CORES));

	fseek(AbreImagem, ponteiroHead->BfOffSetBits, SEEK_SET);

	

//verify memory allocation

	printf("x: %d\t y: %d\n\n", x, y);
	for(i=0;i<y;i++){
		fread(ponteiroCor, sizeof(unsigned char), x*3, AbreImagem);
		if(x%4==3)
			fseek(AbreImagem, +3, SEEK_CUR);
		if(x%4==2)
			fseek(AbreImagem, +2, SEEK_CUR);
		if(x%4==1)
			fseek(AbreImagem, +1, SEEK_CUR);

		for(j=0;j<x;j++){

			filtro[i+(tamanho/2)][j+(tamanho/2)].B = (ponteiroCor+j)->B;
			filtro[i+(tamanho/2)][j+(tamanho/2)].G = (ponteiroCor+j)->G;
			filtro[i+(tamanho/2)][j+(tamanho/2)].R = (ponteiroCor+j)->R;

		}
	}

coresfinais = calloc((y)*(x),sizeof(CORES));
for(i=0;i<y;i++)
		coresfinais[i] = calloc(x, sizeof(CORES));


printf("\n\n\n");

//Mean Filter**********************************************************
for(i=0;i<y;i++)
{
	for(j=0;j<x;j++)
	{
		for(k=i;k<i+tamanho;k++)
		{
			for(l=j;l<j+tamanho;l++)
			{
				//*printf("Cor: %d\n", coresfinais[i][j].G);
				coresfinais[i][j].B+=round(filtro[k][l].B/pow(tamanho,2));
				coresfinais[i][j].G+=round(filtro[k][l].G/pow(tamanho,2));
				coresfinais[i][j].R+=round(filtro[k][l].R/pow(tamanho,2));
			}
		}
		(ponteiroCor+j+(i*x))->B=coresfinais[i][j].B;
		(ponteiroCor+j+(i*x))->G=coresfinais[i][j].G;
		(ponteiroCor+j+(i*x))->R=coresfinais[i][j].R;
	}
}


FechaImagem = fopen(argv[2], "wb+");


/* Move pro começo do arquivo para escrever */
fseek(FechaImagem, 0, SEEK_SET);

fwrite(ponteiroHead, sizeof(unsigned char), 14, FechaImagem);
fwrite(ponteiroInfo, sizeof(unsigned char), 40, FechaImagem);


printf("Onde começa o bitmap: %d\n\n",ponteiroHead->BfOffSetBits);
printf("Onde está o ponteiro: %ld\n\n",ftell(FechaImagem));

for(i=0;i<y;i++)
{
	fwrite(coresfinais[i], sizeof(CORES), x, FechaImagem);
	if(x%4==3)
			fwrite(anulados, sizeof(unsigned char),3, FechaImagem);
	if(x%4==2)
			fwrite(anulados, sizeof(unsigned char),2, FechaImagem);
	if(x%4==1)
			fwrite(anulados, sizeof(unsigned char),1, FechaImagem);
}

fclose(AbreImagem);
fclose(FechaImagem);
return 0;
}

