% SPDX-License-Identifier: Apache-2.0
function HandControl_create()
%HANDCONTROL_CREATE Creates HandControl.slx from swc.yaml with the logic below (BattSimFW, Stufe D, Teil 1).
%   The model is made once; afterwards HandControl.slx is the master and is edited in Simulink. Running this function
%   again starts over (swc_create with Overwrite); generate the code afterwards with swc_build.
%
%   Runnable HandTask10ms (doc/stufe-d-handbetrieb-anzeige.md): the hand mode of the old firmware (STANDALONE, status 2).
%     - Two encoders deliver free-running 16-bit counters (EncVoltCount, EncSelCount); the difference of two steps, with the wrap
%       of the counter, is summed up and cut into whole detents of CountsPerStep counts.
%     - Channel encoder: one detent = one channel, saturating at 1 and 14 (SelectedChannel); its button toggles "single" and "all".
%     - Voltage encoder: one detent = CoarseStep_mV or FineStep_mV on the setpoint of the selected channel (SelectAllMode: the new
%       value of the selected channel is written to all channels); its button toggles coarse and fine (start: coarse). The
%       setpoints are kept in whole mV and limited to 0 .. SetpointLimit_V. The gap between 0 V and HandMinSetpoint_V (the output
%       stage stands at about 0.7 V without load) is jumped over: the first detent up from 0 V sets HandMinSetpoint_V, a detent
%       down that ends below it sets 0 V (HandMinSetpoint_V = 0: no jump). The hand mode still starts at 0 V (F8).
%     - Card keys (CardKey01..14): a debounced press (KeyDebounce_cycles) toggles the release of that channel (hand mode only).
%     - HandActive (BattSimMode, stage D step 2) starts the hand mode: all setpoints 0 V (F8), all channels released. Outside the
%       hand mode HandSetpoint_V is 0 and HandEnable false; the encoders and the selection keep working in every mode.
%     - DisplayValue_mV: ActualVolt_V of the selected channel in mV, 0 .. 9999 (a value that is not a number counts as 0), calmed
%       down: PT1 filter (DisplayFilter_cycles) and hysteresis (DisplayHyst_mV, the shown value changes only when the filtered
%       value is that far away); a new channel or a deviation of DisplayJump_mV from the filtered value is shown at once.
here = fileparts(mfilename('fullpath'));
root = fileparts(fileparts(fileparts(fileparts(here))));
addpath(fullfile(root, 'nextbsw', 'tools', 'swc'), fileparts(here));
swc_load_qlf();
swc_create(here, 'Overwrite', true, 'Logic', @logic);
end

function logic(mdl, ~)
sub = swc_runnable_subsystem(mdl, 'HandTask10ms');
n = 14;
names = @(fmt) arrayfun(@(k) sprintf(fmt, k), 1:n, 'UniformOutput', false);
% vector ports: a MATLAB Function block does not take a symbolic dimension (CellCount), see swc_vec.m
vin  = @(p) swc_vec(sub, p, 'in');
vout = @(p) swc_vec(sub, p, 'out');
swc_core(sub, 'HandCore', coreScript(), ...
    {'HandActive', 'EncVoltCount', 'EncVoltButton', 'EncSelCount', 'EncSelButton', {'mux', names('CardKey%02d')}, vin('ActualVolt_V')}, ...
    {{'CoarseStep_mV', 'single'}, {'FineStep_mV', 'single'}, {'CountsPerStep', 'uint8'}, {'KeyDebounce_cycles', 'uint8'}, ...
     {'SetpointLimit_V', 'single'}, {'HandMinSetpoint_V', 'single'}, {'DisplayFilter_cycles', 'uint16'}, ...
     {'DisplayHyst_mV', 'uint16'}, {'DisplayJump_mV', 'uint16'}}, ...
    {vout('HandSetpoint_V'), vout('HandEnable'), 'SelectedChannel', 'SelectAllMode', 'DisplayValue_mV'});
end

