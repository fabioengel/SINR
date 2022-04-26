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

#include "inet/physicallayer/wireless/common/base/packetlevel/FlatTransmissionBase.h"
#include "inet/common/ProtocolTag_m.h"
#include "ModelSinrError.h"


//Define_Module(ModelSinrError);

Register_Class(ModelSinrError);

ModelSinrError::ModelSinrError() :
    packetErrorRate(NaN),
    bitErrorRate(NaN),
    symbolErrorRate(NaN)
{
}

void ModelSinrError::initialize(int stage)
{
    ErrorModelBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        packetErrorRate = par("packetErrorRate");
        bitErrorRate = par("bitErrorRate");
        symbolErrorRate = par("symbolErrorRate");

        cModule *receiverModule = getModuleByPath("Network.host[0].wlan[0].radio.receiver");
        double snirThresholdemDB = receiverModule->par("snirThreshold");
        snirThreshold = pow(10, (snirThresholdemDB/10) );

        myId =  cSimulation::getActiveSimulation()->getContextModule()->getParentModule()->getParentModule()->getParentModule()->getParentModule()->getIndex();
    }
}

std::ostream& ModelSinrError::printToStream(std::ostream& stream, int level) const
{
    if (level <= PRINT_LEVEL_TRACE)
        stream << "FreePacketError"
               << "packetErrorRate = " << packetErrorRate
               << "bitErrorRate = " << bitErrorRate
               << "symbolErrorRate = " << symbolErrorRate;
    return stream;
}

double ModelSinrError::computePacketErrorRate(const ISnir *snir, IRadioSignal::SignalPart part) const
{
    Enter_Method_Silent();
    const IReception *reception = snir->getReception();
    double sinrReceived = snir->getMin();

    int componentId = getId();
    cModule *parent = getParentModule();

    if(sinrReceived >= snirThreshold){
        return 0;
    }
    else{
        return 1;
    }
}

double ModelSinrError::computeBitErrorRate(const ISnir *snir, IRadioSignal::SignalPart part) const
{
    Enter_Method_Silent();
    if (!std::isnan(bitErrorRate)) {
        return bitErrorRate;
    }
    else {
        cout <<"BER" << endl;
        return 0;
    }
}

double ModelSinrError::computeSymbolErrorRate(const ISnir *snir, IRadioSignal::SignalPart part) const
{
    Enter_Method_Silent();
    return symbolErrorRate;
}


Packet *ModelSinrError::computeCorruptedPacket(const Packet *packet, double ber) const
{
    bool isCorrupted = false;
    Packet *corruptedPacket = nullptr;
    // TODO: this while loop looks bad, but we don't have any other chance now, because the decision whether the reception is successful or not has been already made
    while (!isCorrupted) {
        switch (corruptionMode) {
            case CorruptionMode::CM_PACKET:
                corruptedPacket = corruptPacket(packet, isCorrupted); break;
            case CorruptionMode::CM_CHUNK:
                corruptedPacket = corruptChunks(packet, ber, isCorrupted); break;
            case CorruptionMode::CM_BYTE:
                corruptedPacket = corruptBytes(packet, ber, isCorrupted); break;
            case CorruptionMode::CM_BIT:
                corruptedPacket = corruptBits(packet, ber, isCorrupted); break;
            default:
                throw cRuntimeError("Unknown corruption mode");
        }
        if (!isCorrupted)
            delete corruptedPacket;
    }
    return corruptedPacket;
}

Packet *ModelSinrError::computeCorruptedPacket(const ISnir *snir) const
{
    auto transmittedPacket = snir->getReception()->getTransmission()->getPacket();
    auto ber = computeBitErrorRate(snir, IRadioSignal::SIGNAL_PART_WHOLE);
    auto receivedPacket = computeCorruptedPacket(transmittedPacket, ber);

    receivedPacket->clearTags();
    receivedPacket->addTag<PacketProtocolTag>()->setProtocol(transmittedPacket->getTag<PacketProtocolTag>()->getProtocol());
    return receivedPacket;
}

