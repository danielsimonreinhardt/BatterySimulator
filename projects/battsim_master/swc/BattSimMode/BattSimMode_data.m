% SPDX-License-Identifier: Apache-2.0
% Calibration values of BattSimMode (RTE-03), the only place of these numbers. Read without MATLAB by tools/swc/mdata.py,
% in MATLAB by swc_load_data. Written by tools/gen_swc_ports.py.

VersionMajor = 0;   % firmware version of 0x121
VersionMinor = 5;   % firmware version of 0x121 (0.5.0: hand mode STANDALONE, HandControl and DisplayDriver in the task; 0.4.0: the calibration identifier 0x0201 holds all 14 channels, vector ports, RTE-57; 0.3.4: initial values of the calibration ports, RTE-56; 0.3.3: stage C, plausibility check of the current measurement; 0.3.2: regulator hold 500 ms; 0.2.0 was stage B)
VersionPatch = 6;   % firmware version of 0x121 (0.5.6: the duty (setpoint PWM) is 0 for every channel that is not released, a second shutdown independent of the ENABLE path; 0.5.5: hand mode jumps over the gap 0 V .. HandMinSetpoint_V, the display shows a filtered actual value (PT1 + hysteresis); 0.5.4: display order counter wraps at 255 instead of saturating (the picture froze after 255 bytes); 0.5.3: neXtBSW a18a1e4d, FlexIO I2C muxes SDA/SCL itself (IO-129), display lights; 0.5.2: display bus 9600 Hz and own low-priority task disp; 0.5.1: SCL pull-up, 31.25 kHz, neXtBSW 23642a8d)
ButtonDebounce_cycles = 3;   % [10 ms] a key level counts after this many equal samples
