/*
 * PositionGenerator.cc
 *
 *  Created on: 22 de abr de 2020
 *      Author: fabio
 */

#include "PositionGenerator.h"

double PositionGenerator::xArrayPositions[maxNumber];
double PositionGenerator::yArrayPositions[maxNumber];
int PositionGenerator::n;
int PositionGenerator::idSim;

PositionGenerator::PositionGenerator() {
    // TODO Auto-generated constructor stub
}

PositionGenerator::~PositionGenerator() {
    // TODO Auto-generated destructor stub
}

PositionGenerator::PositionGenerator(int numHosts, int idSimulation) {
    n = numHosts;
    idSim = idSimulation;
    readPosition();
}

void PositionGenerator::readPosition(){
    ifstream file;
    ostringstream filename;
    filename << "positions/" << n << "-"<< idSim <<".txt";
    file.open(filename.str().c_str(),ios::in);

    string line;

    int index = 0;
    while(getline(file, line))
    {
        istringstream iss(line);
        int x, y;
        if (!(iss >> x >> y)) { break; } // error
        PositionGenerator::xArrayPositions[index] = x;
        PositionGenerator::yArrayPositions[index] = y;
        index++;
        // process pair (a,b)
    }

    vector<int> vectX;
    vector<int> vectY;
    for(int i=0; i< n; i++){
        vectX.push_back(xArrayPositions[i]);
        vectY.push_back(yArrayPositions[i]);
    }
}

double* PositionGenerator::getXPositions(){
    return(PositionGenerator::xArrayPositions);
}

double* PositionGenerator::getYPositions(){
    return(PositionGenerator::yArrayPositions);
}

