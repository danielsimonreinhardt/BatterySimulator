% SPDX-License-Identifier: Apache-2.0
function DisplayDriver_create()
%DISPLAYDRIVER_CREATE Creates DisplayDriver.slx from swc.yaml with the logic below (BattSimFW, Stufe D, Teil 1).
%   The model is made once; afterwards DisplayDriver.slx is the master and is edited in Simulink. Running this function
%   again starts over (swc_create with Overwrite); generate the code afterwards with swc_build.
%
%   Runnable DispTask10ms (doc/stufe-d-handbetrieb-anzeige.md, section 5): the 7-segment module (SparkFun Serial 7-Segment, I2C
%   address 0x71) shows DisplayValue_mV as four digits with the decimal point behind the first one (4.123 V).
%     - One byte per write order: DispTxByte with a change of the counter DispTxRequest; the next byte follows when the driver
%       has finished the order (DispTxDone equals DispTxRequest). The picture is: 0x76 (clear, only at the start and after an
%       error), then for each digit 0x79 (cursor), the position 0..3, the digit 0..9, at the end 0x77 and the mask 0x01.
%     - A new picture starts when the value changes or after DisplayRefresh_cycles; the value of a picture is held while it is sent.
%     - A failed write order (DispTxError at the end of an order) stops the picture; after DisplayRetry_cycles it starts again
%       with a clear.
here = fileparts(mfilename('fullpath'));
root = fileparts(fileparts(fileparts(fileparts(here))));
addpath(fullfile(root, 'nextbsw', 'tools', 'swc'), fileparts(here));
swc_load_qlf();
swc_create(here, 'Overwrite', true, 'Logic', @logic);
end

function logic(mdl, ~)
sub = swc_runnable_subsystem(mdl, 'DispTask10ms');
swc_core(sub, 'DispCore', coreScript(), ...
    {'DisplayValue_mV', 'DispTxDone', 'DispTxError'}, ...
    {{'DisplayRefresh_cycles', 'uint16'}, {'DisplayRetry_cycles', 'uint16'}}, ...
    {'DispTxByte', 'DispTxRequest'});
end

function code = coreScript()
lines = {
'function [txByte, txReq] = DispCore(value, done, err, refresh, retry)'
'%DISPCORE Byte sequence for the 7-segment module (see DisplayDriver_create.m).'
'%#codegen'
'persistent req outByte step len frame shown age wait needClear inflight'
'if isempty(req)'
'    req = uint8(0);'
'    outByte = uint8(0);'
'    step = uint8(0);'
'    len = uint8(0);'
'    frame = zeros(15, 1, ''uint8'');'
'    shown = uint16(65535);'
'    age = uint16(0);'
'    wait = uint16(0);'
'    needClear = true;'
'    inflight = false;'
'end'
'% result of the order in flight'
'if inflight && (done == req)'
'    inflight = false;'
'    if err'
'        step = uint8(0);'
'        needClear = true;'
'        shown = uint16(65535);'
'        wait = retry;'
'    end'
'end'
'if wait > uint16(0)'
'    wait = wait - uint16(1);'
'end'
'if age < uint16(65535)'
'    age = age + uint16(1);'
'end'
'if (~inflight) && (wait == uint16(0))'
'    if step == uint8(0)'
'        v = min(value, uint16(9999));'
'        if needClear || (v ~= shown) || (age >= refresh)'
'            n = uint8(0);'
'            if needClear'
'                n = uint8(1);'
'                frame(1) = uint8(118);   % 0x76 clear'
'            end'
'            digits = [idivide(v, uint16(1000)), mod(idivide(v, uint16(100)), uint16(10)), ...'
'                      mod(idivide(v, uint16(10)), uint16(10)), mod(v, uint16(10))];'
'            for k = 1:4'
'                frame(n + 1) = uint8(121);            % 0x79 cursor'
'                frame(n + 2) = uint8(k - 1);          % position'
'                frame(n + 3) = uint8(digits(k));      % digit'
'                n = n + uint8(3);'
'            end'
'            frame(n + 1) = uint8(119);                % 0x77 decimal points'
'            frame(n + 2) = uint8(1);                  % mask: point behind the first digit'
'            len = n + uint8(2);'
'            step = uint8(1);'
'            shown = v;'
'        end'
'    end'
'    if step > uint8(0)'
'        outByte = frame(step);'
'        if req == uint8(255)               % the counter wraps (I2cByteWriter): saturating at 255 stopped all orders'
'            req = uint8(0);'
'        else'
'            req = req + uint8(1);'
'        end'
'        inflight = true;'
'        if step == len'
'            step = uint8(0);'
'            age = uint16(0);'
'            needClear = false;'
'        else'
'            step = step + uint8(1);'
'        end'
'    end'
'end'
'txByte = outByte;'
'txReq = req;'
};
code = strjoin(lines, newline);
end
