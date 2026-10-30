% SPDX-License-Identifier: Apache-2.0
function CellCal_create()
%CELLCAL_CREATE Creates CellCal.slx from swc.yaml with the logic below (BattSimFW, Stufe C).
%   The model is made once; afterwards CellCal.slx is the master and is edited in Simulink. Running this function
%   again starts over (swc_create with Overwrite); generate the code afterwards with swc_build. The RTE constants (QLF_*)
%   come from the generated Rte_Qlf.m: set NX_RTE_GENERATED to build/<preset>/generated/rte of a configured project.
%
%   Runnable CalTask10ms (FW-06, FW-07; concept BS-4, stage C): the calibration of the 14 cell channels, four values each:
%   offset and gain for setting (setpoint -> command) and for measuring (measured -> actual value), y = gain * x + offset.
%     - In the model the values are integer codes as on the wire: offset sint16 in 0.1 mV, gain uint16 in 1e-4 (10000 = 1.0).
%       CellChannels gets them as float32 (V and ratio).
%     - Start: the values in use are the stored ones (NvM blocks, one per kind, one value per channel). A block that was
%       never written (QLF_NOT_AVAILABLE; its default is 0, the Outport initial value does not reach the ARXML) gives the
%       factory values (offset 0, gain 1), which the model writes and the NvM then stores; a defective block (any other
%       qualifier) too, state 2; a stored value outside the limits is replaced by the factory value of its channel, state 2.
%     - A tester stages values per channel with 2E (CalChNN); nothing changes until the routine ApplyCalibration (31 01 0200,
%       a counter that changes). Then, in the same 10 ms step: outputs on -> refused (result 1); a staged channel is "new" when
%       it differs from the value in use and is not all zero (all zero = never written since the start); one new channel with
%       a value outside the limits -> nothing changes, result 2 and the first such channel; no new channel -> result 3; otherwise
%       all new channels are taken over (result 0). The stored blocks follow (NvM write on change).
%     - The result of the latest request stays until the next one (routine results 31 03).
%   State (DID 0x0210): 0 factory values, 1 stored/applied values, 2 storage defective at the start.
here = fileparts(mfilename('fullpath'));
root = fileparts(fileparts(fileparts(fileparts(here))));
addpath(fullfile(root, 'nextbsw', 'tools', 'swc'), fileparts(here));
swc_load_qlf();
swc_create(here, 'Overwrite', true, 'Logic', @logic);
end

function logic(mdl, ~)
sub = swc_runnable_subsystem(mdl, 'CalTask10ms');
n = 14;
names = @(fmt) arrayfun(@(k) sprintf(fmt, k), 1:n, 'UniformOutput', false);
% vector ports: a MATLAB Function block does not take a symbolic dimension (CellCount), see swc_vec.m
vin  = @(p) swc_vec(sub, p, 'in');
vout = @(p) swc_vec(sub, p, 'out');
swc_core(sub, 'CalCore', coreScript(), ...
    {'OutputsOn', 'ApplyRequest', {'mux', names('StSetOffset%02d')}, {'mux', names('StSetGain%02d')}, ...
     {'mux', names('StMeasOffset%02d')}, {'mux', names('StMeasGain%02d')}, ...
     vin('NvSetOffsetIn'), vin('NvSetGainIn'), vin('NvMeasOffsetIn'), vin('NvMeasGainIn'), ...
     'NvSetOffsetQlf', 'NvSetGainQlf', 'NvMeasOffsetQlf', 'NvMeasGainQlf'}, ...
    {{'OffsetLimit_code', 'uint16'}, {'GainMin_code', 'uint16'}, {'GainMax_code', 'uint16'}, ...
     {'QLF_OK', 'uint8'}, {'QLF_NOT_AVAILABLE', 'uint8'}}, ...
    {vout('NvSetOffsetOut'), vout('NvSetGainOut'), vout('NvMeasOffsetOut'), vout('NvMeasGainOut'), ...
     {'demux', names('AcSetOffset%02d')}, {'demux', names('AcSetGain%02d')}, ...
     {'demux', names('AcMeasOffset%02d')}, {'demux', names('AcMeasGain%02d')}, ...
     vout('CalSetOffset_V'), vout('CalSetGain'), vout('CalMeasOffset_V'), vout('CalMeasGain'), ...
     'ApplyResult', 'ApplyBadChannel', 'CalState'});
end

