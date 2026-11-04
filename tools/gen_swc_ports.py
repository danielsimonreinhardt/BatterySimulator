# SPDX-License-Identifier: Apache-2.0
"""swc.yaml, <Swc>_data.m and the connections of rte.yaml of battsim_master from one port table (BattSimFW).

The 14 cell channels have one port per signal (CAN signal, ADC channel, pin): hundreds of lines of the same shape. This
script writes them, so that the table below is the one place that says what a channel has. The calibration of the channels
has one vector port per kind of value (four kinds, 14 channels each, RTE-57: a data identifier element is a field of 14
values served by one vector port). The models (<Swc>_create.m) and the diagnostic description (diag.yaml, between the
markers) follow the table.

    python tools/gen_swc_ports.py     # rewrites the four swc.yaml, the data files, the connections of rte.yaml and the
                                      # calibration identifiers of diag.yaml

Stage B wrote SupplyMonitor, BattSimMode and CellChannels; stage C (this version) adds CellCal and the regulator,
calibration and limit ports of CellChannels. BattSimParams (parameter component) is written by hand.
"""
import json
import re
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
PROJECT = REPO / "projects" / "battsim_master"
SWC = PROJECT / "swc"
CH = range(1, 15)
GROUPS = {1: "01_02_03", 2: "01_02_03", 3: "01_02_03", 4: "04_05_06", 5: "04_05_06", 6: "04_05_06",
          7: "07_08_09", 8: "07_08_09", 9: "07_08_09", 10: "10_11_12", 11: "10_11_12", 12: "10_11_12",
          13: "13_14_15", 14: "13_14_15"}
GLIST = ["01_02_03", "04_05_06", "07_08_09", "10_11_12", "13_14_15"]
ALIVE = {"AliveStatusBattSim": "STATUS_BATTSIM"}
for g in GLIST:
    ALIVE[f"AliveStatusCells{g.replace('_', '')}"] = f"STATUS_CELL{g}"
ALIVE["AliveStatusCell16"] = "STATUS_CELL16"
ALIVE["AliveStatusBattSimExt"] = "STATUS_BATTSIM_EXT"
QLF_MSGS = ["CONTROL_CELL01_02_03", "CONTROL_CELL04_05_06", "CONTROL_CELL07_08_09", "CONTROL_CELL10_11_12",
            "CONTROL_CELL13_14_15", "CONTROL_CELL16", "CONTROL_BATTSIM", "CONTROL_DYNCELLSIM1", "CONTROL_DYNCELLSIM2"]

# Calibration of a channel (stage C, FW-06/FW-07): y = gain * x + offset, once for the setpoint ("Set") and once for the
# actual value ("Meas"). On the wire and in the NvM blocks the integer codes: offset sint16 in 0.1 mV, gain uint16 in
# 1e-4 (10000 = 1.0000); the limits are checked by the diagnosis (min/max of the element) and again by CellCal.
CAL = [  # (key, port stem, wire type, element label, factor, min, max, default code, unit)
    ("SetOffset", "SetOffset", "int16", "Offset of the setpoint (command = gain * setpoint + offset)", 0.0001, -0.2, 0.2, 0, "V"),
    ("SetGain", "SetGain", "uint16", "Gain of the setpoint", 0.0001, 0.9, 1.1, 10000, ""),
    ("MeasOffset", "MeasOffset", "int16", "Offset of the actual value (value = gain * measured + offset)", 0.0001, -0.2, 0.2, 0, "V"),
    ("MeasGain", "MeasGain", "uint16", "Gain of the actual value", 0.0001, 0.9, 1.1, 10000, ""),
]
PORT_TYPE = {"int16": "sint16", "uint16": "uint16"}
PHYS = {"SetOffset": "CalSetOffset_V", "SetGain": "CalSetGain", "MeasOffset": "CalMeasOffset_V", "MeasGain": "CalMeasGain"}
CAL_DID = 0x0201               # one identifier: four fields (SetOffset, SetGain, MeasOffset, MeasGain) of 14 values, channel 1 first

components = {}     # name -> dict(description, runnable, ports, params)
connections = {}    # "Comp.Port.Port" -> target (string or list)


def port(comp, name, direction, ptype, target=None, comment=None, **extra):
    components[comp]["ports"].append((name, direction, ptype, comment, extra))
    if target:
        connections[f"{comp}.{name}.{name}"] = target


