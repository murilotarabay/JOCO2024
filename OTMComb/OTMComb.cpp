#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <Eigen/LU>
#include <Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;

using namespace std;
//Módulo de Elasticidade (kN/m²)
//Número de nós (int)
    //Coordenadas dos nós em metros (float, float)
//Número de barras (int)
    //nós das barras (int, int)
    //rigidez das barras E*A (float)
//apoios

int CriterioDim=0;

float DimensionaBarra(float Nd, float CompFlambagem)
{

float Cantoneiras[9][59] = {{ 0.55,   0.71,  0.87,  1.03,  1.19,  1.51,  1.72,  1.82,  2.14,  2.19,  2.22,  2.46,  2.67,  2.85,  3.14,  3.48,  3.62,   4.11,   4.57,   4.75,   5.52,   5.84,   6.01,   6.88,   7.28,   7.42,   8.55,   8.78,   9.00,   9.81,  10.59,  10.68,  12.17,  12.32,  12.34,  12.58,  13.93,  14.47,  15.34,  16.75,  18.27,  18.99,  21.18,  22.07,  23.35,  24.06,   29.12,   29.68,   35.14,   36.02,   39.25,   42.73,   43.99,   48.68,   57.93,   67.04,   75.97,   84.77, 1000.00  },        //Massa por metro linear (Kg/m)
                            {13.83,  18.24, 22.68, 27.13, 31.59, 40.53, 44.29, 49.48, 58.44, 57.60, 55.26, 67.40, 70.95, 72.88, 84.33, 90.60, 97.71, 108.38, 124.52, 126.19, 151.34, 152.71, 161.89, 177.18, 197.63, 197.23, 233.40, 230.41, 241.86, 269.19, 286.53, 283.81, 331.25, 324.07, 340.80, 337.30, 362.40, 390.86, 420.73, 448.82, 498.08, 504.78, 573.84, 605.38, 610.83, 647.55,  790.52,  788.93,  922.04,  967.42, 1076.76, 1135.75, 1201.92, 1324.98, 1564.18, 1795.99, 2019.11, 2235.08, 10000.00 },        //Ntrd,e(kN) - Força axial resistente de cálculo para escoamento da seção bruta
                            { 0.37,   0.47,  0.57,  0.67,  0.77,  0.98,  0.75,  1.18,  1.39,  0.96,  0.74,  1.59,  1.16,  0.94,  1.36,  1.14,  1.57,   1.34,   1.98,   1.55,   2.39,   1.53,   1.95,   1.51,   2.36,   1.93,   2.77,   1.91,   2.34,   3.18,   2.75,   2.32,   3.16,   2.30,   4.00,   2.73,   2.28,   3.14,   3.98,   3.11,   3.95,   3.09,   3.93,   4.77,   3.05,   3.91,    4.73,    3.87,    3.83,    4.68,    6.36,    4.64,    6.34,    6.32,    6.27,    6.23,    6.19,    6.15, 7.00     },        //Raio de giração em relação aos planos X e Y(cm)
                            { 0.10,   0.20,  0.36,  0.59,  0.91,  1.83,  1.25,  3.24,  5.24,  2.56,  1.53,  7.92,  4.58,  3.19,  7.45,  5.77, 11.33,   9.46,  22.74,  14.47,  40.01,  17.33,  29.27,  19.93,  51.79,  35.33,  83.66,  40.94,  62.86, 126.51, 101.96,  73.21, 154.64,  83.02, 251.63, 119.20,  92.25, 181.34, 308.94, 206.99, 363.81, 231.49, 417.01, 640.13, 277.28, 468.27,  828.63,  565.30,  655.24, 1005.79, 2024.10, 1171.89, 2251.83, 2474.31, 2902.71, 3313.05, 3703.77, 4078.31, 5000.00  },        //Momento de inércia em relação ao eixo x (cm^4)
                            { 1.00,   1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  1.00,  0.97,  1.00,  1.00,  0.92,  1.00,  1.00,  1.00,  1.00,  1.00,   1.00,   0.99,   1.00,   0.92,   1.00,   1.00,   1.00,   1.00,   1.00,   0.97,   1.00,   1.00,   0.92,   1.00,   1.00,   1.00,   1.00,   0.81,   1.00,   1.00,   1.00,   0.92,   1.00,   0.99,   1.00,   1.00,   0.92,   1.00,   1.00,    1.00,    1.00,    1.00,    1.00,    0.92,    1.00,    0.96,    1.00,    1.00,    1.00,    1.00,    1.00, 1.00     },        //Fator de redução Qs (adimensional)
                            { 0.71,   0.91,  1.11,  1.31,  1.51,  1.92,  2.19,  2.32,  2.73,  2.80,  2.82,  3.13,  3.40,  3.63,  4.01,  4.44,  4.61,   5.24,   5.82,   6.05,   7.03,   7.44,   7.66,   8.77,   9.27,   9.45,  10.89,  11.18,  11.47,  12.50,  13.49,  13.60,  15.50,  15.70,  15.73,  16.02,  17.74,  18.44,  19.54,   21.34, 23.27,  24.19,  26.99,  28.11,  29.75,  30.65,   37.10,   37.81,   44.76,   45.88,   50.00,   54.44,   56.03,   62.01,   73.79,   85.40,   96.77,  107.98, 10000.00 },        //Área bruta da seção transversal (cm²)
                            { 0.25,   0.31,  0.37,  0.43,  0.50,  0.62,  0.49,  0.75,  0.88,  0.62,  0.50,  1.01,  0.75,  0.62,  0.87,  0.74,  1.00,   0.87,   1.26,   0.99,   1.51,   0.99,   1.25,   0.99,   1.50,   1.24,   1.76,   1.24,   1.50,   2.02,   1.75,   1.49,   2.01,   1.49,   2.54,   1.74,   1.48,   2.00,   2.52,   1.99,   2.51,   1.99,   2.50,   3.03,   1.98,   2.50,    3.01,    2.48,    2.48,    2.99,    4.04,    2.98,    4.03,    4.02,    4.00,    3.99,    3.97,    3.96, 100.00   },        //Raio de giração em relação ao eixo de menor inércia (cm)
                            { 0.013, 0.016, 0.019, 0.022, 0.025, 0.032, 0.025, 0.038, 0.044, 0.032, 0.025, 0.051, 0.038, 0.032, 0.044, 0.038, 0.051,  0.044,  0.064,  0.051,  0.076,  0.051,  0.064,  0.051,  0.076,  0.064,  0.089,  0.064,  0.076,  0.102,  0.089,  0.076,  0.102,  0.076,  0.127,  0.089,  0.076,  0.102,  0.127,  0.102,  0.127,  0.102,  0.127,  0.152,  0.102,  0.127,   0.152,   0.127,   0.127,   0.152,   0.203,   0.152,   0.203,   0.203,   0.203,   0.203,   0.203,   0.203, 1000     },        //Área de vento (m²/m)
                            { 3.18,   3.18,  3.18,  3.18,  3.18,  3.18,  4.76,  3.18,  3.18,  4.76,  6.35,  3.18,  4.76,  6.35,  4.76,  6.35,  4.76,   6.35,   4.76,   6.35,   4.76,   7.94,   6.35,   9.52,   6.35,   7.94,   6.35,   9.52,   7.94,   6.35,   7.94,   9.52,   7.94,  11.11,   6.35,   9.52,   12.7,   9.52,   7.94,  11.11,   9.52,   12.7,  11.11,   9.52,  15.88,   12.7,    12.7,   15.88,   19.05,   15.88,    12.7,   19.05,   14.29,   15.88,   19.05,   22.23,    25.4,   28.58, 1000     }};       //Espessura cantoneira (mm)

    float MassaLinear;
    float Lx1;
    float rx1;
    float Kx1Lx1;
    float Lx1Divrx1;
    float Nex;
    float Lambda0;
    float FatorX;
    float Ncrd;
    float Esbeltez;

    CriterioDim=0;
    //CRITÉRIOD DE DIMENSIONAMENTO
    //1 - Esbeltez tração
    //2 - Ruptura tração
    //3 - Esbeltez compressão
    //4 - Ruptura compressão

    CompFlambagem=CompFlambagem*100;    //Converte de metros para centímetros

    if (Nd>=0)  //TESTE TRAÇÃO
    {
        for (int i=0;i<59;i++)
        {
            Esbeltez=CompFlambagem/Cantoneiras[6][i];
            if (Esbeltez<=300)
            {
                if ((Nd<=Cantoneiras[1][i]))
                    {
                        MassaLinear=Cantoneiras[0][i];

                        CriterioDim=2;
                        if ((Nd<=Cantoneiras[1][i-1]))
                        {
                            CriterioDim=1;
                        }

                        break;
                    }
            }
        }
    }

    if (Nd<0)  //TESTE COMPRESSÃO
    {
        for (int i=0;i<59;i++)
        {
            Esbeltez=(0.5*CompFlambagem)/Cantoneiras[6][i];
            //cout << Esbeltez << "\n";
            if (Esbeltez<=200)
            {
                //CALCULA LX/RX - ANEXO E.1.4.2
                Lx1=CompFlambagem;
                rx1=Cantoneiras[2][i];

                //Calcula Lx1/rx1 e Kx1Lx1
                Lx1Divrx1=Lx1/rx1;
                if (Lx1Divrx1<=80)
                {
                    Kx1Lx1 = 72*rx1 + 0.75*Lx1;
                }

                if (Lx1Divrx1>80)
                {
                    Kx1Lx1 = 32*rx1 + 1.25*Lx1;
                }
                //Cálculo de NE conforme Anexo E.1.4.1
                Nex=(9.8696*20500*Cantoneiras[3][i])/(Kx1Lx1*Kx1Lx1);

                //Cálculo do índice de esbeltez reduzido Lambda-0
                Lambda0=sqrt(Cantoneiras[4][i]*Cantoneiras[5][i]*25/Nex);

                //Calcula fator de redução X
                if (Lambda0<=1.5)
                {
                    FatorX=pow(0.658,(Lambda0*Lambda0));
                }
                if (Lambda0>1.5)
                {
                    FatorX=0.877/(Lambda0*Lambda0);
                }

                //Calcula força axial resistente Ncrd
                Ncrd=-FatorX*Cantoneiras[4][i]*Cantoneiras[5][i]*25/1.1;

                //Testa resistência compressão
                if (Nd>=Ncrd) //> - valores negativos
                {
                    MassaLinear=Cantoneiras[0][i];

                        CriterioDim=4;
                        if ((Nd>Ncrd*1.1))
                        {
                            CriterioDim=2;
                        }
                    break;
                }
            }
        }
    }
    return(MassaLinear);
}


