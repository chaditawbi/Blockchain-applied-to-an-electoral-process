LU2IN006 - PROJET STRUCTURE DE DONNEES
GROUPE MONO1
CHADI TAWBI 28706563
SAID BOUGJDI 28622769


Makefile:
Changez dans le Makefile: PROGRAMS= le main que vous voulez tester et n'oubliez pas de sauvegarder(CTRL+S) le fichier


PARTIE 1: Implementation d’outils de cryptographie

primal.c
primalH.h
primal_main.c

execution du main: make all && ./run_primal_main && make clean

rsa.c
rsaH.h
rsa_main.c

execution du main: make all && ./run_rsa_main && make clean



PARTIE 2: Creation d’un systeme de declarations securises par chiffrement asymetrique

DeclarationSecurise.c
DeclarationSecuriseH.h
DeclarationSecurise_main.c

execution du main: make all && ./run_DeclarationSecurise_main && make clean


PARTIE 3: Manipulation d’une base centralisee de declarations

voteLC.c
voteLCH.h
voteHT.c
voteHTH.h
vote_main.c

execution du main: make all && ./run_vote_main && make clean

PARTIE 4: Implementation d’un mecanisme de consensus

block.c
blockTree.c
blockH.h
blockTreeH.h
block_main.c

execution du main: make all && ./run_blockTree_main && make clean


Partie 5 : Manipulation d’une base decentralisee de declarations

block.c
blockTree.c
blockH.h
blockTreeH.h
blockElection_main.c

execution du main: make all && ./run_blockElection_main && make clean
