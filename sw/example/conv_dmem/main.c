#include <neorv32.h>
#define BAUD_RATE 19200

volatile uint32_t matrix[13][13] = {
    {  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12},
    { 13,14,15,16,17,18,19,20,21,22,23,24,25},
    { 26,27,28,29,30,31,32,33,34,35,36,37,38},
    { 39,40,41,42,43,44,45,46,47,48,49,50,51},
    { 52,53,54,55,56,57,58,59,60,61,62,63,64},
    { 65,66,67,68,69,70,71,72,73,74,75,76,77},
    { 78,79,80,81,82,83,84,85,86,87,88,89,90},
    { 91,92,93,94,95,96,97,98,99,100,101,102,103},
    {104,105,106,107,108,109,110,111,112,113,114,115,116},
    {117,118,119,120,121,122,123,124,125,126,127,128,129},
    {130,131,132,133,134,135,136,137,138,139,140,141,142},
    {143,144,145,146,147,148,149,150,151,152,153,154,155},
    {156,157,158,159,160,161,162,163,164,165,166,167,168}
};

volatile uint32_t kernel[2][2] = {
    {1,2},
    {3,4}
};


int main() {
    
  // initialize NEORV32 run-time environment
  neorv32_rte_setup();

  // setup UART at default baud rate, no interrupts
  neorv32_uart0_setup(BAUD_RATE, 0);

  // check if UART0 is implemented
  // if (neorv32_uart0_available() == 0) {
  //   return 1; // UART0 not available, exit
  // }

  // // check if Zihpm is implemented at all
  // if ((neorv32_cpu_csr_read(CSR_MXISA) & (1 << CSR_MXISA_ZIHPM)) == 0) {
  //   neorv32_uart0_printf("ERROR! Zihpm CPU extension not implemented!\n");
  //   return 1;
  // }

  // // check if at least one HPM counter is implemented
  // if (neorv32_cpu_hpm_get_num_counters() == 0) {
  //   neorv32_uart0_printf("ERROR! No HPM counters implemented!\n");
  //   return 1;
  // }


  // // intro
  // neorv32_uart0_puts("\n<<< NEORV32 Hardware Performance Monitors (HPMs) Example Program >>>\n\n");
  // neorv32_uart0_puts("[NOTE] This program will use up to 9 HPM counters (if available).\n\n");


  // // show HPM hardware configuration
  // uint32_t hpm_num = neorv32_cpu_hpm_get_num_counters();
  // uint32_t hpm_width = neorv32_cpu_hpm_get_size();
  // neorv32_uart0_printf("%u HPM counters detected, each %u bits wide\n", hpm_num, hpm_width);


  // // stop all CPU counters including HPMs
  // neorv32_cpu_csr_write(CSR_MCOUNTINHIBIT, -1);


  // // clear HPM counters (low and high word);
  // // there will be NO exception if we access a HPM counter register that has not been implemented
  // // as long as Zihpm is implemented
  // if (hpm_num > 0) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER3,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER3H,  0); }
  // if (hpm_num > 1) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER4,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER4H,  0); }
  // if (hpm_num > 2) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER5,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER5H,  0); }
  // if (hpm_num > 3) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER6,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER6H,  0); }
  // if (hpm_num > 4) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER7,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER7H,  0); }
  // if (hpm_num > 5) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER8,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER8H,  0); }
  // if (hpm_num > 6) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER9,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER9H,  0); }
  // if (hpm_num > 7) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER10, 0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER10H, 0); }
  // if (hpm_num > 8) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER11, 0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER11H, 0); }

  // // NOTE regarding HPMs 0..2, which are not "actual" HPMs
  // // - HPM 0 is the machine cycle counter
  // // - HPM 1 is the machine system timer
  // // - HPM 2 is the machine instret counter
  // // these counters have fixed event configurations; however, these according events can also be used for any other "real" HPM

  // // setup base counters if available
  // if ((neorv32_cpu_csr_read(CSR_MXISA) & (1 << CSR_MXISA_ZICNTR))) {
  //   neorv32_cpu_csr_write(CSR_MCYCLE,   0); neorv32_cpu_csr_write(CSR_MCYCLEH,   0);
  //   neorv32_cpu_csr_write(CSR_MINSTRET, 0); neorv32_cpu_csr_write(CSR_MINSTRETH, 0);
  // }

  // // configure events - one event per counter;
  // // we can also configure more than one event; the HPM will increment if _any_ event triggers (logical OR);
  // // there will be NO exception if we access a HPM event register that has not been implemented
  // // as long as Zihpm is implemented
  // if (hpm_num > 0) { neorv32_cpu_csr_write(CSR_MHPMEVENT3,  1 << HPMCNT_EVENT_COMPR);    } // executed compressed instruction
  // if (hpm_num > 1) { neorv32_cpu_csr_write(CSR_MHPMEVENT4,  1 << HPMCNT_EVENT_WAIT_DIS); } // instruction dispatch wait cycle
  // if (hpm_num > 2) { neorv32_cpu_csr_write(CSR_MHPMEVENT5,  1 << HPMCNT_EVENT_WAIT_ALU); } // multi-cycle ALU co-processor wait cycle
  // if (hpm_num > 3) { neorv32_cpu_csr_write(CSR_MHPMEVENT6,  1 << HPMCNT_EVENT_BRANCH);   } // executed branch instruction
  // if (hpm_num > 4) { neorv32_cpu_csr_write(CSR_MHPMEVENT7,  1 << HPMCNT_EVENT_BRANCHED); } // control flow transfer
  // if (hpm_num > 5) { neorv32_cpu_csr_write(CSR_MHPMEVENT8,  1 << HPMCNT_EVENT_LOAD);     } // executed load operation
  // if (hpm_num > 6) { neorv32_cpu_csr_write(CSR_MHPMEVENT9,  1 << HPMCNT_EVENT_STORE);    } // executed store operation
  // if (hpm_num > 7) { neorv32_cpu_csr_write(CSR_MHPMEVENT10, 1 << HPMCNT_EVENT_WAIT_LSU); } // load-store unit memory wait cycle
  // if (hpm_num > 8) { neorv32_cpu_csr_write(CSR_MHPMEVENT11, 1 << HPMCNT_EVENT_TRAP);     } // entered trap

  // for (int i = 0; i < 1600; i++) {
  //   for (int j = 0; j < 1600; j++) {
  //     matrix[i][j] = rand() % 65536; // values similar to your 13x13
  //   }
  // }
    
    uint64_t output[12][12] = {0}; // Resulting size is 12x12 for a 13x13 matrix with a 2x2 kernel
  // uint32_t output[12][12] = {0}; // Resulting size is 12x12 for a 13x13 matrix with a 2x2 kernel


  neorv32_uart0_printf("\nStarting program.\n");


  // enable all CPU counters including HPMs
  // neorv32_cpu_csr_write(CSR_MCOUNTINHIBIT, 0);


  // this is the part of the the program that is going to be "benchmarked" using the HPMs
  // here we are just doing some pointless stuff that will trigger the configured HPM events;
  // note that ALL code being executed will be benchmarked - including traps
  {
    for (int x = 0; x < 13; x++) {
      NEORV32_CFS->REG[0*13 + x] = matrix[0][x];
      // NEORV32_CFS->REG[1*13 + x] = matrix[1][x];
    }
    for (int y_out = 0; y_out < 12; y_out++) {
      int offset = (y_out + 1) % 2;
      for (int x = 0; x < 13; x++) {
        NEORV32_CFS->REG[offset*13 + x] = matrix[y_out + 1][x];
      }
      
      for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            NEORV32_CFS->REG[2*13 + (y+offset+1)%2*2 + x] = kernel[y][x];
        }
      }

      for (int i = 0; i < 12; i++) {
        output[y_out][i] = NEORV32_CFS->REG[i*2] + ((uint64_t)NEORV32_CFS->REG[i*2+1] << 32);
        // output[y_out][i] = NEORV32_CFS->REG[i*2];
      }
    }
  }


  // stop all CPU counters including HPMs
  // neorv32_cpu_csr_write(CSR_MCOUNTINHIBIT, -1);

  // // print HPM counter values (low word only)
  // neorv32_uart0_printf("\nHPM results (low-words only):\n");
  // if ((neorv32_cpu_csr_read(CSR_MXISA) & (1 << CSR_MXISA_ZICNTR))) {
  //   neorv32_uart0_printf(" cycle (active clock cycles)         : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MCYCLE));
  //   neorv32_uart0_printf(" instret (retired instructions)      : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MINSTRET));
  // }
  // if (hpm_num > 0) { neorv32_uart0_printf(" HPM03 (compressed instructions)     : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MHPMCOUNTER3));  }
  // if (hpm_num > 1) { neorv32_uart0_printf(" HPM04 (instr. dispatch wait cycles) : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MHPMCOUNTER4));  }
  // if (hpm_num > 2) { neorv32_uart0_printf(" HPM05 (ALU wait cycles)             : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MHPMCOUNTER5));  }
  // if (hpm_num > 3) { neorv32_uart0_printf(" HPM06 (branch instructions)         : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MHPMCOUNTER6));  }
  // if (hpm_num > 4) { neorv32_uart0_printf(" HPM07 (control flow transfers)      : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MHPMCOUNTER7));  }
  // if (hpm_num > 5) { neorv32_uart0_printf(" HPM08 (load instructions)           : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MHPMCOUNTER8));  }
  // if (hpm_num > 6) { neorv32_uart0_printf(" HPM09 (store instructions)          : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MHPMCOUNTER9));  }
  // if (hpm_num > 7) { neorv32_uart0_printf(" HPM10 (load/store wait cycles)      : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MHPMCOUNTER10)); }
  // if (hpm_num > 8) { neorv32_uart0_printf(" HPM11 (entered traps)               : %u\n", (uint32_t)neorv32_cpu_csr_read(CSR_MHPMCOUNTER11)); }

  //   // clear HPM counters (low and high word);
  // // there will be NO exception if we access a HPM counter register that has not been implemented
  // // as long as Zihpm is implemented
  // if (hpm_num > 0) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER3,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER3H,  0); }
  // if (hpm_num > 1) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER4,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER4H,  0); }
  // if (hpm_num > 2) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER5,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER5H,  0); }
  // if (hpm_num > 3) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER6,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER6H,  0); }
  // if (hpm_num > 4) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER7,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER7H,  0); }
  // if (hpm_num > 5) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER8,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER8H,  0); }
  // if (hpm_num > 6) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER9,  0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER9H,  0); }
  // if (hpm_num > 7) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER10, 0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER10H, 0); }
  // if (hpm_num > 8) { neorv32_cpu_csr_write(CSR_MHPMCOUNTER11, 0); neorv32_cpu_csr_write(CSR_MHPMCOUNTER11H, 0); }


  neorv32_uart0_printf("\nProgram completed.\n");
    
    for (int y = 0; y < 12; y++) {
        for (int x = 0; x < 12; x++) {
            neorv32_uart0_printf("%u\t", output[y][x]);
        }
        neorv32_uart0_printf("\n");
    }    
    
  // Signal jupyter notebook the program is finished, keep this in!
  neorv32_uart0_printf("\nExecution completed.\n");

  return 0;
}

