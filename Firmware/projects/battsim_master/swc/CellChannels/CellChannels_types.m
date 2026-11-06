function CellChannels_types()
%CELLCHANNELS_TYPES Named data types (RTE-15) and system constants (RTE-50) of component CellChannels,
%   generated from its swc.yaml. Do not edit: change types: or the constants (kind: constant) in swc.yaml
%   and run  python tools/swc/swc_yaml.py types <component folder>  (swc_create writes it). Named types
%   are imported from Rte_Type.h. A system constant is an AUTOSAR.Parameter with the storage class
%   SystemConstant: the generated code names only the macro Rte_SysCon_<Name>, which Rte_CellChannels.h
%   of the project defines (tools/rtegen, the bound value); the value here is the component's own value
%   (swc.yaml value) for the model-in-the-loop run - Rte_Const.m of a generated RTE sets the values of
%   that project. Run by the PreLoadFcn of CellChannels.slx and by swc_load_init.

% system constant CellCount = 14 (own value of swc.yaml); the code names the macro Rte_SysCon_CellCount
c = AUTOSAR.Parameter(14);
c.DataType = 'int32';
c.CoderInfo.StorageClass = 'Custom';
c.CoderInfo.CustomStorageClass = 'SystemConstant';
c.Min = 14;
c.Max = 14;
c.Description = 'Anzahl der Zellkanaele (Dimension der Vektorports)';
assignin('base', 'CellCount', c);

end
