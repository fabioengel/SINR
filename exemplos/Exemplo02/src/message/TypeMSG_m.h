//
// Generated file, do not edit! Created by nedtool 6.0 from src/message/TypeMSG.msg.
//

#ifndef __INET_TYPEMSG_M_H
#define __INET_TYPEMSG_M_H

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

class TypeMSG;
}  // namespace inet

#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs

#include "inet/common/packet/chunk/Chunk_m.h" // import inet.common.packet.chunk.Chunk


namespace inet {

/**
 * Class generated from <tt>src/message/TypeMSG.msg:20</tt> by nedtool.
 * <pre>
 * class TypeMSG extends FieldsChunk
 * {
 *     int type;
 * }
 * </pre>
 */
class TypeMSG : public ::inet::FieldsChunk
{
  protected:
    int type = 0;

  private:
    void copy(const TypeMSG& other);

  protected:
    bool operator==(const TypeMSG&) = delete;

  public:
    TypeMSG();
    TypeMSG(const TypeMSG& other);
    virtual ~TypeMSG();
    TypeMSG& operator=(const TypeMSG& other);
    virtual TypeMSG *dup() const override {return new TypeMSG(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getType() const;
    virtual void setType(int type);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const TypeMSG& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, TypeMSG& obj) {obj.parsimUnpack(b);}

}  // namespace inet


namespace omnetpp {

template<> inline inet::TypeMSG *fromAnyPtr(any_ptr ptr) { return check_and_cast<inet::TypeMSG*>(ptr.get<cObject>()); }
}  // namespace omnetpp

#endif // ifndef __INET_TYPEMSG_M_H