void OtimizaEstrutura()
{
    cout << "Iniciando otimização \n";
    char teste;
    cin >> teste;

    string Parametros;
    char ParametrosChar[100];
    int NumNos;   //número de nós da treliça
    int NumBarras;    //número de barras da treliça

    ifstream myfile ("inicial.txt");      //Abre o arquivo descritivo da treliça
    if (myfile.is_open())
    {
        getline (myfile,Parametros);
        strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
        NumNos=atoi(ParametrosChar);                //Leitura do número de nós
        getline (myfile,Parametros);
        strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
        NumBarras=atoi(ParametrosChar);                //Leitura do número de barras
    }

    float CoordX[NumNos]; //Coordenadas X dos nós
    float CoordY[NumNos]; //Coordenadas Y dos nós
    float ForcaX[NumNos]; //Componente X das forças externas nos nós
    float ForcaY[NumNos]; //Componente Y das forças externas nos nós
    int BarraI[NumBarras]; //Nó da extremidade I da barra
    int BarraJ[NumBarras]; //Nó da extremidade J da barra
    float SenoAngulo[NumBarras]; //Seno do ângulo da barra com o eixo X
    float CossenoAngulo[NumBarras]; //Cosseno do ângulo da barra como eixo X
    float ComprimentoBarra[NumBarras]; //Comprimento da barra em metros
    float MassaBarra[NumBarras]; //massa por metro linear da barra dimensionada
    int RestricaoApoioX[NumNos]; //1 - Indica apoio fixo no eixo X
    int RestricaoApoioY[NumNos]; //1 - Indica apoio fixo no eixo Y
    int AuxRestrX=0; //Variável auxiliar para montar os apoios em X a matriz de forças
    int AuxRestrY=0; //Variável auxiliar para montar os apoios em Y a matriz de forças
    float ComprimentoTotal=0; //comprimento total de todas as barras da estrutura
    float MassaTotal=0;    //Massa total da treliça em kg
    float IndiceEficiencia=0; //Índice de eficiência da treliça (somatório do produto do comprimento e do módulo da esforço interno)

    float h=0.3;   //delta inserido nas coordenadas dos nós para cálculo do gradiente.
    //float h=0.01;   //delta inserido nas coordenadas dos nós para cálculo do gradiente.
    float CoordXOTM[NumNos]; //Coordenadas X otimizada dos nós (+/- h)
    float CoordYOTM[NumNos]; //Coordenadas Y otimizada dos nós (+/- h)
    float CoordXAUX[NumNos]; //Coordenadas X otimizada dos nós (+/- h)
    float CoordYAUX[NumNos]; //Coordenadas Y otimizada dos nós (+/- h)

    float DeslocamentoPonta;
    float AreaBarra[NumBarras];
    float MassaTotalOTM=0;
    float IndiceEficienciaOTM=0;
    float ComprimentoTotalOTM=0;
    float DeslocamentoPontaOTM=0;

    MatrixXd MatrizDeForcas(NumNos*2,NumBarras+4);   //4 reações de apoio (para dois apoios de 2º gênero)
    VectorXd VetorForcasExternas(NumNos*2);
    VectorXd VetorEsforcosInternos(NumNos*2);

    for (int i=0;i<(NumNos*2);i++)  //inicializa matriz de rigidez e os vetores para evitar corrigir erros numéricos
    {
        VetorForcasExternas(i)=0;
        VetorEsforcosInternos(i)=0;
        for (int j=0;j<(NumBarras+3);j++)
        {
            MatrizDeForcas(i,j)=0;
        }
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
        }
    }
    myfile.close();

