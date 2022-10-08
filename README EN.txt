LU2IN006 - Data Structure Project
CHADI TAWBI


Makefile:
Change in the Makefile: PROGRAMS= the "main" file you want to test and don't forget to save (CTRL+S) the file


PART 1: Implementation of cryptographic tools

primal.c
primalH.h
primal_main.c

execution of main: make all && ./run_primal_main && make clean

rsa.c
rsaH.h
rsa_main.c

execution of main: make all && ./run_rsa_main && make clean


PART 2: Creating a secure declaration system using asymmetric encryption

DeclarationSecurise.c
DeclarationSecuriseH.h
DeclarationSecurise_main.c

execution of main: make all && ./run_DeclarationSecurise_main && make clean


PART 3: Handling a centralized declaration database

voteLC.c
voteLCH.h
voteHT.c
voteHTH.h
vote_main.c

execution of main: make all && ./run_vote_main && make clean

PART 4: Implementation of a consensus mechanism

block.c
blockTree.c
blockH.h
blockTreeH.h
block_main.c

execution of main: make all && ./run_blockTree_main && make clean


Part 5: Manipulation of a decentralized database of declarations

block.c
blockTree.c
blockH.h
blockTreeH.h
blockElection_main.c

execution of main: make all && ./run_blockElection_main && make clean
