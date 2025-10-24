// ================================================================================ //
// The NEORV32 RISC-V Processor - https://github.com/stnolting/neorv32              //
// Copyright (c) NEORV32 contributors.                                              //
// Copyright (c) 2020 - 2025 Stephan Nolting. All rights reserved.                  //
// Licensed under the BSD-3-Clause license, see LICENSE for details.                //
// SPDX-License-Identifier: BSD-3-Clause                                            //
// ================================================================================ //


/**********************************************************************//**
 * @file hello_world/main.c
 * @author Stephan Nolting
 * @brief Classic 'hello world' demo program.
 **************************************************************************/

#include <neorv32.h>


/**********************************************************************//**
 * @name User configuration
 **************************************************************************/
/**@{*/
/** UART BAUD rate */
#define BAUD_RATE 19200
/**@}*/

#define MATRIX_SIZE 1300
#define KERNEL_SIZE 3


void run_dma() {
    // trigger DMA transfer
  neorv32_dma_start();

  // wait for transfer to complete using polling
  neorv32_uart0_printf("Waiting for DMA... \n");
  int dma_rc;
  while (1) {
    dma_rc = neorv32_dma_status();
    if (dma_rc == DMA_STATUS_DONE) {
      neorv32_uart0_printf("Transfer done.\n");
      break;
    }
    else if (dma_rc == DMA_STATUS_ERROR) {
      neorv32_uart0_printf("Transfer failed!\n");
      break;
    }
  }
}


/**********************************************************************//**
 * Main function; prints some fancy stuff via UART.
 *
 * @note This program requires the UART interface to be synthesized.
 *
 * @return 0 if execution was successful
 **************************************************************************/
int main() {

  // capture all exceptions and give debug info via UART
  // this is not required, but keeps us safe
  neorv32_rte_setup();

  // setup UART at default baud rate, no interrupts
  neorv32_uart0_setup(BAUD_RATE, 0);

  // print project logo via UART
  // neorv32_aux_print_logo();

  // say hello
  neorv32_uart0_puts("Ready\n");

  union {
    unsigned char b[4];
    uintptr_t p;
  } input;

  neorv32_uart0_puts("Awaiting input\n");
  uint32_t bytesRead = 0;
  while (bytesRead < 4) {
    if (neorv32_uart0_char_received()) {
      input.b[bytesRead] = neorv32_uart0_char_received_get();
      neorv32_uart0_printf("Received byte: %u\n", input.b[bytesRead]);
      bytesRead++;
      if (bytesRead < 4) {
        neorv32_uart0_puts("Awaiting input\n");
      }
    }
  }


  union {
    unsigned char b[4];
    uintptr_t p;
  } kernel;

  neorv32_uart0_puts("Awaiting kernel\n");
  bytesRead = 0;
  while (bytesRead < 4) {
    if (neorv32_uart0_char_received()) {
      kernel.b[bytesRead] = neorv32_uart0_char_received_get();
      neorv32_uart0_printf("Received byte: %u\n", kernel.b[bytesRead]);
      bytesRead++;
      if (bytesRead < 4) {
        neorv32_uart0_puts("Awaiting kernel\n");
      }
    }
  }

  union {
    unsigned char b[4];
    uintptr_t p;
  } output;

  neorv32_uart0_puts("Awaiting output\n");
  bytesRead = 0;
  while (bytesRead < 4) {
    if (neorv32_uart0_char_received()) {
      output.b[bytesRead] = neorv32_uart0_char_received_get();
      neorv32_uart0_printf("Received byte: %u\n", output.b[bytesRead]);
      bytesRead++;
      if (bytesRead < 4) {
        neorv32_uart0_puts("Awaiting output\n");
      }
    }
  }


  neorv32_uart0_printf("Input address: %u\n", (uint32_t)input.p);
  neorv32_uart0_printf("Output address: %u\n", (uint32_t)output.p);


  volatile uint32_t* inputPtr = (volatile uint32_t*)input.p;
  volatile uint32_t* kernelPtr = (volatile uint32_t*)kernel.p;
  volatile uint64_t* outputPtr = (volatile uint64_t*)output.p;

  // neorv32_uart0_printf("Input data:\n");
  // for (int y = 0; y < MATRIX_SIZE; y++) {
  //   for (int x = 0; x < MATRIX_SIZE; x++) {
  //     int i = y*MATRIX_SIZE + x;
  //     neorv32_uart0_printf("%u ", inputPtr[i]);
  //   }
  //   neorv32_uart0_printf("\n");
  // }

  neorv32_uart0_printf("Kernel data:\n");
  for (int y = 0; y < KERNEL_SIZE; y++) {
    for (int x = 0; x < KERNEL_SIZE; x++) {
      int i = y*KERNEL_SIZE + x;
      neorv32_uart0_printf("%u ", kernelPtr[i]);
    }
    neorv32_uart0_printf("\n");
  }



  // enable DMA
  // neorv32_dma_enable();
  // for (int y = 0; y < 12; y++) {

  //   neorv32_dma_program(
  //     input.p+y*13*32, // source array base address - byte-aligned
  //     output.p+y*12*64, // destination array base address - byte-aligned
  //     DMA_SRC_INC_BYTE |       // read source data as incrementing bytes
  //     DMA_DST_INC_BYTE |       // write destination data as incrementing bytes
  //     4*13                       // number of elements to transfer: 16
  //   );
  //   run_dma();
  // }
  // neorv32_uart0_printf("Row done\n");


  {
    for (int y = 0; y < (MATRIX_SIZE-KERNEL_SIZE+1); y++) {
      for (int x = 0; x < (MATRIX_SIZE-KERNEL_SIZE+1); x++) {
        uint64_t sum = 0;
        for (int ky = 0; ky < KERNEL_SIZE; ky++) {
          for (int kx = 0; kx < KERNEL_SIZE; kx++) {
            int in_y = y + ky;
            int in_x = x + kx;
            int in_i = in_y * MATRIX_SIZE + in_x;
            int k_i = ky * KERNEL_SIZE + kx;
            sum += inputPtr[in_i] * kernelPtr[k_i];
          }
        }
        int out_i = y * (MATRIX_SIZE - KERNEL_SIZE + 1) + x;
        outputPtr[out_i] = sum;
      }
      // neorv32_uart0_printf("Row done\n");
      // break;
      if (y % 100 == 0) {
        neorv32_uart0_printf("Row done\n");
      }
    }
  }

  neorv32_uart0_printf("Matrix done\n");


  // neorv32_uart0_printf("PL output data:\n");
  // for (int y = 0; y < (MATRIX_SIZE-KERNEL_SIZE+1); y++) {
  //   for (int x = 0; x < (MATRIX_SIZE-KERNEL_SIZE+1); x++) {
  //     int i = y*(MATRIX_SIZE-KERNEL_SIZE+1) + x;
  //     neorv32_uart0_printf("%u ", outputPtr[i]);
  //   }
  //   neorv32_uart0_printf("\n");
  // }

  // Signal jupyter notebook the program is finished, keep this in!
  neorv32_uart0_printf("\nExecution completed.\n");


  return 0;
}