//PROCESSA A ESTRUTURA INICIAL
                AuxRestrX=0;
                AuxRestrY=0;
                IndiceEficiencia=0;
                ComprimentoTotal=0;
                DeslocamentoPonta=0;
                MassaTotal=0;
                for (int i=0;i<NumBarras;i++)
                {
                    ComprimentoBarra[i]=sqrt((pow((CoordX[BarraI[i]]-CoordX[BarraJ[i]]),2))+(pow((CoordY[BarraI[i]]-CoordY[BarraJ[i]]),2)));
                    SenoAngulo[i]=(CoordY[BarraI[i]]-CoordY[BarraJ[i]])/ComprimentoBarra[i];
                    CossenoAngulo[i]=(CoordX[BarraI[i]]-CoordX[BarraJ[i]])/ComprimentoBarra[i];
                }

                //MONTA A MATRIZ DE EQUAÇÕES ISOSTÁTICAS
                //Linhas: Números de nós *2 (X e Y para cada nó)
                //Colunas: (número de barras + 3 reações de apoio + forças externas)
                for (int i=0;i<(NumNos*2);i++)  //inicializa matriz de esforços internos para evitar corrigir erros numéricos
                {
                    for (int j=0;j<(NumBarras+4);j++)
                    {
                        MatrizDeForcas(i, j)=0;
                    }
                }

                for (int i=0;i<(NumNos);i++)  //Montagem da matriz de esforços internos
                {
                    for (int j=0;j<(NumBarras);j++)
                    {
                        if (BarraI[j]==i) MatrizDeForcas(i,j)=-CossenoAngulo[j]; //Somatório em X
                        if (BarraJ[j]==i) MatrizDeForcas(i,j)=CossenoAngulo[j];
                        if (BarraI[j]==i) MatrizDeForcas(i+NumNos,j)=-SenoAngulo[j]; //Somatório em Y
                        if (BarraJ[j]==i) MatrizDeForcas(i+NumNos,j)=SenoAngulo[j];
                    }
                    VetorForcasExternas(i)=-ForcaX[i];
                    VetorForcasExternas(i+NumNos)=-ForcaY[i];
                }

                //Vetor de forças externas e reações de apoio
                for (int i=0;i<NumNos;i++)      //Restrições dos apoios
                {
                    if (RestricaoApoioX[i]==1)
                    {
                        MatrizDeForcas(i,NumBarras+AuxRestrX) = RestricaoApoioX[i];
                        AuxRestrX++;  //Auxiliar para deslocar 1 coluna em caso de mais de uma restrição em X
                    }
                    if (RestricaoApoioY[i]==1)
                    {
                        MatrizDeForcas(i+NumNos,NumBarras+2+AuxRestrY) = RestricaoApoioY[i];
                        AuxRestrY++;  //Auxiliar para deslocar 1 coluna em caso de mais de uma restrição em Y
                    }
                }

                VetorEsforcosInternos=(MatrizDeForcas.transpose() * MatrizDeForcas).ldlt().solve(MatrizDeForcas.transpose() * VetorForcasExternas);
                    for (int l=0;l<NumBarras;l++)
                    {
                        IndiceEficiencia+=abs(VetorEsforcosInternos(l))*ComprimentoBarra[l];
                        ComprimentoTotal=ComprimentoTotal+ComprimentoBarra[l];
                        MassaBarra[l]=DimensionaBarra(VetorEsforcosInternos(l), ComprimentoBarra[l]);
                        MassaTotal=MassaTotal+(ComprimentoBarra[l]*MassaBarra[l]);
                        AreaBarra[l]=1.27388*MassaBarra[l]; //cm²
                        DeslocamentoPonta=DeslocamentoPonta+((abs(VetorEsforcosInternos(l))*abs(VetorEsforcosInternos(l))/100)*ComprimentoBarra[l])/(20500*AreaBarra[l]);
                    }

    cout << "RESULTADO DO PROCESSAMENTO INICIAL: \n";
    cout << "Indice de eficiencia: " << IndiceEficiencia << "\n";
    cout << "Comprimento total: " << ComprimentoTotal << "\n";
    cout << "Massa total (kg): " << MassaTotal << "\n\n";
    cout << "Deslocamento Ponta (m): " << DeslocamentoPonta << "\n\n";

    cin >> teste;

    ofstream RelatorioProcessamento;
    RelatorioProcessamento.open ("relatorioinicial.txt");

    RelatorioProcessamento << NumNos << "\n";
    RelatorioProcessamento << NumBarras << "\n";

    for (int i=0; i<NumNos;i++)
    {
        RelatorioProcessamento << CoordX[i] << " " << CoordY[i] << " " << ForcaX[i] << " " << ForcaY[i] << " " << RestricaoApoioX[i] << " " << RestricaoApoioY[i] << "\n";
    }


    for (int i=0; i<NumBarras;i++)
    {
        RelatorioProcessamento << BarraI[i] << " " << BarraJ[i] << " " << VetorEsforcosInternos(i) << " " << DimensionaBarra(VetorEsforcosInternos(i), ComprimentoBarra[i]) << " " << ComprimentoBarra[i] << "\n";
    }

    RelatorioProcessamento <<  IndiceEficiencia << "\n";  //"IndiceEficiencia: "
    RelatorioProcessamento <<  ComprimentoTotal << "m\n"; //"ComprimentoTotal(m): "
    RelatorioProcessamento <<  MassaTotal<< "kg\n"; //"MassaTotal(kg): "
    RelatorioProcessamento <<  DeslocamentoPonta << "m"; //"deslocamento ponta (m): "
    RelatorioProcessamento.close();

    int NosOTM=(NumNos-3); //Número de nós que serão otimizados
    int NumCombinacoes=pow(9,NosOTM);   //número de combinações a serem testadas

    cout << "Número de combinações: " << NumCombinacoes << "\n\n";

    //GERA MATRIZ DE COMBINAÇÕES DE COORDENADAS POSSÍVEIS
    signed short MatrizCombinacoesOTM[NumCombinacoes][(NosOTM*2)];

        //Inicializa a primeira linha da matriz de combinações com -1 em todas as posições
    for (int i=0; i<(NosOTM*2);i++)
    {
        MatrizCombinacoesOTM[0][i]=-1;
    }

        //Incrementa cada linha da matriz de combinações como se fosse um sistema numérico de base 3 com os valores -1,  0 e 1
    for (int i=1;i<NumCombinacoes;i++)  //Inicia a partir da linha 2
    {
        for (int j=0;j<(NosOTM*2);j++)  //Copia linha anterior
        {
            MatrizCombinacoesOTM[i][j]=MatrizCombinacoesOTM[i-1][j];
        }

        MatrizCombinacoesOTM[i][0]=MatrizCombinacoesOTM[i][0]+1;

        for (int k=0;k<(NosOTM*2);k++)
        {
            if (MatrizCombinacoesOTM[i][k]==2)
            {
                MatrizCombinacoesOTM[i][k]=-1;
                MatrizCombinacoesOTM[i][k+1]=MatrizCombinacoesOTM[i][k+1]+1;
            }
       }
    }


    cin >> teste;

    //Escreve MatrizCombinaçõesOTM em um arquivo TXT
    ofstream MatrizCombTXT;
    MatrizCombTXT.open ("matrizcombinacoes.txt");

