% SPDX-License-Identifier: Apache-2.0
% Calibration values of HandControl (RTE-03), the only place of these numbers. Read without MATLAB by tools/swc/mdata.py,
% in MATLAB by swc_load_data. Written by tools/gen_swc_ports.py.

SetpointLimit_V = 5.0;   % [V]    upper setpoint limit, bound to BattSimParams.CellVoltLimit_V in rte.yaml
CoarseStep_mV = 15;   % [mV]   setpoint step per detent, coarse (derived from EncoderGainCoarse_P = 30 of the old firmware, to be measured at the device)
FineStep_mV = 1;   % [mV]   setpoint step per detent, fine (derived from EncoderGainFine_P = 2, to be measured at the device)
CountsPerStep = 4;   % [-]    counts of the quadrature decoder per detent (x4 decoding)
KeyDebounce_cycles = 3;   % [10 ms] a key level counts after this many equal samples
HandMinSetpoint_V = 0.7;   % [V]    lowest setpoint above 0 V that the voltage encoder sets: the first detent up from 0 V jumps to this value, a detent down from it to 0 V (0 = off); the output stage stands at about 0.7 V without load (OutputFloor_V), the gap below is only dead turning
DisplayFilter_cycles = 30;   % [10 ms] time constant of the PT1 filter of the displayed actual value (1 = no filter)
DisplayHyst_mV = 3;   % [mV]   the displayed value changes only when the filtered value is this far away from it (0 = off)
DisplayJump_mV = 20;   % [mV]   an actual value this far from the filtered one (or a new channel) is shown at once, without filter
