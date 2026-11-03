% SPDX-License-Identifier: Apache-2.0
function BattSimMode_create()
%BATTSIMMODE_CREATE Creates BattSimMode.slx from swc.yaml with the logic below (BattSimFW, Stufe B).
%   The model is made once; afterwards BattSimMode.slx is the master and is edited in Simulink. Running this function
%   again starts over (swc_create with Overwrite); generate the code afterwards with swc_build. The RTE constants (QLF_*)
%   come from the generated Rte_Qlf.m: set NX_RTE_GENERATED to build/<preset>/generated/rte of a configured project.
%
%   Runnable ModeTask10ms (FW-01..05, FW-16; concept doc/firmware-konzept.html of BatterieSimulator, stage B):
%     States STANDBY, STANDALONE (hand mode, stage D), REMOTE/STOP, REMOTE/RUN, FAIL (the cell model comes with stage D, part 2).
%       STANDBY -> STANDALONE  the ALL ON/OFF key (a current measurement that is already invalid goes to FAIL, reason 5, as STOP -> RUN)
%       STANDALONE -> STANDBY  the ALL ON/OFF key, or REMOTE=2 and MODE=1 on a valid 0x130 (REMOTE=1 does not leave it)
%       STANDALONE -> STOP     REMOTE=2 and MODE=2 on a valid 0x130, or the REMOTE key; OUTPUT=2 does not start RUN until it has left 2
%                              once (the hand setpoints must not turn into CAN setpoints at once). MODE=3 (cell model) has no effect.
%       STANDALONE has no 0x130 supervision (the hand mode needs no bus); the cell messages are not evaluated.
%       Outputs on in STANDALONE (OutputEnable), HandActive tells HandControl and CellChannels to use the hand mode.
%       STANDBY -> STOP   REMOTE=2 and MODE=2 on a valid 0x130, or the REMOTE key
%       STOP    -> RUN    OUTPUT=2 on a valid 0x130 (not right after the ALL ON/OFF key switched the output off, until
%                         OUTPUT has left 2 once)
%       RUN     -> STOP   OUTPUT=1, or the ALL ON/OFF key
%       STOP/RUN -> STANDBY  REMOTE=1, REMOTE=2 and MODE=1, or the REMOTE key
%       any     -> FAIL   Overload (reason 3); in RUN the qualifier of 0x130 reports a timeout or bus error (reason 1) or
%                         an alive error (reason 2) - in the same 10 ms step, the outputs are off with OutputEnable;
%                         in RUN and STANDALONE CurrentInvalid (current measurement invalid for 200 ms, review C-1, reason 5) or OverVoltage
%                         (a released channel 100 ms above its setpoint + 0.2 V, review C-2, reason 6). CurrentInvalid comes
%                         from the SupplyMonitor of the same step (outputs off in that step; when it is already true on the way
%                         from STOP to RUN, the step goes to FAIL and the outputs never come on); OverVoltage comes from CellChannels, which runs after this component:
%                         the outputs are off one step later.
%       FAIL    -> STANDBY  REMOTE=2 and MODE=1, or the ALL ON/OFF key, only while Overload is not active
%     No FAIL in STOP when 0x130 fails (as before). An invalid 0x130 counts as REMOTE=3, MODE=7, OUTPUT=3: no transition.
%     Keys are debounced (ButtonDebounce_cycles of 10 ms) and act on the press.
%     BATTSIM_STATUS 1 STANDBY, 2 STANDALONE, 3 REMOTE, 5 FAIL; BATTSIM_OUTPUT_STATUS 2 in RUN and STANDALONE, otherwise 1. LED_REMOTE: on in REMOTE,
%     blinking (200 ms) in FAIL. FAIL reason (0x121): 0 none, 1 timeout 0x130, 2 alive 0x130, 3 overload, 4 watchdog
%     reset (shown in STANDBY after the start until the first state change), 5 current measurement invalid, 6 overvoltage
%     of a channel.
%     Alive of the status messages: 1..14 as the old firmware, +1 per 100 ms, one counter for all messages.
%     0x121 qualifier bits: 1 when the qualifier of the control message is QLF_OK.
here = fileparts(mfilename('fullpath'));
root = fileparts(fileparts(fileparts(fileparts(here))));
addpath(fullfile(root, 'nextbsw', 'tools', 'swc'), fileparts(here));
swc_load_qlf();
swc_create(here, 'Overwrite', true, 'Logic', @logic);
end