def param(comp, name, ptype, values, comment, dim=None, **extra):
    components[comp]["params"].append((name, ptype, values, comment, dim, extra))


# ------------------------------------------------------------------ SupplyMonitor
c = "SupplyMonitor"
components[c] = dict(description="48-V-Versorgung: Strom filtern, Ueberlast mit Hysterese, LIMIT-Leitung",
                     runnable="SupplyTask10ms", ports=[], params=[])
port(c, "ISupply_mV", "in", "uint32", "adc.mv.I_SUPPLY")
port(c, "ISupplyQlf", "in", "uint8", "adc.qlf.I_SUPPLY")
port(c, "Limit", "in", "boolean", "dio.in.LIMIT")
port(c, "Overload", "out", "boolean", "can.tx.STATUS_BATTSIM_EXT.BATTSIM_OVERLOAD")
port(c, "SupplyCurrent_A", "out", "float32", "can.tx.STATUS_BATTSIM_EXT.BATTSIM_SUPPLY_CURRENT")
port(c, "LimitActive", "out", "boolean", "can.tx.STATUS_BATTSIM_EXT.BATTSIM_LIMIT")
port(c, "ChannelsReleased", "in", "boolean", "CellChannels.ChannelsReleased.ChannelsReleased", "output on and at least one channel released (CellChannels, from the previous step): the quiescent current of the supply is always above MinCurrent_mA then (review bs-c1-strommessung S-1)")
port(c, "CurrentInvalid", "out", "boolean", None, "the current measurement was invalid for CurrentInvalid_cycles samples in a row, or implausibly low for MinCurrentTime_cycles samples with the output on and a channel released (C-1): BattSimMode switches off in RUN, FAIL reason 5")
param(c, "SupplyCurrentFilter", "float32", [0.0078125], "[-]   low-pass weight per 10 ms (1/128, exact in single): time constant about 1.3 s, as 0.08 per 100 ms of the old firmware")
param(c, "SupplyCurrentGain", "float32", [1.0], "[mA/mV] INA181A1 on 50 mOhm: 1 V per A, i.e. 1 mA per mV at the ADC pin")
param(c, "OverloadThd_mA", "float32", [1280], "[mA]  overload above this current (IRM-60-48: 1.25 A continuous)")
param(c, "OverloadHyst_mA", "float32", [200], "[mA]  overload ends below OverloadThd_mA - this value (1.08 A)")
param(c, "MinCurrent_mA", "float32", [100], "[mA]  plausibility of the current measurement: with the output on and a channel released, a sample below this value is implausible; the quiescent current of the supply measured at the device is 216 mA, 270 mA with 14 channels (30.10.2026)",
      unit="mA", min=0, max=200, description="Mindeststrom der 48-V-Versorgung bei Ausgang ein")
param(c, "MinCurrentTime_cycles", "uint8", [20], "[10 ms] number of implausibly low samples in a row that count as a failure of the current measurement (20 = 200 ms, review bs-c1-strommessung S-1 a)")
param(c, "CurrentInvalid_cycles", "uint8", [20], "[10 ms] number of invalid samples of the current measurement in a row that count as a failure (20 = 200 ms, review C-1)")

# ------------------------------------------------------------------ BattSimMode
c = "BattSimMode"
components[c] = dict(description="Betriebsarten (Standby, Fernsteuerung, FAIL), Ausgang ein/aus, Tasten, Lebenszeichen und Statusbotschaft 0x121",
                     runnable="ModeTask10ms", ports=[], params=[])
port(c, "RemoteControl", "in", "uint8", "can.rx.CONTROL_BATTSIM.BATTSIM_REMOTE_CONTROL")
port(c, "ModeControl", "in", "uint8", "can.rx.CONTROL_BATTSIM.BATTSIM_MODE_CONTROL")
port(c, "OutputControl", "in", "uint8", "can.rx.CONTROL_BATTSIM.BATTSIM_OUTPUT_CONTROL")
qnames = ["ControlCell010203Qlf", "ControlCell040506Qlf", "ControlCell070809Qlf", "ControlCell101112Qlf",
          "ControlCell131415Qlf", "ControlCell16Qlf", "ControlBattSimQlf", "ControlDynCellSim1Qlf", "ControlDynCellSim2Qlf"]
vnames = ["ValidCell010203", "ValidCell040506", "ValidCell070809", "ValidCell101112", "ValidCell131415",
          "ValidCell16", "ValidBattSim", "ValidDynCellSim1", "ValidDynCellSim2"]
