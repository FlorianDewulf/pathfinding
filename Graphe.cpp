/*!
 * \brief   Implémentation d'un graphe orienté où chaque sommet a un numéro et un nom
 * \author  Mon étudiant préféré
 * \version 1.0
 * \date    ????
 */


#include "Graphe.h"

using namespace std;


// --------------------------------------------------------------------------------------------
//	Classe interne, Sommet : Constructeur et destructeur
// _____________________________________________________________________________________________


/**
* \fn		Graphe::Sommet::Sommet(int numero, const std::string & nom, int x, int y)
*
* \param[in]	numero	Le numéro du sommet
* \param[in]	nom		Le nom d'un sommet
* \param[in]	x		Position du sommet en x
* \param[in]	y		Position du sommet en y
*/
Graphe::Sommet::Sommet(int numero, const std::string & nom, int x, int y)
{
	this->numero = numero;
	this->nom = nom;
	this->coord.x = x;
	this->coord.y = y;
	this->m_cout = -1;
	this->listeDest = 0;
	this->etat = false;
	this->precedent = 0;
	this->suivant = 0;
	this->predecesseur = 0;
	this->m_estAnalyse = false;
	this->m_nouveauPoids = -1;
}

/**
* \fn		Graphe::Sommet::Sommet(Sommet * source)
*
* \param[in]	source	La source sommet à copier
*/
Graphe::Sommet::Sommet(Sommet * source)
{
	this->numero = source->numero;
	this->nom = source->nom;
	this->coord.x = source->coord.x;
	this->coord.y = source->coord.y;
	this->m_cout = -1;
	this->listeDest = 0;
	this->etat = false;
	this->precedent = 0;
	this->suivant = 0;
	this->predecesseur = 0;
	this->m_estAnalyse = false;
	this->m_nouveauPoids = -1;
}

/**
* \fn		Graphe::Sommet::~Sommet()
*/
Graphe::Sommet::~Sommet()
{
	Arc * arcASupprimer = this->listeDest;

	while (arcASupprimer != 0)
	{
		this->listeDest = this->listeDest->suivDest;
		delete arcASupprimer;
		arcASupprimer = this->listeDest;
	}

	// Assertion : arcASupprimer et listeDest sont nuls

	this->precedent = 0;
	this->suivant = 0;
}


// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Classe interne, Arc : Constructeur et destructeur
// _____________________________________________________________________________________________


/**
* \fn		Graphe::Arc::Arc(Sommet * dest, int cout)
*
* \param[in]	dest	L'adresse du sommet de destination
* \param[in]	cout	Le cout de l'arc
*/
Graphe::Arc::Arc(Sommet * dest, int cout)
{
	this->dest = dest;
	this->m_cout = cout;
	this->suivDest = 0;
}

/**
* \fn		Graphe::Arc::~Arc()
*/
Graphe::Arc::~Arc()
{
	this->dest = 0;
	this->suivDest = 0;
}





// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Constructeurs et destructeur
// _____________________________________________________________________________________________

/**
* \fn		Graphe::Graphe()
*/
Graphe::Graphe()
{
	this->listSommets = 0;
	this->nbSommets = 0;
}

/**
* \fn		Graphe::~Graphe()
*/
Graphe::~Graphe()
{
	_detruireGraphe();
}

/**
* \fn		Graphe::Graphe(const Graphe & source) throw (std::bad_alloc)
*
* \param[in]	source	La source du graphe à copier
*/
Graphe::Graphe(const Graphe & source)
{
	_copierGraphe(source);
}





// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Ajout et suppression de sommets et d'arcs
// _____________________________________________________________________________________________


