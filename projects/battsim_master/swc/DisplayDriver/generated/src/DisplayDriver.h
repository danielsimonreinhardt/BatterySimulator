/*
 * File: DisplayDriver.h
 *
 * Code generated for Simulink model 'DisplayDriver'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef DisplayDriver_h_
#define DisplayDriver_h_
#ifndef DisplayDriver_COMMON_INCLUDES_
#define DisplayDriver_COMMON_INCLUDES_
#include "Platform_Types.h"
#include "Rte_DisplayDriver.h"
#endif                                 /* DisplayDriver_COMMON_INCLUDES_ */

#include "DisplayDriver_types.h"

/* PublicStructure Variables for Internal Data, for system '<Root>' */
typedef struct {
  uint16 shown;                        /* '<S1>/DispCore' */
  uint16 age;                          /* '<S1>/DispCore' */
  uint16 wait;                         /* '<S1>/DispCore' */
  uint8 frame[15];                     /* '<S1>/DispCore' */
  uint8 req;                           /* '<S1>/DispCore' */
  uint8 outByte;                       /* '<S1>/DispCore' */
  uint8 step;                          /* '<S1>/DispCore' */
  uint8 len;                           /* '<S1>/DispCore' */
  boolean needClear;                   /* '<S1>/DispCore' */
  boolean inflight;                    /* '<S1>/DispCore' */
} ARID_DEF_DisplayDriver_T;

/* PublicStructure Variables for Internal Data */
extern ARID_DEF_DisplayDriver_T DisplayDriver_ARID_DEF;/* '<S1>/DispCore' */

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
 * '<Root>' : 'DisplayDriver'
 * '<S1>'   : 'DisplayDriver/DisplayDriver_task10ms'
 * '<S2>'   : 'DisplayDriver/DisplayDriver_task10ms/DispCore'
 */
#endif                                 /* DisplayDriver_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