for q, msg in zip(qnames, QLF_MSGS):
    port(c, q, "in", "uint8", f"can.qlf.{msg}")
port(c, "BtnAllOnOff", "in", "boolean", "dio.in.BTN_ALL_ON_OFF")
port(c, "BtnRemote", "in", "boolean", "dio.in.BTN_REMOTE")
port(c, "Overload", "in", "boolean", "SupplyMonitor.Overload.Overload")
port(c, "CurrentInvalid", "in", "boolean", "SupplyMonitor.CurrentInvalid.CurrentInvalid", "current measurement failed (C-1): FAIL reason 5 in RUN")
port(c, "OverVoltage", "in", "boolean", "CellChannels.OverVoltage.OverVoltage", "a channel is above its setpoint for too long (C-2): FAIL reason 6 in RUN")
port(c, "ResetWatchdog", "in", "boolean", "bsw.reset.watchdog")
port(c, "ResetQlf", "in", "uint8", "bsw.reset.qlf")
port(c, "BattSimStatus", "out", "uint8", "can.tx.STATUS_BATTSIM.BATTSIM_STATUS")
port(c, "OutputStatus", "out", "uint8", "can.tx.STATUS_BATTSIM.BATTSIM_OUTPUT_STATUS")
port(c, "LedRemote", "out", "boolean", "dio.out.LED_REMOTE")
port(c, "OutputEnable", "out", "boolean")
port(c, "HandActive", "out", "boolean", None, "hand mode (STANDALONE, status 2) is active: HandControl starts at 0 V, CellChannels takes the setpoints and releases of HandControl instead of the CAN ones (stage D, step 2)")
port(c, "FailReason", "out", "uint8", "can.tx.STATUS_BATTSIM_EXT.BATTSIM_FAIL_REASON")
for p, msg in ALIVE.items():
    port(c, p, "out", "uint8", f"can.tx.{msg}.ALIVE_{msg}")
for v, msg in zip(vnames, QLF_MSGS):
    port(c, v, "out", "boolean", f"can.tx.STATUS_BATTSIM_EXT.QLF_{msg}")
for part in ("Major", "Minor", "Patch"):
    port(c, f"FwVersion{part}", "out", "uint8", f"can.tx.STATUS_BATTSIM_EXT.FW_VERSION_{part.upper()}")
param(c, "VersionMajor", "uint8", [0], "firmware version of 0x121")
param(c, "VersionMinor", "uint8", [5], "firmware version of 0x121 (0.5.0: hand mode STANDALONE, HandControl and DisplayDriver in the task; 0.4.0: the calibration identifier 0x0201 holds all 14 channels, vector ports, RTE-57; 0.3.4: initial values of the calibration ports, RTE-56; 0.3.3: stage C, plausibility check of the current measurement; 0.3.2: regulator hold 500 ms; 0.2.0 was stage B)")
param(c, "VersionPatch", "uint8", [1], "firmware version of 0x121 (0.5.1: SCL pull-up and 31.25 kHz of the display bus, neXtBSW 23642a8d)")
param(c, "ButtonDebounce_cycles", "uint8", [3], "[10 ms] a key level counts after this many equal samples")

# ------------------------------------------------------------------ CellCal (stage C)
c = "CellCal"
components[c] = dict(description="Kalibrierung der Zellkanaele: Werte per Diagnose uebernehmen (nur bei Ausgang aus), im NvM halten, an CellChannels geben",
                     runnable="CalTask10ms", ports=[], params=[])
port(c, "OutputEnable", "in", "boolean", "BattSimMode.OutputEnable.OutputEnable", "outputs enabled (the same name as the port of CellChannels and the provided port of BattSimMode: allowed since RTE-49)")
port(c, "ApplyRequest", "in", "uint8", "routine.ApplyCalibration.start", "counts the requests of the routine ApplyCalibration (31 01)")
for key, stem, wire, label, factor, lo, hi, default, unit in CAL:
    ptype = PORT_TYPE[wire]
    port(c, f"St{stem}", "in", ptype, f"did.CalValues.{key}", "staged values of the 14 channels written by the tester (2E 0201, all kinds at once)",
         dimension="CellCount")
    port(c, f"Nv{stem}In", "in", ptype, f"nvm.Cal{stem}", "stored value", dimension="CellCount")
    port(c, f"Nv{stem}Qlf", "in", "uint8", f"nvm.qlf.Cal{stem}")
