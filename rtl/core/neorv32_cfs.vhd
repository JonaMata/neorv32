-- ================================================================================ --
-- NEORV32 SoC - Custom Functions Subsystem (CFS)                                   --
-- -------------------------------------------------------------------------------- --
-- The NEORV32 RISC-V Processor - https://github.com/stnolting/neorv32              --
-- Copyright (c) NEORV32 contributors.                                              --
-- Copyright (c) 2020 - 2025 Stephan Nolting. All rights reserved.                  --
-- Licensed under the BSD-3-Clause license, see LICENSE for details.                --
-- SPDX-License-Identifier: BSD-3-Clause                                            --
-- ================================================================================ --

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library neorv32;
use neorv32.neorv32_package.all;

entity neorv32_cfs is
  generic (
    MATRIX_SIZE  : integer := 1300; -- size of input matrix
    KERNEL_SIZE  : integer := 3     -- size of kernel matrix
  );
  port (
    -- global control --
    clk_i     : in  std_ulogic; -- global clock line
    rstn_i    : in  std_ulogic; -- global reset line, low-active, use as async
    -- CPU access --
    bus_req_i : in  bus_req_t; -- bus request
    bus_rsp_o : out bus_rsp_t; -- bus response
    -- CPU interrupt --
    irq_o     : out std_ulogic; -- interrupt request
    -- external IO --
    cfs_in_i  : in  std_ulogic_vector(255 downto 0); -- custom inputs conduit
    cfs_out_o : out std_ulogic_vector(255 downto 0) -- custom outputs conduit
  );
end neorv32_cfs;

architecture neorv32_cfs_rtl of neorv32_cfs is

  -- exemplary CFS interface registers --
  type in_row is array (0 to (MATRIX_SIZE-1)) of std_ulogic_vector(31 downto 0);
  type in_mat_type is array (0 to (KERNEL_SIZE-1)) of in_row;
  signal in_mat : in_mat_type := (others => (others => (others => '0')));
  type ker_row is array (0 to (KERNEL_SIZE-1)) of std_ulogic_vector(31 downto 0);
  type ker_mat_type is array (0 to (KERNEL_SIZE-1)) of ker_row;
  signal ker_mat : ker_mat_type := (others => (others => (others => '0')));
  type out_row is array (0 to (MATRIX_SIZE-KERNEL_SIZE)) of std_ulogic_vector(63 downto 0);
  signal out_mat : out_row := (others => (others => '0'));

