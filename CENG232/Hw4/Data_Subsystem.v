`timescale 1ns / 1ps
module Data_Subsystem(
output  [23:0] MemAddr,
input [31:0] fromMemData,
output  [31:0] toMemData,
output  [31:0] Instr,
output  ZE,
output  NG,
output  CY,
output  OV,
input [4:0] AddrA,
input [4:0] AddrB,
input [4:0] AddrC,
input [3:0] ALUop,
input WrC,
input WrPC,
input WrCR,
input WrIR,
input Mem_ALU,
input PC_RA,
input IR_RB,
input ALU_PC,
input ZE_SE,
input Sin_Sout,
input Clk,
input Reset
    );
	
//
//Write your code below
//PS:Interconnections between all data subsystem components should be given below: 

wire [31:0] MemData;
wire [31:0] ALUData;
wire [31:0] DataC;
wire [31:0] DataB;
wire [31:0] DataA;
wire [31:0] PCOut;
wire [31:0] Ain;
wire [31:0] Bin;
wire [31:0] ExOut;
wire [3:0] Cond;

Mux32 Mux1(MemData, ALUData, Mem_ALU, DataC);
Reg_File Regis(AddrA, AddrB, AddrC, DataA, DataB, DataC, WrC, Reset, Clk);
Register24 PC(ALUData[23:0], PCOut, WrPC, Reset, Clk);
Extender Ex(Instr, ZE_SE, ExOut);
Mux32 Mux2({8'b00000000,PCout}, DataA, PC_RA, Ain);
Mux32 Mux3(ExOut, DataB, IR_RB, Bin);
Switch32 Sw(fromMemData, toMemData, MemData, DataB, Sin_Sout);
ALU Alu(Ain, Bin, ALUop, ALUData, Cond);
Register32 IR(MemData, Instr, WrIR, Reset, Clk);
Mux24 Mux4(ALUData[23:0], PCOut, MemAddr);
Register4 ZNCV(Cond, {ZE, NG, CY, OV}, WrCR, Reset, Clk);





endmodule
