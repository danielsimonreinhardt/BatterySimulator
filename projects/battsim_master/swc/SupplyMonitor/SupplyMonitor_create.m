% SPDX-License-Identifier: Apache-2.0
function SupplyMonitor_create()
%SUPPLYMONITOR_CREATE Creates SupplyMonitor.slx from swc.yaml with the logic below (BattSimFW, Stufe B).
%   The model is made once; afterwards SupplyMonitor.slx is the master and is edited in Simulink. Running this
%   function again starts over (swc_create with Overwrite); generate the code afterwards with swc_build. The RTE
%   constants (QLF_*) come from the generated Rte_Qlf.m: set NX_RTE_GENERATED to build/<preset>/generated/rte of a
%   configured project.
%
%   Runnable SupplyTask10ms (FW-08, FW-09):
%     - 48 V current: first-order low-pass of ISupply_mV (1 V per A, mV equals mA with SupplyCurrentGain), only
%       samples with a valid ADC qualifier count;
%     - Overload: set above OverloadThd_mA, cleared below OverloadThd_mA - OverloadHyst_mA (1.28 A / 1.08 A of the old
%       firmware). The component reports the condition; BattSimMode latches it as FAIL and the reset needs the current
%       to be below the lower threshold.
%     - SupplyCurrent_A for the status message 0x121 and LimitActive (the shared LIMIT line of the cards) for 0x121.
%     - CurrentInvalid (review C-1, stage C): set when the ADC qualifier of the current measurement was not valid for
%       CurrentInvalid_cycles samples in a row (20 = 200 ms); a valid sample clears it. Without a valid measurement the
%       overload shut-off is blind (the filter keeps its last value); BattSimMode switches off in RUN with FAIL reason 5.
%       Plausibility (review bs-c1-strommessung S-1 a): with the output on and at least one channel released
%       (ChannelsReleased from CellChannels, previous step) the supply always draws a quiescent current (216 mA at the device,
%       270 mA with 14 channels). A valid sample below MinCurrent_mA (100 mA) for MinCurrentTime_cycles (20 = 200 ms) in a row
%       is implausible - an open shunt or sense line reads about 0 A with a valid qualifier - and sets CurrentInvalid as well.
%       An invalid sample neither counts nor clears; the output going off or no channel released clears the count.
here = fileparts(mfilename('fullpath'));
root = fileparts(fileparts(fileparts(fileparts(here))));
addpath(fullfile(root, 'nextbsw', 'tools', 'swc'), fileparts(here));
swc_load_qlf();
swc_create(here, 'Overwrite', true, 'Logic', @logic);
end

function logic(mdl, ~)
sub = swc_runnable_subsystem(mdl, 'SupplyTask10ms');
swc_core(sub, 'SupplyCore', coreScript(), ...
    {'ISupply_mV', 'ISupplyQlf', 'Limit', 'ChannelsReleased'}, ...
    {{'SupplyCurrentFilter', 'single'}, {'SupplyCurrentGain', 'single'}, {'OverloadThd_mA', 'single'}, ...
     {'OverloadHyst_mA', 'single'}, {'MinCurrent_mA', 'single'}, {'MinCurrentTime_cycles', 'uint8'}, ...
     {'CurrentInvalid_cycles', 'uint8'}, {'QLF_OK', 'uint8'}}, ...
    {'Overload', 'SupplyCurrent_A', 'LimitActive', 'CurrentInvalid'});
end

function code = coreScript()
lines = {
'function [overload, current_A, limitActive, curInvalid] = SupplyCore(mV, qlf, limit, released, filt, gain, thd, hyst, minMa, minN, invN, QlfOk)'
'%SUPPLYCORE 48 V supply current filter, overload with hysteresis (see SupplyMonitor_create.m).'
'%#codegen'
'persistent y ovl bad low'
'if isempty(y)'
'    y = single(0);      % filtered current [mA]'
'    ovl = false;'
'    bad = uint8(0);     % invalid samples in a row (saturating)'
'    low = uint8(0);     % implausibly low valid samples in a row with a channel released (saturating)'
'end'
'if qlf == QlfOk'
'    sample = single(mV) * gain;'
'    y = y + filt * (sample - y);'
'    bad = uint8(0);'
'    if released && (sample < minMa)'
'        if low < uint8(255)'
'            low = low + uint8(1);'
'        end'
'    else'
'        low = uint8(0);'
'    end'
'elseif bad < uint8(255)'
'    bad = bad + uint8(1);'
'end'
'if ~released'
'    low = uint8(0);'
'end'
'if y > thd'
'    ovl = true;'
'elseif y < thd - hyst'
'    ovl = false;'
'end'
'overload = ovl;'
'c = y * single(0.001);'
'if c > single(65.535)'
'    c = single(65.535);'
'end'
'current_A = c;'
'limitActive = limit;'
'curInvalid = (bad >= invN) || (low >= minN);'
};
code = strjoin(lines, newline);
end
