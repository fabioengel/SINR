# encoding: utf-8
import sys
import os
import sh
import random
import scipy
import math
import click
import time
import copy
import numpy as np

def getDistance(xa, ya, xb, yb):
	dist = math.sqrt((xa-xb) **2 + (ya-yb)**2)
	return dist


def getData(diretorio): #retorna os dados do diretório
	numberSimulations = len(os.listdir(diretorio))
	fi = os.listdir(diretorio)
	filesNames = np.sort(fi)#[::-1]

	data = []
	for i in range(numberSimulations):
		
		with open(diretorio + filesNames[i] , 'r') as f:
			try:
				data.append(f.read().splitlines())
			except:
				print('Erro na leitura do arquivo')
		f.close()

	return data


if __name__ == "__main__":

	
	alfa = 4
	N0Dbm = -90 #ruído de fundo em dBm
	NoMw = 10**(N0Dbm/10)	
	NoW = NoMw * 10**(-3) # ruído de fundo em Watt
	
	sinrThresholdDb = 1 #SINR em dB
	sinrThreshold = 10**(sinrThresholdDb/10) #SINR em W


	dados = getData('positions/__computoSINREx02/')
	positions = dados[0]
	numHosts = len(positions)

	p = []
	for i in range(numHosts):
		posi = str.split(positions[i])
		coordenate = [int(posi[0]),int(posi[1])]
		p.append(coordenate)


	minReceptionPowerDbm = -90 #dBm
	
	Ptransmitida = 0.1 #Pt em W


	for index in range(11):
		p[2][0] = p[2][0] - 10

		#selecionar o id do dispositivo a partir do número inteiro
		transmitters = [0,2]
		print("\n\nTransmistem: " + str(transmitters))

		ids = ['a', 'b', 'c', 'd','e', 'f']

		for i in range(numHosts):
			if(i not in transmitters):
				#para cada receptor i,  avalia a SINR de cada transmissor
				print("---------------------------")
				print("Para o receptor " + ids[i])
				
				for j in range(len(transmitters)):
					dist = getDistance(p[i][0], p[i][1], p[transmitters[j]][0], p[transmitters[j]][1])
					pathloss = dist**(-alfa)
					Pr = Ptransmitida * pathloss

					Pinterferencia = 0;
					for k in range(len(transmitters)):
						if(transmitters[k] != transmitters[j]):
							dist = getDistance(p[i][0], p[i][1], p[transmitters[k]][0], p[transmitters[k]][1])
							#print("Distância: " + str(dist))
							pathlossInterferencia = dist**(-alfa)
							Pint = Ptransmitida * pathlossInterferencia
							Pinterferencia = Pinterferencia + Pint


					SINR = Pr/(NoW + Pinterferencia)
					SINRdB = 10 * math.log10(SINR)
					if(SINRdB >= sinrThresholdDb):
						print("recebe transmissão de " + ids[transmitters[j]] + "        ---> " + "SINR: " + str(SINRdB) + " dB" +  " ou " + str(SINR)+ " W  --- " + " Potência do Sinal Recebido: " + str(Pr) + " W")
						
					else:
						print("não recebe transmissão de " + ids[transmitters[j]] + "    ---> " + "SINR: " + str(SINRdB) + " dB" +  " ou " + str(SINR)+ " W  --- " + " Potência do Sinal Recebido: " + str(Pr) + " W")
