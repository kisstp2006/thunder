#ifndef RELIABILITYSYSTEM_H
#define RELIABILITYSYSTEM_H

#include <vector>
#include <list>
#include <cstdint>

// reliability system to support reliable connection
// manages sent, received, pending ack and acked packet queues

using namespace std;

struct PacketData {
    uint32_t        sequence;            // packet sequence number
    float           time;                // time offset since packet was sent or received (depending on context)
    int             size;                // packet size in bytes
};

inline bool sequenceMoreRecent(uint32_t s1, uint32_t s2, uint32_t max_sequence) {
    return ((s1 > s2) && (s1 - s2 <= max_sequence / 2)) || ((s2 > s1) && (s2 - s1 > max_sequence / 2));
}

typedef list<PacketData> PacketQueue;

class ReliabilitySystem {
public:

    ReliabilitySystem(uint32_t max_sequence = 0xFFFFFFFF);

    void            reset               ();

    void            packetSent          (int size);

    void            packetReceived      (uint32_t sequence, int size);

    uint32_t        generateAckBits     ();

    void            processAck          (uint32_t ack, uint32_t bits);

    void            update              (float dt);

    void            validate            ();

    // utility functions

    static bool     sequenceMoreRecent  (uint32_t s1, uint32_t s2, uint32_t max_sequence);

    static int      bitIndexForSequence (uint32_t sequence, uint32_t ack, uint32_t max_sequence);

    static uint32_t generateAckBits     (uint32_t ack, const PacketQueue &received, uint32_t max_sequence);

    static void     processAck          (uint32_t ack, uint32_t ack_bits,
                                         PacketQueue &pending, PacketQueue &acked,
                                         vector<uint32_t> &acks, uint32_t &acked_packets,
                                         float &rtt, uint32_t max_sequence);

    // data accessors

    uint32_t        localSequence       () const { return mLocalSequence; }

    uint32_t        remoteSequence      () const { return mRemoteSequence; }

    uint32_t        maxSequence         () const { return mMaxSequence; }

    void            acks                (uint32_t **acks, int &count) {
        *acks = &mAcks[0];
        count = (int) mAcks.size();
    }

    uint32_t        sentPackets         () const { return mSentPackets; }

    uint32_t        receivedPackets     () const { return mRecvPackets; }

    uint32_t        lostPackets         () const { return mLostPackets; }

    uint32_t        ackedPackets        () const { return mAckedPackets; }

    float           sentBandwidth       () const { return mSentBandwidth; }

    float           ackedBandwidth      () const { return mAckedBandwidth; }

    float           roundTripTime       () const { return mRtt; }

    int             headerSize          () const { return 12; }

protected:

    void            advanceQueueTime    (float dt);

    void            updateQueues        ();

    void            updateStats         ();

    static void     verifySorted        (PacketQueue &list, uint32_t max_sequence);
    static void     insertSorted        (PacketQueue &list, const PacketData &p, uint32_t max_sequence);

private:
    uint32_t        mMaxSequence;       // maximum sequence value before wrap around (used to test sequence wrap at low # values)
    uint32_t        mLocalSequence;     // local sequence number for most recently sent packet
    uint32_t        mRemoteSequence;    // remote sequence number for most recently received packet

    uint32_t        mSentPackets;       // total number of packets sent
    uint32_t        mRecvPackets;       // total number of packets received
    uint32_t        mLostPackets;       // total number of packets lost
    uint32_t        mAckedPackets;      // total number of packets acked

    float           mSentBandwidth;     // approximate sent bandwidth over the last second
    float           mAckedBandwidth;    // approximate acked bandwidth over the last second
    float           mRtt;               // estimated round trip time
    float           mRttMaximum;        // maximum expected round trip time (hard coded to one second for the moment)

    vector<uint32_t> mAcks;             // acked packets from last set of packet receives. cleared each update!

    PacketQueue    mSentQueue;         // sent packets used to calculate sent bandwidth (kept until rtt_maximum)
    PacketQueue    mPendingAckQueue;   // sent packets which have not been acked yet (kept until rtt_maximum * 2 )
    PacketQueue    mReceivedQueue;     // received packets for determining acks to send (kept up to most recent recv sequence - 32)
    PacketQueue    mAckedQueue;        // acked packets (kept until rtt_maximum * 2)
};

#endif // RELIABILITYSYSTEM_H
