#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {

	int nbytes;
	int files[argc - 1];
	char buffer[1024];

	if (argc < 2){
		printf("Missing arguments\n");
		return 1;
	}

	for(int i = 0; i < argc - 1; i++){
		files[i] = open(argv[i + 1], O_WRONLY);
	}

	while((nbytes = read(0, buffer, 1024)) > 0){
		for (int i = 0; i < argc - 1; i++){
			write(files[i], buffer, nbytes);
		}
	}

	for(int i = 0; i < argc - 1; i++){
		close(files[i]);
	}

	return 0;
}