for key, stem, wire, label, factor, lo, hi, default, unit in CAL:
    ptype = PORT_TYPE[wire]
    # RTE-56: the initial value of a port reaches the RTE buffer and the NvM default, so a block that was never written starts
    # with the factory value (a gain 1.0000 = code 10000), not with 0. Only the written ports (Nv...Out) and the ones read back
    # (Ac...) get it; the staged values (St...) keep 0, which means "never written" there.
    init_code = {"init": default} if default else {}
    init_phys = {"init": round(default * factor, 6)} if default else {}
    port(c, f"Nv{stem}Out", "out", ptype, f"nvm.Cal{stem}", "value to store", dimension="CellCount", **init_code)
    port(c, f"Ac{stem}", "out", ptype, ["rte", f"did.CalValues.{key}"], "values in use of the 14 channels (read back by the tester, 22 0201)",
         dimension="CellCount", **init_code)
    port(c, PHYS[key], "out", "float32", None, "value in use, physical, for CellChannels", dimension="CellCount", **init_phys)
port(c, "ApplyResult", "out", "uint8", ["rte", "routine.ApplyCalibration.results.result"])
port(c, "ApplyBadChannel", "out", "uint8", ["rte", "routine.ApplyCalibration.results.badChannel"])
port(c, "CalState", "out", "uint8", ["rte", "did.CalibrationState.state"])
param(c, "CellCount", "sint32", [14], "dimension of the vector ports, bound to BattSimParams.CellCount in rte.yaml", kind="constant",
      min=14, max=14, description="Anzahl der Zellkanaele (Dimension der Vektorports)")
param(c, "OffsetLimit_code", "uint16", [2000], "[0.1 mV] largest offset the component accepts (0.2 V); the diagnosis checks the same range",
      description="Grenze des Offsets in 0,1-mV-Schritten")
param(c, "GainMin_code", "uint16", [9000], "[1e-4] smallest gain the component accepts (0.9)", description="kleinste Steigung")
param(c, "GainMax_code", "uint16", [11000], "[1e-4] largest gain the component accepts (1.1)", description="groesste Steigung")

# ------------------------------------------------------------------ CellChannels
c = "CellChannels"
components[c] = dict(description="14 Zellkanaele: Sollwert auf Tastgrad, Freigabe, Istwert, Kalibrierung, Regler und Status",
                     runnable="CellTask10ms", ports=[], params=[])
port(c, "OutputEnable", "in", "boolean", "BattSimMode.OutputEnable.OutputEnable")
port(c, "LimitActive", "in", "boolean", "SupplyMonitor.LimitActive.LimitActive", "LIMIT line of the 48 V supply: the regulator stands still")
port(c, "ControllerOffReq", "in", "uint8", "did.ControllerControl.controllerOff", "1 = regulator switched off by the diagnosis (0 after every start: on)")
port(c, "HandActive", "in", "boolean", "BattSimMode.HandActive.HandActive", "hand mode: setpoints and releases come from HandControl, not from the CAN messages (stage D, step 2)")
port(c, "HandSetpoint_V", "in", "float32", "HandControl.HandSetpoint_V.HandSetpoint_V", "setpoints of the hand mode (HandControl, from the previous step)", dimension="CellCount")
port(c, "HandEnable", "in", "boolean", "HandControl.HandEnable.HandEnable", "releases of the hand mode (HandControl, from the previous step)", dimension="CellCount")
port(c, "SelectedChannel", "in", "uint8", "HandControl.SelectedChannel.SelectedChannel", "selected channel 1..14 (SELECT LED)")
port(c, "SelectAllMode", "in", "boolean", "HandControl.SelectAllMode.SelectAllMode", "all channels are edited together (SELECT LED of all channels)")
for key, stem, wire, label, factor, lo, hi, default, unit in CAL:
    port(c, PHYS[key], "in", "float32", f"CellCal.{PHYS[key]}.{PHYS[key]}", "from CellCal", dimension="CellCount")
