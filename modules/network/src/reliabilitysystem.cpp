#include "reliabilitysystem.h"

ReliabilitySystem::ReliabilitySystem(uint32_t max_sequence) {
    mMaxSequence = max_sequence;
    reset();
}

void ReliabilitySystem::reset() {
    mLocalSequence      = 0;
    mRemoteSequence     = 0;

    mSentPackets        = 0;
    mRecvPackets        = 0;
    mLostPackets        = 0;
    mAckedPackets       = 0;
    mSentBandwidth      = 0.0f;
    mAckedBandwidth     = 0.0f;
    mRtt                = 0.0f;
    mRttMaximum         = 1.0f;

    mSentQueue.clear();
    mReceivedQueue.clear();
    mPendingAckQueue.clear();
    mAckedQueue.clear();
}

void ReliabilitySystem::packetSent(int size) {
    PacketData data;
    data.sequence = mLocalSequence;
    data.time = 0.0f;
    data.size = size;

    mSentQueue.push_back(data);
    mPendingAckQueue.push_back(data);

    mSentPackets++;
    mLocalSequence++;
    if(mLocalSequence > mMaxSequence) {
        mLocalSequence = 0;
    }
}

void ReliabilitySystem::packetReceived(uint32_t sequence, int size) {
    mRecvPackets++;

    for(auto it = mReceivedQueue.begin(); it != mReceivedQueue.end(); ++it) {
        if(it->sequence == sequence) {
            return;
        }
    }

    PacketData data;
    data.sequence = sequence;
    data.time = 0.0f;
    data.size = size;

    mReceivedQueue.push_back(data);

    if(sequenceMoreRecent(sequence, mRemoteSequence, mMaxSequence)) {
        mRemoteSequence = sequence;
    }
}

uint32_t ReliabilitySystem::generateAckBits() {
    return generateAckBits(remoteSequence(), mReceivedQueue, mMaxSequence);
}

void ReliabilitySystem::processAck(uint32_t ack, uint32_t bits) {
    processAck(ack, bits, mPendingAckQueue, mAckedQueue, mAcks, mAckedPackets, mRtt, mMaxSequence);
}

void ReliabilitySystem::update(float dt) {
    mAcks.clear();
    advanceQueueTime(dt);
    updateQueues();
    updateStats();
}

void ReliabilitySystem::validate() {
    verifySorted(mSentQueue,        mMaxSequence);
    verifySorted(mReceivedQueue,    mMaxSequence);
    verifySorted(mPendingAckQueue,  mMaxSequence);
    verifySorted(mAckedQueue,       mMaxSequence);
}

// utility functions

bool ReliabilitySystem::sequenceMoreRecent(uint32_t s1, uint32_t s2, uint32_t max_sequence) {
    return ((s1 > s2) && (s1 - s2 <= max_sequence / 2)) || ((s2 > s1) && (s2 - s1 > max_sequence / 2));
}

int ReliabilitySystem::bitIndexForSequence(uint32_t sequence, uint32_t ack, uint32_t max_sequence) {
    if(sequence > ack) {
        return ack + (max_sequence - sequence);
    } else {
        return ack - 1 - sequence;
    }
}

uint32_t ReliabilitySystem::generateAckBits(uint32_t ack, const PacketQueue &received, uint32_t max_sequence) {
    uint32_t ack_bits = 0;
    for(auto it = received.begin(); it != received.end(); ++it) {
        if(it->sequence == ack || sequenceMoreRecent(it->sequence, ack, max_sequence)) {
            break;
        }
        int bit_index = bitIndexForSequence(it->sequence, ack, max_sequence);
        if(bit_index <= 31) {
            ack_bits |= 1 << bit_index;
        }
    }
    return ack_bits;
}

