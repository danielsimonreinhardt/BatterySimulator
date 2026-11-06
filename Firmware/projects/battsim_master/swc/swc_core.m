% SPDX-License-Identifier: Apache-2.0
function swc_core(sub, coreName, script, inputs, constants, outputs)
%SWC_CORE Puts the logic of a runnable into its function-call subsystem as one MATLAB Function block (BattSimFW).
%   Called by the <Swc>_create.m of the components of this project from the Logic option of swc_create. The
%   subsystem `sub` already holds one In block per read port and one Out block per written port (named after the
%   port); this function adds
%     - the MATLAB Function block `coreName` with the text `script`,
%     - one Constant block per entry of `constants` ({name, type}); the name is a calibration parameter of the
%       model workspace or a constant such as QLF_OK (never a number, COM-03),
%     - wiring in the order of the function signature: `inputs` first, then `constants`, outputs from `outputs`.
%   An entry of `inputs` is a port name, or {'mux', {port, port, ...}} for a vector built from scalar ports. An entry of
%   `outputs` is a port name, {'demux', {port, ...}} for a vector split into scalar ports, or {'fan', {port, ...}}
%   for one value written to several ports. Ports no entry names keep the placeholders swc_create adds
%   (Terminator on an unused input, Ground on an unwritten output).

x0 = 30;
core = [sub '/' coreName];
add_block('simulink/User-Defined Functions/MATLAB Function', core, 'Position', [420 40 640 40 + 30 * (numel(inputs) + numel(constants) + 4)]);
chart = sfroot().find('-isa', 'Stateflow.EMChart', 'Path', core);
chart.Script = script;

in = 0;
for k = 1:numel(inputs)
    item = inputs{k};
    in = in + 1;
    if ischar(item)
        add_line(sub, [item '/1'], sprintf('%s/%d', coreName, in), 'autorouting', 'on');
    else
        names = item{2};
        mux = [sub '/' coreName '_in' num2str(in)];
        add_block('simulink/Signal Routing/Mux', mux, 'Inputs', num2str(numel(names)), 'DisplayOption', 'bar', ...
            'Position', [300 40 + 20 * in 305 40 + 20 * in + 16 * numel(names)]);
        for j = 1:numel(names)
            add_line(sub, [names{j} '/1'], sprintf('%s_in%d/%d', coreName, in, j), 'autorouting', 'on');
        end
        add_line(sub, sprintf('%s_in%d/1', coreName, in), sprintf('%s/%d', coreName, in), 'autorouting', 'on');
    end
end
for k = 1:numel(constants)
    in = in + 1;
    c = [sub '/' constants{k}{1} '_Const'];
    add_block('simulink/Sources/Constant', c, 'Position', [x0 + 200, 40 + 40 * in, x0 + 320, 64 + 40 * in], ...
        'Value', constants{k}{1}, 'OutDataTypeStr', constants{k}{2});
    add_line(sub, [constants{k}{1} '_Const/1'], sprintf('%s/%d', coreName, in), 'autorouting', 'on');
end
out = 0;
for k = 1:numel(outputs)
    item = outputs{k};
    out = out + 1;
    if ischar(item)
        add_line(sub, sprintf('%s/%d', coreName, out), [item '/1'], 'autorouting', 'on');
    elseif strcmp(item{1}, 'fan')
        for j = 1:numel(item{2})
            add_line(sub, sprintf('%s/%d', coreName, out), [item{2}{j} '/1'], 'autorouting', 'on');
        end
    else
        names = item{2};
        dm = [sub '/' coreName '_out' num2str(out)];
        add_block('simulink/Signal Routing/Demux', dm, 'Outputs', num2str(numel(names)), 'DisplayOption', 'bar', ...
            'Position', [700 40 + 20 * out 705 40 + 20 * out + 16 * numel(names)]);
        add_line(sub, sprintf('%s/%d', coreName, out), sprintf('%s_out%d/1', coreName, out), 'autorouting', 'on');
        for j = 1:numel(names)
            add_line(sub, sprintf('%s_out%d/%d', coreName, out, j), [names{j} '/1'], 'autorouting', 'on');
        end
    end
end
end
