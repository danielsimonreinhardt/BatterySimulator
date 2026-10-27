<!-- SPDX-License-Identifier: Apache-2.0 WITH AdditionRef-neXtBSW-Generator-Output-Exception -->
<!-- Generator output exception (NOTICE): this generated file may be used under terms of your choice. -->
# Board `battsim_master_v1`

Own ECU board - S32K146 in package LQFP144 (family `s32k1xx`, core cortex-m4f).

Created with the new-board wizard of neXtBSW (`nxconfig board new`, neXtBSW Studio: *Neues Board anlegen*). The board file [`board.yaml`](board.yaml) carries **hardware facts only**: what is fitted and how it is wired. What a software project uses of it - clock, bit rates, tasks, safety pins - is decided in `projects/<project>/project.yaml` (architecture section 4).

## What is confirmed and what is not

Every row of `board.yaml` was **entered by hand** in the wizard and is marked `verify: true`. The pin functions are the only part that is backed: the wizard offered only pins that can carry the function in this package (`mcudb/s32k1xx/S32K146_LQFP144.pinmux.yaml`). Before the first bring-up:

1. read the schematic and confirm every pin, every active level, every pull resistor, the crystal, the supply and the ADC reference;
2. name the schematic (document, revision, date) in the `[SCH]` line of the source block;
3. drop `verify: true` from every row the document confirms.

A row that is still marked `verify` is not wrong - it is unproven, and `nxconfig check` says so.

## What the board offers

| Section | Entries |
|---|---|
| `can` | `CAN0` (FlexCAN0) |
| `i2c` | `DISPLAY` (FlexIO) |
| `dio.inputs` | `ST_01`, `ST_02`, `ST_03`, `ST_04`, `ST_05`, `ST_06`, `ST_07`, `ST_08`, `ST_09`, `ST_10`, `ST_11`, `ST_12`, `ST_13`, `ST_14`, `BTN_ALL_ON_OFF`, `BTN_REMOTE`, `LIMIT`, `ENC_VOLT_BTN`, `ENC_SEL_BTN` |
| `dio.outputs` | `EN_01`, `SEL_01`, `EN_02`, `SEL_02`, `EN_03`, `SEL_03`, `EN_04`, `SEL_04`, `EN_05`, `SEL_05`, `EN_06`, `SEL_06`, `EN_07`, `SEL_07`, `EN_08`, `SEL_08`, `EN_09`, `SEL_09`, `EN_10`, `SEL_10`, `EN_11`, `SEL_11`, `EN_12`, `SEL_12`, `EN_13`, `SEL_13`, `EN_14`, `SEL_14`, `LED1`, `LED2`, `LED_REMOTE` |
| `adc` | `U_CELL_01` (ADC0), `U_CELL_02` (ADC1), `U_CELL_03` (ADC1), `U_CELL_04` (ADC1), `U_CELL_05` (ADC1), `U_CELL_06` (ADC1), `U_CELL_07` (ADC1), `U_CELL_08` (ADC1), `U_CELL_09` (ADC1), `U_CELL_10` (ADC1), `U_CELL_11` (ADC1), `U_CELL_12` (ADC1), `U_CELL_13` (ADC1), `U_CELL_14` (ADC1), `I_SUPPLY` (ADC0) |

## Building a project on this board

```sh
.venv/Scripts/python tools/nxconfig new <project> --board battsim_master_v1
.venv/Scripts/python tools/nxconfig check projects/<project>/project.yaml
```

No board port was written. Since **ARCH-58** that is not needed to build: the feature switches are derived from the project and from `ecu/platforms/s32k1xx/features.yaml`. Add `ecu/platforms/battsim_master_v1/Options.cmake` only for a justified exception of this board (`ecu/platforms/README.md`).

## What the family port of `s32k1xx` needs

Stated in [`platforms/s32k1xx/port/port.yaml`](../../../../../../../../../../WORK/vscode_workspace/S32K14x basic software/platforms/s32k1xx/port/port.yaml) and checked by `nxconfig check`:

- `board.adc_reference_mv` of this file - the nominal voltage at VREFH the reference monitor compares its bandgap reading with (generated as bsp/adc/adcReferenceConfiguration.h, ARCH-32); without it nothing can be said about the reference (platforms/s32k1xx/port/safety/safeSystem/src/safeSystem/SafeSystem.cpp); stated in platforms/s32k1xx/port/port.yaml
- an ADC instance of S32K146 that offers the internal channel `BANDGAP`; the project reads it as `adc_internal` entry `BANDGAP`
- an ADC instance of S32K146 that offers the internal channel `INTERNAL0`; the project reads it as `adc_internal` entry `ADCINTERNAL0`
- section `safety` of the project (the project wizard writes it)
- section `memory` of the project (the project wizard writes it)
