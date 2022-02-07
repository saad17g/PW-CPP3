#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

#include "LogReader.h"

const size_t LIMIT_TOP_HITS = 10;
const string BANNED_EXTENTIONS[] = {"jpg", "js", "css", "png", "ics"};
const string CONFIGURATION_FILENAME = ".analog";

typedef unordered_map<size_t, size_t> node;
typedef unordered_map<size_t, node> Graph;
typedef map<size_t, size_t> Hits;

bool isValidURL(string& url, const string[]);
bool isValidRequest(log& l);

typedef struct KeyStore {
	unordered_map<string, size_t> store;
	size_t seed;

	KeyStore(size_t s = 0): seed(s) {};
	inline size_t GetKey(string& url)
	{
		// La clé existe déjà
		if (store.find(url) != store.end())
		{
			return store[url];
		} else

		// La clé n'existe pas 
		{
			store[url] = seed;
			return seed++;
		}
	};

	inline string reverseFind(size_t key)
	{
		for (const auto& item : store)
		{
			if (item.second == key) 
				return item.first;
		}

		return "";
	};
} KeyStore;

void exportDigraph(Graph& graph, Hits& hits, KeyStore& ks, ofstream& outFile);

int main(int argc, char* argv[])
{
	// Vérification du nombre minimum d'arguments
	if (argc < 2)
	{
		cerr << "Fichier de log invalide ou inexistant." << endl;
		return 1;
	}

	// Récupère le fichier de log
	LogReader reader(argv[argc - 1]);
	
	// Vérifie que le fichier est valide
	if (!reader)
	{
		cerr << "Fichier de log invalide ou inexistant." << endl;
		return 1;
	}

	// Parcours des options
	bool gFlag = false;
	bool eFlag = false;
	bool tFlag = false;
	bool iFlag = false;

	// Variable d'options
	string graphFilename;
	int referenceHour = 0;

	int nbOptions = argc - 1;
	int argi = 1;

	while (argi < nbOptions)
	{
		// Récupère l'argument
		string arg = string(argv[argi]);

		// Traite les différentes options
		if (arg == "-g")
		{
			gFlag = true;

			// Vérifie que le nombre d'option est valide
			if (++argi >= nbOptions)
			{
				cerr << "Option -g : Fichier de graphe invalide ou manquant." << endl;
				return 1;
			}
			
			// Récupère le nom du fichier de graphe
			graphFilename = string(argv[argi]);

		} else if (arg == "-e")
		{
			eFlag = true;
		} else if (arg == "-i")
		{
			iFlag = true;
		} else if (arg == "-t")
		{
			tFlag = true;

			// Vérifie que le nombre d'option est valide.
			if (++argi >= nbOptions)
			{
				cerr << "Option -t : Heure manquante." << endl;
				return 1;
			}
			
			referenceHour = atoi(argv[argi]);
			
			// Vérifie que l'heure est valide
			if (string(argv[argi-1]) != "0" && (referenceHour <= 0 || referenceHour > 23))
			{
				cerr << "Option -t : Heure invalide." << endl;
				return 1;
			}

		} else 
		{
			cerr << "Option inconnue." << endl;
			return 1;
		}

		argi++;
	}

	// Variable de configuration
	string baseURL = "intranet-if.insa-lyon.fr";

	// Récupère le fichier de configuration
	ifstream configFile;
	configFile.open(CONFIGURATION_FILENAME);

	// Fichier de configuration illisible
	if (!configFile)
	{
		cerr << "Fichier de configuration invalide" << endl;
		return 1;
	}

	getline(configFile, baseURL);	

	log current = reader.GetNextLog();
	KeyStore ks = KeyStore();
	Graph logGraph;
	Hits totalHits;

	while (!reader.eof())
	{
		// Vérifie si l'URL doit-être prit en compte
		if (eFlag && !isValidURL(current.target, BANNED_EXTENTIONS))
		{
			current = reader.GetNextLog();
			continue;
		}

		// Vérifie que la tranche horaire est correct
		if (tFlag && !(referenceHour <= current.date.hour && current.date.hour < (referenceHour + 1)))
		{
			current = reader.GetNextLog();
			continue;
		}

		// Vérifie que la requêtes était valide
		if (iFlag && !isValidRequest(current))
		{
			current = reader.GetNextLog();
			continue;
        }

		// Retire l'url de base
		StripBaseURL(current.origin, baseURL);
        StripBaseURL(current.target, baseURL);

		// Récupère les clés des urls
		auto targetKey = ks.GetKey(current.target);
		auto originKey = ks.GetKey(current.origin);

		// La cible existe déjà dans le Graphe
		if (logGraph.find(targetKey) != logGraph.end())
		{
			node& hits = logGraph[targetKey];
			totalHits[targetKey]++;	
			
			// L'origine existe déjà dans le hits
			if (hits.find(originKey) != hits.end())
			{
				hits[originKey]++;
			} else
			// L'origine n'existe pas
			{
				hits[originKey] = 1;
			}

		} else
		// La cible n'existe pas 
		{
			logGraph[targetKey][originKey] = 1;
			totalHits[targetKey] = 1;
		}

		current = reader.GetNextLog();
	}

	// Si le flag -g est activé, désactive la création du top10
	if (!gFlag)
	{
		// Construction du top 10.
		vector<pair<size_t, size_t>> top;
		for (const auto& item : totalHits)
		{
			top.emplace_back(item);
		}
		
		// Tri les éléments sur le nombre de hits
		sort(top.begin(), top.end(), 
//			[] (const auto& a, const auto&b) {return a.second > b.second;}
			[] (const pair<size_t,size_t>& a, const pair<size_t,size_t>&b) {return a.second > b.second;}
		);
		
		// Ne conserve que les 10 premiers éléments
		size_t offset = min(LIMIT_TOP_HITS, top.size());
		vector<pair<size_t, size_t>> top10 = {top.begin(), top.begin() + offset};

		for (const auto& item : top10)
		{
			cout << "[" << item.second << " hit(s) : " 
			<< ks.reverseFind(item.first) << "]" << endl;
		}
	} else
	// Génère un fichier de graphe
	{
		ofstream graphFile;
		graphFile.open(graphFilename);

		if (!graphFile)
		{
			cerr << "Erreur lors de l'ouverture du fichier" << endl;
		} else
		{
			exportDigraph(logGraph, totalHits, ks, graphFile);
			graphFile.close();
		}
	}

	return 0;
}


