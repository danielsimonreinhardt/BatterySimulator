% SPDX-License-Identifier: Apache-2.0
function CellChannels_create()
%CELLCHANNELS_CREATE Creates CellChannels.slx from swc.yaml with the logic below (BattSimFW, Stufe B).
%   The model is made once; afterwards CellChannels.slx is the master and is edited in Simulink. Running this function
%   again starts over (swc_create with Overwrite); generate the code afterwards with swc_build. The RTE constants (QLF_*)
%   come from the generated Rte_Qlf.m: set NX_RTE_GENERATED to build/<preset>/generated/rte of a configured project.
%
%   Runnable CellTask10ms, 14 channels as vectors (FW-02, FW-15, FW-16):
%     - Setpoint: limited to 0 .. SetpointLimit_V (above it the channel reports status 3 "setpoint error"), curve
%       VoltageSetValue_Ax -> VoltageSetPwm_T (the table of the old firmware, 5 V = 0.9566) gives the duty, in 0.01 %
%       for the PWM target. The duty is 0 while OutputEnable is off (STANDBY, STOP, FAIL): setpoints are 0 V there, held
%       CAN values do not act (FW-16). The old firmware had no regulator in the loop (VoltageControllerEnable_P false);
%       the regulator comes in stage C.
%     - Release: the ENABLE output of a channel is on when OutputEnable is on and CELLn_ENABLE = 2 (0 and 1 are off).
%     - Actual value: curve VoltageSenseMeas_Ax (mV at the ADC pin) -> VoltageSenseValue_T (V), first-order low-pass
%       VoltageSenseFilter per 10 ms; only samples with a valid ADC qualifier count.
%     - Status CELLn_STATUS as in the DBC: 2 enabled, 1 disabled, 3 setpoint error while enabled (as the old firmware
%       it follows the command, not OutputEnable). Channels 15 and 16 are not fitted: status 1, voltage 0, as before.
here = fileparts(mfilename('fullpath'));
root = fileparts(fileparts(fileparts(fileparts(here))));
addpath(fullfile(root, 'nextbsw', 'tools', 'swc'), fileparts(here));
swc_load_qlf();
swc_create(here, 'Overwrite', true, 'Logic', @logic);
end

function logic(mdl, ~)
sub = swc_runnable_subsystem(mdl, 'CellTask10ms');
n = 14;
names = @(fmt) arrayfun(@(k) sprintf(fmt, k), 1:n, 'UniformOutput', false);
swc_core(sub, 'CellCore', coreScript(), ...
    {{'mux', names('Cell%02dVoltSet_V')}, {'mux', names('Cell%02dEnable')}, {'mux', names('UCell%02d_mV')}, ...
     {'mux', names('UCell%02dQlf')}, 'OutputEnable'}, ...
    {{'VoltageSetValue_Ax', 'single'}, {'VoltageSetPwm_T', 'single'}, {'VoltageSenseMeas_Ax', 'single'}, ...
     {'VoltageSenseValue_T', 'single'}, {'SetpointLimit_V', 'single'}, {'VoltageSenseFilter', 'single'}, ...
     {'QLF_OK', 'uint8'}}, ...
    {{'demux', names('En%02d')}, {'demux', names('Cell%02dVoltAct_V')}, {'demux', names('Cell%02dState')}, ...
     {'demux', names('Duty%02d')}});
% channels 15 and 16 are not fitted: status "disabled" (voltage 0 is the placeholder of the port), as the old firmware
for ch = [15 16]
    add_block('simulink/Sources/Constant', sprintf('%s/Cell%dState_Const', sub, ch), ...
        'Position', [30, 900 + 40 * (ch - 14), 150, 924 + 40 * (ch - 14)], 'Value', '1', 'OutDataTypeStr', 'uint8');
    add_line(sub, sprintf('Cell%dState_Const/1', ch), sprintf('Cell%dState/1', ch), 'autorouting', 'on');
end
end

function code = coreScript()
lines = {
'function [en, vAct, state, duty] = CellCore(setV, enable, mV, qlf, outEn, setAx, setT, senseAx, senseT, limitV, filt, QlfOk)'
'%CELLCORE 14 cell channels: setpoint -> duty, release, actual value, status (see CellChannels_create.m).'
'%#codegen'
'persistent y seen'
'if isempty(y)'
'    y = zeros(14, 1, ''single'');'
'    seen = false(14, 1);'
'end'
'en = false(14, 1);'
'vAct = zeros(14, 1, ''single'');'
'state = zeros(14, 1, ''uint8'');'
'duty = zeros(14, 1, ''uint16'');'
'for k = 1:14'
'    commanded = (enable(k) == uint8(2));'
'    v = setV(k);'
'    clipped = (v > limitV);'
'    if clipped'
'        v = limitV;'
'    end'
'    if ~(v > single(0))'
'        v = single(0);'
'    end'
'    if outEn'
'        d = interp1d(setAx, setT, v) * single(10000);'
'        d = min(max(d, single(0)), single(10000));'
'        duty(k) = uint16(floor(d + single(0.5)));'
'    end'
'    en(k) = outEn && commanded;'
'    if qlf(k) == QlfOk'
'        mv = interp1d(senseAx, senseT, single(mV(k)));'
'        if seen(k)'
'            y(k) = y(k) + filt * (mv - y(k));'
'        else'
'            y(k) = mv;'
'            seen(k) = true;'
'        end'
'    end'
'    vAct(k) = y(k);'
'    if ~commanded'
'        state(k) = uint8(1);'
'    elseif clipped'
'        state(k) = uint8(3);'
'    else'
'        state(k) = uint8(2);'
'    end'
'end'
''
'function y = interp1d(ax, tab, v)'
'% linear interpolation, constant outside the breakpoints'
'n = numel(ax);'
'y = tab(n);'
'if v <= ax(1)'
'    y = tab(1);'
'else'
'    for i = 1:n - 1'
'        if v <= ax(i + 1)'
'            y = tab(i) + (tab(i + 1) - tab(i)) * (v - ax(i)) / (ax(i + 1) - ax(i));'
'            break;'
'        end'
'    end'
'end'
};
code = strjoin(lines, newline);
end
