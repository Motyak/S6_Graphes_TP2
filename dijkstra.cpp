#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

#include "dijkstra.h"

Cmp::Cmp(int* d)
{
    this->d = d;
}

// pour une min heap -> j'ai changé '<' en '>='
bool Cmp::operator()(const int a, const int b)
{
    return this->d[a] >= this->d[b];
}

/****************************************/
/* Objectif : Constructeur avec comme argument
le nom du fichier contenant le graphe.
Il doit :
-> ouvrir le fichier filename
-> initialiser, avec son contenu, les attributs "n", "m" et "L",
/****************************************/
graphe::graphe(char* filename)
{
    // !!! TODO !!! //
    ifstream file(filename);

    file >> n;
    file >> m;
    
    T = new int[n];
    I = new int[n];
    d = new int[n];
    pere = new int[n];

    int i,j,k,l;

    for(i = 0 ; i < n; i++){
        vector<sommetadjacent> vect;
        L.push_back(vect);
    }

    for(i = 0 ; i < m; i++){
        file >> j;
        file >> k;        
        file >> l;    
        L[j].push_back(make_pair(k,l));
        L[k].push_back(make_pair(j,l));
    }    
        
    file.close();
}


/****************************************/
/* Objectif : Affichage des arêtes et longueurs
/****************************************/
void graphe::donnees()
{
	cout << "arête : " << "longueur" << endl;
	for(int i = 0; i < L.size(); i++)
		for(int j = 0; j < L[i].size(); j++)
			if(i < L[i][j].first)
				cout << "(" << i << "," << L[i][j].first << ")" << ": " << L[i][j].second << endl;
}

/****************************************/
/* Objectif : Affichage des valeurs des plus courts
chemins trouvés avec l'algorithme de dijkstra avec tas
/****************************************/
void graphe::resultats(int s)
{
	cout << "La valeur du plus court chemin de " << s << " à : " << endl ;
	for(int i = 0; i < n; i++)
		cout << "-> " <<  i << " est égale à " << d[i] << endl;
}

/****************************************/
/* Objectif : Calcul de tous les plus courts de s aux 
autres sommets. A la fin de la méthode :
-> le tableau d doit contenir la valeur des plus courts chemins
/****************************************/
void graphe::dijkstra(int s)
{
    // !!! TODO !!! //
    int i = 0;
    for(; i < this->n - 1 ; ++i)
        this->T[i] = i + 1;
    for(i = 0 ; i < this->n ; ++i)
        this->I[i] = i - 1;
    this->d[0] = 0;
    for(i = 1 ; i < this->n ; ++i)
        this->d[i] = infini;
    /* tableau pere initialisé à 0 par défaut en c++ */
    int j = 0;  //noeud pivot initial

    for(int l = 1 ; l < this->n  ; ++l)
    {
        // pour chaque voisin de j..
        for(sommetadjacent s : this->L[j])
        {
            int i = s.first, cji = s.second;
            int sum = this->d[j] + cji;

            if(this->I[i] > -1 && sum < this->d[i])
            { 
                this->d[i] = sum;
                this->pere[i] = j;

                //reorganisation du tas T [ à partir de l'indice I[i] ]
                std::make_heap(this->T, this->T + this->n - l, Cmp(this->d));

            }
        }

        //Recherche dans T, de l'indice j de plus petite valeur d[i]
        j = this->T[0]; //sinon j reste à 0

        //Suppression de l'indice j du tas (premier element tas)
        std::pop_heap(this->T, this->T + this->n - l, Cmp(this->d));

        //Mettre a jour I en fonction du nouveau T
        this->I[j] = -1;    //j est la valeur du noeud qui a disparue du tas
        for(int z = 0 ; z < this->n - 1 - l ; ++z)
            this->I[this->T[z]] = z;
    }  
}
