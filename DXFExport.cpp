#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main ()
{
    int Opcao=0;

    cout << "Gera arquivo DXF de torres trelicadas a partir do arquivo trelica.txt. Eixo de simetria na origem. \n";
    cout << "Selecione uma opção: \n";
    cout << "1 - Gerar arquivo DXF 2D \n";
    cout << "2 - Gerar arquivo DXF 3D \n";

    cin >> Opcao;

    string Parametros;
    char ParametrosChar[100];
    int NumNos;                             //número de nós da treliça
    int NumBarras;                          //número de barras da treliça

    ifstream myfile ("trelica.txt");        //Abre o arquivo descritivo da treliça
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


    //  float h=0.2;   //delta inserido nas coordenadas dos nós para cálculo do gradiente.
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
    float RaioCircOTM=0;

    int Segmentos=0;
    float TamanhoSegmento=0;






    if (myfile.is_open())
    {
        for (int i=0; i<NumNos; i++)
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

        for (int i=0; i<NumBarras; i++)
        {
            getline (myfile,Parametros);
            strcpy(ParametrosChar, Parametros.c_str());  //converte string em char
            char* FimChar;
            BarraI[i] = strtof(ParametrosChar, &FimChar);
            BarraJ[i] = strtof(FimChar, &FimChar);
        }
    }
    myfile.close();


    cout << "RESULTADO DO PROCESSAMENTO INICIAL: \n";
    cout << "Indice de eficiencia: " << IndiceEficiencia << "\n";
    cout << "Comprimento total: " << ComprimentoTotal << "\n";
    cout << "Massa total (kg): " << MassaTotal << "\n\n";
    cout << "Deslocamento Ponta (m): " << DeslocamentoPonta << "\n\n";


    ofstream ArquivoDXF;


    if (Opcao==1)
    {
        ArquivoDXF.open ("trelica2D.dxf");
        ArquivoDXF <<  "0\nSECTION\n2\nTABLES\n0\nTABLE\n2\nVPORT\n70\n1\n0\nVPORT\n2\n*ACTIVE\n70\n0\n 10\n0.0\n 20\n0.0\n 11\n1.0\n 21\n1.0\n 12\n1500\n 22\n0\n 13\n0.0\n 23\n0.0\n 14\n10.0\n 24\n10.0\n 15\n10.0\n 25\n10.0\n 16\n0.0\n 26\n0.0\n 36\n1.0\n 17\n0.0\n 27\n0.0\n 37\n0.0\n 40\n1500\n 41\n0\n 42\n50.0\n 43\n0.0\n 44\n0.0\n 50\n0.0\n 51\n0.0\n 71\n     0\n 72\n  1000\n 73\n     1\n 74\n     3\n 75\n     0\n 76\n     0\n 77\n     0\n 78\n     0\n  0\nENDTAB\n  0\nENDSEC\n  0\nSECTION\n  2\nENTITIES\n  0\n";
        for (int i=0; i<NumNos; i++)
        {
            ArquivoDXF << "POINT\n  8\n0\n 10\n" << CoordX[i]   << "\n 20\n"<<    CoordY[i]    << "\n 30\n0.0\n  0\n";
        }

        for (int i=0; i<NumBarras; i++)
        {
            ArquivoDXF << "LINE\n  8\n0\n 10\n " << CoordX[BarraI[i]] << "\n 20\n" << CoordY[BarraI[i]] << "\n30\n0.0\n 11\n" << CoordX[BarraJ[i]] << "\n 21\n" << CoordY[BarraJ[i]] << "\n 31\n0.0\n  0\n";
        }
        ArquivoDXF <<  "ENDSEC\n  0\nEOF";
        ArquivoDXF.close();
    }




    if (Opcao==2)
    {
        int teste;
        Segmentos=1+((NumNos-5)/3);
        TamanhoSegmento=CoordY[2]/Segmentos;
        cout << "Segmentos: " << Segmentos << "\n\n";
        cout << "Tamanho segmento(m): " << TamanhoSegmento << "\n\n";
        cin >> teste;




        ArquivoDXF.open ("trelica3D.dxf");
        ArquivoDXF <<  "0\nSECTION\n2\nTABLES\n0\nTABLE\n2\nVPORT\n70\n1\n0\nVPORT\n2\n*ACTIVE\n70\n0\n 10\n0.0\n 20\n0.0\n 11\n1.0\n 21\n1.0\n 12\n1500\n 22\n0\n 13\n0.0\n 23\n0.0\n 14\n10.0\n 24\n10.0\n 15\n10.0\n 25\n10.0\n 16\n0.0\n 26\n0.0\n 36\n1.0\n 17\n0.0\n 27\n0.0\n 37\n0.0\n 40\n1500\n 41\n0\n 42\n50.0\n 43\n0.0\n 44\n0.0\n 50\n0.0\n 51\n0.0\n 71\n     0\n 72\n  1000\n 73\n     1\n 74\n     3\n 75\n     0\n 76\n     0\n 77\n     0\n 78\n     0\n  0\nENDTAB\n  0\nENDSEC\n  0\nSECTION\n  2\nENTITIES\n  0\n";

        //PLANO Z POSITIVO
        for (int i=0; i<NumBarras; i++)
        {
            if (CoordX[BarraI[i]]!=0 && CoordX[BarraJ[i]]!=0)
            {
                ArquivoDXF << "LINE\n  8\n0\n 10\n " << CoordX[BarraI[i]] << "\n 20\n" << CoordY[BarraI[i]] << "\n 30\n" << fabs(CoordX[BarraI[i]]) << "\n 11\n" << CoordX[BarraJ[i]] << "\n 21\n" << CoordY[BarraJ[i]] << "\n 31\n" << fabs(CoordX[BarraJ[i]]) << "\n  0\n";
            }

            if (CoordX[BarraI[i]]==0)
            {
                float MediaY=0;
                float MediaX=0;
                float XMax=0;
                float XMin=100;
                float XInf=0;
                float XSup=0;

                for (int z=0; z<NumBarras; z++)
                {
                    if (BarraI[z]==BarraI[i])
                    {
                        MediaY=MediaY+fabs(CoordY[BarraJ[z]]);
                        MediaX=MediaX+fabs(CoordX[BarraJ[z]]);
                        if (fabs(CoordX[BarraJ[z]])>XMax)
                        {
                            XMax=fabs(CoordX[BarraJ[z]]);
                            if (CoordY[BarraJ[z]]>CoordY[BarraI[z]]) XSup=XMax;
                            if (CoordY[BarraJ[z]]<CoordY[BarraI[z]]) XInf=XMax;
                        }
                        if (fabs(CoordX[BarraJ[z]])<XMin)
                        {
                            XMin=fabs(CoordX[BarraJ[z]]);
                            if (CoordY[BarraJ[z]]>CoordY[BarraI[z]]) XSup=XMin;
                            if (CoordY[BarraJ[z]]<CoordY[BarraI[z]]) XInf=XMin;
                        }
                    }

                    if (BarraJ[z]==BarraI[i])
                    {
                        MediaY=MediaY+fabs(CoordY[BarraI[z]]);
                        MediaX=MediaX+fabs(CoordX[BarraI[z]]);
                        if (fabs(CoordX[BarraI[z]])>XMax)
                        {
                            XMax=fabs(CoordX[BarraI[z]]);
                            if (CoordY[BarraI[z]]>CoordY[BarraJ[z]]) XSup=XMax;
                            if (CoordY[BarraI[z]]<CoordY[BarraJ[z]]) XInf=XMax;

                        }
                        if (fabs(CoordX[BarraI[z]])<XMin)
                        {
                            XMin=fabs(CoordX[BarraI[z]]);
                            if (CoordY[BarraI[z]]>CoordY[BarraJ[z]]) XSup=XMin;
                            if (CoordY[BarraI[z]]<CoordY[BarraJ[z]]) XInf=XMin;
                        }
                    }
                }
                MediaY=MediaY/4;
                MediaX=MediaX/4;
                MediaX=MediaX-((((XSup-XInf))*(MediaY-CoordY[BarraI[i]]))/TamanhoSegmento);

                ArquivoDXF << "LINE\n  8\n0\n 10\n " << CoordX[BarraI[i]] << "\n 20\n" << CoordY[BarraI[i]] << "\n 30\n" << MediaX << "\n 11\n" << CoordX[BarraJ[i]] << "\n 21\n" << CoordY[BarraJ[i]] << "\n 31\n" << fabs(CoordX[BarraJ[i]]) << "\n  0\n";
            }
        }

        //PLANO Z NEGATIVO
        for (int i=0; i<NumBarras; i++)
        {
            if (CoordX[BarraI[i]]!=0 && CoordX[BarraJ[i]]!=0)
            {
                ArquivoDXF << "LINE\n  8\n0\n 10\n " << CoordX[BarraI[i]] << "\n 20\n" << CoordY[BarraI[i]] << "\n 30\n" << -fabs(CoordX[BarraI[i]]) << "\n 11\n" << CoordX[BarraJ[i]] << "\n 21\n" << CoordY[BarraJ[i]] << "\n 31\n" << -fabs(CoordX[BarraJ[i]]) << "\n  0\n";
            }

            if (CoordX[BarraI[i]]==0)
            {


             /*   for (int z=0; z<NumBarras; z++)
                {
                    if (BarraI[z]==BarraI[i])
                        MediaX=MediaX+fabs(CoordX[BarraJ[z]]);
                    if (BarraJ[z]==BarraI[i])
                        MediaX=MediaX+fabs(CoordX[BarraI[z]]);
                }*/



                float MediaY=0;
                float MediaX=0;
                float XMax=0;
                float XMin=100;
                float XInf=0;
                float XSup=0;

                for (int z=0; z<NumBarras; z++)
                {
                    if (BarraI[z]==BarraI[i])
                    {
                        MediaY=MediaY+fabs(CoordY[BarraJ[z]]);
                        MediaX=MediaX+fabs(CoordX[BarraJ[z]]);
                        if (fabs(CoordX[BarraJ[z]])>XMax)
                        {
                            XMax=fabs(CoordX[BarraJ[z]]);
                            if (CoordY[BarraJ[z]]>CoordY[BarraI[z]]) XSup=XMax;
                            if (CoordY[BarraJ[z]]<CoordY[BarraI[z]]) XInf=XMax;
                        }
                        if (fabs(CoordX[BarraJ[z]])<XMin)
                        {
                            XMin=fabs(CoordX[BarraJ[z]]);
                            if (CoordY[BarraJ[z]]>CoordY[BarraI[z]]) XSup=XMin;
                            if (CoordY[BarraJ[z]]<CoordY[BarraI[z]]) XInf=XMin;
                        }
                    }

                    if (BarraJ[z]==BarraI[i])
                    {
                        MediaY=MediaY+fabs(CoordY[BarraI[z]]);
                        MediaX=MediaX+fabs(CoordX[BarraI[z]]);
                        if (fabs(CoordX[BarraI[z]])>XMax)
                        {
                            XMax=fabs(CoordX[BarraI[z]]);
                            if (CoordY[BarraI[z]]>CoordY[BarraJ[z]]) XSup=XMax;
                            if (CoordY[BarraI[z]]<CoordY[BarraJ[z]]) XInf=XMax;

                        }
                        if (fabs(CoordX[BarraI[z]])<XMin)
                        {
                            XMin=fabs(CoordX[BarraI[z]]);
                            if (CoordY[BarraI[z]]>CoordY[BarraJ[z]]) XSup=XMin;
                            if (CoordY[BarraI[z]]<CoordY[BarraJ[z]]) XInf=XMin;
                        }
                    }
                }
                MediaY=MediaY/4;
                MediaX=MediaX/4;
                MediaX=MediaX-((((XSup-XInf))*(MediaY-CoordY[BarraI[i]]))/TamanhoSegmento);

                ArquivoDXF << "LINE\n  8\n0\n 10\n " << CoordX[BarraI[i]] << "\n 20\n" << CoordY[BarraI[i]] << "\n 30\n" << -MediaX << "\n 11\n" << CoordX[BarraJ[i]] << "\n 21\n" << CoordY[BarraJ[i]] << "\n 31\n" << -fabs(CoordX[BarraJ[i]]) << "\n  0\n";
            }
        }

        //PLANO LATERAL POSITIVO (Apenas barras que ligam ao X=0 na treliça plana, laterais já construídas)
        for (int i=0; i<NumBarras; i++)
        {
            if (CoordX[BarraI[i]]==0)
            {
                float MediaY=0;
                float MediaX=0;
                float XMax=0;
                float XMin=100;
                float XInf=0;
                float XSup=0;

                for (int z=0; z<NumBarras; z++)
                {
                    if (BarraI[z]==BarraI[i])
                    {
                        MediaY=MediaY+fabs(CoordY[BarraJ[z]]);
                        MediaX=MediaX+fabs(CoordX[BarraJ[z]]);
                        if (fabs(CoordX[BarraJ[z]])>XMax)
                        {
                            XMax=fabs(CoordX[BarraJ[z]]);
                            if (CoordY[BarraJ[z]]>CoordY[BarraI[z]]) XSup=XMax;
                            if (CoordY[BarraJ[z]]<CoordY[BarraI[z]]) XInf=XMax;
                        }
                        if (fabs(CoordX[BarraJ[z]])<XMin)
                        {
                            XMin=fabs(CoordX[BarraJ[z]]);
                            if (CoordY[BarraJ[z]]>CoordY[BarraI[z]]) XSup=XMin;
                            if (CoordY[BarraJ[z]]<CoordY[BarraI[z]]) XInf=XMin;
                        }
                    }

                    if (BarraJ[z]==BarraI[i])
                    {
                        MediaY=MediaY+fabs(CoordY[BarraI[z]]);
                        MediaX=MediaX+fabs(CoordX[BarraI[z]]);
                        if (fabs(CoordX[BarraI[z]])>XMax)
                        {
                            XMax=fabs(CoordX[BarraI[z]]);
                            if (CoordY[BarraI[z]]>CoordY[BarraJ[z]]) XSup=XMax;
                            if (CoordY[BarraI[z]]<CoordY[BarraJ[z]]) XInf=XMax;

                        }
                        if (fabs(CoordX[BarraI[z]])<XMin)
                        {
                            XMin=fabs(CoordX[BarraI[z]]);
                            if (CoordY[BarraI[z]]>CoordY[BarraJ[z]]) XSup=XMin;
                            if (CoordY[BarraI[z]]<CoordY[BarraJ[z]]) XInf=XMin;
                        }
                    }
                }
                MediaY=MediaY/4;
                MediaX=MediaX/4;
                MediaX=MediaX-((((XSup-XInf))*(MediaY-CoordY[BarraI[i]]))/TamanhoSegmento);


                ArquivoDXF << "LINE\n  8\n0\n 10\n " << MediaX << "\n 20\n" << CoordY[BarraI[i]] << "\n 30\n" << CoordX[BarraI[i]] << "\n 11\n" << fabs(CoordX[BarraJ[i]]) << "\n 21\n" << CoordY[BarraJ[i]] << "\n 31\n" << CoordX[BarraJ[i]] << "\n  0\n";
            }
        }


        //PLANO LATERAL NEGATIVO (Apenas barras que ligam ao X=0 na treliça plana, laterais já construídas)
        for (int i=0; i<NumBarras; i++)
        {
            if (CoordX[BarraI[i]]==0)
            {
                float MediaY=0;
                float MediaX=0;
                float XMax=0;
                float XMin=100;
                float XInf=0;
                float XSup=0;

                for (int z=0; z<NumBarras; z++)
                {
                    if (BarraI[z]==BarraI[i])
                    {
                        MediaY=MediaY+fabs(CoordY[BarraJ[z]]);
                        MediaX=MediaX+fabs(CoordX[BarraJ[z]]);
                        if (fabs(CoordX[BarraJ[z]])>XMax)
                        {
                            XMax=fabs(CoordX[BarraJ[z]]);
                            if (CoordY[BarraJ[z]]>CoordY[BarraI[z]]) XSup=XMax;
                            if (CoordY[BarraJ[z]]<CoordY[BarraI[z]]) XInf=XMax;
                        }
                        if (fabs(CoordX[BarraJ[z]])<XMin)
                        {
                            XMin=fabs(CoordX[BarraJ[z]]);
                            if (CoordY[BarraJ[z]]>CoordY[BarraI[z]]) XSup=XMin;
                            if (CoordY[BarraJ[z]]<CoordY[BarraI[z]]) XInf=XMin;
                        }
                    }

                    if (BarraJ[z]==BarraI[i])
                    {
                        MediaY=MediaY+fabs(CoordY[BarraI[z]]);
                        MediaX=MediaX+fabs(CoordX[BarraI[z]]);
                        if (fabs(CoordX[BarraI[z]])>XMax)
                        {
                            XMax=fabs(CoordX[BarraI[z]]);
                            if (CoordY[BarraI[z]]>CoordY[BarraJ[z]]) XSup=XMax;
                            if (CoordY[BarraI[z]]<CoordY[BarraJ[z]]) XInf=XMax;

                        }
                        if (fabs(CoordX[BarraI[z]])<XMin)
                        {
                            XMin=fabs(CoordX[BarraI[z]]);
                            if (CoordY[BarraI[z]]>CoordY[BarraJ[z]]) XSup=XMin;
                            if (CoordY[BarraI[z]]<CoordY[BarraJ[z]]) XInf=XMin;
                        }
                    }
                }
                MediaY=MediaY/4;
                MediaX=MediaX/4;
                MediaX=MediaX-((((XSup-XInf))*(MediaY-CoordY[BarraI[i]]))/TamanhoSegmento);

                ArquivoDXF << "LINE\n  8\n0\n 10\n " << -MediaX << "\n 20\n" << CoordY[BarraI[i]] << "\n 30\n" << CoordX[BarraI[i]] << "\n 11\n" << -fabs(CoordX[BarraJ[i]]) << "\n 21\n" << CoordY[BarraJ[i]] << "\n 31\n" << CoordX[BarraJ[i]] << "\n  0\n";
            }
        }
        ArquivoDXF <<  "ENDSEC\n  0\nEOF";
        ArquivoDXF.close();
    }

}