begin

  -- CFS Generics ---------------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  -- In it's default version the CFS provides three configuration generics:
  -- > CFS_IN_SIZE  - configures the size (in bits) of the CFS input conduit cfs_in_i
  -- > CFS_OUT_SIZE - configures the size (in bits) of the CFS output conduit cfs_out_o
  -- > CFS_CONFIG   - is a blank 32-bit generic. It is intended as a "generic conduit" to propagate
  --                  custom configuration flags from the top entity down to this module.


  -- CFS IOs --------------------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  -- By default, the CFS provides two IO signals (cfs_in_i and cfs_out_o) that are available at the processor's top entity.
  -- These are intended as "conduits" to propagate custom signals from this module and the processor top entity.
  --
  -- If the CFU output signals are to be used outside the chip, it is recommended to register these signals.

  cfs_out_o <= (others => '0'); -- not used for this minimal example


  -- Reset System ---------------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  -- The CFS can be reset using the global rstn_i signal. This signal should be used as asynchronous reset and is active-low.
  -- Note that rstn_i can be asserted by a processor-external reset, the on-chip debugger and also by the watchdog.
  --
  -- Most default peripheral devices of the NEORV32 do NOT use a dedicated hardware reset at all. Instead, these units are
  -- reset by writing ZERO to a specific "control register" located right at the beginning of the device's address space
  -- (so this register is cleared at first). The crt0 start-up code writes ZERO to every single address in the processor's
  -- IO space - including the CFS. Make sure that this initial clearing does not cause any unintended CFS actions.


  -- Interrupt ------------------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  -- The CFS features a single interrupt signal, which is connected to the CPU's "fast interrupt" channel 1 (FIRQ1).
  -- The according CPU interrupt becomes pending as long as <irq_o> is high.

  irq_o <= '0'; -- not used for this minimal example


  -- Read/Write Access ----------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------
  -- Here we are reading/writing from/to the interface registers of the module and generate the CPU access handshake (bus response).
  --
  -- The CFS provides up to 64kB of memory-mapped address space (16 address bits, byte-addressing) that can be used for custom
  -- memories and interface registers. If the complete 16-bit address space is not required, only the minimum LSBs required for
  -- address decoding can be used. In this case, however, the implemented registers are replicated (several times) across the CFS
  -- address space.
  --
  -- Following the interface protocol, each read or write access has to be acknowledged in the following cycle using the ack_o
  -- signal (or even later if the module needs additional time). If no ACK is generated at all, the bus access will time out
  -- and cause a bus access fault exception. The current CPU privilege level is available via the 'priv_i' signal (0 = user mode,
  -- 1 = machine mode), which can be used to constrain access to certain registers or features to privileged software only.
  --
  -- This module also provides an optional ERROR signal to indicate a faulty access operation (for example when accessing an
  -- unused, read-only or "locked" CFS register address). This signal may only be set when the module is actually accessed
  -- and is set INSTEAD of the ACK signal. Setting the ERR signal will raise a bus access exception with a "Device Error" qualifier
  -- that can be handled by the application software. Note that the current privilege level should not be exposed to software to
  -- maintain full virtualization. Hence, CFS-based "privilege escalation" should trigger a bus access exception (e.g. by setting 'err_o').
  --
  -- Host access example: Read and write access to the interface registers + bus transfer acknowledge. This example only
  -- implements four physical r/w register (the four lowest CFS registers). The remaining addresses of the CFS are not associated
  -- with any physical registers - any access to those is simply ignored but still acknowledged. Only full-word write accesses are
  -- supported (and acknowledged) by this example. Sub-word write access will not alter any CFS register state and will cause
  -- a "bus store access" exception (with a "Device Timeout" qualifier as not ACK is generated in that case).

  bus_access: process(rstn_i, clk_i)
  variable address : integer;
  begin
    if (rstn_i = '0') then
      in_mat <= (others => (others => (others => '0')));
      ker_mat <= (others => (others => (others => '0')));
      out_mat <= (others => (others => (others => '0')));
      bus_rsp_o     <= rsp_terminate_c;
    elsif rising_edge(clk_i) then -- synchronous interface for read and write accesses
      -- transfer/access acknowledge --
      bus_rsp_o.ack <= bus_req_i.stb;

      -- tie to zero if not explicitly used --
      bus_rsp_o.err <= '0'; -- set high together with bus_rsp_o.ack if there is an access error

      -- defaults --
      bus_rsp_o.data <= (others => '0'); -- the output HAS TO BE ZERO if there is no actual (read) access

      -- bus access --
      if (bus_req_i.stb = '1') then -- valid access cycle, STB is high for one cycle
        address := to_integer(unsigned(bus_req_i.addr(15 downto 2)));
        -- write access (word-wise) --
        if (bus_req_i.rw = '1') then
          if (address < (MATRIX_SIZE*KERNEL_SIZE)) then
            in_mat(address / MATRIX_SIZE)(address mod MATRIX_SIZE) <= bus_req_i.data;
          elsif (address < (MATRIX_SIZE*KERNEL_SIZE) + (KERNEL_SIZE*KERNEL_SIZE)) then
            address := address - (MATRIX_SIZE*KERNEL_SIZE);
            ker_mat(address / KERNEL_SIZE)(address mod KERNEL_SIZE) <= bus_req_i.data;
          end if;

        -- read access (word-wise) --
        else
          if (address < 2*(MATRIX_SIZE-KERNEL_SIZE+1)) then
            if (address mod 2 = 0) then
              bus_rsp_o.data(31 downto 0) <= out_mat(address / 2)(31 downto 0);
            else
              bus_rsp_o.data(31 downto 0) <= out_mat(address / 2)(63 downto 32);
            end if;
          end if;
        end if;

      end if;
    end if;
  end process bus_access;


  -- CFS Function Core ----------------------------------------------------------------------
  -- -------------------------------------------------------------------------------------------

  -- This is where the actual functionality can be implemented.
  -- The logic below is just a very simple example that transforms data
  -- from an input register into data in an output register.

  -- cfs_reg_rd(0) <= x"0000000" & "000" & or_reduce_f(cfs_reg_wr(0)); -- OR all bits
  -- cfs_reg_rd(1) <= x"0000000" & "000" & xor_reduce_f(cfs_reg_wr(1)); -- XOR all bits
  -- cfs_reg_rd(2) <= bit_rev_f(cfs_reg_wr(2)); -- bit reversal
  -- cfs_reg_rd(3) <= (others => '1');

  -- gen_multiply_add: for i in 0 to (MATRIX_SIZE-KERNEL_SIZE+1) generate
  --   out_mat(i) <= std_ulogic_vector(
  --     unsigned(in_mat(0)(i)) * unsigned(ker_mat(0)(0)) +
  --     unsigned(in_mat(0)(i+1)) * unsigned(ker_mat(0)(1)) +
  --     unsigned(in_mat(1)(i)) * unsigned(ker_mat(1)(0)) +
  --     unsigned(in_mat(1)(i+1)) * unsigned(ker_mat(1)(1)));
  -- end generate gen_multiply_add;

  gen_multiply_add: for i in 0 to (MATRIX_SIZE - KERNEL_SIZE) generate
  process(in_mat, ker_mat)
    variable sum : unsigned(63 downto 0);
  begin
    sum := (others => '0');

    for r in 0 to KERNEL_SIZE-1 loop
      for c in 0 to KERNEL_SIZE-1 loop
        sum := sum +
          unsigned(in_mat(r)(i + c)) * unsigned(ker_mat(r)(c));
      end loop;
    end loop;

    out_mat(i) <= std_ulogic_vector(sum);
  end process;
end generate gen_multiply_add;


end neorv32_cfs_rtl;
