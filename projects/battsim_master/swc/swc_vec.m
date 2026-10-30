% SPDX-License-Identifier: Apache-2.0
function name = swc_vec(sub, port, direction)
%SWC_VEC Reshape block between a vector port with a symbolic dimension (CellCount) and the MATLAB Function block (BattSimFW).
%   A MATLAB Function block does not support symbolic dimension propagation (Embedded Coder: "does not support symbolic
%   dimensions propagation"). The Reshape block gives the signal the number of the build (14) before the function sees it.
%   The model code loops over 14 channels, so the constant CellCount is fixed to 14 by min = max in swc.yaml; a project cannot
%   bind another number (rtegen refuses it).
%     name = swc_vec(sub, 'NvSetGainIn', 'in')    input port: In block -> Reshape; use `name` as the input of swc_core
%     name = swc_vec(sub, 'NvSetGainOut', 'out')  output port: Reshape -> Out block; use `name` as the output of swc_core
%   Called by the <Swc>_create.m of the components of this project, between swc_create and swc_core.

name = [port '_r'];
blk = [sub '/' name];
if strcmp(direction, 'in')
    add_block('simulink/Math Operations/Reshape', blk, 'OutputDimensionality', 'Customize', 'OutputDimensions', '[14 1]');
    clearOutgoing(sub, port);
    add_line(sub, [port '/1'], [name '/1'], 'autorouting', 'on');
else
    add_block('simulink/Math Operations/Reshape', blk, 'OutputDimensionality', '1-D array');
    clearIncoming(sub, port);
    add_line(sub, [name '/1'], [port '/1'], 'autorouting', 'on');
end
end

function clearOutgoing(sub, port)
% the placeholder swc_create wires on an unused input (Terminator): remove the lines leaving the In block
lines = get_param([sub '/' port], 'LineHandles');
if lines.Outport(1) > 0
    dst = get_param(lines.Outport(1), 'DstBlockHandle');
    delete_line(lines.Outport(1));
    for k = 1:numel(dst)
        if dst(k) > 0 && strcmp(get_param(dst(k), 'BlockType'), 'Terminator')
            delete_block(dst(k));
        end
    end
end
end

function clearIncoming(sub, port)
% the placeholder swc_create wires on an unwritten output (Ground): remove the line entering the Out block
lines = get_param([sub '/' port], 'LineHandles');
if lines.Inport(1) > 0
    src = get_param(lines.Inport(1), 'SrcBlockHandle');
    delete_line(lines.Inport(1));
    if src > 0 && strcmp(get_param(src, 'BlockType'), 'Ground')
        delete_block(src);
    end
end
end
