//
// Generated file, do not edit! Created by nedtool 6.0 from src/message/Mensagem.msg.
//

#ifndef __INET_MENSAGEM_M_H
#define __INET_MENSAGEM_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif


namespace inet {

class Mensagem;
}  // namespace inet

#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs

#include "inet/common/packet/chunk/Chunk_m.h" // import inet.common.packet.chunk.Chunk


namespace inet {

/**
 * Class generated from <tt>src/message/Mensagem.msg:20</tt> by nedtool.
 * <pre>
 * class Mensagem extends FieldsChunk
 * {
 *     int emissor \@packetData;
 *     int receptor \@packetData;
 *     int rodada \@packetData;
 * }
 * </pre>
 */
class Mensagem : public ::inet::FieldsChunk
{
  protected:
    int emissor = 0;
    int receptor = 0;
    int rodada = 0;

  private:
    void copy(const Mensagem& other);

  protected:
    bool operator==(const Mensagem&) = delete;

  public:
    Mensagem();
    Mensagem(const Mensagem& other);
    virtual ~Mensagem();
    Mensagem& operator=(const Mensagem& other);
    virtual Mensagem *dup() const override {return new Mensagem(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getEmissor() const;
    virtual void setEmissor(int emissor);
    virtual int getReceptor() const;
    virtual void setReceptor(int receptor);
    virtual int getRodada() const;
    virtual void setRodada(int rodada);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Mensagem& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Mensagem& obj) {obj.parsimUnpack(b);}

}  // namespace inet


namespace omnetpp {

template<> inline inet::Mensagem *fromAnyPtr(any_ptr ptr) { return check_and_cast<inet::Mensagem*>(ptr.get<cObject>()); }
}  // namespace omnetpp

#endif // ifndef __INET_MENSAGEM_M_H

