/******************************************************************************
 * @file     ncj29d5.ld
 * @brief    GNU Linker Script for Cortex-M based device
 * @version  V2.0.0
 * @date     21. May 2019
 ******************************************************************************/
/*
 * Copyright (c) 2009-2019 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

INCLUDE user-application-flash.ld;

__ROM_BASE = __SECONDARY_FLASH_START;
__ROM_SIZE = __SECONDARY_FLASH_SIZE; 

__ROM_END = (__ROM_BASE + __ROM_SIZE - 1);


/*--------------------- Embedded RAM Configuration ----------------------------
  <h> RAM Configuration
    <o0> RAM Base Address    <0x0-0xFFFFFFFF:8>
    <o1> RAM Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
 -----------------------------------------------------------------------------*/
__RAM_BASE = __SECONDARY_RAM_START;
__RAM_SIZE = __SECONDARY_RAM_SIZE;
__RAM_END = (__RAM_BASE + __RAM_SIZE - 1);

/*--------------------- Stack / Heap Configuration ----------------------------
  <h> Stack / Heap Configuration
    <o0> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
    <o1> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
  -----------------------------------------------------------------------------*/
__PROCESS_STACK_SIZE = 0x00000000;
__MAIN_STACK_SIZE = 0x00000000;
__HEAP_SIZE  = 0x00000000;


/*
 *-------------------- <<< end of configuration section >>> -------------------
 */



MEMORY
{
  FLASH (rx)  : ORIGIN = __ROM_BASE, LENGTH = __ROM_SIZE
  RAM   (rwx) : ORIGIN = __RAM_BASE, LENGTH = __RAM_SIZE
}

ENTRY(main)

SECTIONS
{

   .text  : {

      __bootblock_start__ = .;
      KEEP(*(.flash.fwHeader))
      __bootblock_end__ = .;     

        __text_start = .;
        *(.text)                 /* .text sections (code) */
        *(.text*)                /* .text* sections (code) */
        *(.rodata)               /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
        *(.glue_7)               /* glue arm to thumb code */
        *(.glue_7t)              /* glue thumb to arm code */
        *(.eh_frame)
        KEEP (*(.init))
        KEEP (*(.fini))

        /* . = ALIGN(256); */



   } > FLASH

  /*ASSERT((__bootblock_end__ - __bootblock_start__) == 20, "Boot block size is incorrect")*/

   .ARM.extab : {
        *(.ARM.extab .ARM.extab* .gnu.linkonce.armextab.*);
   } > FLASH

   .ARM.exidx : {
        *(.ARM.exidx .ARM.exidx* .gnu.linkonce.armexidx.*);
   } > FLASH

   __text_end__ = .;
   __text_size__ = SIZEOF(.text);

  /**
   * Location counter can end up 2byte aligned with narrow Thumb code but
   * __etext is assumed by startup code to be the LMA of a section in RAM
   * which must be 4byte aligned 
   */
  __etext = ALIGN (4);

  .data : AT (__text_end__) ALIGN(4)
  {
    __data_start__ = .;
    /*   *(vtable) */
    *(.data)
    *(.data.*)


    . = ALIGN(4);
    /* preinit data */
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    . = ALIGN(4);
    /* init data */
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);

    . = ALIGN(4);
    /* finit data */
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    . = ALIGN(4);
    /* All data end */
    __data_end__ = .;

  } > RAM

   __data_size__ = SIZEOF(.data);
   __flash_size = LENGTH(FLASH);
   ASSERT((__text_size__ + __data_size__) <= __flash_size, "Flash memory overflowed with .text and .data sections")

  .bss (NOLOAD) : ALIGN(4)
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > RAM AT > RAM


  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    __end__ = .;
    __HeapBase = .;
    PROVIDE(end = .);
    . = . + __HEAP_SIZE;
    . = ALIGN(8);
    __HeapLimit = .;
  } > RAM


  .process_stack (ORIGIN(RAM) + LENGTH(RAM) - __MAIN_STACK_SIZE - __PROCESS_STACK_SIZE) (COPY) :
  {
    . = ALIGN(8);
    __Process_StackLimit = .;
    . = . + __PROCESS_STACK_SIZE;
    . = ALIGN(8);
    __Process_StackTop = .;
  } > RAM
  PROVIDE(__process_stack = __Process_StackTop);

  .main_stack (ORIGIN(RAM) + LENGTH(RAM) - __MAIN_STACK_SIZE) (COPY) :
  {
    . = ALIGN(8);
    __Main_StackLimit = .;
    . = . + __MAIN_STACK_SIZE;
    . = ALIGN(8);
    __Main_StackTop = .;
  } > RAM
  PROVIDE(__main_stack = __Main_StackTop);

}