/**
* \fn		void Graphe::ajouterSommet(int numero, const std::string & nom, int x, int y) throw(std::bad_alloc, std::logic_error)
*
* \param[in]	numero	Le numero du sommet à ajouter
* \param[in]	nom		Le nom du sommet à ajouter
* \param[in]	x		Position du sommet en x
* \param[in]	y		Position du sommet en y
*/
void Graphe::ajouterSommet(int numero, const std::string & nom, int x, int y)
{
	if(this->listSommets == 0)
	{
		this->listSommets = new Sommet(numero, nom, x, y);
	}
	else
	{
		// Assertion : il y a au moins un sommet dans le graphe

		Sommet * dernierSommet = this->listSommets;

		// Assertion : dernierSommet n'est pas nul

		while (dernierSommet->suivant != 0)
		{
			if (dernierSommet->numero == numero)
				throw logic_error("Graphe::ajouterSommet : Le sommet existe déjà");

			dernierSommet = dernierSommet->suivant;
		}

		// Assertion : dernierSommet contient le dernier sommet de la liste du graphe
		// Assertion : les sommets qui PRÉCéDENT le sommet dernierSommet n'ont pas le même numéro que le sommet à créer

		if (dernierSommet->numero == numero)
			throw logic_error("Graphe::ajouterSommet : Le sommet existe déjà");

		dernierSommet->suivant = new Sommet(numero, nom, x, y);
		dernierSommet->suivant->precedent = dernierSommet;
	}

	this->nbSommets++;
}

/**
* \fn		void Graphe::enleverSommet(int numero) throw(std::logic_error)
*
* \param[in]	numero	Le numéro du sommet à enlever
*/
void Graphe::enleverSommet(int numero)
{
	Sommet * sommetAEnlever = _getSommet(numero);

	if (sommetAEnlever == 0)
		throw logic_error("Graphe::enleverSommet : Le sommet n'existe pas");

	this->_enleverSommet(sommetAEnlever);
}

/**
* \fn		void Graphe::enleverSommets(const std::string& nom) throw(std::logic_error)
*
* \param[in]	nom		Le nom des sommets à enlever
*/
void Graphe::enleverSommets(const std::string& nom)
{
	if (this->listSommets == 0)
		throw logic_error("Graphe::enleverSommets : Aucun sommet dans le graphe");

	Sommet * sommetActuel = this->listSommets;
	Sommet * sommetAEnlever = 0;

	while (sommetActuel != 0)
	{
		if (sommetActuel->nom == nom)
		{
			sommetAEnlever = sommetActuel;
			sommetActuel = sommetActuel->suivant;
			this->_enleverSommet(sommetAEnlever);
		}
		else
		{
			sommetActuel = sommetActuel->suivant;
		}
	}
}

/**
* \fn		void Graphe::ajouterArc(int numOrigine, int numDestination, int cout) throw(std::bad_alloc, std::logic_error)
*
* \param[in]	numOrigine		Le numéro du sommet d'origine
* \param[in]	numDestination	Le numéro du sommet de destination
*/
void Graphe::ajouterArc(int numOrigine, int numDestination, int cout)
{
	Sommet * origine = _getSommet(numOrigine);
	Sommet * destination = _getSommet(numDestination);

	if (origine == 0 || destination == 0)
		throw logic_error("Graphe::ajouterArc : L'un des sommets n'existe pas");

	// Assertion : origine et destination ne sont pas nuls

	if (origine->listeDest == 0)
	{
		origine->listeDest = new Arc(destination, cout);
	}
	else
	{
		// Assertion : il y a au moins un arc dans la liste

		Arc * arcActuel = origine->listeDest;

		// Assertion : arcActuel n'est pas nul

		while (arcActuel->suivDest != 0)
		{
			// Assertion : arcActuel n'est pas le dernier arc

			if (arcActuel->dest == destination)
				throw logic_error("L'arc existe déjà");

			arcActuel = arcActuel->suivDest;

			// Assertion : arcActuel n'est pas nul
		}

		// Assertion : arcActuel est le dernier arc de la liste
		// Assertion : les arcs qui PRÉCéDENT arcActuel n'ont pas le sommet 'destination' comme destination

		if (arcActuel->dest == destination)
			throw logic_error("L'arc existe déjà");

		arcActuel->suivDest = new Arc(destination, cout);
	}
}

