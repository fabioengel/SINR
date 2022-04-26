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

#ifndef __EXAMPLEOMNET_APLICATIVO_H_
#define __EXAMPLEOMNET_APLICATIVO_H_

#include <omnetpp.h>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <string>
#include <time.h>
#include <fstream>
#include <math.h>


#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/applications/udpapp/UdpBasicApp.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/FragmentationTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
#include "inet/mobility/contract/IMobility.h"
#include "inet/mobility/static/StationaryMobility.h"
#include "inet/mobility/single/LinearMobility.h"


#include "inet/physicallayer/wireless/apsk/packetlevel/ApskScalarTransmitter.h"
#include "inet/common/Units.h"


#include "src/message/TypeMSG_m.h"
#include "src/message/Mensagem_m.h"
#include "inet/physicallayer/wireless/common/medium/MediumLimitCache.h"

#include <stdio.h>
#include <sys/stat.h>
#include <bits/stdc++.h>


using namespace omnetpp;
using namespace inet;
using namespace std;

#define PAST -2;
#define FUTURE 0.0001
//#define INTERVAL 0.1
#define POSTFUTURE 0.0002


class Aplicativo : public ApplicationBase, public UdpSocket::ICallback
{
public:
    // state
    UdpSocket socket;
    cMessage *selfMsg = nullptr;
    simtime_t startTime;
    simtime_t stopTime;
    simtime_t maxTimeSim;

    int networkSize;
    int myId;
    int portApp = 1010;
    bool dontFragment = false;
    const char *packetName = nullptr;
    L3Address broadcast;
    std::vector<L3Address> destAddresses;
    std::vector<std::string> destAddressStr;

    //Controller* controller;
    //GraphHandler *graphHandler;


    //times Next
    simtime_t nextCheckRound;
    simtime_t nextSendMsg;

    simtime_t slotInterval;


    enum SelfMsgKinds {START = 0, CHECKROUNDTIME, SENDTIME, STOP};
    enum MsgType {MensagemMSG = 0};

    int round; //inicia na rodada 0
    int roundQueTransmite;
    int qtdRounds;
    int countSendMsg;
    int getDistFrom;
    bool showReceivedMsg;
    bool sMobility;

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;
    virtual void refreshDisplay() const override;

    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;
    void setSocketOptions();


    void processStart();
    void processStop();
    void handleNextOperation();

    float getDistanceFromDevice(int idD);

    void checkIfTransmit();

    void sendMessage();
    void arrivedMessage(Packet *pktArrived);


public:
    Aplicativo() {}
    ~Aplicativo();

};

#endif
