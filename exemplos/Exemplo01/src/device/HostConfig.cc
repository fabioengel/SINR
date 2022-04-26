//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "HostConfig.h"

Define_Module(HostConfig);

void HostConfig::initialize()
{
    setAddress();
    setPosition();
}

void HostConfig::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

void HostConfig::setAddress(){
    cModule *parent = getParentModule();
    parent->par("myId") = parent->getIndex();
    address = parent->getIndex();
}

void HostConfig::setPosition(){
    cModule *parent = getParentModule();
    std::ostringstream mod;
    mod << parent->getFullName() << ".mobility";

    cModule *currentModule = getModuleByPath(mod.str().c_str());

    PositionGenerator posG = PositionGenerator();

    double* xArray = posG.getXPositions();
    double* yArray = posG.getYPositions();


    int id = parent->getIndex();
    double x = xArray[id];
    currentModule->par("initialX") = x;
    double y = yArray[id];
    currentModule->par("initialY") = y;
    double z = 0;
    currentModule->par("initialZ") = z;
}

cNEDValue startPositionGeneration(cComponent *context, cNEDValue argv[], int argc){
    int numHosts = (int) argv[0].intValue();
    int idSim = (int) argv[1].intValue();

    PositionGenerator pos = PositionGenerator(numHosts, idSim);
    return true;
}
Define_NED_Function(startPositionGeneration, "bool startPositionGeneration(int numHosts, int idSimulation)");
