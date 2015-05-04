/**
 * \file	Graphe.h
 * \brief	Définition de l'interface pour un graphe
 * \author	LeProf
 * \version	1.0
 * \date	????
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <queue>


#ifndef GRAPHE__H
#define GRAPHE__H

/**
* \class	Graphe
* \brief	Classe représentant un graphe orienté
*/
class Graphe
{
private:

	// -------------------------------------------------------------------------------------------------
	//	Classes internes
	// _________________________________________________________________________________________________


		class Arc;

		/**
		* \struct	Coordonnees
		* \brief	Structure représentant les coordonnées d'un sommet dans un graphe
		*/
		struct Coordonnees
		{
			int x;	/**< La position d'un sommet en x */
			int y;	/**< La position d'un sommet en y */
		};

		/**
		* \class	Sommet
		* \brief	Classe représentant un sommet dans un graphe
		*/
		class Sommet
		{
			public:

				/**
				 * \brief		Constructeur avec paramétres
				 * \post		Le sommet est initialisé avec les paramétres indiqués
				 */
				Sommet(int numero, const std::string & nom, int x, int y);

				/**
				 * \brief		Constructeur de copie
				 * \post		Une copie profonde du sommet a été effectuée à partir du sommet source
				 */
				Sommet(Sommet * source);

				/**
				 * \brief		Destructeur
				 * \post		Le sommet est détruit
				 */
				~Sommet();

				int numero;				/**< Le numéro d'une station */
				std::string nom;		/**< Le nom de la station */
				Coordonnees coord;		/**< Les coordonnées de la station */

				Arc * listeDest;		/**< La liste des arcs entre la station et les stations qui lui sont adjacentes */

				bool etat;			    /**< Pour marquer une station */
				Sommet * predecesseur;	/**< Pour Dijkstra, Bellman-Ford */
				int m_cout;				/**< Pour Dijkstra, Bellman-Ford */
				bool m_estAnalyse;				/**< Pour Dijkstra */
				int m_nouveauPoids;				/**< Pour Bellman-Ford */

				Sommet *precedent;		/**< La station précédente dans la liste des stations */
				Sommet *suivant;		/**< La prochaine station dans la liste */
		};

		/**
		* \class	Arc
		* \brief	Classe représentant un arc entre deux sommets dans le graphe
		*/
		class Arc
		{
			public:

				/**
				 * \brief		Constructeur avec paramétres
				 * \post		Un arc est créé avec les paramétres indiqués
				 */
				Arc(Sommet * dest, int cout);

				/**
				 * \brief		Destructeur
				 * \post		Un arc est détruit
				 */
				~Arc();

				Sommet * dest;		/**< Un pointeur sur la station de destination */
				int m_cout;			/**< Le cout du trajet (durée en seconde) entre les deux stations reliées par l'arc */
				Arc *  suivDest;	/**< La prochaine station de destination */
		};



	public:


	// -------------------------------------------------------------------------------------------------
	//	Constructeurs et destructeur
	// _________________________________________________________________________________________________

		/**
		 * \brief		Constructeur sans paramètre
		 * \post		Un graphe vide est créé
		 */
		Graphe();

		/**
		 * \brief		Constructeur de copie
		 * \exception	bad_alloc	Il n'y a pas assez de mémoire pour copier le graphe 'source'
		 */
		Graphe(const Graphe &source);

		/**
		 * \brief		Destructeur
		 * \post		Le graphe est détruit
		 */
		~Graphe();





	// ------------------------------------------------------------------------------------------------
	//	Ajout/suppression de sommets et d'arcs
	// _________________________________________________________________________________________________

		/**
		 * \brief		Ajout d'un sommet au graphe
		 * \pre			Il n'y a aucun sommet avec le même numéro dans le graphe
		 * \post		Un sommet contenant les informations passées en argument a été ajouté au Graphe
		 * \exception	bad_alloc		Il n'y a pas assez de mémoire pour ajouter un sommet
		 * \exception	logic_error		Il y a un sommet avec le méme numéro dans le graphe
		 */
		void ajouterSommet(int numero, const std::string& nom, int x, int y);

		/**
		 * \brief		Enléve un sommet du graphe ainsi que tous les arcs qui vont et partent de ce sommet
		 * \pre			Il y a un sommet avec le numéro 'numero' dans le graphe
		 * \post		Le sommet identifié par 'numero' a été enlevé du graphe
		 * \exception	logic_error		Il n'y a aucun sommet avec le numéro 'numero' dans le graphe
		 */
		void enleverSommet(int numero);

		/**
		 * \brief		Enlève tous les sommet du graphe qui ont le nom 'nom' ainsi que tous les arcs
		 *				qui venaient ou partaient de ces sommets
		 * \pre			Il y a au moins un sommet dans le graphe
		 * \post		Le(s) sommet(s) identifié(s) par 'nom' a été enlevé du graphe
		 * \exception	logic_error		Il n'y a pas de sommets dans le graphe
		 */
		void enleverSommets(const std::string& nom);

