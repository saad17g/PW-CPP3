Ce dossier contient l'ensemble des fichiers du TP4 pour le binôme B3307 : Saad GUESSOUS et Jules DUCANGE

Le manuel d'utilisation du programme est le fichier man.md

Le fichier "CR TP4 B3307.pdf" contient les spécifications détaillées vis-à-vis du programme.

Le répertoire src contient tous les codes .h et .cpp du programme.

Le fichier ".analog" contient le domaine utilisé par le programme. Dans le cas du TP il s'agit de "https://intranet-if.insa-lyon.fr".
	-Il faut obligatoirement exécuter le programme à partir d'un répertoire contenant le fichier (caché) ".analog" 
	 => soit le répertoire principal ../B3307, soit le répertoire de test ../B3307/tests, soit le répertoire ../B3307/bin

Le répertoire logs contient les fichiers de logs utilisés pour les tests.

Le répertoire tests contient l'intégralité des tests.


--------------------

Pour compiler le programme et générer l'exécutable, utilisez la commande make en étant dans le répertoire principal.

L'exécutable se trouvera dans le répertoire /bin/

Pour supprimer les .o ainsi que l'exécutable, utilisez la commande make clean, en étant dans le répertoire principal.



Les tests s'exécutent depuis le dossier tests : 

	-Pour exécuter un test en particulier, utilisez la commande ./test.sh [nomTest]
	-Pour exécuter tous les tests, utilisez la commande ./mktest.sh