for (int i=0;i<NumCombinacoes;i++)
{
    for (int j=0;j<(NosOTM*2);j++)
    {
     MatrizCombTXT << MatrizCombinacoesOTM[i][j] << "\t";
    }
    MatrizCombTXT << "\n";
}

    //INICIALIZA OTIMIZAÇÂO
    MassaTotalOTM=MassaTotal;
    IndiceEficienciaOTM=IndiceEficiencia;
    ComprimentoTotalOTM=ComprimentoTotal;

        for (int i=0; i<NumNos; i++)
        {
            CoordXOTM[i]=CoordX[i];
            CoordYOTM[i]=CoordY[i];
        }

for(int CicloOTM=0;CicloOTM<29;CicloOTM++)
//for(int CicloOTM=0;CicloOTM<1;CicloOTM++)
{
    for (int i=0; i<NumNos; i++)
    {
        CoordX[i]=CoordXOTM[i];
        CoordY[i]=CoordYOTM[i];
    }

    h=h-0.01;

    for (int Linha=0;Linha<NumCombinacoes;Linha++)
    {
        for (int i=0; i<NumNos; i++)
        {
            CoordXAUX[i]=CoordX[i];
            CoordYAUX[i]=CoordY[i];
        }

        for (int i=3;i<NumNos;i++)
        {

            if (MatrizCombinacoesOTM[Linha][i-3]==MatrizCombinacoesOTM[Linha][i-3+NosOTM])
            {
                CoordXAUX[i]=CoordXAUX[i]+0.707*h*MatrizCombinacoesOTM[Linha][i-3];
                CoordYAUX[i]=CoordYAUX[i]+0.707*h*MatrizCombinacoesOTM[Linha][i-3+NosOTM];
            }
            else
            {
                CoordXAUX[i]=CoordXAUX[i]+h*MatrizCombinacoesOTM[Linha][i-3];
                CoordYAUX[i]=CoordYAUX[i]+h*MatrizCombinacoesOTM[Linha][i-3+NosOTM];
            }



        }

        //PROCESSA A ESTRUTURA ALTERADA
                AuxRestrX=0;
                AuxRestrY=0;
                IndiceEficiencia=0;
                ComprimentoTotal=0;
                DeslocamentoPonta=0;
                MassaTotal=0;
                for (int i=0;i<NumBarras;i++)
                {
                    ComprimentoBarra[i]=sqrt((pow((CoordXAUX[BarraI[i]]-CoordXAUX[BarraJ[i]]),2))+(pow((CoordYAUX[BarraI[i]]-CoordYAUX[BarraJ[i]]),2)));
                    SenoAngulo[i]=(CoordYAUX[BarraI[i]]-CoordYAUX[BarraJ[i]])/ComprimentoBarra[i];
                    CossenoAngulo[i]=(CoordXAUX[BarraI[i]]-CoordXAUX[BarraJ[i]])/ComprimentoBarra[i];
                }

                //MONTA A MATRIZ DE EQUAÇÕES ISOSTÁTICAS
                //Linhas: Números de nós *2 (X e Y para cada nó)
                //Colunas: (número de barras + 3 reações de apoio + forças externas)
                for (int i=0;i<(NumNos*2);i++)  //inicializa matriz de esforços internos para evitar corrigir erros numéricos
                {
                    for (int j=0;j<(NumBarras+4);j++)
                    {
                        MatrizDeForcas(i, j)=0;
                    }
                }

                for (int i=0;i<(NumNos);i++)  //Montagem da matriz de esforços internos
                {
                    for (int j=0;j<(NumBarras);j++)
                    {
                        if (BarraI[j]==i) MatrizDeForcas(i,j)=-CossenoAngulo[j]; //Somatório em X
                        if (BarraJ[j]==i) MatrizDeForcas(i,j)=CossenoAngulo[j];
                        if (BarraI[j]==i) MatrizDeForcas(i+NumNos,j)=-SenoAngulo[j]; //Somatório em Y
                        if (BarraJ[j]==i) MatrizDeForcas(i+NumNos,j)=SenoAngulo[j];
                    }
                    VetorForcasExternas(i)=-ForcaX[i];
                    VetorForcasExternas(i+NumNos)=-ForcaY[i];
                }

                //Vetor de forças externas e reações de apoio
                for (int i=0;i<NumNos;i++)      //Restrições dos apoios
                {
                    if (RestricaoApoioX[i]==1)
                    {
                        MatrizDeForcas(i,NumBarras+AuxRestrX) = RestricaoApoioX[i];
                        AuxRestrX++;  //Auxiliar para deslocar 1 coluna em caso de mais de uma restrição em X
                    }
                    if (RestricaoApoioY[i]==1)
                    {
                        MatrizDeForcas(i+NumNos,NumBarras+2+AuxRestrY) = RestricaoApoioY[i];
                        AuxRestrY++;  //Auxiliar para deslocar 1 coluna em caso de mais de uma restrição em Y
                    }
                }

                VetorEsforcosInternos=(MatrizDeForcas.transpose() * MatrizDeForcas).ldlt().solve(MatrizDeForcas.transpose() * VetorForcasExternas);
                    for (int l=0;l<NumBarras;l++)
                    {
                        IndiceEficiencia+=abs(VetorEsforcosInternos(l))*ComprimentoBarra[l];
                        ComprimentoTotal=ComprimentoTotal+ComprimentoBarra[l];
                        MassaBarra[l]=DimensionaBarra(VetorEsforcosInternos(l), ComprimentoBarra[l]);
                        MassaTotal=MassaTotal+(ComprimentoBarra[l]*MassaBarra[l]);

                        AreaBarra[l]=1.27388*MassaBarra[l]; //cm²
                        DeslocamentoPonta=DeslocamentoPonta+((abs(VetorEsforcosInternos(l))*abs(VetorEsforcosInternos(l))/100)*ComprimentoBarra[l])/(20500*AreaBarra[l]);

                    }

            if (MassaTotal<MassaTotalOTM)
            {
                MassaTotalOTM=MassaTotal;
                IndiceEficienciaOTM=IndiceEficiencia;
                ComprimentoTotalOTM=ComprimentoTotal;
                DeslocamentoPontaOTM=DeslocamentoPonta;


                for (int i=0; i<NumNos;i++)
                {
                    CoordXOTM[i]=CoordXAUX[i];
                    CoordYOTM[i]=CoordYAUX[i];
                }
            }
    }

}
    ofstream RelatorioOtimizada;
    RelatorioOtimizada.open ("relatoriootimizada.txt");

    RelatorioOtimizada << NumNos << "\n";
    RelatorioOtimizada << NumBarras << "\n";

    for (int i=0; i<NumNos;i++)
    {
        RelatorioOtimizada << CoordXOTM[i] << " " << CoordYOTM[i] << " " << ForcaX[i] << " " << ForcaY[i] << " " << RestricaoApoioX[i] << " " << RestricaoApoioY[i] << "\n";
    }

    for (int i=0; i<NumBarras;i++)
    {
        RelatorioOtimizada << BarraI[i] << " " << BarraJ[i] << " " << VetorEsforcosInternos(i) << " " << DimensionaBarra(VetorEsforcosInternos(i), ComprimentoBarra[i]) << " " << ComprimentoBarra[i] << "\n";
    }

    RelatorioOtimizada <<  IndiceEficienciaOTM << "\n";  //"IndiceEficiencia: "
    RelatorioOtimizada <<  ComprimentoTotalOTM << "m\n"; //"ComprimentoTotal(m): "
    RelatorioOtimizada <<  MassaTotalOTM<< "kg\n"; //"MassaTotal(kg): "
    RelatorioOtimizada <<  DeslocamentoPontaOTM << "m"; //"deslocamento ponta (m): "
    RelatorioOtimizada.close();

    ofstream arquivosaida;
    arquivosaida.open ("otimizada.txt");
    arquivosaida << NumNos << "\n";
    arquivosaida << NumBarras << "\n";

    for (int i=0; i<NumNos;i++)
    {
        arquivosaida << CoordXOTM[i] << " " << CoordYOTM[i] << " " << ForcaX[i] << " " << ForcaY[i] << " " << RestricaoApoioX[i] << " " << RestricaoApoioY[i] << "\n";
    }

    for (int i=0;i<NumBarras;i++)
    {
        arquivosaida << BarraI[i] << " " << BarraJ[i] << "\n";
    }
    arquivosaida << IndiceEficiencia << "  -  "  << ComprimentoTotal;
    arquivosaida.close();


    cout << "Indice de eficiencia: " << IndiceEficienciaOTM << "\n";
    cout << "Comprimento total: " << ComprimentoTotalOTM << "\n";
    cout << "Massa total (kg): " << MassaTotalOTM << "\n\n";
    cout << "Deslocamento Ponta (m): " << DeslocamentoPontaOTM << "\n\n";

    cin >> teste;

}

int main ()
{
  //GERA TABELA DE DIMENSIONAMENTO
 /*   ofstream TabelaDimensionamento;
    TabelaDimensionamento.open ("tabela.txt");

    TabelaDimensionamento << "Elementos kg/m \n";
    TabelaDimensionamento << "Comprimento/carga(kN) \n";

    for (float comprimento=0.1; comprimento<=6;comprimento=comprimento+0.1)
    {
    TabelaDimensionamento << comprimento << " ";
        for (float esforco=1; esforco<=50; esforco=esforco+1)
        {
            TabelaDimensionamento << DimensionaBarra(-1*esforco, comprimento) << " ";
        }
    TabelaDimensionamento << "\n";
    }
    TabelaDimensionamento.close();*/

    //ResolveInicial();
    OtimizaEstrutura();
}
