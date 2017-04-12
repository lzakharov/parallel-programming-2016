mpicc $1.c -o $1 && mpirun -np 4 ./$1 && rm $1