		/**
		 * \brief		Ajout d'un arc au graphe
		 * \pre			Les deux sommets doivent exister
		 * \post		Un arc a été ajouté entre le sommet 'numeroOrigine' et le sommet 'numeroDestination'
		 * \exception	bad_alloc		Pas assez de mémoire pour alloué le nouvel arc
		 * \exception	logic_error		Un des deux sommets n'existe pas
		 * \exception	logic_error		Il y a déjé un arc orienté entre ces deux sommets
		 */
		void ajouterArc(int numOrigine, int numDestination, int cout);

		/**
		 * \brief		Enléve un arc du graphe
		 * \pre			Les deux sommets identifiés apr leur numéro doivent appartenir au graphe.
		 * \pre			Un arc reliant les deux stations doit exister.
		 * \post		Le graphe contient un arc en moins, le graphe inchangé sinon
		 * \exception	logic_error		Un des deux sommets, ou les deux, n'existent pas
		 * \exception	logic_error		Il n'y a pas d'arc entre les 2 sommets
		 */
		void enleverArc(int numOrigine, int numDestination);





	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Données sur le graphe (incluant listes de sommets)
	// _________________________________________________________________________________________________

		/**
		 * \brief		Retourne le nombre de sommets dans la liste des sommets
		 * \post		Le graphe reste inchangé.
		 */
		int nombreSommets() const;

		/**
		 * \brief		Indique si la liste des sommets est vide
		 * \post		Le graphe reste inchangé.
		 */
		bool estVide() const;

		/**
		 * \brief		Retourne la liste des numéros des sommets dans un vecteur d'entiers
		 * \post		Le graphe reste inchangé.
		 */
		std::vector<int> listerSommets() const;

		/**
		 * \brief		Retourne la liste des noms des sommets dans un vecteur
		 * \post		Le graphe reste inchangé.
		 */
		std::vector<std::string> listerNomsSommets() const;


	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Trouver numéro sommet
	// _________________________________________________________________________________________________

		/**
		 * \brief		Retourne le numéro d'un sommet selon son nom
		 * \pre			Le sommet doit exister
		 * \post		Le graphe reste inchangé.
		 * \exception	logic_error		Il n'y a pas de sommets dans le graphe
		 * \exception	logic_error		Le sommet n'existe pas
		 */
		int getNumeroSommet(const std::string& nom) const;

		/**
		 * \brief		Retourne le numéro d'un sommet selon ses coordonnées
		 * \pre			Le sommet doit faire partie du graphe
		 * \post		Le graphe reste inchangé.
		 * \exception	logic_error		Il n'y a pas de sommets dans le graphe
		 * \exception	logic_error		Le sommet n'existe pas
		 */
		int getNumeroSommet(int x, int y) const;





	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Données à propos des sommets
	// _________________________________________________________________________________________________

		/**
		 * \brief		Retourne l'existence d'un sommet
		 * \post		Le graphe reste inchangé.
		 */
		bool sommetExiste(int numero) const;

		/**
		 * \brief		Retourne le nom du sommet dont le numéro est passé en argument
		 * \pre			Le sommet doit exister
		 * \post		Le graphe reste inchangé.
		 * \exception	logic_error		Le sommet n'existe pas
		 */
		std::string getNomSommet(int numero) const;

		/**
		 * \brief		Retourne les coordonnées d'un sommet
		 * \pre			Le sommet doit exister
		 * \exception	logic_error		Il n'y a pas de sommets dans le graphe
		 * \exception	logic_error		Le sommet n'existe pas
		 */
		void getCoordonnesSommet(int numero, int& x, int& y) const;

		/**
		 * \brief		Retourne l'ordre de sortie d'un sommet
		 * \pre			Le sommet doit exister
		 * \exception	logic_error		Le sommet n'existe pas
		 */
		int ordreSortieSommet(int numero) const;

		/**
		 * \brief		Retourne l'ordre d'entrée d'un sommet
		 * \pre			Le sommet doit exister
		 * \post		Le graphe reste inchangé.
		 * \exception	logic_error		Le sommet n'existe pas
		 */
		int ordreEntreeSommet(int numero) const;

		/**
		 * \brief		Retourne la liste des numéros des sommets adjacents au sommet passé en argument dans un vecteur d'entiers
		 * \pre			Le sommet doit appartenir au graphe
		 * \post		Le graphe reste inchangé.
		 * \exception	logic_error		si le sommet n'existe pas
		 */
		std::vector<int> listerSommetsAdjacents(int numero) const;

		// ------------------

		/**
		* \brief		Retourne l'attribut d'analyse d'un sommet
		* \pre			Le sommet doit exister
		* \post			Le graphe reste inchangé.
		* \exception	logic_error		Le sommet n'existe pas
		*/
		bool	estAnalyse(int);

		/**
		* \brief		Change l'attribut d'analyse d'un sommet
		* \pre			Le sommet doit exister
		* \post			Le graphe reste inchangé.
		* \exception	logic_error		Le sommet n'existe pas
		*/
		void	asgAnalyse(int);

		/**
		* \brief		Change l'attribut de distance d'un sommet
		* \pre			Le sommet doit exister
		* \post			Le graphe reste inchangé.
		* \exception	logic_error		Le sommet n'existe pas
		*/
		void	asgDistance(int, const int &);