for n in CH:
    nn, g = f"{n:02d}", GROUPS[n]
    port(c, f"Cell{nn}VoltSet_V", "in", "float32", f"can.rx.CONTROL_CELL{g}.CELL{nn}_VOLTAGE_SET")
    port(c, f"Cell{nn}Enable", "in", "uint8", f"can.rx.CONTROL_CELL{g}.CELL{nn}_ENABLE")
    port(c, f"UCell{nn}_mV", "in", "uint32", f"adc.mv.U_CELL_{nn}")
    port(c, f"UCell{nn}Qlf", "in", "uint8", f"adc.qlf.U_CELL_{nn}")
    port(c, f"En{nn}", "out", "boolean", f"dio.out.EN_{nn}")
    port(c, f"Sel{nn}", "out", "boolean", f"dio.out.SEL_{nn}", "SELECT LED of the card, hand mode of stage D")
    port(c, f"Cell{nn}VoltAct_V", "out", "float32", f"can.tx.STATUS_CELL{g}.CELL{nn}_VOLTAGE_ACT")
    port(c, f"Cell{nn}State", "out", "uint8", f"can.tx.STATUS_CELL{g}.CELL{nn}_STATUS")
    port(c, f"Duty{nn}", "out", "uint16", f"pwm.SET_{nn}", "duty in 0.01 %")
port(c, "ActualVolt_V", "out", "float32", None, "actual values of the 14 channels (the ones of the CAN status messages), for the display of HandControl", dimension="CellCount")
port(c, "ControllerOn", "out", "boolean", "can.tx.STATUS_BATTSIM_EXT.BATTSIM_CONTROLLER_ON", "regulator active (not switched off)")
port(c, "ControllerOffAct", "out", "uint8", ["rte", "did.ControllerControl.controllerOff"], "request of the diagnosis as the model uses it")
port(c, "ChannelsReleased", "out", "boolean", None, "output on and at least one channel released: feeds the plausibility check of the current measurement in SupplyMonitor")
port(c, "OverVoltage", "out", "boolean", None, "a released channel was above setpoint + OverVoltThd_V for OverVoltTime_cycles samples (C-2): BattSimMode switches off in RUN, FAIL reason 6")
port(c, "Cell15State", "out", "uint8", "can.tx.STATUS_CELL13_14_15.CELL15_STATUS", "not fitted: disabled")
port(c, "Cell15VoltAct_V", "out", "float32", "can.tx.STATUS_CELL13_14_15.CELL15_VOLTAGE_ACT", "not fitted: 0 V")
port(c, "Cell16State", "out", "uint8", "can.tx.STATUS_CELL16.CELL16_STATUS", "not fitted: disabled")
port(c, "Cell16VoltAct_V", "out", "float32", "can.tx.STATUS_CELL16.CELL16_VOLTAGE_ACT", "not fitted: 0 V")
SENSE_RAW = [0, 796, 1591, 2391, 3192, 3991]
SENSE_MV = [round(r * 2048 / 4095, 1) for r in SENSE_RAW]
param(c, "CellCount", "sint32", [14], "dimension of the vector ports, bound to BattSimParams.CellCount in rte.yaml", kind="constant",
      min=14, max=14, description="Anzahl der Zellkanaele (Dimension der Vektorports)")
param(c, "VoltageSetValue_Ax", "float32", [0, 1, 2, 3, 4, 5], "[V]    setpoint breakpoints (VoltageSetValue_Ax of the old firmware)", 6)
param(c, "VoltageSetPwm_T", "float32", [0, 0.185875, 0.378085, 0.570892, 0.763797, 0.9566], "[-]    duty (share of the period) at the breakpoints (VoltageSetPwm_T, 5 V = 0.9566)", 6)
param(c, "VoltageSenseMeas_Ax", "float32", SENSE_MV, "[mV]   ADC pin voltage at the breakpoints: counts [0 796 1591 2391 3192 3991] of the old firmware (12 bit) x 2048 mV / 4095", 6)
param(c, "VoltageSenseValue_T", "float32", [0, 1, 2, 3, 4, 5], "[V]    cell voltage at the breakpoints (VoltageSenseValue_T)", 6)
param(c, "SetpointLimit_V", "float32", [5.0], "[V]    upper setpoint limit, bound to BattSimParams.CellVoltLimit_V in rte.yaml (default 5.0 V; the 4.30 V limit stays in LabControl)",
      unit="V", min=0, max=5.0, description="Obere Grenze jedes Sollwerts")
param(c, "OverVoltThd_V", "float32", [0.2], "[V]    overvoltage: the calibrated actual value (sample, not low-passed) is this far above the setpoint (review C-2: 0.2 V)",
      unit="V", min=0.1, max=1.0, description="Schwelle der Ueberspannungserkennung")