/**
* \fn		void Graphe::enleverArc(int numOrigine, int numDestination) throw(std::logic_error)
*
* \param[in]	numOrigine		Le numéro du sommet d'origine
* \param[in]	numDestination	Le numéro du sommet de destination
*/
void Graphe::enleverArc(int numOrigine, int numDestination)
{
	Sommet * origine = _getSommet(numOrigine);
	Sommet * destination = _getSommet(numDestination);

	if (origine == 0 || destination == 0)
		throw logic_error("L'un des sommets n'existe pas.");

	// Assertion : origine et destination ne sont pas nuls

	Arc * arcPrecedent = 0;
	Arc * arcActuel = origine->listeDest;

	// Ici on passe à travers les arcs du sommet 'origine' jusqu'à ce qu'on trouve un
	// arc qui pointe vers le sommet 'destination' ou qu'on arrive au dernier sommet

	while (arcActuel != 0 && arcActuel->dest != destination)
	{
		arcPrecedent = arcActuel;
		arcActuel = arcActuel->suivDest;
	}

	// Assertion : s'il n'y a pas d'arc entre le sommet 'origine' et le sommet 'destination', arcActuel est nul

	if (arcActuel == 0)
		throw logic_error("L'arc n'existe pas... liste vide.");

	// Assertion : il y a un arc entre le sommet 'origine' et le sommet 'destination'
	// Assertion : arcActuel pointe sur l'arc entre le sommet 'origine' et le sommet 'destination'
	// Assertion : arcPrecedent pointe sur l'arc avant arcActuel

	if (arcPrecedent == 0) // Si on supprime le premier arc de la liste
		origine->listeDest = origine->listeDest->suivDest;
	else
		arcPrecedent->suivDest = arcActuel->suivDest;

	delete arcActuel;
}





// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Données sur le graphe (incluant listes de sommets)
// _____________________________________________________________________________________________

/**
* \fn		int Graphe::nombreSommets() const
*
* \return	Le nombre de sommets dans le graphe
*/
int Graphe::nombreSommets() const
{
	return this->nbSommets;
}

/**
* \fn		bool Graphe::estVide() const
*
* \return	True si le graphe ne contient pas de sommets, false autrement
*/
bool Graphe::estVide() const
{
	return this->nbSommets == 0;
}

/**
* \fn		std::vector<int> Graphe::listerSommets() const
*
*
* \return	Un vector de type int qui contient une liste de tous les numéros des sommets
*/
std::vector<int> Graphe::listerSommets() const
{
	std::vector<int> sommets;

	sommets.reserve(this->nbSommets);

	for (Sommet * sommetActuel = this->listSommets ; sommetActuel != 0 ; sommetActuel = sommetActuel->suivant)
		sommets.push_back(sommetActuel->numero);

	return sommets;
}

/**
* \fn		std::vector<std::string> Graphe::listerNomsSommets() const
*
* \return	Un vector de type string qui contient une liste de tous les noms des sommets
*/
std::vector<std::string> Graphe::listerNomsSommets() const
{
	std::vector<std::string> sommets;

	sommets.reserve(this->nbSommets);

	for (Sommet * sommetActuel = this->listSommets ; sommetActuel != 0 ; sommetActuel = sommetActuel->suivant)
		sommets.push_back(sommetActuel->nom);

	return sommets;
}



// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Trouver numéro sommet
// _____________________________________________________________________________________________


/**
* \fn		int Graphe::getNumeroSommet(const std::string& nom) const throw(std::logic_error)
*
* \param[in]	nom		Le nom d'un sommet
*
* \return	Le numéro du sommet recherché
*/
int Graphe::getNumeroSommet(const std::string& nom) const
{
	for (Sommet * sommetActuel = this->listSommets ; sommetActuel != 0 ; sommetActuel = sommetActuel->suivant)
	{
		if (sommetActuel->nom == nom)
			return sommetActuel->numero;
	}

	throw logic_error("Aucun sommet avec ce nom.");
}

/**
* \fn		int Graphe::getNumeroSommet(int x, int y) const throw(std::logic_error)
*
* \param[in]	x		Position du sommet en x
* \param[in]	y		Position du sommet en y
*
* \return	Le numéro du sommet recherché
*/
int Graphe::getNumeroSommet(int x, int y) const
{
	for (Sommet * sommetActuel = this->listSommets ; sommetActuel != 0 ; sommetActuel = sommetActuel->suivant)
	{
		if (sommetActuel->coord.x == x && sommetActuel->coord.y == y)
			return sommetActuel->numero;
	}

	throw logic_error("Aucun sommet à ces coordonnées.");
}





// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Données à propos des sommets
// _____________________________________________________________________________________________


/**
* \fn		bool Graphe::sommetExiste(int numero) const
*
* \param[in]	numero	Le numéro du sommet
*
* \return	True si le sommet existe, false autrement
*/
bool Graphe::sommetExiste(int numero) const
{
	return _getSommet(numero) != 0;
}

/**
* \fn		std::string Graphe::getNomSommet(int numero) const throw(std::logic_error)
*
* \param[in]	numero	Le numéro du sommet
*
* \return	Le nom du sommet recherché
*/
std::string Graphe::getNomSommet(int numero) const
{
	Sommet * sommet = _getSommet(numero);

	if (sommet == 0)
		throw logic_error("Graphe::getNomSommet : Le sommet n'existe pas");

	return sommet->nom;
}

/**
* \fn		void Graphe::getCoordonnesSommet(int numero, int& x, int& y) const throw(std::logic_error)
*
* \param[in]	numero	Le numéro du sommet
* \param[out]	x		Position du sommet en x
* \param[out]	y		Position du sommet en y
*/
void Graphe::getCoordonnesSommet(int numero, int& x, int& y) const
{
	Sommet * sommet = _getSommet(numero);

	if (sommet == 0)
		throw logic_error("Graphe::getCoordonnesSommet : Le sommet n'existe pas");

	x = sommet->coord.x;
	y = sommet->coord.y;
}

/**
* \fn		int Graphe::ordreSortieSommet(int numero) const throw(std::logic_error)
*
* \param[in]	numero	Le numéro du sommet
*
* \return	L'odre de sortie d'un sommet
*/
int Graphe::ordreSortieSommet(int numero) const
{
	Sommet * sommet = _getSommet(numero);

	if (sommet == 0)
		throw logic_error("Graphe::ordreSortieSommet : Le sommet n'existe pas");

	int ordreSortie = 0;

	for (Arc * arcActuel = sommet->listeDest ; arcActuel != 0 ; arcActuel = arcActuel->suivDest)
		ordreSortie++;

	return ordreSortie;
}

/**
* \fn		int Graphe::ordreEntreeSommet(int numero) const throw(std::logic_error)
*
* \param[in]	numero	Le numéro du sommet
*
* \return	L'odre d'entrée d'un sommet
*/
int Graphe::ordreEntreeSommet(int numero) const
{
	Sommet * sommet = _getSommet(numero);

	if (sommet == 0)
		throw logic_error("Graphe::ordreEntreeSommet : Le sommet n'existe pas");

	int ordreEntree = 0;

	for (Sommet * sommetActuel = this->listSommets ; sommetActuel != 0 ; sommetActuel = sommetActuel->suivant)
	{
		if (_getArc(sommetActuel, sommet) != 0)
			ordreEntree++;
	}

	return ordreEntree;
}

/**
* \fn		std::vector<int> Graphe::listerSommetsAdjacents(int numero) const throw(std::logic_error)
*
* \param[in]	numero	Le numéro du sommet
*
* \return	Un vector de type int qui contient une liste de tous les numéros des sommets adjacents au sommet demandé
*/
std::vector<int> Graphe::listerSommetsAdjacents(int numero) const
{
	Sommet * sommet = _getSommet(numero);

	if (sommet == 0)
		throw logic_error("Graphe::listerSommetsAdjacents : Le sommet n'existe pas");

	std::vector<int> sommets;


	// -- étape 1 : Marquer tous les sommets comme n'ayant pas été placés dans le tableau ---------------------------

	for (Sommet * sommetActuel = this->listSommets ; sommetActuel != 0 ; sommetActuel = sommetActuel->suivant)
		sommetActuel->etat = false;


	// -- étape 2 : Marquer le sommet en question comme "ne pas ajouter" --------------------------------------------

	sommet->etat = true;


	// -- étape 3 : Arcs sortants -----------------------------------------------------------------------------------

	for (Arc * arcActuel = sommet->listeDest ; arcActuel != 0 ; arcActuel = arcActuel->suivDest)
	{
		if (arcActuel->dest->etat) // Vérifier que le sommet n'est pas marqué comme "ne pas ajouter"
		{
			arcActuel->dest->etat = true; // Marquer le sommet comme "ne pas ajouter"
			sommets.push_back(arcActuel->dest->numero);
		}
	}


	// -- …tape 4 : Arcs entrants -----------------------------------------------------------------------------------

	for (Sommet * sommetActuel = this->listSommets ; sommetActuel != 0 ; sommetActuel = sommetActuel->suivant)
	{
		// Si le sommet actuel n'est pas marqué comme "ne pas ajouter"
		// ET qu'il y a un arc du sommet actuel vers le sommet en question...
		if (!sommetActuel->etat == false && _getArc(sommetActuel, sommet) != 0)
			sommets.push_back(sommetActuel->numero);
	}

	return sommets;
}

