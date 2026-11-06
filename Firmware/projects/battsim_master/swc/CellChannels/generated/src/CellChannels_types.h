/*
 * File: CellChannels_types.h
 *
 * Code generated for Simulink model 'CellChannels'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef CellChannels_types_h_
#define CellChannels_types_h_

/* Includes for objects with custom storage classes */
#include "Rte_Cfg.h"

/*
 * Registered constraints for dimension variants
 */
/* Constraint 'Rte_SysCon_CellCount == 14' registered by:
 * '<S1>/CalMeasGain_r'
 * '<S1>/CalMeasOffset_V_r'
 * '<S1>/CalSetGain_r'
 * '<S1>/CalSetOffset_V_r'
 * '<S1>/HandEnable_r'
 * '<S1>/HandSetpoint_V_r'
 */
#if Rte_SysCon_CellCount != 14
# error "The preprocessor definition 'Rte_SysCon_CellCount' must be equal to '14'"
#endif
#endif                                 /* CellChannels_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
