#include <math.h>
#include <string.h>		// para usar strings

// Rotinas para acesso da OpenGL
#include "opengl.h"

// Rotinas para leitura de arquivos .hdr
#include "rgbe.h"

// Variáveis globais a serem utilizadas:

// Dimensões da imagem de entrada
int sizeX, sizeY;

// Imagem de entrada
RGBf* image;

// Imagem de saída
RGB8* image8;

// Fator de exposição inicial
float exposure = 1.0;

// Modo de exibição atual
int modo;

// Função pow mais eficiente (cerca de 7x mais rápida)
float fastpow(float a, float b);
float fastpow(float a, float b) {
     union { float f; int i; }
      u = { a };
      u.i = (int)(b * (u.i - 1065307417) + 1065307417);
      return u.f;
}

// Função principal de processamento: ela deve chamar outras funções
// quando for necessário (ex: algoritmos de tone mapping, etc)
void process()
{
    printf("Exposure: %.3f\n", exposure);
    //
    // EXEMPLO: preenche a imagem com pixels cor de laranja...
    //
    //
    // SUBSTITUA este código pelos algoritmos a serem implementados
    //

    printf("%f", fmin(1.0, 2));

    int pos;
    for(pos=0; pos<sizeX*sizeY; pos++) {
        float r = image[pos].r * exposure;
        float g = image[pos].g * exposure;
        float b = image[pos].b * exposure;
        float c = 0.5;
        r = r / (r + c);
        g = g / (g + c);
        b = b / (b + c);
        image8[pos].r = (unsigned char) (fmin(1.0,r) * 255);
        image8[pos].g = (unsigned char) (fmin(1.0,g) * 255);
        image8[pos].b = (unsigned char) (fmin(1.0,b) * 255);
    }

    //
    // NÃO ALTERAR A PARTIR DAQUI!!!!
    //
    buildTex();
}

int main(int argc, char** argv)
{
    if(argc==1) {
        printf("hdrvis [image file.hdr]\n");
        exit(1);
    }

    // Inicialização da janela gráfica
    init(argc,argv);

    FILE* arq = fopen("./assets/memorial.hdr","rb");

    RGBE_ReadHeader(arq, &sizeX, &sizeY, NULL);

    image = (RGBf*) malloc(sizeof(RGBf) * sizeX * sizeY);

    int result = RGBE_ReadPixels_RLE(arq, (float*)image, sizeX, sizeY);
    if (result == RGBE_RETURN_FAILURE) {
        printf("Erro de leitura dos pixels");
    }
    fclose(arq);

    // Aloca memória para imagem de 24 bits
    image8 = (RGB8*) malloc(sizeof(RGB8) * sizeX * sizeY);

    exposure = 1.0f; // exposição inicial

    // Aplica processamento inicial
    process();

    // Não retorna... a partir daqui, interação via teclado e mouse apenas, na janela gráfica
    glutMainLoop();
    return 0;
}