// --------------------------------

/**
* \fn		bool	Graphe::estAnalyse(int p_numero) throw(std::logic_error)
*
* \param[in]	p_numero	Le numéro du sommet
*
* \return	L'attribut 'm_estAnalyse' du sommet demandé
*/
bool	Graphe::estAnalyse(int p_numero)
{
	Sommet	*sommet;

	if ((sommet = this->_getSommet(p_numero)) == 0)
		throw logic_error("Graphe::estAnalyse : Le sommet n'existe pas");
	return sommet->m_estAnalyse;
}

/**
* \fn		void	asgAnalyse(int p_numero) throw(std::logic_error)
*
* \param[in]	p_numero	Le numéro du sommet
*/
void	Graphe::asgAnalyse(int p_numero)
{
	Sommet	*sommet;

	if ((sommet = this->_getSommet(p_numero)) == 0)
		throw logic_error("Graphe::asgAnalyse : Le sommet n'existe pas");
	sommet->m_estAnalyse = true;
}

/**
* \fn		void	Graphe::asgDistance(int p_numero, const int &p_valeur) throw(std::logic_error)
*
* \param[in]	p_numero	Le numéro du sommet
* \param[in]	p_valeur	La distance à assigner
*/
void	Graphe::asgDistance(int p_numero, const int &p_valeur)
{
	Sommet	*sommet;

		if ((sommet = this->_getSommet(p_numero)) == 0)
			throw logic_error("Graphe::asgDistance : Le sommet n'existe pas");
		sommet->m_cout = p_valeur;
}

/**
* \fn		int	Graphe::reqDistance(int p_numero) throw(std::logic_error)
*
* \param[in]	p_numero	Le numéro du sommet
*
* \return	La distance du sommet demandé
*/
int		Graphe::reqDistance(int p_numero)
{
	Sommet	*sommet;

	if ((sommet = this->_getSommet(p_numero)) == 0)
		throw logic_error("Graphe::reqDistance : Le sommet n'existe pas");
	return sommet->m_cout;
}

/**
* \fn		void	Graphe::asgNouveaupoids(int p_numero, const int &p_valeur) throw(std::logic_error)
*
* \param[in]	p_numero	Le numéro du sommet
* \param[in]	p_valeur	La valeur à assigner
*/
void	Graphe::asgNouveaupoids(int p_numero, const int &p_valeur)
{
	Sommet	*sommet;

		if ((sommet = this->_getSommet(p_numero)) == 0)
			throw logic_error("Graphe::asgNouveaupoids : Le sommet n'existe pas");
		sommet->m_nouveauPoids = p_valeur;
}

/**
* \fn		int	Graphe::reqNouveaupoids(int p_numero) throw(std::logic_error)
*
* \param[in]	p_numero	Le numéro du sommet
*
* \return	La valeur nouveauPoids du sommet en paramètre
*/
int		Graphe::reqNouveaupoids(int p_numero)
{
	Sommet	*sommet;

	if ((sommet = this->_getSommet(p_numero)) == 0)
		throw logic_error("Graphe::reqNouveaupoids : Le sommet n'existe pas");
	return sommet->m_nouveauPoids;
}