param(c, "OverVoltTime_cycles", "uint8", [10], "[10 ms] number of samples in a row above the threshold that count as overvoltage (10 = 100 ms, review C-2)")
param(c, "OutputFloor_V", "float32", [0.7], "[V]    level floor of the output stage WITHOUT any external load (measured 0.70 V, 30.10.2026; with 1 kOhm or 100 Ohm at the channel the output follows the setpoint down to 0.3 V, the floor only sinks with a load): the overvoltage comparison uses max(setpoint, floor) + OverVoltThd_V, a setpoint below it gets status 4 (OL, open load: setpoint below the level floor without load), a hint, no fault (review C-2c, T-1: stays 0.7 V); to be measured with the test object",
      unit="V", min=0, max=2.0, description="Pegelboden der Endstufe ohne Last")
param(c, "StepThreshold_V", "float32", [0.2], "[V]    a change of the (limited) setpoint of a channel larger than this stops its regulator for StepHold_cycles (review R-1)",
      unit="V", min=0, max=5.0, description="Sollwertsprung, der den Regler anhaelt")
param(c, "StepHold_cycles", "uint8", [50], "[10 ms] hold time of the regulator after a setpoint step, until the low-passed actual value has settled (50 = 500 ms, board bs-haltezeit-500 a; 30 = 300 ms in 0.3.1 left -0.035 V at the device after 4.8 V -> 1.0 V; review R-1; low-pass time constant about 95 ms)")
param(c, "VoltageSenseFilter", "float32", [0.1], "[-]    low-pass weight of the actual value per 10 ms (time constant about 95 ms)")
param(c, "CtrlKp", "float32", [0.2], "[V/V]  proportional gain of the regulator of a channel (start value, to be tuned at the device)",
      unit="-", min=0, max=2.0, description="Proportionalanteil des Reglers")
param(c, "CtrlKi_per_s", "float32", [1.0], "[1/s]  integral gain of the regulator (start value, to be tuned at the device)",
      unit="1/s", min=0, max=10.0, description="Integralanteil des Reglers")
param(c, "CtrlLimit_V", "float32", [0.1], "[V]    the regulator corrects by at most this much in both directions (FW-07: 0.1 V)",
      unit="V", min=0, max=0.1, description="Begrenzung der Reglerkorrektur")

# ------------------------------------------------------------------ HandControl (stage D, part 1)
# Hand mode and selection (doc/stufe-d-handbetrieb-anzeige.md): the inputs come from the drivers of neXtBSW IO-118 (enc.*), the
# digital inputs (dio.in.*) and IO-117 (i2c.DISPLAY.write.*); the SIL test writes the RTE buffers.
c = "HandControl"
components[c] = dict(description="Handbetrieb: Drehgeber und Karten-Tasten zu Sollwerten und Freigaben der Kanaele, Kanalwahl, Anzeigewert",
                     runnable="HandTask10ms", ports=[], params=[])
port(c, "HandActive", "in", "boolean", "BattSimMode.HandActive.HandActive", "hand mode (STANDALONE) is active (BattSimMode)")
port(c, "EncVoltCount", "in", "uint16", "enc.ENC_VOLTAGE.count", "free-running counter of the voltage encoder (enc.ENC_VOLTAGE.count, IO-118), wraps")
port(c, "EncVoltButton", "in", "boolean", "dio.in.ENC_VOLT_BTN", "push button of the voltage encoder (dio.in.ENC_VOLT_BTN), true = pressed")
port(c, "EncSelCount", "in", "uint16", "enc.ENC_SELECT.count", "free-running counter of the channel encoder (enc.ENC_SELECT.count, IO-118), wraps")
port(c, "EncSelButton", "in", "boolean", "dio.in.ENC_SEL_BTN", "push button of the channel encoder (dio.in.ENC_SEL_BTN), true = pressed")
for n in CH:
    port(c, f"CardKey{n:02d}", "in", "boolean", f"dio.in.ST_{n:02d}", f"STATUS key of card {n} (dio.in.ST_{n:02d}), true = pressed")
port(c, "ActualVolt_V", "in", "float32", "CellChannels.ActualVolt_V.ActualVolt_V", "actual values of the channels (CellChannels, from the same step)", dimension="CellCount")
port(c, "HandSetpoint_V", "out", "float32", None, "setpoints of the hand mode", dimension="CellCount")
port(c, "HandEnable", "out", "boolean", None, "release of the channels in the hand mode", dimension="CellCount")
port(c, "SelectedChannel", "out", "uint8", None, "selected channel 1..14 (SELECT LED of the card)")
port(c, "SelectAllMode", "out", "boolean", None, "all channels are edited together")
port(c, "DisplayValue_mV", "out", "uint16", None, "actual value of the selected channel in mV (0..9999) for the display")
param(c, "CellCount", "sint32", [14], "dimension of the vector ports, bound to BattSimParams.CellCount in rte.yaml", kind="constant",
      min=14, max=14, description="Anzahl der Zellkanaele (Dimension der Vektorports)")
