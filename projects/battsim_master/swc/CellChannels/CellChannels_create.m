% SPDX-License-Identifier: Apache-2.0
function CellChannels_create()
%CELLCHANNELS_CREATE Creates CellChannels.slx from swc.yaml with the logic below (BattSimFW, Stufe B and C).
%   The model is made once; afterwards CellChannels.slx is the master and is edited in Simulink. Running this function
%   again starts over (swc_create with Overwrite); generate the code afterwards with swc_build. The RTE constants (QLF_*)
%   come from the generated Rte_Qlf.m: set NX_RTE_GENERATED to build/<preset>/generated/rte of a configured project.
%
%   Runnable CellTask10ms, 14 channels as vectors (FW-02, FW-06, FW-07, FW-15, FW-16):
%     - Setpoint: limited to 0 .. SetpointLimit_V (above it the channel reports status 3 "setpoint error"). Regulator
%       correction and channel calibration (below) are added to the limited setpoint: command = SetGain * (setpoint +
%       correction) + SetOffset, the sum limited to 0 .. SetpointLimit_V before the calibration. Curve VoltageSetValue_Ax ->
%       VoltageSetPwm_T (the table of the old firmware, 5 V = 0.9566) gives the duty, in 0.01 % for the PWM target. The
%       duty is 0 while OutputEnable is off (STANDBY, STOP, FAIL) and for a setpoint of 0 V: setpoints are 0 V there, held
%       CAN values do not act (FW-16), and the calibration (two points, 1 V and 4 V) does not claim to reach 0 V.
%     - Release: the ENABLE output of a channel is on when OutputEnable is on and CELLn_ENABLE = 2 (0 and 1 are off).
%     - Actual value: curve VoltageSenseMeas_Ax (mV at the ADC pin) -> VoltageSenseValue_T (V), channel calibration
%       (MeasGain * value + MeasOffset), first-order low-pass VoltageSenseFilter per 10 ms; only samples with a valid ADC
%       qualifier count. A calibration pair outside 0.5 .. 1.5 (gain) or +-0.5 V (offset) - which CellCal never delivers,
%       but the port is zero before its first run - counts as the factory value.
%     - Regulator (FW-07, stage C): PI per channel, 10 ms, correction limited to +-CtrlLimit_V with the integral part
%       clamped likewise (anti-windup). It works when the diagnosis has not switched it off (ControllerOffReq, 0 after every
%       start), OutputEnable is on, the channel is released, the setpoint is not clipped and above 0, and the ADC qualifier is
%       valid. LIMIT of the 48 V supply: the correction stays as it was, the integral part does not move. Otherwise the
%       correction is 0 and the integral part is cleared.
%     - Status CELLn_STATUS as in the DBC: 2 enabled, 1 disabled, 3 setpoint error while enabled (as the old firmware
%       it follows the command, not OutputEnable). Channels 15 and 16 are not fitted: status 1, voltage 0, as before.
%     - ControllerOn (0x121 BATTSIM_CONTROLLER_ON) is 1 when the regulator is not switched off by the diagnosis.
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
% vector ports: a MATLAB Function block does not take a symbolic dimension (CellCount), see swc_vec.m
vin = @(p) swc_vec(sub, p, 'in');
swc_core(sub, 'CellCore', coreScript(), ...
    {{'mux', names('Cell%02dVoltSet_V')}, {'mux', names('Cell%02dEnable')}, {'mux', names('UCell%02d_mV')}, ...
     {'mux', names('UCell%02dQlf')}, 'OutputEnable', 'LimitActive', 'ControllerOffReq', ...
     vin('CalSetOffset_V'), vin('CalSetGain'), vin('CalMeasOffset_V'), vin('CalMeasGain')}, ...
    {{'VoltageSetValue_Ax', 'single'}, {'VoltageSetPwm_T', 'single'}, {'VoltageSenseMeas_Ax', 'single'}, ...
     {'VoltageSenseValue_T', 'single'}, {'SetpointLimit_V', 'single'}, {'VoltageSenseFilter', 'single'}, ...
     {'CtrlKp', 'single'}, {'CtrlKi_per_s', 'single'}, {'CtrlLimit_V', 'single'}, {'QLF_OK', 'uint8'}}, ...
    {{'demux', names('En%02d')}, {'demux', names('Cell%02dVoltAct_V')}, {'demux', names('Cell%02dState')}, ...
     {'demux', names('Duty%02d')}, 'ControllerOn', 'ControllerOffAct'});
