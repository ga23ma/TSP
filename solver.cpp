#include "solver.h"


double getDistance(Point p1, Point p2){
	double delta_x = p1.x - p2.x;
	double delta_y = p1.y - p2.y;

	return sqrt(delta_x * delta_x + delta_y * delta_y); 
}

vector<int> solveBottomUp(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started){
	Node ** tabela = new Node *[(1<<instance.n)];
    for(int i = 0; i<(1<<instance.n);i++){
        tabela[i]=new Node[instance.n];
    }
    for(int j = 0;j<instance.n; j++){
        tabela[0][j].valor=getDistance(instance.points.at(0),instance.points.at(j));
        tabela[0][j].k=0;

        }
    for(int i = 1; i<(1<<instance.n);i++){
        for(int j = 0;j<instance.n; j++){
            int kx=0;
            double menor = DBL_MAX;
            int kMenor;
            for(int k=i;k>0;k=(k>>1)){
                if(k % 2 == 1){
                    double novoponto =(getDistance(instance.points.at(j),instance.points.at(kx))+tabela[( i & (INT_MAX-((1<<kx))) )][kx].valor);
                    if( menor > novoponto ){
                        menor=novoponto;
                        kMenor=kx;
                    }
                }
                kx++;
            }
            tabela [i][j].valor=menor;
            tabela [i][j].k=kMenor;
        }
    }

    int visitado = ~((1<<(instance.n))-1);
    visitado = visitado+(1<<0);
    vector<int> caminho;
    int onde;
    int k = instance.n-1;
    for(int i=0;i<instance.n-1;i++){
        onde = ~visitado;
        k=tabela[onde][k].k;
        caminho.push_back(k);
        visitado = visitado+(1<<k);
    }
    vector<int> aux;
    for(int i=caminho.size()-1; i>=0;i--){
        aux.push_back(caminho.at(i));
    }
    aux.pop_back();
    return aux;
}

Path solverR(Instance instance, int marcado,int no, Path ** tabela){
    marcado = marcado|(1<<no);
    bool marcou=0;
    double menordist=0;
    int noMenorDist;
    double dist=0;
    vector<int> menorCaminho;
    Path p;
    if(tabela[marcado][no].distancia != -1)
    {
        return tabela [marcado][no];
    }
    for(int i = 0; i < (instance.n-1); i++){
        if((marcado&(1<<i)) == 0){
            p = solverR(instance, marcado, i, tabela);
            dist = p.distancia;
            dist = dist+ getDistance(instance.points.at(no), instance.points.at(i));
            if(marcou == 0 || dist<menordist){
                menordist=dist;
                noMenorDist=i;
                menorCaminho = p.lugar;
            }
            marcou=1;
        }
    }

    if(marcou){
       p.distancia=menordist;
       p.lugar=menorCaminho;
       p.lugar.push_back(noMenorDist);
       return tabela[marcado][no]=p;
    }
    p.distancia=getDistance(instance.points.at(no), instance.points.at(instance.n-1));
    p.lugar=menorCaminho;
    return tabela[marcado][no]=p;
}


vector<int> solveTopDown(Instance &instance, int timelimit, chrono::high_resolution_clock::time_point &started){
	int marcado = 0;
    vector<int> aux;
    Path p;
    Path **tabela = new Path *[(1<<instance.n)];
    for(int i=0; i<(1<<instance.n); i++)
    {
        tabela[i]=new  Path [instance.n];
        for(int j=0; j<instance.n;j++)
        {
            tabela[i][j].distancia=-1;
        }
    }

    p = solverR(instance,marcado,0,tabela);
    for(int i=p.lugar.size()-1; i>=0;i--){
        aux.push_back(p.lugar.at(i));
    }
    p.lugar=aux;
    return p.lugar;
}
