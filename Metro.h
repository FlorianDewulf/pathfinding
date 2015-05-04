/**
 * \file	Metro.h
 * \brief	Gestion d'un métro.
 * \author	LeProf
 * \version	1.0
 * \date	???
 */

#include "Graphe.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <limits>
#include <algorithm>    // std::reverse

//#pragma warning( disable : 4290 )//pour visual studio

#ifndef _METRO__H
#define _METRO__H

class Metro
{

	public:

	// 000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	//	Constructeurs et destructeur
	// _________________________________________________________________________________________________

		/**
		 * \brief		Constructeur sans paramètre
		 * \post		Un métro vide est instancié.
		 */
		Metro() { };

		/**
		 * \brief		Constructeur é partir d'un fichier en entrée.
		 * \param[in]	fichierEntree, pointeur sur le fichier contenant la liste de stations é charger.
		 * \pre			Il y a assez de mémoire pour charger toute les stations de la liste.
		 * \pre			Le fichier \a fichierEntree est ouvert corectement.
		 * \post		Le fichier \a fichierEntree n'est pas fermé par la fonction.
		 * \post		Si les préconditions sont respectées, les données du métro contenu
		 *				dans le fichier en entrée sont organisées dans un graphe en mémoire.
		 * \exception	bad_alloc si pas assez de mémoire contenir toute la liste du fichier.
		 * \exception	logic_error si le fichier \a fichierEntree n'est pas ouvert correctement.
		 */
		Metro(std::ifstream &fichierEntree);

		/**
		 * \brief Constructeur de copie.
		 * \pre Il y a assez de mémoire.
		 * \exception bad_alloc S'il n'y a pas assez de mémoire.
		 */
		Metro(const Metro &source);

		/**
		 * \brief		Détruit et libére tout la mémoire allouée auparavant pour le métro.
		 * \post		Tout la mémoire allouée auparavant pour le Metro est libérée.
		 * \post		L'Objet Metro n'est plus valide.
		 */
		~Metro() { } ;





	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Affichage
	// _________________________________________________________________________________________________

		/**
		 * \brief		Affiche une liste de stations de métro é l'écran.
		 * \param[in]	stations : Une liste de stations dans un vector.
		 * \post		Le contenu de la liste v est affiché.
		 */
		void afficherStations(const std::vector<std::string>& stations) const;

		/**
		 * \brief		Affiche la liste de toutes les stations du métro é l'écran
		 * \post		La liste de toutes les stations du métro sont affichées.
		 */
		void afficherStations() const;




	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Chemin le plus court, chemin le plus long...
	// _____________________________________________________________________________________________

		/**
		 * \brief		Trouve le plus court chemin entre deux points en utilisant l'algorithme de Dijsktra et le retourne
		 * \pre			Il y a assez de mémoire pour retourner les composantes du chemin
		 * \post		Le temps pris pour parcourir la distance est retournée, -1 s'il n'y a pas de chemin
		 * \post		Le graphe original reste inchangé
		 * \post		La liste des stations é parcourir est retournée
		 * \exception	bad_alloc		Il n'y a pas assez de mémoire pour retourner le chemin
		 * \exception	logic_error		Le sommet d'origine ou de destination n'existe pas
		 */
		std::vector<std::string> dijkstra(const int & origine, const int & destination, int & nbSec);

		/**
		 * \brief		Trouve le plus court chemin entre deux points en utilisant l'algorithme de Bellman-Ford et le retourne
		 * \pre			Il y a assez de mémoire pour retourner les composantes du chemin
		 * \post		Le temps pris pour parcourir la distance est retournée, -1 s'il n'y a pas de chemin
		 * \post		Le graphe original reste inchangé
		 * \post		La liste des stations é parcourir est retournée
		 * \exception	bad_alloc		Il n'y a pas assez de mémoire pour retourner le chemin
		 * \exception	logic_error		Le sommet d'origine ou de destination n'existe pas
		 */
		std::vector<std::string> bellManFord(const int & origine, const int & destination, int & nbSec);

		/**
		* \brief		Calcul les chemins pour toute origine/destination avec l'algorithme de Dijkstra
		*/
		void tempsDijkstra();

		/**
		* \brief		Calcul les chemins pour toute origine/destination avec l'algorithme de Bellman-Ford
		*/
		void tempsFord();





	// ééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
	//	Surcharge d'opérateurs
	// _____________________________________________________________________________________________

		/**
		 * \brief		Surcharge de l'opérateur d'affectation.
		 * \pre			Il doit y avoir assez de mémoire.
		 * \post		Le Metro a un contenu identique é src.
		 * \exception	bad_alloc	S'il n'y a pas assez de mémoire.
		 */
		Metro& operator=(const Metro& src) throw(std::bad_alloc);

private:

	Graphe unGraphe;	/**< Le graphe du métro */

};

#endif
