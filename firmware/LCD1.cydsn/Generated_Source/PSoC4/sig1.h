/*******************************************************************************
* File Name: sig1.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_sig1_H) /* Pins sig1_H */
#define CY_PINS_sig1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "sig1_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} sig1_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   sig1_Read(void);
void    sig1_Write(uint8 value);
uint8   sig1_ReadDataReg(void);
#if defined(sig1__PC) || (CY_PSOC4_4200L) 
    void    sig1_SetDriveMode(uint8 mode);
#endif
void    sig1_SetInterruptMode(uint16 position, uint16 mode);
uint8   sig1_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void sig1_Sleep(void); 
void sig1_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(sig1__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define sig1_DRIVE_MODE_BITS        (3)
    #define sig1_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - sig1_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the sig1_SetDriveMode() function.
         *  @{
         */
        #define sig1_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define sig1_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define sig1_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define sig1_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define sig1_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define sig1_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define sig1_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define sig1_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define sig1_MASK               sig1__MASK
#define sig1_SHIFT              sig1__SHIFT
#define sig1_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in sig1_SetInterruptMode() function.
     *  @{
     */
        #define sig1_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define sig1_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define sig1_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define sig1_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(sig1__SIO)
    #define sig1_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(sig1__PC) && (CY_PSOC4_4200L)
    #define sig1_USBIO_ENABLE               ((uint32)0x80000000u)
    #define sig1_USBIO_DISABLE              ((uint32)(~sig1_USBIO_ENABLE))
    #define sig1_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define sig1_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define sig1_USBIO_ENTER_SLEEP          ((uint32)((1u << sig1_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << sig1_USBIO_SUSPEND_DEL_SHIFT)))
    #define sig1_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << sig1_USBIO_SUSPEND_SHIFT)))
    #define sig1_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << sig1_USBIO_SUSPEND_DEL_SHIFT)))
    #define sig1_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(sig1__PC)
    /* Port Configuration */
    #define sig1_PC                 (* (reg32 *) sig1__PC)
#endif
/* Pin State */
#define sig1_PS                     (* (reg32 *) sig1__PS)
/* Data Register */
#define sig1_DR                     (* (reg32 *) sig1__DR)
/* Input Buffer Disable Override */
#define sig1_INP_DIS                (* (reg32 *) sig1__PC2)

/* Interrupt configuration Registers */
#define sig1_INTCFG                 (* (reg32 *) sig1__INTCFG)
#define sig1_INTSTAT                (* (reg32 *) sig1__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define sig1_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(sig1__SIO)
    #define sig1_SIO_REG            (* (reg32 *) sig1__SIO)
#endif /* (sig1__SIO_CFG) */

/* USBIO registers */
#if !defined(sig1__PC) && (CY_PSOC4_4200L)
    #define sig1_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define sig1_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define sig1_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define sig1_DRIVE_MODE_SHIFT       (0x00u)
#define sig1_DRIVE_MODE_MASK        (0x07u << sig1_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins sig1_H */


/* [] END OF FILE */