/**
* \fn		void	Graphe::asgPredecesseur(int p_numero, const int &p_valeur) throw(std::logic_error)
*
* \param[in]	p_numero	Le numéro du sommet
* \param[in]	p_valeur	Le numéro du sommet du prédécesseur à assigner
*/
void	Graphe::asgPredecesseur(int p_numero, const int &p_valeur)
{
	Sommet	*sommet;
	Sommet 	*precedent;

	if ((sommet = this->_getSommet(p_numero)) == 0 || (precedent = this->_getSommet(p_valeur)) == 0)
		throw logic_error("Graphe::asgPredecesseur : Le sommet n'existe pas");
	sommet->predecesseur = precedent;
}

/**
* \fn		int	Graphe::reqPredecesseur(int p_numero) throw(std::logic_error)
*
* \param[in]	p_numero	Le numéro du sommet
*
* \return	Le numéro du prédécesseur
*/
int		Graphe::reqPredecesseur(int p_numero)
{
	Sommet	*sommet;

	if ((sommet = this->_getSommet(p_numero)) == 0)
		throw logic_error("Graphe::reqPredecesseur : Le sommet n'existe pas");
	return (sommet->predecesseur == 0) ? -1 : sommet->predecesseur->numero;
}

/**
* \fn		std::vector<int>	Graphe::reqDestArc(int p_numero) throw(std::logic_error)
*
* \param[in]	p_numero	Le numéro du sommet
*
* \return	Une liste d'arc dont le sommet en paramètre est la source
*/
std::vector<int>	Graphe::reqDestArc(int p_numero)
{
	Sommet	*sommet;
	std::vector<int>	liste;

	if ((sommet = this->_getSommet(p_numero)) == 0)
		throw logic_error("Graphe::reqDestArc : Le sommet n'existe pas");
	for (Arc *arcActuel = sommet->listeDest ; arcActuel != 0 ; arcActuel = arcActuel->suivDest)
		liste.push_back(arcActuel->dest->numero);
	return liste;
}

/**
* \fn		void Graphe::remiseAZero()
*/
void Graphe::remiseAZero()
{
	for (Sommet * sommetActuel = this->listSommets ; sommetActuel != 0 ; sommetActuel = sommetActuel->suivant)
	{
		sommetActuel->m_cout = -1;
		sommetActuel->predecesseur = 0;
		sommetActuel->m_estAnalyse = false;
		sommetActuel->m_nouveauPoids = -1;
	}
}

/**
* \fn		bool Graphe::triTas(const std::pair<int,int> &p_gauche, const std::pair<int,int> &p_droite)
*
* \param[in]	p_gauche	Le premier élément à comparer
* \param[in]	p_droite	Le deuxième élément à comparer
*
* \return	True si le premier est plus grand, false sinon
*/
bool Graphe::triTas(const std::pair<int,int> &p_gauche, const std::pair<int,int> &p_droite)
{
	int valG = std::numeric_limits<int>::max();
	int valD = std::numeric_limits<int>::max();

	if (p_gauche.second != -1)
		valG = p_gauche.second;
	if (p_droite.second != -1)
		valD = p_droite.second;

	if (valG > valD)
		return true;
	else
		return false;
}

// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Données à propos des arcs
// _____________________________________________________________________________________________


/**
* \fn		bool Graphe::arcExiste(int numOrigine, int numDestination) const throw(std::logic_error)
*
* \param[in]	numOrigine		Le numéro du sommet d'origine
* \param[in]	numDestination	Le numéro du sommet de destination
*
* \return	True si un arc existe entre les deux sommets, false autrement
*/
bool Graphe::arcExiste(int numOrigine, int numDestination) const
{
	Sommet * origine = _getSommet(numOrigine);
	Sommet * destination = _getSommet(numDestination);

	if (origine == 0 || destination == 0)
		throw logic_error("Graphe::arcExiste : L'un des sommets n'existe pas");

	// Assertion : origine et destination ne sont pas nuls

	for (Arc * arcActuel = origine->listeDest ; arcActuel != 0 ; arcActuel = arcActuel->suivDest)
	{
		if (arcActuel->dest == destination)
			return true;
	}

	return false;
}