% channels 15 and 16 are not fitted: status "disabled" (voltage 0 is the placeholder of the port), as the old firmware
for ch = [15 16]
    add_block('simulink/Sources/Constant', sprintf('%s/Cell%dState_Const', sub, ch), ...
        'Position', [30, 900 + 40 * (ch - 14), 150, 924 + 40 * (ch - 14)], 'Value', '1', 'OutDataTypeStr', 'uint8');
    add_line(sub, sprintf('Cell%dState_Const/1', ch), sprintf('Cell%dState/1', ch), 'autorouting', 'on');
end
end

function code = coreScript()
lines = {
'function [en, vAct, state, duty, ctrlOn, ctrlOffAct] = CellCore(setV, enable, mV, qlf, outEn, limitActive, ctrlOffReq, ...'
'    calSO, calSG, calMO, calMG, setAx, setT, senseAx, senseT, limitV, filt, kp, kiPerS, ctrlLim, QlfOk)'
'%CELLCORE 14 cell channels: setpoint -> duty, release, actual value, regulator, status (see CellChannels_create.m).'
'%#codegen'
'persistent y seen integ corrHold'
'if isempty(y)'
'    y = zeros(14, 1, ''single'');'
'    seen = false(14, 1);'
'    integ = zeros(14, 1, ''single'');'
'    corrHold = zeros(14, 1, ''single'');'
'end'
'en = false(14, 1);'
'vAct = zeros(14, 1, ''single'');'
'state = zeros(14, 1, ''uint8'');'
'duty = zeros(14, 1, ''uint16'');'
'ctrlActive = (ctrlOffReq == uint8(0));'
'ctrlOn = ctrlActive;'
'ctrlOffAct = uint8(ctrlOffReq ~= uint8(0));'
'ts = single(0.01);'
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
'    [sg, so] = plausible(calSG(k), calSO(k));'
'    [mg, mo] = plausible(calMG(k), calMO(k));'
'    % actual value: curve, calibration, low-pass'
'    if qlf(k) == QlfOk'
'        mv = mg * interp1d(senseAx, senseT, single(mV(k))) + mo;'
'        if seen(k)'
'            y(k) = y(k) + filt * (mv - y(k));'
'        else'
'            y(k) = mv;'
'            seen(k) = true;'
'        end'
'    end'
'    vAct(k) = y(k);'
'    % regulator: PI on the actual value, correction limited, integral part clamped (anti-windup)'
'    corr = single(0);'
'    working = ctrlActive && outEn && commanded && ~clipped && (v > single(0)) && (qlf(k) == QlfOk) && seen(k);'
'    if working && limitActive'
'        corr = corrHold(k);                % supply at its limit: stand still'
'    elseif working'
'        e = v - y(k);'
'        iNew = integ(k) + kiPerS * ts * e;'
'        iNew = min(max(iNew, -ctrlLim), ctrlLim);'
'        integ(k) = iNew;'
'        corr = min(max(kp * e + iNew, -ctrlLim), ctrlLim);'
'        corrHold(k) = corr;'
'    else'
'        integ(k) = single(0);'
'        corrHold(k) = single(0);'
'    end'
'    % setpoint -> command -> duty'
'    if outEn && (v > single(0))'
'        cmd = min(max(v + corr, single(0)), limitV);'
'        cmd = sg * cmd + so;'
'        d = interp1d(setAx, setT, cmd) * single(10000);'
'        d = min(max(d, single(0)), single(10000));'
'        duty(k) = uint16(floor(d + single(0.5)));'
'    end'
'    en(k) = outEn && commanded;'
'    if ~commanded'
'        state(k) = uint8(1);'
'    elseif clipped'
'        state(k) = uint8(3);'
'    else'
'        state(k) = uint8(2);'
'    end'
'end'
''
'function [g, o] = plausible(gain, offset)'
'% a calibration pair outside 0.5 .. 1.5 / +-0.5 V (CellCal never delivers one; the port is 0 before its first run) is the factory value'
'g = gain;'
'o = offset;'
'if ~((gain >= single(0.5)) && (gain <= single(1.5)) && (abs(offset) <= single(0.5)))'
'    g = single(1);'
'    o = single(0);'
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
