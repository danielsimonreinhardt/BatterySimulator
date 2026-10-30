/*
 * File: SupplyMonitor.h
 *
 * Code generated for Simulink model 'SupplyMonitor'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef SupplyMonitor_h_
#define SupplyMonitor_h_
#ifndef SupplyMonitor_COMMON_INCLUDES_
#define SupplyMonitor_COMMON_INCLUDES_
#include "Platform_Types.h"
#include "Rte_SupplyMonitor.h"
#endif                                 /* SupplyMonitor_COMMON_INCLUDES_ */

#include "SupplyMonitor_types.h"

/* PublicStructure Variables for Internal Data, for system '<Root>' */
typedef struct {
  float32 y;                           /* '<S1>/SupplyCore' */
  uint8 bad;                           /* '<S1>/SupplyCore' */
  boolean ovl;                         /* '<S1>/SupplyCore' */
} ARID_DEF_SupplyMonitor_T;

/* PublicStructure Variables for Internal Data */
extern ARID_DEF_SupplyMonitor_T SupplyMonitor_ARID_DEF;/* '<S1>/SupplyCore' */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SupplyMonitor'
 * '<S1>'   : 'SupplyMonitor/SupplyMonitor_task10ms'
 * '<S2>'   : 'SupplyMonitor/SupplyMonitor_task10ms/SupplyCore'
 */
#endif                                 /* SupplyMonitor_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