function logic(mdl, ~)
sub = swc_runnable_subsystem(mdl, 'ModeTask10ms');
% qualifiers in the order of the bits in 0x121 (0x100 .. 0x105, 0x130, 0x131, 0x132)
qin = {'ControlCell010203Qlf', 'ControlCell040506Qlf', 'ControlCell070809Qlf', 'ControlCell101112Qlf', ...
       'ControlCell131415Qlf', 'ControlCell16Qlf', 'ControlBattSimQlf', 'ControlDynCellSim1Qlf', 'ControlDynCellSim2Qlf'};
qout = {'ValidCell010203', 'ValidCell040506', 'ValidCell070809', 'ValidCell101112', 'ValidCell131415', ...
        'ValidCell16', 'ValidBattSim', 'ValidDynCellSim1', 'ValidDynCellSim2'};
alive = {'AliveStatusBattSim', 'AliveStatusCells010203', 'AliveStatusCells040506', 'AliveStatusCells070809', ...
         'AliveStatusCells101112', 'AliveStatusCells131415', 'AliveStatusCell16', 'AliveStatusBattSimExt'};
swc_core(sub, 'ModeCore', coreScript(), ...
    {'RemoteControl', 'ModeControl', 'OutputControl', {'mux', qin}, 'BtnAllOnOff', 'BtnRemote', 'Overload', ...
     'CurrentInvalid', 'OverVoltage', 'ResetWatchdog', 'ResetQlf'}, ...
    {{'ButtonDebounce_cycles', 'uint8'}, {'QLF_OK', 'uint8'}, {'QLF_ERROR_ALIVE', 'uint8'}, ...
     {'QLF_ERROR_TIMEOUT', 'uint8'}, {'QLF_ERROR', 'uint8'}}, ...
    {'BattSimStatus', 'OutputStatus', 'LedRemote', 'OutputEnable', 'HandActive', 'FailReason', {'fan', alive}, {'demux', qout}});
% firmware version: calibration parameters straight to the ports of 0x121
ver = {'Major', 'Minor', 'Patch'};
for k = 1:numel(ver)
    add_block('simulink/Sources/Constant', [sub '/Version' ver{k} '_Const'], 'Position', [30, 700 + 40 * k, 150, 724 + 40 * k], ...
        'Value', ['Version' ver{k}], 'OutDataTypeStr', 'uint8');
    add_line(sub, ['Version' ver{k} '_Const/1'], ['FwVersion' ver{k} '/1'], 'autorouting', 'on');
end
end

