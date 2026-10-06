#ifndef ARM_CHI_TRAFFIC_GENERATOR_H
#define ARM_CHI_TRAFFIC_GENERATOR_H

#include <ARM/TLM/arm_chi.h>

#include "CHIUtilities.h"

#include <map>

namespace ARM {
namespace CHI {
namespace Examples {

class CHITrafficGenerator : public sc_core::sc_module
{
protected:
    SC_HAS_PROCESS(CHITrafficGenerator);

    CHIChannelState channels[CHI_NUM_CHANNELS];
    unsigned data_width_bytes;
    uint16_t src_id;
    uint16_t tgt_id;
    uint16_t txn_id = 0;

    /* Read data beats received so far, by TxnID, for reads not yet complete. */
    std::map<uint16_t, unsigned> read_beats_received;
    unsigned completed = 0;

    void clock_posedge();
    void clock_negedge();

    void handle_dbid_resp(const CHIFlit& dbid_flit);
    void handle_read_data(const CHIFlit& dat_flit);

    tlm::tlm_sync_enum nb_transport_bw(ARM::CHI::Payload& payload, ARM::CHI::Phase& phase);

public:
    /* src_id and tgt_id are the node IDs every request is sent from and to. */
    explicit CHITrafficGenerator(const sc_core::sc_module_name& name, unsigned data_width_bits = 128,
                                 uint16_t src_id = 1, uint16_t tgt_id = 2);

    /* Add a payload to the traffic queue. */
    void add_payload(ARM::CHI::ReqOpcode req_opcode, uint64_t address, ARM::CHI::Size size);

    /* Transactions completed: a read on its last data beat, a write on its Comp. */
    unsigned completed_transactions() const { return completed; }

    ARM::CHI::SimpleInitiatorSocket<CHITrafficGenerator> initiator;

    sc_core::sc_in<bool> clock;
};

} // namespace Examples
} // namespace CHI
} // namespace ARM

#endif // ARM_CHI_TRAFFIC_GENERATOR_H
