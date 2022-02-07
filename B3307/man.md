% ANALOG(1) analog 1.0
% Jules DUCANGE & Saad GUESSOUS
% Janvier 2022

# NAME
analog - analyse un fichier de log apache.

# SYNOPSIS
**bin/program** [-g fichier] [-e] [-i] [-t heure] fichier

# DESCRIPTION
	Le programme analog analyse un fichier de log apache et affiche un top 10
	des documents les plus consultés. Les logs/documents pris en compte dans 
	l'analyse peuvent être ajustés à l'aide d'options. Il est également possible
	de créer un fichier contenant un graphe des différents documents à la
	place du top 10.

	L'ordre des options n'a pas d'importance, si une option est répétée plusieurs
	fois seule la dernière sera prise en compte. Le dernier argument de la commande
	doit absolument être le fichier de log.

# OPTIONS

	**-g fichier**	Change le fonctionnement par défaut, produit un fichier au 
					format GraphViz, contenant le graphe des documents analysés.

	**-e** 			Exclut certains documents de l'analyse selon leur extension
					Liste des extension à exclure : jpg, js, css, png, ics.

	**-t heure** 	Exclut certaines requêtes en fonction de leur heure d'envoi.
					Toutes les requêtes en dehors de l'intervalle [heure; heure+1[
					sont ignorées.

	**-i**			Exclut les requêtes considérées comme invalide. Ces requêtes
					sont définies comme celle dont le code de retour est:
						- Invalide (Pas composé de 3 chiffres).
						- De la forme 4XX ou 5XX

# EXAMPLES

	Affichage du top 10 des documents les plus consultés entre 19 et 20h, en 
	ignorant les fichiers image et style.

	$ bin/program -t 19 -e /tmp/anonyme.log