param(c, "SetpointLimit_V", "float32", [5.0], "[V]    upper setpoint limit, bound to BattSimParams.CellVoltLimit_V in rte.yaml",
      unit="V", min=0, max=5.0, description="Obere Grenze jedes Sollwerts")
param(c, "CoarseStep_mV", "float32", [15], "[mV]   setpoint step per detent, coarse (derived from EncoderGainCoarse_P = 30 of the old firmware, to be measured at the device)",
      unit="mV", min=1, max=100, description="Sollwertschritt je Raste grob")
param(c, "FineStep_mV", "float32", [1], "[mV]   setpoint step per detent, fine (derived from EncoderGainFine_P = 2, to be measured at the device)",
      unit="mV", min=0.1, max=10, description="Sollwertschritt je Raste fein")
param(c, "CountsPerStep", "uint8", [4], "[-]    counts of the quadrature decoder per detent (x4 decoding)",
      unit="-", min=1, max=8, description="Zaehlerschritte je Raste")
param(c, "KeyDebounce_cycles", "uint8", [3], "[10 ms] a key level counts after this many equal samples")

# ------------------------------------------------------------------ DisplayDriver (stage D, part 1)
c = "DisplayDriver"
components[c] = dict(description="Anzeige: 4 Ziffern des 7-Segment-Moduls (I2C 0x71), ein Byte je Schreibauftrag",
                     runnable="DispTask10ms", ports=[], params=[])
port(c, "DisplayValue_mV", "in", "uint16", "HandControl.DisplayValue_mV.DisplayValue_mV", "value to show in mV, from HandControl")
port(c, "DispTxDone", "in", "uint8", "i2c.DISPLAY.write.done", "counter of the finished write orders (i2c.DISPLAY.write.done, IO-117); equal to DispTxRequest = free")
port(c, "DispTxError", "in", "boolean", "i2c.DISPLAY.write.error", "the last write order failed (i2c.DISPLAY.write.error, IO-117)")
port(c, "DispTxByte", "out", "uint8", "i2c.DISPLAY.write.byte", "the next byte for address 0x71 (i2c.DISPLAY.write.byte)")
port(c, "DispTxRequest", "out", "uint8", "i2c.DISPLAY.write.request", "counter: every change is one write order of one byte (i2c.DISPLAY.write.request)")
param(c, "DisplayRefresh_cycles", "uint16", [100], "[10 ms] the picture is sent again after this time even if the value did not change (a module that lost its power comes back)")
param(c, "DisplayRetry_cycles", "uint16", [50], "[10 ms] pause after a failed write order, then the picture starts again with a clear")


def fmt(v):
    return repr(float(v)) if isinstance(v, float) else str(v)


def yaml_scalar(v):
    if isinstance(v, str):
        return json.dumps(v, ensure_ascii=False)
    return fmt(v)


