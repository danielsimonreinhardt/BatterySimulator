% SPDX-License-Identifier: Apache-2.0
% Calibration values of BattSimParams (RTE-03, RTE-47), the only place of these numbers. Read without MATLAB by
% tools/swc/mdata.py, in MATLAB by swc_load_data. The constant CellCount stands in swc.yaml (RTE-48).

CellVoltLimit_V = 5.0;   % [V]    upper setpoint limit of every cell channel (stage B concept: default 5.0 V; the 4.30 V limit stays in LabControl)
