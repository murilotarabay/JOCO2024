#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <windows.h>
#include <iomanip>
#include <graphics.h>
#include <winbgim.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int NumNos;                                     //número de nós da treliça
int NumBarras;                                  //número de barras da treliça
float CoordX[1000];                             //Coordenadas X dos nós
float CoordY[1000];                             //Coordenadas Y dos nós
float ForcaX[1000];                             //Componente X das forças externas nos nós
float ForcaY[1000];                             //Componente Y das forças externas nos nós
int BarraI[1000];                               //Nó da extremidade I da barra
int BarraJ[1000];                               //Nó da extremidade J da barra
int RestricaoApoioX[1000];                      //1 - Indica apoio fixo no eixo X
int RestricaoApoioY[1000];                      //1 - Indica apoio fixo no eixo Y
float EsforcoInterno[1000];
float EsforcoLegenda[100];
float PesoLinearBarra[1000];
int CriterioDimensionamento[1000];
float ComprimentoBarra[1000];
float CorTracao;
float CorCompressao;
char ParametrosChar[100];
float IndiceEficiencia;                         //Índice de eficiência da treliça (somatório do produto do comprimento e do módulo da esforço interno)
float ComprimentoTotal;                         //comprimento total de todas as barras da estrutura
float MassaTotal;                               //massa total em kg
float DeslocamentoLateral;                      //deslcamento da extremidade (mm)
int DisplayFatorAmpliacao=40;
int DisplayDeslocY=980;
char Texto[32];
int XTextoDados=10;                             //Posição X dos textos