/**
* \fn		int Graphe::getCoutArc(int numOrigine, int numDestination) const throw(std::logic_error)
*
* \param[in]	numOrigine		Le numéro du sommet d'origine
* \param[in]	numDestination	Le numéro du sommet de destination
*
* \return	Le co˚t de l'arc entre deux sommets
*/
int Graphe::getCoutArc(int numOrigine, int numDestination) const
{
	Sommet * origine = _getSommet(numOrigine);
	Sommet * destination = _getSommet(numDestination);

	if (origine == 0 || destination == 0)
		throw logic_error("Graphe::getCoutArc : L'un des sommets n'existe pas");

	// Assertion : origine et destination ne sont pas nuls

	for (Arc * arcActuel = origine->listeDest ; arcActuel != 0 ; arcActuel = arcActuel->suivDest)
	{
		if (arcActuel->dest == destination)
			return arcActuel->m_cout;
	}

	throw logic_error("L'arc n'existe pas, face de singe.");
}




// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Surcharge d'opérateurs
// _____________________________________________________________________________________________


/**
* \fn		Graphe& Graphe::operator = (const Graphe & source) throw (std::bad_alloc)
*
* \param[in]	source	Le graphe à copier
*/
Graphe& Graphe::operator = (const Graphe & source)
{
	_detruireGraphe();
	_copierGraphe(source);

	return (*this);
}

/**
* \fn std::ostream& operator<<(std::ostream& out, const Graphe& g)
*
* \param[in]	out		Le flux de sortie.
* \param[in]	g		Le graphe à† afficher.
*
* \return	Le flux de sortie.
*/
ostream& operator << (ostream& out, const Graphe& g)
{
	out << "Le graphe contient " << g.nbSommets << " sommet(s)" << std::endl;
	Graphe::Sommet* vertex = g.listSommets;

	while (vertex != NULL)
	{
		out << "Sommet no " << vertex->numero << ", nom : " << vertex->nom << std::endl;
		// Afficher les arcs.
		Graphe::Arc* arc = vertex->listeDest;
		if(arc!=NULL)
		{
			out << "Ce sommet a des liens vers le(s) sommet(s) : ";
			while (arc->suivDest != NULL)
			{
				out << arc->dest->numero << "(" << arc->m_cout << "), ";
				arc = arc->suivDest;
			}
			out << arc->dest->numero << "(" << arc->m_cout << ")";
		}
		out << std::endl << std::endl;
		vertex = vertex->suivant;
	}

	return out;
}


// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Méthodes auxiliaires (getters)
// _____________________________________________________________________________________________


/**
* \fn		Graphe::Sommet *Graphe::_getSommet(int numero) const
*
* \param[in]	numero	Le numéro du sommet
*/
Graphe::Sommet *Graphe::_getSommet(int numero) const
{
	for (Sommet * sommetActuel = this->listSommets ; sommetActuel != 0 ; sommetActuel = sommetActuel->suivant)
	{
		if (sommetActuel->numero == numero)
			return sommetActuel;
	}

	// Assertion : Aucun sommet n'a été trouvé avec le numéro 'numero'

	return 0;
}

/**
* \fn		Graphe::Arc * Graphe::_getArc(Sommet * origine, Sommet * destination) const
*
* \param[in]	origine			L'adresse du sommet d'origine
* \param[in]	destination		L'adresse du sommet de destination
*
* \return	L'adresse de l'arc trouvé entre le sommet d'origine et le sommet de destination
*/
Graphe::Arc * Graphe::_getArc(Sommet * origine, Sommet * destination) const
{
	for (Arc * arcActuel = origine->listeDest ; arcActuel != 0 ; arcActuel = arcActuel->suivDest)
	{
		if (arcActuel->dest == destination)
			return arcActuel;
	}

	// Assertion : Aucun arc n'a été trouvé avec la destination 'destination'

	return 0;
}









// ØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØØ
//	Méthodes auxiliaires (destruction + copie)
// _____________________________________________________________________________________________


