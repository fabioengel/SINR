/*
 * PositionGenerator.h
 *
 *  Created on: 22 de abr de 2020
 *      Author: fabio
 */

#ifndef SRC_DEVICE_POSITIONGENERATOR_H_
#define SRC_DEVICE_POSITIONGENERATOR_H_
#define maxNumber 500
#include <fstream>
#include <iostream>
#include <math.h>
#include <iterator>
#include <random>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

class PositionGenerator {
public:
    static int n; // número de dispositivos
    static int idSim;
    static double xArrayPositions[maxNumber];
    static double yArrayPositions[maxNumber];

    virtual double* getXPositions();
    virtual double* getYPositions();

    void readPosition();
    PositionGenerator(int numHosts, int idSimulation);
    PositionGenerator();
    virtual ~PositionGenerator();
};

#endif /* SRC_DEVICE_POSITIONGENERATOR_H_ */
