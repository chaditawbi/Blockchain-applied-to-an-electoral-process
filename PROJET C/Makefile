CFLAGS =-Wall
CC = gcc

#CHANGEZ ICI LE MAIN QUE VOUS VOULEZ
PROGRAMS = run_blockElection_main


all: $(PROGRAMS)



#LES DIFFERENTS MAINS A CHOISIR POUR TESTER

run_blockElection_main:rsa.o primal.o DeclarationSecurise.o voteLC.o voteHT.o block.o blockTree.o blockElection_main.o
	$(CC) -o $@ $(CFLAGS) $^  -lm -lssl -lcrypto


run_blockTree_main: rsa.o primal.o DeclarationSecurise.o voteLC.o voteHT.o block.o blockTree.o block_main.o
	$(CC) -o $@ $(CFLAGS) $^ -lm -lssl -lcrypto

run_vote_main:rsa.o primal.o DeclarationSecurise.o voteLC.o voteHT.o vote_main.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

run_DeclarationSecurise_main:rsa.o primal.o DeclarationSecurise.o DeclarationSecurise_main.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

run_rsa_main:rsa.o primal.o rsa_main.o
	$(CC) -o $@ $(CFLAGS) $^ -lm
	
run_primal_main:primal.o primal_main.o
	$(CC) -o $@ $(CFLAGS) $^ -lm


#LES COMPILATIONS
primal_main.o:primal_main.c
	$(CC) -c $(CFLAGS) primal_main.c

rsa_main.o:rsa_main.c
	$(CC) -c $(CFLAGS) rsa_main.c

DeclarationSecurise_main.o:DeclarationSecurise_main.c
	$(CC) -c $(CFLAGS) DeclarationSecurise_main.c

vote_main.o:vote_main.c
	$(CC) -c $(CFLAGS) vote_main.c

blockElection_main.o:blockElection_main.c
	$(CC) -c $(CFLAGS) blockElection_main.c

rsa.o:rsa.c
	$(CC) -c $(CFLAGS) rsa.c

primal.o: primal.c
	$(CC) -c $(CFLAGS) primal.c

DeclarationSecurise.o: DeclarationSecurise.c
	$(CC) -c $(CFLAGS) DeclarationSecurise.c

voteLC.o: voteLC.c
	$(CC) -c $(CFLAGS) voteLC.c

voteHT.o:voteHT.c
	$(CC) -c $(CFLAGS) voteHT.c

block.o:block.c
	$(CC) -c $(CFLAGS) block.c

blockTree.o:blockTree.c
	$(CC) -c $(CFLAGS) blockTree.c




#CLEAN NETOYE TOUT LES FICHIERS
clean:
	rm -f *.o *~ $(PROGRAMS)