for comp, d in components.items():
    r = d["runnable"]
    lines = ["# SPDX-License-Identifier: Apache-2.0",
             "# Description of " + comp + ": the model " + comp + ".slx is the master (made by " + comp + "_create.m); this file is the",
             "# description it matches, and tools/rtegen compares it with the generated ARXML. Written by tools/gen_swc_ports.py.",
             "version: 1", f"name: {comp}", "description: " + json.dumps(d["description"], ensure_ascii=False),
             "implementation: simulink", "runnables:", f"  - name: {r}", "    period_ms: 10", "ports:"]
    for name, direction, ptype, comment, extra in d["ports"]:
        lines.append(f"  - name: {name}" + (f"           # {comment}" if comment else ""))
        lines += [f"    direction: {direction}", f"    type: {ptype}"]
        if "dimension" in extra:
            lines.append(f"    dimension: {extra['dimension']}")
        if "init" in extra:
            lines.append(f"    init: {fmt(extra['init'])}")
        lines.append(f"    runnables: [{r}]")
    lines.append("parameters:")
    data = ["% SPDX-License-Identifier: Apache-2.0",
            f"% Calibration values of {comp} (RTE-03), the only place of these numbers. Read without MATLAB by tools/swc/mdata.py,",
            "% in MATLAB by swc_load_data. Written by tools/gen_swc_ports.py.", ""]
    for name, ptype, values, comment, dim, extra in d["params"]:
        lines.append(f"  - name: {name}")
        if extra.get("kind"):
            lines.append(f"    kind: {extra['kind']}")
        lines.append(f"    type: {ptype}")
        if dim:
            lines.append(f"    dim: {dim}")
        for key in ("unit", "min", "max", "description"):
            if key in extra:
                lines.append(f"    {key}: {yaml_scalar(extra[key])}")
        if extra.get("kind") == "constant":
            lines.append(f"    value: {fmt(values[0])}")
        else:
            lines.append(f"    runnables: [{r}]")
            rhs = fmt(values[0]) if len(values) == 1 else "[" + " ".join(fmt(v) for v in values) + "]"
            data.append(f"{name} = {rhs};   % {comment}")
    (SWC / comp).mkdir(exist_ok=True)
    (SWC / comp / "swc.yaml").write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    (SWC / comp / f"{comp}_data.m").write_text("\n".join(data) + "\n", encoding="utf-8", newline="\n")
    print(comp, len(d["ports"]), "ports", len(d["params"]), "parameters")

# ------------------------------------------------------------------ connections of rte.yaml
# key: the port that is connected; value: its source (required port) or its targets (provided port with a CAN signal,
# a pin, a data identifier, ...). A provided port that only feeds components has no line.
order = ["SupplyMonitor", "BattSimMode", "CellCal", "CellChannels", "HandControl", "DisplayDriver"]
out = ["connections:"]
for comp in order:
    for key, target in connections.items():
        if key.startswith(comp + "."):
            value = target if isinstance(target, str) else "[" + ", ".join(target) + "]"
            out.append(f"  {key}: {value}")
conn_text = "\n".join(out) + "\n"
rte = PROJECT / "rte.yaml"
t = rte.read_bytes().decode("utf-8")
nl = "\r\n" if "\r\n" in t else "\n"
t = t.replace("\r\n", "\n")
head = t[:t.index("connections:")]
rte.write_bytes((head + conn_text).replace("\n", nl).encode("utf-8"))
print("connections", len(out) - 1)

# ------------------------------------------------------------------ data identifiers of the calibration in diag.yaml
BEGIN = "  # ---- begin generated by tools/gen_swc_ports.py: calibration of the cell channels (stage C) ----"
END = "  # ---- end generated by tools/gen_swc_ports.py ----"
did = [BEGIN]
did += [
    f"  - identifier: 0x{CAL_DID:04X}",
    "    name: CalValues",
    "    label: Calibration of the cell channels",
    "    description: >-",
    "      Offset and gain for the setpoint and for the actual value of the 14 channels (y = gain * x + offset), four fields of",
    "      14 values each, channel 1 first: SetOffset, SetGain, MeasOffset, MeasGain (112 bytes). Reading gives the values in",
    "      use; writing stages new values for all channels at once (read first and write back the unchanged channels), which",
    "      take effect only with the routine ApplyCalibration (outputs off).",
    "    group: calibration",
    "    read: true",
    "    read_sessions: [Default, Extended]",
    "    write: true",
    "    write_sessions: [Extended]",
    "    write_security: Application",
    "    data:"]
for key, stem, wire, label, factor, lo, hi, default, unit in CAL:
    did += [f"      - name: {key}",
            f"        label: {label}",
            f"        type: {wire}",
            f"        count: {len(CH)}",
            f"        factor: {factor}"]
    if unit:
        did.append(f"        unit: {unit}")
    did += [f"        min: {lo}",
            f"        max: {hi}",
            f"        source: {{port: CellCal.Ac{stem}.Ac{stem}}}"]
did += [END]
diag = PROJECT / "diag.yaml"
text = diag.read_bytes().decode("utf-8")
nl = "\r\n" if "\r\n" in text else "\n"
text = text.replace("\r\n", "\n")
block = "\n".join(did) + "\n"
if BEGIN in text:
    text = re.sub(re.escape(BEGIN) + r".*?" + re.escape(END) + "\n", lambda m: block, text, flags=re.S)
    diag.write_bytes(text.replace("\n", nl).encode("utf-8"))
    print("diag.yaml: block replaced")
else:
    print("diag.yaml: markers missing; add the begin and end line inside data_identifiers: and run again")