function code = coreScript()
lines = {
'function [nvSO, nvSG, nvMO, nvMG, acSO, acSG, acMO, acMG, phSO, phSG, phMO, phMG, result, badCh, state] = CalCore( ...'
'    outEn, cnt, stSO, stSG, stMO, stMG, inSO, inSG, inMO, inMG, qSO, qSG, qMO, qMG, offLim, gMin, gMax, QlfOk, QlfNa)'
'%CALCORE Calibration of 14 channels: stored values, staged values, takeover by request (see CalCell_create.m).'
'%#codegen'
'persistent aSO aSG aMO aMG last res bad st started'
'if isempty(started)'
'    started = true;'
'    [aSO, s1] = loadOffsets(inSO, qSO, offLim, QlfOk, QlfNa);'
'    [aSG, s2] = loadGains(inSG, qSG, gMin, gMax, QlfOk, QlfNa);'
'    [aMO, s3] = loadOffsets(inMO, qMO, offLim, QlfOk, QlfNa);'
'    [aMG, s4] = loadGains(inMG, qMG, gMin, gMax, QlfOk, QlfNa);'
'    st = max(max(s1, s2), max(s3, s4));'
'    last = cnt;'
'    res = uint8(3);'
'    bad = uint8(0);'
'end'
'if cnt ~= last'
'    last = cnt;'
'    res = uint8(3);'
'    bad = uint8(0);'
'    if outEn'
'        res = uint8(1);'
'    else'
'        req = false(14, 1);'
'        for k = 1:14'
'            written = (stSO(k) ~= int16(0)) || (stSG(k) ~= uint16(0)) || (stMO(k) ~= int16(0)) || (stMG(k) ~= uint16(0));'
'            differs = (stSO(k) ~= aSO(k)) || (stSG(k) ~= aSG(k)) || (stMO(k) ~= aMO(k)) || (stMG(k) ~= aMG(k));'
'            if written && differs'
'                req(k) = true;'
'                inRange = (abs(int32(stSO(k))) <= int32(offLim)) && (abs(int32(stMO(k))) <= int32(offLim)) && ...'
'                    (stSG(k) >= gMin) && (stSG(k) <= gMax) && (stMG(k) >= gMin) && (stMG(k) <= gMax);'
'                if ~inRange && bad == uint8(0)'
'                    bad = uint8(k);'
'                end'
'            end'
'        end'
'        if bad ~= uint8(0)'
'            res = uint8(2);'
'        elseif any(req)'
'            for k = 1:14'
'                if req(k)'
'                    aSO(k) = stSO(k);'
'                    aSG(k) = stSG(k);'
'                    aMO(k) = stMO(k);'
'                    aMG(k) = stMG(k);'
'                end'
'            end'
'            res = uint8(0);'
'            if st == uint8(0)'
'                st = uint8(1);'
'            end'
'        end'
'    end'
'end'
'nvSO = aSO; nvSG = aSG; nvMO = aMO; nvMG = aMG;'
'acSO = aSO; acSG = aSG; acMO = aMO; acMG = aMG;'
'phSO = single(aSO) * single(0.0001);'
'phSG = single(aSG) * single(0.0001);'
'phMO = single(aMO) * single(0.0001);'
'phMG = single(aMG) * single(0.0001);'
'result = res;'
'badCh = bad;'
'state = st;'
''
'function [v, s] = loadOffsets(inV, q, lim, QlfOk, QlfNa)'
'% stored offsets; the factory value 0 for a never written or defective block, or a value outside the limit'
'v = zeros(14, 1, ''int16'');'
's = uint8(0);'
'if q == QlfOk'
'    s = uint8(1);'
'    for k = 1:14'
'        if abs(int32(inV(k))) <= int32(lim)'
'            v(k) = inV(k);'
'        else'
'            s = uint8(2);'
'        end'
'    end'
'elseif q ~= QlfNa'
'    s = uint8(2);'
'end'
''
'function [v, s] = loadGains(inV, q, lo, hi, QlfOk, QlfNa)'
'% stored gains; the factory value 1.0000 (code 10000) for a never written or defective block, or a value outside the limits'
'v = uint16(10000) * ones(14, 1, ''uint16'');'
's = uint8(0);'
'if q == QlfOk'
'    s = uint8(1);'
'    for k = 1:14'
'        if (inV(k) >= lo) && (inV(k) <= hi)'
'            v(k) = inV(k);'
'        else'
'            s = uint8(2);'
'        end'
'    end'
'elseif q ~= QlfNa'
'    s = uint8(2);'
'end'
};
code = strjoin(lines, newline);
end
