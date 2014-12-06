// Anmol Raina
//demonstarting the use of condition variables and locks

//sorry I didn't define the functions after main ...I got lazy
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int phylo_pop = 0;  //how many phyllo members are in the room
int pyDE_pop = 0;   //pyDE members in the room
int pyDEwait_pop = 0; //pyDE members waiting
int phylowait_pop = 0;  //phylo members waiting
int sign = 0; //avail=0;phylo=1;pyDE=2

pthread_cond_t phylo_wait;
pthread_cond_t pyDE_wait;

pthread_mutex_t room_lock;

void phyloArrive(){
    pthread_mutex_lock(&room_lock);
    while (sign == 2){
            phylowait_pop++;
            pthread_cond_wait(&phylo_wait, &room_lock);
            phylowait_pop--;
            sign = 1;
    }
    printf("Member of phylo entered room\n"); 
    fflush(stdout);
    phylo_pop++;
    sign = 1;
    
    pthread_mutex_unlock(&room_lock);
}

void phyloLeave(){
    pthread_mutex_lock(&room_lock);
    phylo_pop--;
    printf("Member of phylo left room\n");
    fflush(stdout);
    
    if (phylowait_pop > 0){
         pthread_cond_signal(&phylo_wait);
    }
    else if (phylo_pop == 0){
            sign = 0;
            if (pyDEwait_pop > 0){
                 pthread_cond_signal(&pyDE_wait);
            }
         }
    pthread_mutex_unlock(&room_lock);
}

void pyDEArrive(){
    pthread_mutex_lock(&room_lock);
    while (sign == 1){
            pyDEwait_pop++;
            pthread_cond_wait(&pyDE_wait, &room_lock);
            pyDEwait_pop--;
            sign = 2;
     }  
    printf("Member of pyDE entered room\n");
    fflush(stdout);
    pyDE_pop++;
    sign = 2;
   
    pthread_mutex_unlock(&room_lock);
    
}

void pyDELeave(){
    pthread_mutex_lock(&room_lock);
    pyDE_pop--;
    printf("Member of pyDE left room\n");
    fflush(stdout);
    
    if (pyDEwait_pop >0){
        //sign = 2;
        pthread_cond_signal(&pyDE_wait);
    }
    else if (pyDE_pop == 0){
             sign = 0;
             if (phylowait_pop > 0){
                 pthread_cond_signal(&phylo_wait);
             }
         } 
    pthread_mutex_unlock(&room_lock);
}

void *phylo(void *t){
    phyloArrive();
    //printf("Member of phylo entered room\n");
    //fflush(stdout);
    phyloLeave();
    //printf("Member of phylo left room\n");
    //fflush(stdout);
}

void *pyDE(void *t){
    pyDEArrive();
    //printf("Member of pyDE entered room\n");
    fflush(stdout);
    pyDELeave();
    //printf("Member of pyDE left room\n");
    fflush(stdout);
}

void begin(int nphylo, int npyDE){

	int i;
	pthread_t phyloThread[nphylo];
	
	for (i=0; i<nphylo; i++) {
	pthread_create(&phyloThread[i], NULL, phylo, NULL);
	}
	
	pthread_t pyDEThread[npyDE];
	
	for (i=0; i<npyDE; i++) {
	pthread_create(&pyDEThread[i], NULL, pyDE, NULL);
	}

	for (i=0; i< nphylo; i++) {
	pthread_join(phyloThread[i], NULL);
	}
	for (i=0; i<npyDE; i++) {
	pthread_join(pyDEThread[i], NULL);
	}
}

int main(){
    printf("ksjhdkjh\n");
    fflush(stdout);
    pthread_mutex_init(&room_lock, NULL);
	pthread_cond_init(&phylo_wait, NULL);
	pthread_cond_init(&pyDE_wait, NULL);
	
	int phylo = 6;
	int pyDE = 4;
		
	begin(phylo, pyDE);
	
	pthread_mutex_destroy(&room_lock);
  	pthread_cond_destroy(&phylo_wait);
  	pthread_cond_destroy(&pyDE_wait);

	return 0;
}