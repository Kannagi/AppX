
#define SIZE 1000000


unsigned long long rdtsc(void) {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

__attribute__((noinline))  void test(double *tableau,int n)
{
	  // Boucle pour ajouter 5 à chaque élément du tableau
	  for (int i = 0; i < n; i++) {
		tableau[i] += 5.12;

	  }

}


/*
 start = rdtsc(); // Mesure le cycle de début



	end = rdtsc(); // Mesure le cycle de fin




    double tableau[SIZE];
  memset(tableau, 1.0d, sizeof(tableau)); // Initialisation avec des valeurs par défaut à 0


	test(tableau,SIZE);




    cycles = end - start; // Calcule le nombre de cycles écoulés

    printf("Nombre de cycles : %llu\n", cycles);
*/
