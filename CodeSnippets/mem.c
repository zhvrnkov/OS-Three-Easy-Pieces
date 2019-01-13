int main(int argc, char *argv[]) { 
	int *p = malloc(sizeof(int)); // a1
	assert(p != NULL);
	printf("(%d) memory address of p: %08x\n", 
		getpid(), (unsigned) p); // a2 
	*p=0; //a3 
	while (1) {
        	Spin(1);
        	*p = *p + 1;
        	printf("(%d) p: %d\n", getpid(), *p); // a4
	}	
	return 0; 	
}
// OUTPUT:
/**
./mem
	(2134) memory address of p: 00200000
	(2134) p: 1
	(2134) p: 2
	(2134) p: 3
	(2134) p: 4
	(2134) p: 5
**/
/**
./mem &; ./mem &
      [1] 24113
      [2] 24114
      (24113) memory address of p: 00200000
      (24114) memory address of p: 00200000
      (24113) p: 1
      (24114) p: 1
      (24114) p: 2
      (24113) p: 2
      (24113) p: 3
      (24114) p: 3
      (24113) p: 4
      (24114) p: 4
**/
