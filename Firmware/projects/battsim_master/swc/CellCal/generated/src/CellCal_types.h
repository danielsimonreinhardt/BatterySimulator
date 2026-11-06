/*
 * File: CellCal_types.h
 *
 * Code generated for Simulink model 'CellCal'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef CellCal_types_h_
#define CellCal_types_h_

/* Includes for objects with custom storage classes */
#include "Rte_Cfg.h"

/*
 * Registered constraints for dimension variants
 */
/* Constraint 'Rte_SysCon_CellCount == 14' registered by:
 * '<S1>/NvMeasGainIn_r'
 * '<S1>/NvMeasOffsetIn_r'
 * '<S1>/NvSetGainIn_r'
 * '<S1>/NvSetOffsetIn_r'
 * '<S1>/StMeasGain_r'
 * '<S1>/StMeasOffset_r'
 * '<S1>/StSetGain_r'
 * '<S1>/StSetOffset_r'
 */
#if Rte_SysCon_CellCount != 14
# error "The preprocessor definition 'Rte_SysCon_CellCount' must be equal to '14'"
#endif
#endif                                 /* CellCal_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