void PlotaEstrutura(int Arquivo, int DeslX) //Arquivo 0=relatorioinicial.txt 1=relatoriootimizada.txt  // Deslocamento em X
{
  string Parametros;
  const char * Arq;   // = "inicial.txt";
  if (Arquivo==0) Arq = "relatorioinicial.txt";
  if (Arquivo==1) Arq = "relatoriootimizada.txt";
  ifstream myfile (Arq);        //Abre o arquivo descritivo da treliça

  if (myfile.is_open())
  {
    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
    NumNos=atoi(ParametrosChar);                //Leitura do número de nós
    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
    NumBarras=atoi(ParametrosChar);                //Leitura do número de barras
  }


  if (myfile.is_open())
  {
    for (int i=0;i<NumNos;i++)
    {
        getline (myfile,Parametros);
        strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
        char* FimChar;
        CoordX[i] = strtof(ParametrosChar, &FimChar);
        CoordY[i] = strtof(FimChar, &FimChar);
        ForcaX[i] = strtof(FimChar, &FimChar);
        ForcaY[i] = strtof(FimChar, &FimChar);
        RestricaoApoioX[i] = strtof(FimChar, &FimChar);
        RestricaoApoioY[i] = strtof(FimChar, &FimChar);
    }

    for (int i=0;i<NumBarras;i++)
    {
        getline (myfile,Parametros);
        strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
        char* FimChar;
        BarraI[i] = strtof(ParametrosChar, &FimChar);
        BarraJ[i] = strtof(FimChar, &FimChar);
        EsforcoInterno[i] = strtof(FimChar, &FimChar);
        PesoLinearBarra[i] = strtof(FimChar, &FimChar);
    }

    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
    IndiceEficiencia=atof(ParametrosChar);                //Índice de eficiência

    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
    ComprimentoTotal=atof(ParametrosChar);                //Leitura comprimento total

    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
    MassaTotal=atof(ParametrosChar);                //Leitura da massa total

    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
    DeslocamentoLateral=atof(ParametrosChar);                //Leitura do Deslocamento lateral

    }
    myfile.close();




    float MaxTracao=0;
    float MaxCompressao=0;
    float MinTracao=0;
    float MinCompressao=0;

    MinTracao=10000;
    MinCompressao=-10000;

    for (int i=0;i<NumBarras;i++)
    {
        if (EsforcoInterno[i]>=0)
        {
            if (EsforcoInterno[i]>MaxTracao) MaxTracao=EsforcoInterno[i];
            if (EsforcoInterno[i]<MinTracao) MinTracao=EsforcoInterno[i];
        }

        if (EsforcoInterno[i]<0)
        {
            if (EsforcoInterno[i]<MaxCompressao) MaxCompressao=EsforcoInterno[i];
            if (EsforcoInterno[i]>MinCompressao) MinCompressao=EsforcoInterno[i];
        }
    }

    setcolor(15);
    setbkcolor(15);         //fundo do texto branco
    setlinestyle(0, 1, 2);
    for (int i=0;i<NumBarras;i++)   //desenha barras e valor dos esforços internos
    {
        CorTracao=(sqrt((EsforcoInterno[i]/MaxTracao)));
        CorCompressao=(sqrt((EsforcoInterno[i]/MaxCompressao)));

        if (EsforcoInterno[i]>0) setcolor(COLOR(0, 0, 255*CorTracao)); //BARRAS TRACIONADAS
        if (EsforcoInterno[i]<0) setcolor(COLOR(255*CorCompressao, 0, 0)); //BARRAS COMPRIMIDAS

        line(DeslX+CoordX[BarraI[i]]*DisplayFatorAmpliacao, DisplayDeslocY-CoordY[BarraI[i]]*DisplayFatorAmpliacao, DeslX+CoordX[BarraJ[i]]*DisplayFatorAmpliacao, DisplayDeslocY-CoordY[BarraJ[i]]*DisplayFatorAmpliacao);
        sprintf(Texto, "%.01f", EsforcoInterno[i]);
        outtextxy((((DeslX+4*CoordX[BarraI[i]]*DisplayFatorAmpliacao)+(DeslX+4*CoordX[BarraJ[i]]*DisplayFatorAmpliacao)-0)/2-20) ,(((DisplayDeslocY-CoordY[BarraI[i]]*DisplayFatorAmpliacao)+(DisplayDeslocY-CoordY[BarraJ[i]]*DisplayFatorAmpliacao))/2-5), Texto);
    }

    setfillstyle(1, 15); //Desenha fundo branco para o texto
    bar(DeslX+261, 181, DeslX+500, 340);  //fundo textos
    bar(DeslX+261, 341, DeslX+500, 540); //fundo legenda

    setcolor(0);
    setlinestyle(0, 1, 1);

    if (Arquivo==0) outtextxy(DeslX+270,190,"ESTRUTURA INICIAL");
    if (Arquivo==1) outtextxy(DeslX+270,190,"ESTRUTURA OTIMIZADA");

    sprintf(Texto, "Massa total: %.02f kg", MassaTotal);
    outtextxy(DeslX+270, 212, Texto);

    sprintf(Texto, "IEI: %.02f kNm", IndiceEficiencia);
    outtextxy(DeslX+270, 228, Texto);

    sprintf(Texto, "Comprimento total: %.02f m", ComprimentoTotal);
    outtextxy(DeslX+270, 244, Texto);

    sprintf(Texto, "Deslocamento no topo: %.02f mm", DeslocamentoLateral*1000);
    outtextxy(DeslX+270, 260, Texto);

    sprintf(Texto, "Máx. Tração: %.02f kN", MaxTracao);
    outtextxy(DeslX+270, 276, Texto);

    sprintf(Texto, "Mín. Tração: %.02f kN", MinTracao);
    outtextxy(DeslX+270, 292, Texto);

    sprintf(Texto, "Mín. Compressão: %.02f kN", MinCompressao);
    outtextxy(DeslX+270, 308, Texto);

    sprintf(Texto, "Máx. Compressão: %.02f kN", MaxCompressao);
    outtextxy(DeslX+270, 324, Texto);

    setlinestyle(0, 1, 1);
    setcolor(0); //volta a cor para preto para desenhar os apoios
    setfillstyle(1, 15); //Desenha fundo branco
    for (int i=0;i<NumNos;i++)  //desenha nós e apoios
    {
        //setcolor(0); //volta a cor para branco para desenhar os apoios
        if (RestricaoApoioY[i]>0)
        {
            line(DeslX+CoordX[i]*DisplayFatorAmpliacao, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao, DeslX+CoordX[i]*DisplayFatorAmpliacao-10, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+20);
            line(DeslX+CoordX[i]*DisplayFatorAmpliacao, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao, DeslX+CoordX[i]*DisplayFatorAmpliacao+10, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+20);
            line(DeslX+CoordX[i]*DisplayFatorAmpliacao-10, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+20, DeslX+CoordX[i]*DisplayFatorAmpliacao+10, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+20);
        }
        if (RestricaoApoioX[i]>0)
        {
            line(DeslX+CoordX[i]*DisplayFatorAmpliacao-10, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+20, DeslX+CoordX[i]*DisplayFatorAmpliacao-5, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+25);
            line(DeslX+CoordX[i]*DisplayFatorAmpliacao-5, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+20, DeslX+CoordX[i]*DisplayFatorAmpliacao+0, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+25);
            line(DeslX+CoordX[i]*DisplayFatorAmpliacao+0, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+20, DeslX+CoordX[i]*DisplayFatorAmpliacao+5, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+25);
            line(DeslX+CoordX[i]*DisplayFatorAmpliacao+5, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+20, DeslX+CoordX[i]*DisplayFatorAmpliacao+10, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+25);
            line(DeslX+CoordX[i]*DisplayFatorAmpliacao+10, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+20, DeslX+CoordX[i]*DisplayFatorAmpliacao+15, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao+25);
        }
        fillellipse(DeslX+CoordX[i]*DisplayFatorAmpliacao, DisplayDeslocY-CoordY[i]*DisplayFatorAmpliacao, 3, 3 );
    }




    //Monta legenda de cores
    setlinestyle(0, 1, 4);
    for (int i=0; i<41; i++)
    {
        EsforcoLegenda[i]=MaxTracao-i*(MaxTracao-MaxCompressao)/40;
        CorTracao=(sqrt((EsforcoLegenda[i]/MaxTracao)));
        CorCompressao=(sqrt((EsforcoLegenda[i]/MaxCompressao)));
        if (EsforcoLegenda[i]>0) setcolor(COLOR(0, 0, 255*CorTracao)); //BARRAS TRACIONADAS
        if (EsforcoLegenda[i]<0) setcolor(COLOR(255*CorCompressao, 0, 0)); //BARRAS COMPRIMIDAS
        line(DeslX+300, 360+(i*4),DeslX+330,360+(i*4));

    }

        setcolor(COLOR(0, 0, 255));
        sprintf(Texto, "%.02f kN", MaxTracao);
        outtextxy(DeslX+340,350,Texto);

        setcolor(COLOR(255, 0, 0));
        sprintf(Texto, "%.02f kN", MaxCompressao);
        outtextxy(DeslX+340, 510,Texto);


}


int main ()
{
    int DisplayLargura=1920;
    int DisplayAltura=1080;

    initwindow( DisplayLargura, DisplayAltura, "Treliça" );
    cleardevice();

    setfillstyle(1, 15); //Desenha fundo branco
    bar(0, 0, DisplayLargura, DisplayAltura);  //fundo textos

    //Desenha linhas de grade no fundo
    setcolor(COLOR(240,240,240));
    setlinestyle(0, 1, 1);
    for(int i=0;i<100;i++)
    {
        line(i*20, 0, i*20, DisplayAltura);
        line(0, i*20, DisplayLargura, i*20);
    }


    PlotaEstrutura(0,300);
    PlotaEstrutura(1,1200);
    while ( !kbhit() ) {}
}