function code = coreScript()
lines = {
'function [sp, en, selOut, allOut, dispMv] = HandCore(active, vCnt, vBtn, sCnt, sBtn, keys, actual, coarseMv, fineMv, cps, deb, limitV, minV, filtN, hyst, jump)'
'%HANDCORE Hand mode: encoders, keys, setpoints, releases, selection and display value (see HandControl_create.m).'
'%#codegen'
'persistent started lastV lastS accV accS sel coarse selAll spMv en0 wasActive cntV cntS cntK filt shown lastSel dispInit'
'if isempty(started)'
'    started = true;'
'    lastV = vCnt;'
'    lastS = sCnt;'
'    accV = int32(0);'
'    accS = int32(0);'
'    sel = uint8(1);'
'    coarse = true;'
'    selAll = false;'
'    spMv = zeros(14, 1, ''int32'');'
'    en0 = false(14, 1);'
'    wasActive = false;'
'    cntV = uint8(0);'
'    cntS = uint8(0);'
'    cntK = zeros(14, 1, ''uint8'');'
'    filt = single(0);'
'    shown = single(0);'
'    lastSel = uint8(1);'
'    dispInit = false;'
'end'
'debounce = max(deb, uint8(1));'
'% encoders: counter difference with the wrap of the 16 bit counter, whole detents'
'accV = accV + wrapDiff(vCnt, lastV);'
'accS = accS + wrapDiff(sCnt, lastS);'
'lastV = vCnt;'
'lastS = sCnt;'
'perStep = int32(max(cps, uint8(1)));'
'stepsV = int32(0);'
'stepsS = int32(0);'
'if abs(accV) >= perStep'
'    stepsV = idivide(accV, perStep, ''fix'');'
'    accV = accV - stepsV * perStep;'
'end'
'if abs(accS) >= perStep'
'    stepsS = idivide(accS, perStep, ''fix'');'
'    accS = accS - stepsS * perStep;'
'end'
'% keys: one event per press, after the debounce time'
'[cntV, evV] = pressed(vBtn, cntV, debounce);'
'[cntS, evS] = pressed(sBtn, cntS, debounce);'
'evK = false(14, 1);'
'for k = 1:14'
'    [cntK(k), evK(k)] = pressed(keys(k), cntK(k), debounce);'
'end'
'if evV'
'    coarse = ~coarse;'
'end'
'if evS'
'    selAll = ~selAll;'
'end'
'% channel selection, saturating'
'newSel = int32(sel) + stepsS;'
'if newSel < int32(1)'
'    newSel = int32(1);'
'elseif newSel > int32(14)'
'    newSel = int32(14);'
'end'
'sel = uint8(newSel);'
'limitMv = int32(round(limitV * single(1000)));'
'if active'
'    if ~wasActive'
'        spMv = zeros(14, 1, ''int32'');'
'        en0 = true(14, 1);'
'    end'
'    if stepsV ~= int32(0)'
'        if coarse'
'            stepMv = coarseMv;'
'        else'
'            stepMv = fineMv;'
'        end'
'        v = spMv(sel) + int32(round(single(stepsV) * stepMv));'
'        minMv = int32(round(minV * single(1000)));'
'        if (v > int32(0)) && (v < minMv)       % the gap 0 .. minimum is jumped over, up to the minimum or down to 0'
'            if stepsV > int32(0)'
'                v = minMv;'
'            else'
'                v = int32(0);'
'            end'
'        end'
'        if v < int32(0)'
'            v = int32(0);'
'        elseif v > limitMv'
'            v = limitMv;'
'        end'
'        if selAll'
'            spMv = v * ones(14, 1, ''int32'');'
'        else'
'            spMv(sel) = v;'
'        end'
'    end'
'    for k = 1:14'
'        if evK(k)'
'            en0(k) = ~en0(k);'
'        end'
'    end'
'else'
'    spMv = zeros(14, 1, ''int32'');'
'    en0 = false(14, 1);'
'end'
'wasActive = active;'
'sp = single(spMv) / single(1000);'
'en = en0;'
'selOut = sel;'
'allOut = selAll;'
'x = actual(sel) * single(1000);'
'% written so that a value that is not a number ends in the last branch (0), also after the optimisation of the code generator'
'y = single(0);'
'if x > single(9999)'
'    y = single(9999);'
'elseif x >= single(0)'
'    y = x;'
'end'
'% display: new channel or a large deviation at once, otherwise PT1 and a hysteresis on the shown value'
'n = single(max(filtN, uint16(1)));'
'if (~dispInit) || (sel ~= lastSel) || (abs(y - filt) >= single(jump))'
'    filt = y;'
'    shown = round(y);'
'else'
'    filt = filt + (y - filt) / n;'
'    if abs(filt - shown) >= single(hyst)'
'        shown = round(filt);'
'    end'
'end'
'lastSel = sel;'
'dispInit = true;'
'dispMv = uint16(shown);'
''
'function d = wrapDiff(cur, prev)'
'% difference of two readings of a 16 bit counter, -32768 .. 32767'
'd = int32(cur) - int32(prev);'
'if d > int32(32767)'
'    d = d - int32(65536);'
'elseif d < int32(-32768)'
'    d = d + int32(65536);'
'end'
''
'function [cnt, ev] = pressed(down, cnt, debounce)'
'ev = false;'
'if down'
'    if cnt < uint8(255)'
'        cnt = cnt + uint8(1);'
'        if cnt == debounce'
'            ev = true;'
'        end'
'    end'
'else'
'    cnt = uint8(0);'
'end'
};
code = strjoin(lines, newline);
end
