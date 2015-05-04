/**
* \file TP2.cpp
* \brief Point d'entrée du programme lançant les algorithmes
* \author Florian Dewulf
* \version 1.3
* \date 10/11/2014
*/
#include  <iostream>
#include  <fstream>
#include  <string>
#include  <ctime>
#include  "Metro.h"
#include  "Graphe.h"

/**
* \fn			void affichageStations(const std::vector<std::string> &p_stations, const Metro &p_metro,
*                                const int &p_nbSec, const std::string &p_algorithme);
* \param[in]	p_stations La liste des stations parcourues
* \param[in]	p_metro L'objet Metro qui possède la méthode 'afficherStations'
* \param[in]	p_nbSec Le temps de parcours du trajet
* \param[in]	p_algorithme Le nom de l'algorithme
*/
static void affichageStations(const std::vector<std::string> &p_stations, const Metro &p_metro,
                              const int &p_nbSec, const std::string &p_algorithme)
{
  std::cout << "Le plus court chemin trouvé par " + p_algorithme + " est:" << std::endl;
  p_metro.afficherStations(p_stations);
  std::cout << "avec un temps estimé de " << p_nbSec << " secondes" << std::endl << std::endl;
}

/**
* \fn			void traitement (std::ifstream &p_fichier);
* \param[in]	p_fichier Le fichier ouvert
*/
static void traitement (std::ifstream &p_fichier)
{
  Metro contenuFichier(p_fichier);
  int   origine = 0, destination = 0, nbSec = -1;
  std::vector<std::string> stationsDijkstra;
  std::vector<std::string> stationsFord;

  std::cout << "Entrez le numéro de la station de départ" << std::endl;
  std::cin >> origine;
  std::cout << "Entrez le numéro de la station d'arrivée" << std::endl;
  std::cin >> destination;
  stationsDijkstra = contenuFichier.dijkstra(origine, destination, nbSec);
  affichageStations(stationsDijkstra, contenuFichier, nbSec, "Dijkstra");
  nbSec = -1;
  stationsFord = contenuFichier.bellManFord(origine, destination, nbSec);
  affichageStations(stationsFord, contenuFichier, nbSec, "Bellman-Ford");
  std::cout << "Détermination du temps d'exécution de l'algorithme de Dijkstra pour toutes les paires origine/destination..." << std::endl;
  contenuFichier.tempsDijkstra();
  std::cout << "Détermination du temps d'exécution de l'algorithme de Bellman-Ford pour toutes les paires origine/destination..." << std::endl;
  contenuFichier.tempsFord();
}

int       main()
{
  std::ifstream fichier;

  fichier.open("Metro.txt");
  if (fichier.is_open())
  {
    try
    {
      traitement(fichier);
    }
    catch (const std::logic_error &e)
    {
      std::cerr << e.what() << std::endl;
    }
    fichier.close();
  }
  else
  {
    std::cerr << "Erreur : Impossible d'ouvrir le fichier Metro.txt" << std::endl;
  }
}

/*
Dijkstra est plus rapide que Bellman-Ford, notamment avec l'optimisation de l'implémentation par tas.
(les temps le prouvent clairement)
*/
