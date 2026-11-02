% SPDX-License-Identifier: Apache-2.0
% Calibration values of DisplayDriver (RTE-03), the only place of these numbers. Read without MATLAB by tools/swc/mdata.py,
% in MATLAB by swc_load_data. Written by tools/gen_swc_ports.py.

DisplayRefresh_cycles = 100;   % [10 ms] the picture is sent again after this time even if the value did not change (a module that lost its power comes back)
DisplayRetry_cycles = 50;   % [10 ms] pause after a failed write order, then the picture starts again with a clear
