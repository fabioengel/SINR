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

#include "Aplicativo.h"

Define_Module(Aplicativo);

Aplicativo::~Aplicativo(){
    cancelAndDelete(selfMsg);
}


void Aplicativo::initialize(int stage){
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        selfMsg = new cMessage("Timer");

        //acesso ao módulo para obter mydId
        cModule *networkModule = getModuleByPath("Network");
        networkSize = networkModule->par("numHosts").intValue();

        cModule *parent = getParentModule();
        myId = parent->getIndex();

        //para obter tempo máximo de simulação
        cConfigOption simTimeConfig("sim-time-limit", true,cConfigOption::Type::CFG_DOUBLE, "s", "300", "");
        maxTimeSim = cSimulation::getActiveSimulation()->getEnvir()->getConfig()->getAsDouble(&simTimeConfig);

        //inicializa srand
        srand (time(NULL));
        round = -1;

        //parâmetros
        startTime = par("startTime");
        stopTime = par("stopTime");
        slotInterval = par("slotInterval");

        roundQueTransmite = par("roundQueTransmite");
        qtdRounds = par("qtdRounds");

        //tempos
        nextCheckRound = PAST;
        nextSendMsg = PAST;

        //valores iniciais
        broadcast = L3Address("255.255.255.255");

        if(roundQueTransmite==-1){
            if (hasGUI()){
                    getParentModule()->getDisplayString().set("i=device/mobileR");
                }
        }

    }
}

void Aplicativo::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        ASSERT(msg == selfMsg);
        switch (selfMsg->getKind()) {
        case START:{
            nextCheckRound = 1;
            processStart();
            handleNextOperation();
            break;
        }

        case CHECKROUNDTIME:{
            checkIfTransmit();
            handleNextOperation();
            break;
        }

        case SENDTIME:{
            sendMessage();
            handleNextOperation();
            break;
        }

        default:
            throw cRuntimeError("Invalid kind %d in self message", (int)selfMsg->getKind());
        }
    }
    else{
        socket.processMessage(msg);
    }
}

void Aplicativo::finish(){
    ApplicationBase::finish();
}

void Aplicativo::refreshDisplay()const{
    ApplicationBase::refreshDisplay();
}

void Aplicativo::handleStartOperation(LifecycleOperation *operation){
    simtime_t start = std::max(startTime, simTime());
    if ((stopTime < SIMTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime)) {
        selfMsg->setKind(START);
        scheduleAt(start, selfMsg);
    }
}

