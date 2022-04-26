# encoding: utf-8
import sys
import os
import sh
import random
import scipy
import math
import click
import time

def getDistance(xa, ya, xb, yb):
	dist = math.sqrt((xa-xb) **2 + (ya-yb)**2)
	return dist

def getDistanceMatrix(positions):
	matrix =[]
	numCandidatos = len(positions)

	for k in range(numCandidatos):
		vetoresPosition = positions[k]
		matrixPositions = []

		nUsers = len(vetoresPosition)

		for i in range(nUsers):
			a = vetoresPosition[i]
			linha = []
			for j in range(nUsers):
				b = vetoresPosition[j]
				dist = getDistance(a[0], a[1], b[0], b[1])
				linha.append(dist)
			matrixPositions.append(linha)

		matrix.append(matrixPositions)
	return matrix

if __name__ == "__main__": 	# Script apaga o conteúdo de "/positions/" e gera novos arquivos com as coordenados de acordo com as variáveis a seguir
	#////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	qtdArg = len(sys.argv)

	numHost = None
	numSimula = None
	x_sup = None
	y_sup = None

	if(len(sys.argv) == 5):
		numHost = int(sys.argv[1])
		numSimula = int(sys.argv[2])
		x_sup = int(sys.argv[3])
		y_sup = int(sys.argv[4])

	else:
		numHost = 10	#Quantidade de dispositivos
		numSimula = 1	#Quantidade de simulações
		x_sup = 100
		y_sup = 100

	#Define o espaço no qual os dispositivos são distribuídos de maneira aleatória
	x_inf = 0	
	y_inf = 0
	

	#////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	try:
		sh.rm(sh.glob('positions/__generic/*'))		
	except:
		print("Não foi possível deletar arquivos do diretório")

	
	random.seed(time.time())

	for i in range(numSimula):
		vetor = []

		for j in range(numHost):
			ok = True
			x = None
			y = None

			while(ok):
				random.seed(time.time())
				x = int(random.uniform(x_inf, x_sup))
				random.seed(time.time())
				y = int(random.uniform(y_inf, y_sup))

				newCoord = [x,y]

				if(newCoord in vetor):
					ok = True
				else:
					ok = False

			vetor.append([x,y])


		numTxt = str(numSimula)
		digitos = len(numTxt)

		forma = "{:0"+ str(digitos) +"d}"
		

		fileName = "positions/__generic/" + str(forma.format(numHost)) +'-' +  str(forma.format(i))+ '-' + str(x_sup) + '-' + str(y_sup) +".txt"				
		file = open(fileName, 'w')

		for j in range(numHost):
			par = vetor[j]
			file.write(str(par[0]) + " " + str(par[1]) + "\n")
		file.close()
		print("Conjunto de posições gerados com sucesso")






