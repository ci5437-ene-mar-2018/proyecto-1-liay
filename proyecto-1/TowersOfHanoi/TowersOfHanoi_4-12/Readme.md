TowersOfHanoi 4-12


Paso 1: 

Generar los PDBs utilizando cada una de las abstracciones hechas utilizando los siguientes comandos:

make -f makePDB.mk towers-of-hanoi-4-12/abst_4-12_1.pdb
make -f makePDB.mk towers-of-hanoi-4-12/abst_4-12_2.pdb
make -f makePDB.mk towers-of-hanoi-4-12/abst_4-12_3.pdb


Paso 2:

Generar algoritmos de búsqueda para alcanzar una meta utilizando las heurísticas ya generadas en el paso 1

- Opción 1: 

Generar A* utilizando el siguiente comando:

make -f makeSearch.mk towers-of-hanoi-4-12.aStar 

Luego, para correr el algoritmo escribir el siguiente comando:

./towers-of-hanoi-4-12.aStar

A continuación el programa le pedirá que ingrese un archivo de prueba, el cual debe escribir en formato .txt (los archivos de prueba se encuentran incluidos en la carpeta TestFiles dentro de la carpeta donde se encuentra)

- Opción 2:

Generar IDA* utilizando el siguiente comando:

make -f makeSearch.mk towers-of-hanoi-4-12.idaStar 

Luego, para correr el algoritmo escribir el siguiente comando:

./towers-of-hanoi-4-12.idaStar

A continuación el programa le pedirá que ingrese un archivo de prueba, el cual debe escribir en formato .txt (los archivos de prueba se encuentran incluidos en la carpeta TestFiles dentro de la carpeta donde se encuentra)


Pasos opcionales:

- Generar un árbol de búsqueda a partir del estado goal y obtener una tabla de estadísticas en la cual se incluye la profundidad del mismo, el número de nodos abiertos y el factor de ramificación en cada profundidad. Para esto debe utilizar los siguientes comandos:

make -f makeBFS.mk towers-of-hanoi-4-12.bfs para compilarlo

./towers-of-hanoi-4-12.bfs para correrlo con el argumento:

0: si no desea eliminación parcial de duplicados 
1: si desea eliminación parcial de duplicados 


- Generar casos de prueba utilizando el generador incluido en PSVN. Para esto debe utilizar los siguientes comandos:

make -f makeSearch.mk towers-of-hanoi-4-12.gen para compilarlo

./towers-of-hanoi-4-12.gen para correrlo con los argumentos:

seed n depth-bound

Donde seed se refiere a la semilla inicial, n es la cantidad de casos de prueba y depth-bound es la cota de profundidad


