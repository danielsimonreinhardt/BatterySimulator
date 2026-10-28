% SPDX-License-Identifier: Apache-2.0
% Calibration values of CellChannels (RTE-03), the only place of these numbers. Read without MATLAB by tools/swc/mdata.py,
% in MATLAB by swc_load_data. Written by stufeb_gen.py.

VoltageSetValue_Ax = [0 1 2 3 4 5];   % [V]    setpoint breakpoints (VoltageSetValue_Ax of the old firmware)
VoltageSetPwm_T = [0 0.185875 0.378085 0.570892 0.763797 0.9566];   % [-]    duty (share of the period) at the breakpoints (VoltageSetPwm_T, 5 V = 0.9566)
VoltageSenseMeas_Ax = [0.0 398.1 795.7 1195.8 1596.4 1996.0];   % [mV]   ADC pin voltage at the breakpoints: counts [0 796 1591 2391 3192 3991] of the old firmware (12 bit) x 2048 mV / 4095
VoltageSenseValue_T = [0 1 2 3 4 5];   % [V]    cell voltage at the breakpoints (VoltageSenseValue_T)
SetpointLimit_V = 5.0;   % [V]    upper setpoint limit (stage B concept: default 5.0 V; the 4.30 V limit stays in LabControl)
VoltageSenseFilter = 0.1;   % [-]    low-pass weight of the actual value per 10 ms (time constant about 95 ms)
