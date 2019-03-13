`timescale 1ns / 1ps
module Processor(
output  [23:0] MemAddr  , // memory address bus
input [31:0] fromMemData  , // data bus from memory 
output  [31:0] toMemData, // data bus to memory 
output  MemLength, // memory operand length
output  MemRd, // memory read control signal
output  MemWr, // memory write control signal
output  MemEnable, // memory enable signal
input MemRdy, // memory completion signal
output  [2:0] Status, // processor status signal 0:p_reset, 1:fetch, 2:execute, 3:memop
input Reset , // reset signal
input Clk // clock signal
    );

//
//Write your code below
wire [31:0] Instr;
wire ZE;
wire NG;
wire CY;
wire OV;
wire [4:0] AddrA;
wire [4:0] AddrB;
wire [4:0] AddrC;
wire [3:0] ALUop;
wire WrC;
wire WrPC;
wire WrCR;
wire WrIR;
wire Mem_ALU;
wire PC_RA;
wire IR_RB;
wire ALU_PC;
wire ZE_SE;
wire Sin_Sout;
Data_Subsystem data(MemAddr, fromMemData, toMemData, Instr, ZE, NG, CY, OV, AddrA, AddrB, AddrC, ALUop, WrC, WrPC, WrCR, WrIR, Mem_ALU, PC_RA, IR_RB, ALU_PC, ZE_SE,Sin_Sout, Clk, Reset);
Ctrl_Subsystem ctrl(Instr, ZE, NG, CY, OV, AddrA, AddrB, AddrC, ALUop, WrC, WrPC, WrCR, WrIR, Mem_ALU, PC_RA, IR_RB, ALU_PC, ZE_SE, Sin_Sout, MemRd, MemWr, MemLength, MemEnable, MemRdy, Status, Clk, Reset);
//


endmodule

