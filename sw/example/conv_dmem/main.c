#include <neorv32.h>
#define BAUD_RATE 19200

volatile uint32_t matrix[13][13] = {
    {56318, 55435, 39177, 22213, 58312, 22620, 25135, 63007, 19718, 25969, 7824, 34034, 54937},
    {50697, 40447, 36839, 32714, 57451, 64484, 19561, 12492, 7940, 5808, 40023, 53039, 45524},
    {17339, 46509, 41720, 17743, 43777, 18454, 53883, 35867, 9173, 15225, 8334, 16205, 9098},
    {36283, 223, 53009, 32364, 43146, 814, 27450, 58986, 4452, 15319, 10053, 16022, 3122},
    {40009, 3993, 21945, 61121, 42736, 65338, 13274, 36133, 59577, 60451, 41909, 28608, 31519},
    {12851, 53332, 49060, 58231, 18026, 3843, 6614, 61522, 33426, 41991, 8824, 46200, 35444},
    {47163, 22304, 56786, 48662, 10487, 6428, 42075, 43076, 7003, 21968, 7323, 29736, 21388},
    {57285, 59295, 35671, 61019, 6141, 43184, 41962, 41288, 14951, 27409, 43637, 60882, 30348},
    {26628, 23618, 41793, 27902, 24210, 46694, 33393, 40528, 47553, 48951, 11936, 27547, 1171},
    {15159, 46182, 5166, 38079, 31541, 35958, 57666, 12310, 29341, 62563, 24859, 7780, 36857},
    {28781, 63208, 6635, 47065, 55871, 65144, 5827, 53107, 49837, 31788, 57265, 15869, 62462},
    {43173, 40021, 18362, 27579, 43204, 60972, 55150, 21328, 22400, 24888, 17593, 36659, 10939},
    {63257, 48374, 58052, 24604, 39581, 4516, 5788, 64307, 41008, 53389, 56660, 64997, 35516},
};
// volatile uint32_t matrix[1600][1600] = {0};

volatile uint32_t kernel[2][2] = {
    {15, 2},
    {3, 5},
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
      NEORV32_CFS->REG[1*13 + x] = matrix[1][x];
    }
    for (int y_out = 0; y_out < 12; y_out++) {
      int offset = y_out % 2 + 1;
      for (int x = 0; x < 13; x++) {
        NEORV32_CFS->REG[offset*13 + x] = matrix[y_out + 2][x];
      }
      
      for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 2; x++) {
            NEORV32_CFS->REG[3*13 + (y+offset-1)%2*2 + x] = kernel[y][x];
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

