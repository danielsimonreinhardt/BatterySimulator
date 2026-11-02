% SPDX-License-Identifier: Apache-2.0
% Calibration values of HandControl (RTE-03), the only place of these numbers. Read without MATLAB by tools/swc/mdata.py,
% in MATLAB by swc_load_data. Written by tools/gen_swc_ports.py.

SetpointLimit_V = 5.0;   % [V]    upper setpoint limit, bound to BattSimParams.CellVoltLimit_V in rte.yaml
CoarseStep_mV = 15;   % [mV]   setpoint step per detent, coarse (derived from EncoderGainCoarse_P = 30 of the old firmware, to be measured at the device)
FineStep_mV = 1;   % [mV]   setpoint step per detent, fine (derived from EncoderGainFine_P = 2, to be measured at the device)
CountsPerStep = 4;   % [-]    counts of the quadrature decoder per detent (x4 decoding)
KeyDebounce_cycles = 3;   % [10 ms] a key level counts after this many equal samples
