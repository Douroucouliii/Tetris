PROJET TETRIS

-Perin Clément
-Mackowiack Carole

-------------------------------------------

-Comment compiler :

'make clean' pour supprimer les .o et les .d
'make' pour compiler et générer la documentation avec Doxyfile
'./exec' pour executer le programme
'make doc' pour générer la documentation

-------------------------------------------

-Controleurs :

menu/selection de niveau/options (SDL):
z,s,fleche : se déplacer dans le menu
entrer : selectionner/valider
-----
jeu :
q, s, d, fleche : mouvement des tetriminos
a,e : rotation des tetriminos
m : changer d'interface
-----
fin de partie (SDL):
q, d, fleche : se déplacer dans la fin de partie
entrer : selectionner/valider
pour enregistrer un nouveau record si c'est le cas : tab pour commencer à écrire son nom, tab pour finir d'écrire son nom (ne pas oublier d'appuyer sur enregistrer avec entrer)

-------------------------------------------

-Implémentations détaillées :

modèle :

mouvement/rotation des tetriminos

système de score basé sur tetris NES, pour ajouter un score : (n+1)L, où n est le niveau et L=40 pour 1 ligne, L=100 pour deux, L=300 pour 3 et L=1200 pour un tetris

statistique des pièces (nombre de piece apparu sur le jeu)

prochaine pièce à apparaître

système de niveau NES, un niveau augmente toutes les 10 lignes, ce qui augmente la vitesse de chute des pièces et change la couleur du jeu (voir suite),
sauf pour le premier level up qui est calculé en fonction du niveau que l'on a commencé :
start level 0-9 : premier level up à (startLevel + 1) * 10 lignes
level 10-19 start : premier level up à max((startLevel - 5) * 10, 100) lignes

chute des pièces en fonction du niveau : système de delta time en 60 fps, 1 tour de boucle = 1 frame, ainsi pour chaque tour de boucle on décrémente une variable
plus ou moins grande (calculé selon le niveau en cours), et à 0, cela entraine la chute de la piece de 1 vers le bas.
frames requis pour la chute/niveau :  48/0, 43/1, 38/2, 33/3, 28/4, 23/5, 18/6, 13/7, 8/8, 6/9, 5/(10/11/12), 4/(13/14/15), 3/(16/17/18), 2/(19-28), 1/29+

holdTime (temps d'attente) quand une piece arrive en bas de l'écran, permet d'avoir un repis de quelques frames avant de faire apparaitre une nouvelle piece (utile pour les niveaux durs genre 29+)
le holdTime est calculé en fonction de la hauteur du jeu (une ligne est comptabilisé si elle possede au moins une cellule) : 10 frames par ligne pour les 2 premieres lignes + 2 frame pour chaque 4 lignes ajouté

changement de couleurs lorsqu'on passe à un niveau supérieur : toutes les couleurs des pieces sont redéfinis aléatoirement (sans répétition), et toutes les pièces du en jeu changent de couleur.

mode panic : si le joueur est proche de perdre, on rentre en mode panique, la musique s'accélère beaucoup, voir "graphisme : SDL"

switch d'interface : en jeu, appuyer sur m pour changer d'interface (nCurses / SDL)

correction du "problème du hold", quand on hold sur la gauche ou droite, comme notre jeu tourne à 60 fps, avec 1 input / frame, le fait de hold ralentissait la chute des pieces.
la correction a été d'ajuster les fps dans ces cas là afin d'avoir un 60 fps stable quoiqu'il arrive

-----

graphisme :

nCurses :
menu : entrer un niveau entre 0 et 19 puis appuyer sur entrer pour lancer le jeu
jeu : affichage du jeu, avec les statistiques, le score, le niveau et la prochaine piece
fin de partie : possibilité d'entrer son score s'il fait parti des 10 meilleurs scores (fichier local), entrer son nom et appuyer sur entrer.


SDL :
icon du jeu
sons : 3 musiques intégrées (1 menu / 1 jeu / 1 fin de partie) ainsi que 10 sons (mouvement, rotation, fall, chargement d'une page, ligne clear, tetris, selectionne, perdre, level up)
menu : 3 options : jouer, options et exit
	exit : quitter le niveau
	options : lancer les options pour régler les sons ou la musique du jeu
	jouer : lancer le selecteur de niveau
selecteur de niveau : grille de 19 boutons à séléctionner afin de lancer le niveau souhaitable
jeu : affichage du jeu, avec les statistiques, le score, le niveau et la prochaine piece
fin de partie : possibilité d'entrer son score s'il fait parti des 10 meilleurs scores (fichier local), appuyer sur tab, entrer son nom , appuyer sur tab, et sur le bouton enregistrer







