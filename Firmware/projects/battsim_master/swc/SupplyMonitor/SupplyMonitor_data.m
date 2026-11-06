% SPDX-License-Identifier: Apache-2.0
% Calibration values of SupplyMonitor (RTE-03), the only place of these numbers. Read without MATLAB by tools/swc/mdata.py,
% in MATLAB by swc_load_data. Written by tools/gen_swc_ports.py.

SupplyCurrentFilter = 0.0078125;   % [-]   low-pass weight per 10 ms (1/128, exact in single): time constant about 1.3 s, as 0.08 per 100 ms of the old firmware
SupplyCurrentGain = 1.0;   % [mA/mV] INA181A1 on 50 mOhm: 1 V per A, i.e. 1 mA per mV at the ADC pin
OverloadThd_mA = 1280;   % [mA]  overload above this current (IRM-60-48: 1.25 A continuous)
OverloadHyst_mA = 200;   % [mA]  overload ends below OverloadThd_mA - this value (1.08 A)
MinCurrent_mA = 100;   % [mA]  plausibility of the current measurement: with the output on and a channel released, a sample below this value is implausible; the quiescent current of the supply measured at the device is 216 mA, 270 mA with 14 channels (30.10.2026)
MinCurrentTime_cycles = 20;   % [10 ms] number of implausibly low samples in a row that count as a failure of the current measurement (20 = 200 ms, review bs-c1-strommessung S-1 a)
CurrentInvalid_cycles = 20;   % [10 ms] number of invalid samples of the current measurement in a row that count as a failure (20 = 200 ms, review C-1)