function code = coreScript()
lines = {
'function [status, outStatus, ledRemote, outEnable, handActive, failReason, alive, qlfOk] = ModeCore(remote, mode, output, qlf, ...'
'    btnAll, btnRem, ovl, curInv, overV, wdReset, resetQlf, debounceN, QlfOk, QlfAlive, QlfTimeout, QlfBus)'
'%MODECORE Operating modes of the battery simulator, one call per 10 ms (see BattSimMode_create.m).'
'%#codegen'
'persistent st reason aliveCnt tick outBlock levA cntA levR cntR wdDone'
'if isempty(st)'
'    st = uint8(0);             % 0 STANDBY, 1 STOP, 2 RUN, 3 FAIL, 4 STANDALONE'
'    reason = uint8(0);'
'    aliveCnt = uint8(1);'
'    tick = uint8(0);'
'    outBlock = false;'
'    levA = false;'
'    cntA = uint8(0);'
'    levR = false;'
'    cntR = uint8(0);'
'    wdDone = false;'
'end'
''
'% start-up reason: the reset information is read once, as soon as its qualifier is valid'
'if ~wdDone && resetQlf == QlfOk'
'    wdDone = true;'
'    if wdReset && st == uint8(0) && reason == uint8(0)'
'        reason = uint8(4);     % this start followed a watchdog or safety reset'
'    end'
'end'
''
'[levA, cntA, pressAll] = debounce(btnAll, levA, cntA, debounceN);'
'[levR, cntR, pressRem] = debounce(btnRem, levR, cntR, debounceN);'
''
'% control message 0x130 (index 7 of the qualifiers): an invalid one is REMOTE=3, MODE=7, OUTPUT=3'
'q = qlf(7);'
'valid = (q == QlfOk);'
'r = uint8(3);'
'm = uint8(7);'
'o = uint8(3);'
'if valid'
'    r = remote;'
'    m = mode;'
'    o = output;'
'    if o ~= uint8(2)'
'        outBlock = false;'
'    end'
'end'
'resetCmd = (r == uint8(2)) && (m == uint8(1));'
'standbyCmd = (r == uint8(1)) || resetCmd;'
''
'if ovl && st ~= uint8(3)'
'    st = uint8(3);'
'    reason = uint8(3);'
'elseif st == uint8(2) && ~valid && (bitand(q, bitor(QlfTimeout, QlfBus)) ~= uint8(0))'
'    st = uint8(3);'
'    reason = uint8(1);'
'elseif st == uint8(2) && ~valid && (bitand(q, QlfAlive) ~= uint8(0))'
'    st = uint8(3);'
'    reason = uint8(2);'
'elseif (st == uint8(2) || st == uint8(4)) && curInv'
'    st = uint8(3);'
'    reason = uint8(5);'
'elseif (st == uint8(2) || st == uint8(4)) && overV'
'    st = uint8(3);'
'    reason = uint8(6);'
'else'
'    switch st'
'        case uint8(0)'
'            if pressRem || (r == uint8(2) && m == uint8(2))'
'                st = uint8(1);'
'                if reason == uint8(4)'
'                    reason = uint8(0);'
'                end'
'            elseif pressAll'
'                if curInv'
'                    st = uint8(3);         % hand mode is refused: the overload shut-off would be blind'
'                    reason = uint8(5);'
'                else'
'                    st = uint8(4);'
'                    if reason == uint8(4)'
'                        reason = uint8(0);'
'                    end'
'                end'
'            end'
'        case uint8(1)'
'            if pressRem || standbyCmd'
'                st = uint8(0);'
'            elseif o == uint8(2) && ~outBlock'
'                if curInv'
'                    st = uint8(3);         % RUN is refused: the overload shut-off would be blind'
'                    reason = uint8(5);'
'                else'
'                    st = uint8(2);'
'                end'
'            end'
'        case uint8(2)'
'            if pressRem || standbyCmd'
'                st = uint8(0);'
'            elseif pressAll'
'                st = uint8(1);'
'                outBlock = true;'
'            elseif o == uint8(1)'
'                st = uint8(1);'
'            end'
'        case uint8(4)'
'            if pressAll || resetCmd'
'                st = uint8(0);'
'            elseif pressRem || (r == uint8(2) && m == uint8(2))'
'                st = uint8(1);'
'                outBlock = true;           % the hand setpoints do not turn into CAN setpoints at once'
'            end'
'        otherwise'
'            if ~ovl && (resetCmd || pressAll)'
'                st = uint8(0);'
'                reason = uint8(0);'
'            end'
'    end'
'end'
''
'status = uint8(1);'
'if st == uint8(1) || st == uint8(2)'
'    status = uint8(3);'
'elseif st == uint8(3)'
'    status = uint8(5);'
'elseif st == uint8(4)'
'    status = uint8(2);'
'end'
'outStatus = uint8(1);'
'outEnable = (st == uint8(2)) || (st == uint8(4));'
'handActive = (st == uint8(4));'
'if outEnable'
'    outStatus = uint8(2);'
'end'
'ledRemote = (st == uint8(1)) || (st == uint8(2)) || ((st == uint8(3)) && (tick < uint8(20)));'
'failReason = reason;'
'alive = aliveCnt;'
'qlfOk = (qlf == QlfOk);'
''
'% counters for the next call: alive 1..14 once per 100 ms, tick 0..39 (200 ms blink)'
'if mod(tick, uint8(10)) == uint8(9)'
'    if aliveCnt >= uint8(14)'
'        aliveCnt = uint8(1);'
'    else'
'        aliveCnt = aliveCnt + uint8(1);'
'    end'
'end'
'if tick >= uint8(39)'
'    tick = uint8(0);'
'else'
'    tick = tick + uint8(1);'
'end'
''
'function [lvl, cnt, press] = debounce(in, lvl, cnt, n)'
'press = false;'
'if in ~= lvl'
'    cnt = cnt + uint8(1);'
'    if cnt >= n'
'        lvl = in;'
'        cnt = uint8(0);'
'        press = in;'
'    end'
'else'
'    cnt = uint8(0);'
'end'
};
code = strjoin(lines, newline);
end
