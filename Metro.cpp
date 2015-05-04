/**
 * \file Metro.cpp
 * \brief Implémentation de la classe Metro.
 * \author Mon étudiant préféré
 * \version 0.1
 * \date ????
 *
 *  Travail pratique numéro 2
 *
 */

#include "Metro.h"

using namespace std;


// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Constructeur et destructeur
// _____________________________________________________________________________________________

/**
 * \fn			Metro::Metro(std::ifstream &f) throw(std::logic_error, std::bad_alloc)
 * \param[in]	f un flux sur un fichier d'entrÈe
 */
Metro::Metro(std::ifstream &f)
{
	if (f  == NULL) throw std::logic_error("Metro: fichier non ouvert\n");

	//Lecture Ligne Entete
	int nbStations, noStation, nbLiens;
	f >> nbStations >> nbLiens;

	//std::cout << nbStations << " " << nbLiens << std::endl;

	//Lecture du nom des stations
	std:: vector<std::string> l; //Pour le nom des stations
	l.resize(nbStations);
	char buff[255];

	for(int i =0; i< nbStations ; i++)
	{

		f >> noStation;
		f.ignore();
		f.getline(buff, 100);
		std:: string s(buff);
		l[i] = s;
	}

	//Lecture des coordonnées et placement des sommets en mémoire
	std::string ligne;
	if(getline(f,ligne))
	{
		//std::cout << ligne << endl; //ignorer ce séparateur
	}
	else
	{
		throw logic_error("Metro::Metro : $ is expected");
	}
	for(int i=0; i <nbStations; ++i)
	{
		if(getline(f,ligne))
			{
				int aInt, x, y;
				std::stringstream ss(ligne);
				ss >> aInt >> x >> y;
				if (aInt!=i)
				{
					throw logic_error("Metro::Metro : aInt==i expected");
				}
				//std::cout << i << " " << x << " " << y << std::endl;
				unGraphe.ajouterSommet(i, l[i], x, y);
			}
			else
			{
				throw logic_error("Metro::Metro : text line is expected");
			}
	}

	//Lecture des arc et placement des arcs dans le graphe en mémoire
	f.ignore();f.ignore();
	int s1, s2, l_cout;
	//std::cout << "$" << endl;
	for (int i =0; i<nbLiens; i++)
	{
		f >> s1 >> s2 >> l_cout;
		unGraphe.ajouterArc(s1, s2, l_cout);
		//std::cout << s1 << " " << s2 << " " << l_cout << std::endl;
	}
}

/**
* \fn		Metro::Metro(const Metro &source) throw (std::bad_alloc)
*
* \param[in]	source	La source du mÈtro ‡ copier
*/
Metro::Metro(const Metro &source)
{
	this->unGraphe = source.unGraphe;
}




// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Affichage
// _____________________________________________________________________________________________

/**
 * \fn			void Metro::afficherStations()
 */
void Metro::afficherStations() const
{
	std::vector<std::string> stations = this->unGraphe.listerNomsSommets();
	afficherStations(stations);
}

/**
 * \fn			void Metro::afficherStations(std::vector<std::string>&v)
 * \param[in]	v un vector de string
 */
void Metro::afficherStations(const std::vector<std::string> & v) const
{
	if (v.size() == 0)
	{
		std::cout << "La liste est vide\n";
		return;
	}

	for (unsigned int i=v.size(); i>0; i--)
 	{
	 	std::cout << v[i-1] << std::endl;
 	}
}


// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Algorithme
// _____________________________________________________________________________________________

/**
* \fn			std::vector<std::string> Metro::dijkstra(const int & origine, const int & destination, int & nbSec);
* \param[in]	origine Le numéro du sommet d'origine
* \param[in]	destination Le numéro du sommet de destination
* \param[out]	nbSec Le temps de parcours du trajet
*/
std::vector<std::string> Metro::dijkstra(const int & origine, const int & destination, int & nbSec)
{
	this->unGraphe.remiseAZero();
	std::vector<std::string>					stations;		// La valeur de retour
	std::vector<std::pair<int,int> >	tasActuel;
	std::vector<int>									sommetAdj;
	int																nouveauPoids = 0;

	this->unGraphe.asgDistance(origine, 0);
	tasActuel.push_back(std::pair<int,int>(origine, 0));

	if (origine == destination)
		nbSec = 0;

	while (!tasActuel.empty())
	{
		int 	sommetActuel = tasActuel.back().first;
		tasActuel.pop_back();
		if (sommetActuel == destination)
			break;
		this->unGraphe.asgAnalyse(sommetActuel);																// marque comme analysé
		sommetAdj = this->unGraphe.reqDestArc(sommetActuel);	//liste des sommets dont les arcs ont pour origine le sommet actuel
		for (std::vector<int>::iterator it = sommetAdj.begin() ; it != sommetAdj.end() ; ++it)
		{
			if (this->unGraphe.estAnalyse(*it) == false)
			{
				nouveauPoids = this->unGraphe.reqDistance(sommetActuel) + this->unGraphe.getCoutArc(sommetActuel, *it);
				if (this->unGraphe.reqDistance(*it) == -1 || nouveauPoids < this->unGraphe.reqDistance(*it))
				{
					this->unGraphe.asgDistance(*it, nouveauPoids);
					this->unGraphe.asgPredecesseur(*it, sommetActuel);
					tasActuel.push_back(std::pair<int,int>(*it, this->unGraphe.reqDistance(*it)));
					std::sort (tasActuel.begin(), tasActuel.end(), Graphe::triTas);
				}
			}
		}
	}

	if (this->unGraphe.sommetExiste(destination) && this->unGraphe.reqPredecesseur(destination) != -1 && this->unGraphe.reqDistance(destination) != -1)
	{
		nbSec = this->unGraphe.reqDistance(destination);
		int sommetActuel = destination;
		while (sommetActuel != -1)
		{
			stations.push_back(this->unGraphe.getNomSommet(sommetActuel));
			if ((sommetActuel = this->unGraphe.reqPredecesseur(sommetActuel)) != -1)
				nbSec += 20;
		}
	}
	return stations;
}