void ReliabilitySystem::processAck(uint32_t ack, uint32_t ack_bits, PacketQueue &pending, PacketQueue &acked,
                                   vector<uint32_t> &acks, uint32_t &acked_packets, float &rtt, uint32_t max_sequence) {
    if(pending.empty()) {
        return;
    }

    auto it = pending.begin();
    while(it != pending.end()) {
        bool ackedFlag  = false;

        if(it->sequence == ack) {
            ackedFlag   = true;
        } else if(!sequenceMoreRecent(it->sequence, ack, max_sequence)) {
            int bit_index = bitIndexForSequence(it->sequence, ack, max_sequence);
            if(bit_index <= 31) {
                ackedFlag   = (ack_bits >> bit_index) & 1;
            }
        }

        if(ackedFlag) {
            rtt += (it->time - rtt) * 0.1f;

            insertSorted(acked, *it, max_sequence);
            acks.push_back(it->sequence);
            acked_packets++;
            it = pending.erase(it);
        } else {
            ++it;
        }
    }
}

void ReliabilitySystem::verifySorted(PacketQueue &list, uint32_t max_sequence) {
    auto prev = list.end();
    for(auto it = list.begin(); it != list.end(); ++it) {
        if(prev != list.end()) {
            prev = it;
        }
    }
}

void ReliabilitySystem::insertSorted(PacketQueue &list, const PacketData &p, uint32_t max_sequence) {
    if(list.empty()) {
        list.push_back(p);
    } else {
        if(!sequenceMoreRecent(p.sequence, list.front().sequence, max_sequence)) {
            list.push_front(p);
        } else if(sequenceMoreRecent(p.sequence, list.back().sequence, max_sequence)) {
            list.push_back(p);
        } else {
            for(auto it = list.begin(); it != list.end(); ++it) {
                if(sequenceMoreRecent(it->sequence, p.sequence, max_sequence)) {
                    list.insert(it, p);
                    break;
                }
            }
        }
    }
}

void ReliabilitySystem::advanceQueueTime(float dt) {
    for(auto it = mSentQueue.begin(); it != mSentQueue.end(); ++it) {
        it->time += dt;
    }
    for(auto it = mReceivedQueue.begin(); it != mReceivedQueue.end(); ++it) {
        it->time += dt;
    }
    for(auto it = mPendingAckQueue.begin(); it != mPendingAckQueue.end(); ++it) {
        it->time += dt;
    }
    for(auto it = mAckedQueue.begin(); it != mAckedQueue.end(); ++it) {
        it->time += dt;
    }
}

void ReliabilitySystem::updateQueues() {
    const float epsilon = 0.001f;

    while(mSentQueue.size() && mSentQueue.front().time > mRttMaximum + epsilon) {
        mSentQueue.pop_front();
    }
    if(mReceivedQueue.size()) {
        const uint32_t latest_sequence = mReceivedQueue.back().sequence;
        const uint32_t minimum_sequence = latest_sequence >= 34 ? (latest_sequence - 34) : mMaxSequence - (34 - latest_sequence);
        while(mReceivedQueue.size() && !sequenceMoreRecent(mReceivedQueue.front().sequence, minimum_sequence, mMaxSequence)) {
            mReceivedQueue.pop_front();
        }
    }

    while(mAckedQueue.size() && mAckedQueue.front().time > mRttMaximum * 2 - epsilon) {
        mAckedQueue.pop_front();
    }
    while(mPendingAckQueue.size() && mPendingAckQueue.front().time > mRttMaximum + epsilon) {
        mPendingAckQueue.pop_front();
        mLostPackets++;
    }
}

void ReliabilitySystem::updateStats() {
    int sent_bytes_per_second = 0;
    for(auto it = mSentQueue.begin(); it != mSentQueue.end(); ++it) {
        sent_bytes_per_second += it->size;
    }
    int acked_packets_per_second = 0;
    int acked_bytes_per_second = 0;
    for(auto it = mAckedQueue.begin(); it != mAckedQueue.end(); ++it) {
        if(it->time >= mRttMaximum) {
            acked_packets_per_second++;
            acked_bytes_per_second += it->size;
        }
    }
    sent_bytes_per_second /= mRttMaximum;
    acked_bytes_per_second /= mRttMaximum;
    mSentBandwidth = sent_bytes_per_second * (8 / 1000.0f);
    mAckedBandwidth = acked_bytes_per_second * (8 / 1000.0f);
}
