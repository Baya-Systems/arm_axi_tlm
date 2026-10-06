# arm_tlm

Public repository to make ARM TLM 2.0 available for use with CMake

## Targets

| Target | What it is |
| --- | --- |
| `ARM::armtlmchi`, `ARM::armtlmaxi4`, `ARM::arm_tlm` | ARM's AMBA TLM libraries |
| `ARM::armtlmchi_examples` | ARM's CHI example traffic generator, memory and monitor |
| `ARM::armtlmaxi4_examples` | ARM's AXI example traffic generator, memory, monitor and transactors |

The example classes live in `ARM::CHI::Examples` and `ARM::AXI4::Examples`, and their
headers are included as `<ARM/TLM/examples/chi/...>` and `<ARM/TLM/examples/axi/...>`.

## Changes from ARM's examples

- Every example class is in a namespace, so a consumer can define its own classes
  of the same names.
- `CHITrafficGenerator` takes the node IDs it sends from and to, and counts the
  transactions it has seen complete.
- `CHIMemory` counts write data beats correctly for a write wider than the data
  bus, finds a write's DBID in the data's TxnID, and answers the write's Comp with
  the request's TxnID.