/**
* \fn			std::vector<std::string> Metro::bellManFord(const int & origine, const int & destination, int & nbSec);
* \param[in]	origine Le numéro du sommet d'origine
* \param[in]	destination Le numéro du sommet de destination
* \param[out]	nbSec Le temps de parcours du trajet
*/
std::vector<std::string> Metro::bellManFord(const int & origine, const int & destination, int & nbSec)
{
	this->unGraphe.remiseAZero();

	// La valeur de retour
	std::vector<std::string>	stations;

	this->unGraphe.asgDistance(origine, 0);
	this->unGraphe.asgNouveaupoids(origine, 0);

	if (origine == destination)
		nbSec = 0;
	else
	{
		for (int i = 1 ; i < this->unGraphe.nombreSommets() - 1 ; i++)
		{
			bool poidsInchanges = true;
			std::vector<int>	liste = this->unGraphe.listerSommets();
			for (std::vector<int>::iterator sommet = liste.begin() ; sommet != liste.end() ; ++sommet)
			{
				if (this->unGraphe.reqNouveaupoids(*sommet) == i - 1)
				{
					std::vector<int>	sommetAdj = this->unGraphe.reqDestArc(*sommet);	//liste des sommets dont les arcs ont pour origine le sommet actuel
					for (std::vector<int>::iterator it = sommetAdj.begin() ; it != sommetAdj.end() ; ++it)
					{
						int nouveau_poids = this->unGraphe.reqDistance(*sommet) + this->unGraphe.getCoutArc(*sommet, *it);
						if (nouveau_poids < this->unGraphe.reqDistance(*it) || this->unGraphe.reqDistance(*it) == -1)
						{
							this->unGraphe.asgPredecesseur(*it, *sommet);
							this->unGraphe.asgDistance(*it, nouveau_poids);
							this->unGraphe.asgNouveaupoids(*it, i);
							poidsInchanges = false;
						}
					}
				}
			}
			if (poidsInchanges == true)
				break;
		}
	}

	if (this->unGraphe.sommetExiste(destination) && this->unGraphe.reqPredecesseur(destination) != -1 && this->unGraphe.reqDistance(destination) != -1)
	{
		nbSec = this->unGraphe.reqDistance(destination);
		int sommetActuel = destination;
		while (sommetActuel != -1)
		{
			stations.push_back(this->unGraphe.getNomSommet(sommetActuel));
			if ((sommetActuel = this->unGraphe.reqPredecesseur(sommetActuel)) != -1)
				nbSec += 20;
		}
	}
	return stations;
}

/**
* \fn			std::vector<std::string> Metro::tempsDijkstra();
*/
void Metro::tempsDijkstra()
{
	clock_t t;
	int 		nbSec;
	int 		iteration = 0, nbSommets = 0;
	std::vector<int>	liste = this->unGraphe.listerSommets();
	t = clock();
	for (std::vector<int>::iterator origine = liste.begin() ; origine != liste.end() ; ++origine)
	{
		nbSommets = 0;	// Parce que parcourir toutes les paires prendraient plus de 7 minutes, je réduis à 376 * 100 iterations, ce qui est suffisant pour une bonne moyenne
		for (std::vector<int>::iterator destination = liste.begin() ; destination != liste.end() && nbSommets < 100  ; ++destination, ++nbSommets)
		{
			nbSec = -1;
			++iteration;
			this->dijkstra(*origine, *destination, nbSec);
		}
	}
	t = clock() - t;
	if (iteration != 0)
		std::cout << "Temps d'exécution de l'algorithme de Dijkstra = " << (((float)t)/CLOCKS_PER_SEC) / iteration << " secondes" << std::endl;
}

/**
* \fn			std::vector<std::string> Metro::tempsFord();
*/
void Metro::tempsFord()
{
	clock_t t;
	int 		nbSec;
	int 		iteration = 0, nbSommet = 0;
	std::vector<int>	liste = this->unGraphe.listerSommets();
	t = clock();
	for (std::vector<int>::iterator origine = liste.begin() ; origine != liste.end() ; ++origine)
	{
		nbSommet = 0;	// Parce que parcourir toutes les paires prendraient plus de 30 minutes, je réduis à 376 * 40 iterations, ce qui est suffisant pour une bonne moyenne
		for (std::vector<int>::iterator destination = liste.begin() ; destination != liste.end() && nbSommet < 40 ; ++destination, ++nbSommet)
		{
			nbSec = -1;
			++iteration;
			this->bellManFord(*origine, *destination, nbSec);
		}
	}
	t = clock() - t;
	if (iteration != 0)
		std::cout << "Temps d'exécution de l'algorithme de Bellman Ford = " << (((float)t)/CLOCKS_PER_SEC) / iteration << " secondes" << std::endl;
}