void Aplicativo::handleStopOperation(LifecycleOperation *operation){
    cancelEvent(selfMsg);
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void Aplicativo::handleCrashOperation(LifecycleOperation *operation){
    cancelEvent(selfMsg);
    socket.destroy();
}

void Aplicativo::socketDataArrived(UdpSocket *socket, Packet *packet){
    auto header = packet->popAtFront<TypeMSG>();
    int type = (int) header->getType();

    switch(type){
    case MensagemMSG:{
        arrivedMessage(packet);
        break;
    }

    default:
        throw cRuntimeError("Invalid type");
    }
    delete packet;
}

void Aplicativo::socketErrorArrived(UdpSocket *socket, Indication *indication){
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void Aplicativo::socketClosed(UdpSocket *socket){
    if (operationalState == State::STOPPING_OPERATION)
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void Aplicativo::setSocketOptions(){
    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
        socket.setTimeToLive(timeToLive);

    int dscp = par("dscp");
    if (dscp != -1)
        socket.setDscp(dscp);

    int tos = par("tos");
    if (tos != -1)
        socket.setTos(tos);

    const char *multicastInterface = par("multicastInterface");
    if (multicastInterface[0]) {
        IInterfaceTable *ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
        NetworkInterface *ie = ift->findInterfaceByName(multicastInterface);
        if (!ie)
            throw cRuntimeError("Wrong multicastInterface setting: no interface named \"%s\"", multicastInterface);
        socket.setMulticastOutputInterface(ie->getInterfaceId());
    }

    bool receiveBroadcast = par("receiveBroadcast");
    if (receiveBroadcast)
        socket.setBroadcast(true);

    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");
    if (joinLocalMulticastGroups) {
        MulticastGroupList mgl = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this)->collectMulticastGroups();
        socket.joinLocalMulticastGroups(mgl);
    }
    socket.setCallback(this);
}

void Aplicativo::processStart(){
    socket.setOutputGate(gate("socketOut"));
    socket.bind(portApp);
    setSocketOptions();

    nextCheckRound = simTime() + slotInterval;
}

void Aplicativo::processStop(){
    socket.close();
}

void Aplicativo::handleNextOperation(){ //método tem como objetivo verificar todos next* (tempos para executar ações) e decidir qual é o próximo a ser executado.
    Enter_Method_Silent();
    vector<simtime_t> ev = {nextCheckRound, nextSendMsg, stopTime};
    int evSize = ev.size();
    vector<int> indexV;
    indexV.resize(evSize);
    for(int i=0; i<evSize; i++){
        indexV[i] = i;
    }

    auto minEv = std::min_element(ev.begin(), ev.end());
    simtime_t now = simTime();
    bool fim = true;
    int idEv = -1;

    if(simTime() > stopTime){
        cancelEvent(selfMsg);
        selfMsg->setKind(STOP);
        scheduleAt(now, selfMsg);
        return;
    }

    while(fim){
        minEv = std::min_element(ev.begin(), ev.end());
        int qntdEv = (int) ev.size();

        for(int i=0; i< qntdEv; i++){
            if(*minEv == ev[i]){
                idEv = i;
                break;
            }
        }

        if(*minEv < now){
            if(ev.size() == 0){
                fim = false;
                idEv = indexV.size() - 1; // FIM
                stopTime = simTime()+ FUTURE;
                cancelEvent(selfMsg);
                break;
                //cout << "Atenção: time: " << simTime() << endl;

            }
            else{
                ev.erase(ev.begin() + idEv);
                indexV.erase(indexV.begin() + idEv);
            }
        }
        else{
            fim = false;
        }
    }

    switch (indexV[idEv]) {
    case 0:{
        selfMsg->setKind(CHECKROUNDTIME);
        scheduleAt(nextCheckRound, selfMsg);
        break;
    }

    case 1:{
        selfMsg->setKind(SENDTIME);
        scheduleAt(nextSendMsg, selfMsg);
        break;
    }

    case 2:{
        selfMsg->setKind(STOP);
        scheduleAt(stopTime, selfMsg);
        break;
    }

    default:
        throw cRuntimeError("Invalid type");
    }

}


void Aplicativo::sendMessage(){
        cout << myId << " transmite na rodada " << round << " -- tempo atual: "<< simTime() << "s" << endl;
        EV_INFO << "++++++++++ " << myId << " transmite na rodada " << round << " -- tempo atual: "<< simTime() << "s ++++++++++" << endl;

        std::ostringstream packetName;
        packetName << "Mensagem de " << myId;
        Packet *packet = new Packet(packetName.str().c_str());
        packet->setKind(MensagemMSG);
        if(dontFragment)
            packet->addTag<FragmentationReq>()->setDontFragment(true);

        const auto& head = makeShared<TypeMSG>();
        head->setChunkLength(B(8));
        head->setType(MensagemMSG);

        const auto& payload = makeShared<Mensagem>();
        payload->setChunkLength(B(par("messageLength")));
        payload->setEmissor(myId);
        payload->setRodada(round);
        payload->addTag<CreationTimeTag>()->setCreationTime(simTime());

        packet->insertAtFront(head);
        packet->insertAtBack(payload);
        socket.sendTo(packet, broadcast, portApp);

        nextSendMsg = PAST;


}


void Aplicativo::arrivedMessage(Packet *pktArrived){

    const auto& msg = pktArrived->peekData<Mensagem>();
    int idEmissor = msg->getEmissor();
    int rodadaTransmitida = msg->getRodada();

    if (hasGUI()&& (idEmissor != myId)) {
            cModule *parent = getParentModule();
            char text[32];
            sprintf(text, "Recebe Msg de %d", idEmissor);
            parent->bubble(text);
        }

    if((idEmissor != myId) && (roundQueTransmite != rodadaTransmitida)){
        cout << "  -- " << myId << " recebeu mensagem de " << idEmissor <<  " : " << simTime() << "s" <<endl;
        EV_INFO << "++++++++++ " << myId << " recebeu mensagem de " << idEmissor <<  " : " << simTime() << "s ++++++++++" << endl;
    }


}

void Aplicativo::checkIfTransmit(){
    round = round +1;
    if(round  == qtdRounds){ //desse modo o roud possui ciclos de 0 à qtdRounds
        round = 0;
    }

    nextCheckRound = simTime() + slotInterval;
    if(roundQueTransmite == round){
        nextSendMsg = simTime() + slotInterval/2; //a transmissão ocorre na metade do tempo que cada rodada utiliza.
    }
}