		/**
		* \brief		Retourne l'attribut de distance d'un sommet
		* \pre			Le sommet doit exister
		* \post			Le graphe reste inchangé.
		* \exception	logic_error		Le sommet n'existe pas
		*/
		int	reqDistance(int);

		/**
		* \brief		Change l'attribut 'm_nouveauPoids' d'un sommet
		* \pre			Le sommet doit exister
		* \post			Le graphe reste inchangé.
		* \exception	logic_error		Le sommet n'existe pas
		*/
		void	asgNouveaupoids(int, const int &);

		/**
		* \brief		Retourne l'attribut 'm_nouveauPoids' d'un sommet
		* \pre			Le sommet doit exister
		* \post			Le graphe reste inchangé.
		* \exception	logic_error		Le sommet n'existe pas
		*/
		int	reqNouveaupoids(int);

		/**
		* \brief		Change le prédécesseur d'un sommet
		* \pre			Le sommet doit exister
		* \post			Le graphe reste inchangé.
		* \exception	logic_error		Le sommet n'existe pas
		*/
		void	asgPredecesseur(int, const int &);

		/**
		* \brief		Retourne l'attribut de distance d'un sommet
		* \pre			Le sommet doit exister
		* \post			Le graphe reste inchangé.
		* \exception	logic_error		Le sommet n'existe pas
		*/
		int	reqPredecesseur(int);

		/**
		* \brief		Retourne la liste des sommets reliés à un sommet (dont ce dernier est la source)
		* \pre			Le sommet doit exister
		* \post			Le graphe reste inchangé.
		* \exception	logic_error		Le sommet n'existe pas
		*/
		std::vector<int>	reqDestArc(int);

		/**
		* \brief		Remet à l'état d'origine les sommets
		* \pre			Le graphe doit exister
		* \post			Le graphe reste inchangé.
		*/
		void remiseAZero();

		/**
		* \brief		Pour le tri du tas
		* \pre			Le graphe doit exister
		* \post			Le graphe reste inchangé.
		*/
		static bool triTas(const std::pair<int,int> &, const std::pair<int,int> &);



	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Données à propos des arcs
	// _________________________________________________________________________________________________

		/**
		 * \brief		Retourne l'existence d'un arc
		 * \pre			Les sommets doivent appartenir au graphe
		 * \post			Le graphe reste inchangé.
		 * \exception	logic_error		L'un ou l'autre, ou les 2 sommets ne font pas partie du graphe
		 */
		bool arcExiste(int numOrigine, int numDestination) const;

		/**
		* \brief		Retourne le coét d'un arc passé en argument
		* \pre			L'arc doit exister
		* \post			Le graphe reste inchangé.
		* \exception	logic_error		Le sommet source et/ou destination n'existent pas
		* \exception	logic_error		L'arc n'existe pas
		*/
		int getCoutArc(int numOrigine, int numDestination) const;





	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Surcharge d'opérateurs
	// _________________________________________________________________________________________________

		/**
		 * \brief		Surcharge de l'opérateur d'affectation.
		 * \post		Le graphe a un contenu identique é 'source'.
		 * \exception	bad_alloc		S'il n'y a pas assez de mémoire.
		 */
		Graphe& operator = (const Graphe& source);

		/**
		 * \brief		Surcharge de l'opérateur de sortie.
		 * \post		Le nombre de sommets du graphe sera affiché
		 * \post		Pour chaque sommet, son numéro et son nom seront affichés
		 * \post		Pour chaque sommet, tous ses liens, le numéro des voisins, seront affichés
		 */
		friend std::ostream& operator << (std::ostream& out, const Graphe& g);



	private:

	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Propriétés privées
	// _________________________________________________________________________________________________

		int nbSommets;					/**< Le nombre de sommets dans le graphe */
		Sommet * listSommets;			/**< Liste des sommets du graphe */


	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Méthodes auxiliaires (getters)
	// _________________________________________________________________________________________________

		/**
		 * \brief		Trouve l'adresse d'un sommet à partir de son numéro
		 * \post		L'adresse du sommet recherché est retournée
		 */
		Sommet *_getSommet(int numero) const;

		/**
		 * \brief		Trouve l'adresse d'un arc entre deux sommets
		 * \post		L'adresse de l'arc recherché est retournée, 0 si aucun arc n'a été trouvé
		 */
		Arc * _getArc(Sommet * sommet1, Sommet * sommet2) const;


	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Méthodes auxiliaires (destruction + copie)
	// _________________________________________________________________________________________________

		/**
		 * \brief		Destruction du graphe
		 * \post		La graphe est entiérement supprimé
		 */
		void _detruireGraphe();

		/**
		 * \brief		Copie d'un graphe
		 * \post		Une copie profonde à partir du graphe source a été effectuée
		 */
		void _copierGraphe(const Graphe &source);

		/**
		 * \brief		Enlever un sommet é partir de son adresse
		 * \post		Le sommet a été enlevé, ne fait rien s'il n'existe pas
		 */
		void _enleverSommet(Sommet * sommetAEnlever);



};

#endif
