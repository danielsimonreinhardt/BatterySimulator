% SPDX-License-Identifier: Apache-2.0
% Calibration values of CellCal (RTE-03), the only place of these numbers. Read without MATLAB by tools/swc/mdata.py,
% in MATLAB by swc_load_data. Written by tools/gen_swc_ports.py.

OffsetLimit_code = 2000;   % [0.1 mV] largest offset the component accepts (0.2 V); the diagnosis checks the same range
GainMin_code = 9000;   % [1e-4] smallest gain the component accepts (0.9)
GainMax_code = 11000;   % [1e-4] largest gain the component accepts (1.1)