/**
* \fn		void Graphe::_detruireGraphe()
*/
void Graphe::_detruireGraphe()
{
	Sommet * sommetASupprimer = this->listSommets;

	while (sommetASupprimer != 0)
	{
		this->listSommets = this->listSommets->suivant;
		delete sommetASupprimer;
		sommetASupprimer = this->listSommets;
	}

	this->nbSommets = 0;
}

/**
* \fn		void Graphe::_copierGraphe(const Graphe &source) throw(std::bad_alloc)
*
* \param[in]	source	Le graphe à copier
*/
void Graphe::_copierGraphe(const Graphe &source)
{
	if (source.listSommets != 0)
	{
		Sommet * derniereCopie = new Sommet(source.listSommets);
		Sommet * prochaineSourceCopie = source.listSommets->suivant;
		this->listSommets = derniereCopie;

		try
		{
			while (prochaineSourceCopie != 0)
			{
				derniereCopie->suivant = new Sommet(prochaineSourceCopie);
				derniereCopie->suivant->precedent = derniereCopie;

				derniereCopie = derniereCopie->suivant;
				prochaineSourceCopie = prochaineSourceCopie->suivant;
			}

			this->nbSommets = source.nbSommets;

			Sommet * sommetSource = source.listSommets;
			Sommet * sommetCopie = this->listSommets;
			Arc * arcSource;
			Arc * arcCopie;

			do // Passer les sommets
			{
				// Le premier arc à copier et le premier arc du sommet source actuel
				arcSource = sommetSource->listeDest;

				// S'il y a un premier arc
				if (arcSource != 0)
				{
					// Copier le premier arc
					sommetCopie->listeDest = new Arc(_getSommet(arcSource->dest->numero), arcSource->m_cout);

					// arcSource va maintenant pointer sur le prochain arc à copier
					arcSource = arcSource->suivDest;

					// arcCopie va maintenant pointer sur le dernier arc copié
					arcCopie = sommetCopie->listeDest;

					// Tant qu'il y a un autre arc à copier...
					while (arcSource != 0)
					{
						// Copier le prochain arc
						arcCopie->suivDest = new Arc(_getSommet(arcSource->dest->numero), arcSource->m_cout);

						// arcSource va maintenant pointer sur le prochain arc à copier
						arcSource = arcSource->suivDest;

						// arcCopie va maintenant pointer sur le dernier arc copié
						arcCopie = arcCopie->suivDest;
					}
				}

				sommetSource = sommetSource->suivant;
				sommetCopie = sommetCopie->suivant;
			}
			while(sommetSource != 0);
		}
		catch (std::bad_alloc & erreur)
		{
			_detruireGraphe();
			throw erreur;
		}
	}
}

/**
* \fn		void Graphe::_enleverSommet(Sommet * sommetAEnlever)
*
* \param[in]	sommetAEnlever	L'adresse du sommet à enlever
*/
void Graphe::_enleverSommet(Sommet * sommetAEnlever)
{
	Arc * arcPrecedent;

	for (Sommet * sommetActuel = this->listSommets ; sommetActuel != 0 ; sommetActuel = sommetActuel->suivant)
	{
		if (sommetActuel != sommetAEnlever)
		{
			arcPrecedent = 0;

			for (Arc * arcActuel = sommetActuel->listeDest ; arcActuel != 0 ; arcActuel = arcActuel->suivDest)
			{
				if (arcActuel->dest == sommetAEnlever)
				{
					if (arcActuel == sommetActuel->listeDest) // si c'est le premier de la liste
						sommetActuel->listeDest = arcActuel->suivDest;
					else
						arcPrecedent->suivDest = arcActuel->suivDest;

					delete arcActuel;
					break;
				}

				arcPrecedent = arcActuel;
			}
		}
	}

	if (sommetAEnlever->precedent == 0)
	{
		this->listSommets = sommetAEnlever->suivant;
	}
	else if (sommetAEnlever->suivant == 0)
	{
		sommetAEnlever->precedent->suivant = 0;
	}
	else
	{
		sommetAEnlever->suivant->precedent = sommetAEnlever->precedent;
		sommetAEnlever->precedent->suivant = sommetAEnlever->suivant;
	}

	delete sommetAEnlever;
	this->nbSommets--;
}
