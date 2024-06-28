
/* ******************************************************************************
 *
 * Module: Common - Platform Types Abstraction
 *
 * File Name: STD_TYPES.h
 *
 * Description: Types for STM32F103C8 bluepill
 * 
 * Author: Fady
 * 
 *******************************************************************************/

#ifndef _STD_TYPES_H
#define _STD_TYPES_H


#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE                        static inline
#endif

typedef enum 
{
	FALSE,
	TRUE
}bool;


/* Boolean Data Type */
typedef unsigned char bool_t;

/* Boolean Values */
#ifndef FALSE
#define FALSE       (0u)
#endif
#ifndef TRUE
#define TRUE        (1u)
#endif

#define HIGH        (1u)
#define LOW         (0u)

/* NULL Value */
#define NULL_PTR	(void*)0
#define NULL_t		(void*)0


typedef unsigned char* 			pu8_t;
typedef unsigned char* 			pu16_t;
typedef unsigned char* 			pu32_t;

typedef unsigned char 			uint8_t;			/*       0 .. 255             		*/
typedef unsigned short int 		uint16_t;			/*       0 .. 65535           		*/
typedef unsigned long int  		uint32_t;			/*       0 .. 4294967295      		*/
typedef unsigned long long    	uint64_t;			/*       0 .. 18446744073709551615  */

typedef signed char 			sint8_t;			/*        -128 .. +127            	*/
typedef signed short int 		sint16_t;			/*      -32768 .. +32767         	*/
typedef signed long int  		sint32_t;			/*	 -2147483648 .. +2147483647     */
typedef signed long long      	sint64_t;

typedef float  					float32_t;
typedef double					float64_t;

typedef unsigned char* 			pu8;
typedef unsigned char* 			pu16;
typedef unsigned char* 			pu32;

typedef unsigned char 			u8;				/*       0 .. 255             		*/
typedef unsigned short int 		u16;			/*       0 .. 65535           		*/
typedef unsigned long int  		u32;			/*       0 .. 4294967295      		*/
typedef unsigned long long    	u64;			/*       0 .. 18446744073709551615  */

typedef signed char 			s8;				/*        -128 .. +127            	*/
typedef signed short int 		s6;				/*      -32768 .. +32767         	*/
typedef signed long int  		s32;			/*	 -2147483648 .. +2147483647     */
typedef signed long long      	s64;

typedef float  					f32;
typedef double					f64;


#endif /* STD_TYPES_H_ */
