/*******************************************************************************
* File Name: sig1.h  
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

#if !defined(CY_PINS_sig1_ALIASES_H) /* Pins sig1_ALIASES_H */
#define CY_PINS_sig1_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define sig1_0			(sig1__0__PC)
#define sig1_0_PS		(sig1__0__PS)
#define sig1_0_PC		(sig1__0__PC)
#define sig1_0_DR		(sig1__0__DR)
#define sig1_0_SHIFT	(sig1__0__SHIFT)
#define sig1_0_INTR	((uint16)((uint16)0x0003u << (sig1__0__SHIFT*2u)))

#define sig1_INTR_ALL	 ((uint16)(sig1_0_INTR))


#endif /* End Pins sig1_ALIASES_H */


/* [] END OF FILE */
