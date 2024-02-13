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
//#include <Eigen/LU>
//#include <Eigen/Dense>
//using Eigen::MatrixXf;
//using Eigen::VectorXf;

int NumNos;                                       //número de nós da treliça
int NumBarras;                                    //número de barras da treliça
float CoordX[1000];                             //Coordenadas X dos nós
float CoordY[1000];                             //Coordenadas Y dos nós
float ForcaX[1000];                             //Componente X das forças externas nos nós
float ForcaY[1000];                             //Componente Y das forças externas nos nós
int BarraI[1000];                            //Nó da extremidade I da barra
int BarraJ[1000];                            //Nó da extremidade J da barra
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
int DisplayFatorAmpliacao=150;
int DisplayDeslocX=360;
char Texto[32];
int XTextoDados=10;                               //Posição X dos textos

using namespace std;

void PlotaEstrutura(int Arquivo, int DeslY)         //Arquivo 0=relatorioinicial.txt 1=relatoriootimizada.txt  // Deslocamento em X
{
  string Parametros;
  const char * Arq;
  if (Arquivo==0) Arq = "relatorioinicial.txt";
  if (Arquivo==1) Arq = "relatoriootimizada.txt";
  ifstream myfile (Arq);                            //Abre o arquivo descritivo da treliça

  if (myfile.is_open())
  {
    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());     //converte string em char
    NumNos=atoi(ParametrosChar);                    //Leitura do número de nós
    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());     //converte string em char
    NumBarras=atoi(ParametrosChar);                 //Leitura do número de barras
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
        strcpy(ParametrosChar, Parametros.c_str());         //converte string em char
        char* FimChar;
        BarraI[i] = strtof(ParametrosChar, &FimChar);
        BarraJ[i] = strtof(FimChar, &FimChar);
        EsforcoInterno[i] = strtof(FimChar, &FimChar);
        PesoLinearBarra[i] = strtof(FimChar, &FimChar);
        ComprimentoBarra[i] = strtof(FimChar, &FimChar);
        CriterioDimensionamento[i] = strtof(FimChar, &FimChar);
    }

    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());             //converte string em char
    IndiceEficiencia=atof(ParametrosChar);                  //Índice de eficiência

    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());             //converte string em char
    ComprimentoTotal=atof(ParametrosChar);                  //Leitura comprimento total

    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());             //converte string em char
    MassaTotal=atof(ParametrosChar);                        //Leitura da massa total

    getline (myfile,Parametros);
    strcpy(ParametrosChar, Parametros.c_str());             //converte string em char
    DeslocamentoLateral=atof(ParametrosChar);               //Leitura do Deslocamento lateral

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
    setlinestyle(0, 1, 1);
    for (int i=0;i<NumBarras;i++)   //desenha barras e valor dos esforços internos
    {
        CorTracao=sqrt((EsforcoInterno[i]/MaxTracao));
        CorCompressao=sqrt((EsforcoInterno[i]/MaxCompressao));
        if (EsforcoInterno[i]>0) setcolor(COLOR(0, 0, 255*CorTracao)); //BARRAS TRACIONADAS
        if (EsforcoInterno[i]<0) setcolor(COLOR(255*CorCompressao, 0, 0)); //BARRAS COMPRIMIDAS

        line(DisplayDeslocX+CoordX[BarraI[i]]*DisplayFatorAmpliacao, DeslY-CoordY[BarraI[i]]*DisplayFatorAmpliacao, DisplayDeslocX+CoordX[BarraJ[i]]*DisplayFatorAmpliacao, DeslY-CoordY[BarraJ[i]]*DisplayFatorAmpliacao);

        sprintf(Texto, "%d", i+1);
        outtextxy((((DisplayDeslocX+CoordX[BarraI[i]]*DisplayFatorAmpliacao)+(DisplayDeslocX+CoordX[BarraJ[i]]*DisplayFatorAmpliacao))/2-5),(((DeslY-CoordY[BarraI[i]]*DisplayFatorAmpliacao)+(DeslY-CoordY[BarraJ[i]]*DisplayFatorAmpliacao))/2-5), Texto);
    }

    setfillstyle(1, 15);                        //Desenha fundo branco
    bar(1, DeslY-164  , 270, DeslY+45);         //fundo textos
    bar(1081, DeslY-164, 1230, DeslY+45);       //fundo legenda

    setfillstyle(1, 15);

    setcolor(0);                                //cor preta para textos
    setlinestyle(0, 1, 1);

    if (Arquivo==0)
    {
       outtextxy(XTextoDados,DeslY-150, "ESTRUTURA INICIAL");
    }

    if (Arquivo==1)
    {
       outtextxy(XTextoDados,DeslY-150, "ESTRUTURA OTIMIZADA");
    }

    sprintf(Texto, "Massa total: %.02f kg", MassaTotal);
    outtextxy(XTextoDados,DeslY-130,Texto);                                             //Plota texto Massa Total
    sprintf(Texto, "IEI: %.02f kNm", IndiceEficiencia);
    outtextxy(XTextoDados,DeslY-110,Texto);                                             //Plota texto Indice Eficiência
    sprintf(Texto, "Comprimento total: %.02f m", ComprimentoTotal);
    outtextxy(XTextoDados,DeslY-90,Texto);
    sprintf(Texto, "Deslocamento extremidade: %.02f mm", DeslocamentoLateral*1000);
    outtextxy(XTextoDados,DeslY-70,Texto);
    sprintf(Texto, "Máx Tração: %.02f kN", MaxTracao);
    outtextxy(XTextoDados,DeslY-50,Texto);
    sprintf(Texto, "Mín. Tração: %.02f kN", MinTracao);
    outtextxy(XTextoDados,DeslY-30,Texto);
    sprintf(Texto, "Mín. Compressão: %.02f kN", MinCompressao);
    outtextxy(XTextoDados,DeslY-10,Texto);
    sprintf(Texto, "Máx. Compressão: %.02f kN", MaxCompressao);
    outtextxy(XTextoDados,DeslY+10,Texto);




    setlinestyle(0, 1, 1);
    for (int i=0;i<NumNos;i++)                              //desenha nós e apoios
    {

        setcolor(COLOR(0,200,0));                           //Cor verde para os vetores de forças
        if (ForcaY[i]<0)
        {
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao, DeslY-CoordY[i]*DisplayFatorAmpliacao-30, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao, DeslY-CoordY[i]*DisplayFatorAmpliacao-10);
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao, DeslY-CoordY[i]*DisplayFatorAmpliacao-10, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao+5, DeslY-CoordY[i]*DisplayFatorAmpliacao-15);
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao, DeslY-CoordY[i]*DisplayFatorAmpliacao-10, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao-5, DeslY-CoordY[i]*DisplayFatorAmpliacao-15);
            sprintf(Texto, "%.01f kN", ForcaY[i]);
            setbkcolor(15);         //fundo do texto branco
            outtextxy(CoordX[i]*DisplayFatorAmpliacao+10+DisplayDeslocX, DeslY-CoordY[i]*DisplayFatorAmpliacao-30, Texto);
        }

        setcolor(0); //volta a cor para preto para desenhar os apoios
        setfillstyle(1, 0); //Desenha fundo branco

        if (RestricaoApoioY[i]>0)
        {
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao, DeslY-CoordY[i]*DisplayFatorAmpliacao, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao-10, DeslY-CoordY[i]*DisplayFatorAmpliacao+20);
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao, DeslY-CoordY[i]*DisplayFatorAmpliacao, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao+10, DeslY-CoordY[i]*DisplayFatorAmpliacao+20);
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao-10, DeslY-CoordY[i]*DisplayFatorAmpliacao+20, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao+10, DeslY-CoordY[i]*DisplayFatorAmpliacao+20);
        }
        if (RestricaoApoioX[i]>0)
        {
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao-10, DeslY-CoordY[i]*DisplayFatorAmpliacao+20, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao-5, DeslY-CoordY[i]*DisplayFatorAmpliacao+25);
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao-5, DeslY-CoordY[i]*DisplayFatorAmpliacao+20, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao+0, DeslY-CoordY[i]*DisplayFatorAmpliacao+25);
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao+0, DeslY-CoordY[i]*DisplayFatorAmpliacao+20, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao+5, DeslY-CoordY[i]*DisplayFatorAmpliacao+25);
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao+5, DeslY-CoordY[i]*DisplayFatorAmpliacao+20, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao+10, DeslY-CoordY[i]*DisplayFatorAmpliacao+25);
            line(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao+10, DeslY-CoordY[i]*DisplayFatorAmpliacao+20, DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao+15, DeslY-CoordY[i]*DisplayFatorAmpliacao+25);
        }
        setfillstyle(1, 15); //Desenha fundo branco
        fillellipse(DisplayDeslocX+CoordX[i]*DisplayFatorAmpliacao, DeslY-CoordY[i]*DisplayFatorAmpliacao, 5, 5 );     //desenha nós
    }

    //Monta legenda de cores
    setlinestyle(0, 1, 4);
    for (int i=0; i<41; i++)
    {
        EsforcoLegenda[i]=MaxTracao-i*(MaxTracao-MaxCompressao)/40;
        CorTracao=sqrt((EsforcoLegenda[i]/MaxTracao));
        CorCompressao=sqrt((EsforcoLegenda[i]/MaxCompressao));
        if (EsforcoLegenda[i]>0) setcolor(COLOR(0, 0, 255*CorTracao)); //BARRAS TRACIONADAS
        if (EsforcoLegenda[i]<0) setcolor(COLOR(255*CorCompressao, 0, 0)); //BARRAS COMPRIMIDAS
        line(1100, DeslY-140+(i*4),1130,DeslY-140+(i*4));

    }
        setcolor(COLOR(0, 0, 255));
        sprintf(Texto, "%.02f kN", MaxTracao);
        outtextxy(1140,DeslY-150,Texto);

        setcolor(COLOR(255, 0, 0));
        sprintf(Texto, "%.02f kN", MaxCompressao);
        outtextxy(1140,DeslY+10,Texto);
}


int main ()
{
    int DisplayLargura=1280;
    int DisplayAltura=1024;
    int Grade=15;

    initwindow( 1280, 1024, "Treliça" );
    cleardevice();


    setfillstyle(1, 15); //Desenha fundo branco
    bar(0, 0, DisplayLargura, DisplayAltura);  //fundo textos

    //Desenha linhas de grade no fundo
    setcolor(COLOR(240,240,240));
    setlinestyle(0, 1, 1);
    for(int i=0;i<100;i++)
    {
        line(i*Grade, 0, i*Grade, DisplayAltura);
        line(0, i*Grade, DisplayLargura, i*Grade);
    }

    PlotaEstrutura(0,300);
    PlotaEstrutura(1,600);
    while ( !kbhit() ) {}
}