bool isValidURL(string& url, const string bannedExtentions[])
{
	// Découpe l'url en tokens
	stringstream ss(url);
	string tmp;
	vector<string> tokens;
	
	while (!ss.eof())
	{
		getline(ss, tmp, '.');
		tokens.push_back(tmp);
	}

	for (const auto& extention : BANNED_EXTENTIONS)
	{
		if (tokens.back() == extention) return false;
	}
	
	return true;
}

void exportDigraph(Graph& graph, Hits& hits, KeyStore& ks, ofstream& outFile)
{
	if (!outFile)
	{
		cout << "Impossible d'ouvrir le fichier." << endl;
		return;
	}

	outFile << "digraph {" << endl;
	// Parcours le graphe pour lister les nodes
	for (const auto &keyPair : ks.store)
	{
		outFile << "node" << keyPair.second << " [label=\"" 
		<< keyPair.first << "\"];" << endl;
	}
	// Parcours le graphe pour lister le détails des relations
	for (const auto &hits : graph)
	{
		for (const auto &keyPair : hits.second)
		{
			outFile << "node" <<  keyPair.first << " -> " << "node" << hits.first
			<< " [label=\"" << keyPair.second << "\"];" << endl;
		}
	}
	outFile << "}" << endl;
}

inline bool isValidRequest(log& l)
{
	// Code de retour invalide
	if (l.ret_code.length() < 3) return false;
	if (l.ret_code[0] == '4' || l.ret_code[0] == '5') return false;
	return true;
}
