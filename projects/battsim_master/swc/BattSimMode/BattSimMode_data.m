% SPDX-License-Identifier: Apache-2.0
% Calibration values of BattSimMode (RTE-03), the only place of these numbers. Read without MATLAB by tools/swc/mdata.py,
% in MATLAB by swc_load_data. Written by tools/gen_swc_ports.py.

VersionMajor = 0;   % firmware version of 0x121
VersionMinor = 3;   % firmware version of 0x121 (0.3.4: initial values of the calibration ports, RTE-56; 0.3.3: stage C, plausibility check of the current measurement; 0.3.2: regulator hold 500 ms; 0.2.0 was stage B)
VersionPatch = 4;   % firmware version of 0x121
ButtonDebounce_cycles = 3;   % [10 ms] a key level counts after this many equal samples
