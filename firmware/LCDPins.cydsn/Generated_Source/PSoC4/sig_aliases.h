/*******************************************************************************
* File Name: sig.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_sig_ALIASES_H) /* Pins sig_ALIASES_H */
#define CY_PINS_sig_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define sig_0			(sig__0__PC)
#define sig_0_PS		(sig__0__PS)
#define sig_0_PC		(sig__0__PC)
#define sig_0_DR		(sig__0__DR)
#define sig_0_SHIFT	(sig__0__SHIFT)
#define sig_0_INTR	((uint16)((uint16)0x0003u << (sig__0__SHIFT*2u)))

#define sig_INTR_ALL	 ((uint16)(sig_0_INTR))


#endif /* End Pins sig_ALIASES_H */


/* [] END OF FILE */
