#!/bin/bash
#N=100000000 # N1 = cem milhões
N=50000000
V=20
N2=$(( $N + $N ))
N4=$(( $N2 + $N2 ))
Nmeio=$(( $N / $N ))

echo "gerando entrada de tamanho $N..."
./gerador $N > N1a.txt
./gerador $N > N1b.txt
echo "gerando entrada de tamanho $N2..."
./gerador $N2 > N2a.txt
./gerador $N2 > N2b.txt
echo "gerando entrada de tamanho $N4..."
./gerador $N4 > N4a.txt
./gerador $N4 > N4b.txt

rm *out
make clean
make all

for i in $(seq $V)
do
  echo "quicksort N1 $i..."
  ./quicksort < N1a.txt >> quicksortn1a.out
  ./quicksort < N1b.txt >> quicksortn1b.out
  echo "quicksort N2 $i..."
  ./quicksort < N2a.txt >> quicksortn2a.out
  ./quicksort < N2b.txt >> quicksortn2b.out
  echo "quicksort N4 $i..."
  ./quicksort < N4a.txt >> quicksortn4a.out
  ./quicksort < N4b.txt >> quicksortn4b.out
done

for p in 1 2 4
do
  for i in $(seq $V)
  do
    echo "mpi N1 p=${p} $i..."
    mpirun -np $p mpi < N1a.txt >> mpin1a-${p}p.out
    mpirun -np $p mpi < N1b.txt >> mpin1b-${p}p.out
    echo "mpi N2 p=${p} $i..."
    mpirun -np $p mpi < N2a.txt >> mpin2a-${p}p.out
    mpirun -np $p mpi < N2b.txt >> mpin2b-${p}p.out
    echo "mpi N4 p=${p} $i..."
    mpirun -np $p mpi < N4a.txt >> mpin4a-${p}p.out
    mpirun -np $p mpi < N4b.txt >> mpin4b-${p}p.out
  done
done
