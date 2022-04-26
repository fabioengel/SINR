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

#ifndef SRC_ERRORMODEL_MODELSINRERROR_H_
#define SRC_ERRORMODEL_MODELSINRERROR_H_
#include "inet/physicallayer/wireless/common/base/packetlevel/ErrorModelBase.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/physicallayer/wireless/common/contract/packetlevel/IErrorModel.h"
#include <omnetpp.h>
#include <iostream>


using namespace omnetpp;
using namespace inet;
using namespace std;
using namespace physicallayer;

class ModelSinrError: public ErrorModelBase {
protected:
  double packetErrorRate;
  double bitErrorRate;
  double symbolErrorRate;
  double snirThreshold;

  int myId;

protected:
  virtual void initialize(int stage) override;

public:
  ModelSinrError();

  virtual std::ostream& printToStream(std::ostream& stream, int level) const;

  virtual double computePacketErrorRate(const ISnir *snir, IRadioSignal::SignalPart part) const override;
  virtual double computeBitErrorRate(const ISnir *snir, IRadioSignal::SignalPart part) const override;
  virtual double computeSymbolErrorRate(const ISnir *snir, IRadioSignal::SignalPart part) const override;

  virtual Packet *computeCorruptedPacket(const Packet *packet, double ber) const override;
  virtual Packet *computeCorruptedPacket(const ISnir *snir) const override;

};

#endif /* SRC_ERRORMODEL_MODELSINRERROR_H_ */
