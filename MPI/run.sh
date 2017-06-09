mpicc $1.c -o $1 && mpirun -np 2 ./$1 && rm $1
