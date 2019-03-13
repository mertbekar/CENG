`timescale 1ns / 1ps
module Ctrl_Subsystem(
	input [31:0] Instr,
	input ZE, NG, CY, OV ,
	output reg [4:0] AddrA, AddrB, AddrC,
	output reg [3:0] ALUOp,
	output reg WrC, WrPC, WrCR, WrIR,
	output reg Mem_ALU, PC_RA, IR_RB,
	output reg ALU_PC, ZE_SE, Sin_Sout,
	output reg MemRd,MemWr,
	output reg MemLength,
	output reg MemEnable,
	input MemRdy,
	output reg [2:0] Status, // 0:p_reset, 1:fetch, 2:execute, 3:memop
	input Clk, Reset
    );
	
	reg[1:0] init;
	
	localparam p_reset = 3'b000;
	localparam fetch   = 3'b001;
	localparam execute = 3'b010;
	localparam memop   = 3'b011;
	
	localparam Ctrl_delay= 0.5; //500 ps
	localparam Reset_delay= 0.5; //500 ps
	localparam Dec_delay= 3; //3 ns
	localparam MemRd_delay= 2.5; //2500 ps
	localparam MemRd_pulse= MemRd_delay + 3 ; //5500 ps
	localparam MemWr_delay= 2.5; //2500 ps
	localparam MemWr_pulse= MemWr_delay + 3 ; //5500 ps
	
//
always@(Reset)
begin
if(Reset == 1'b1)
	Status <=#Reset_delay 3'b000;
end

always@(posedge Clk)
begin
	if(Status == 3'b000)
		Status <=#Ctrl_delay 3'b001;
	else if(Status == 3'b001)
		Status <=#Ctrl_delay 3'b010;
	else if(Status == 3'b010)
	begin
		if(Instr[31:26] == 6'b011000)
			Status <=#Ctrl_delay 3'b001;
		if(Instr[31:26] == 6'b010001)
			Status <=#Ctrl_delay 3'b001;
		if(Instr[31:26] == 6'b100001)
			Status <=#Ctrl_delay 3'b011;
		if(Instr[31:26] == 6'b100010)
			Status <=#Ctrl_delay 3'b011;
		end
	else if(Status == 3'b011)
		Status <=#Ctrl_delay 3'b001;
	end
			
always@ (Status)
begin
if(Status == 3'b000)
begin
	ALUOp <= 4'b0000;
	MemRd <= 1'b0;
	MemEnable <= 1'b0;
	MemWr <= 1'b0;
	MemLength <= 1'b0;
end
else if(Status == 3'b001)
begin
	WrCR <=#Ctrl_delay 1'b0;
	WrC <=#Ctrl_delay 1'b0;
	ALU_PC <=#Ctrl_delay 1'b1;
	MemLength <=#Ctrl_delay 1'b1;
	MemEnable <=#Ctrl_delay 1'b1;
	Sin_Sout <=#Ctrl_delay 1'b0;
	MemRd <=#MemRd_delay 1'b1;
	MemRd <=#MemRd_pulse 1'b0;
	WrIR <=#Ctrl_delay 1'b1;
	PC_RA <=#Ctrl_delay 1'b0;
	ALUOp <=#Ctrl_delay 4'b1110;
	WrPC <=#Ctrl_delay 1'b1;
end
else if(Status == 3'b010)
begin
	WrIR <=#Ctrl_delay 1'b0;
	WrPC <=#Ctrl_delay 1'b0;
	MemEnable <=#Ctrl_delay 1'b0;
end
else if(Status == 3'b011)
begin
	ALU_PC <=#Ctrl_delay 1'b0;
	MemEnable <=#Ctrl_delay 1'b1;
	if(Instr[31:26] == 6'b100001)
	begin
		MemLength <=#Ctrl_delay 1'b1;
		MemRd <=#MemRd_delay 1'b1;
		MemRd <=#MemRd_pulse 1'b0;
		Mem_ALU <=#Ctrl_delay 1'b0;
		WrC <=#Ctrl_delay 1'b1;
	end
	else if(Instr[31:26] == 6'b100010)
	begin
		MemLength <=#Ctrl_delay 1'b0;
		MemWr <=#MemWr_delay 1'b1;
		MemWr <=#MemWr_pulse 1'b0;
		Sin_Sout <=#Ctrl_delay 1'b1;
		//WrC <=#Ctrl_delay 1'b1;
	end
end
end

always @(Instr)
begin
	if(Instr[31:26] == 6'b011000)
	begin
		AddrA <=#Dec_delay Instr[20:16];
		AddrB <=#Dec_delay Instr[15:11];
		AddrC <=#Dec_delay Instr[25:21];
		PC_RA <=#Ctrl_delay 1'b1;
		ALUOp <=#Ctrl_delay 4'b0110;
		IR_RB <=#Ctrl_delay 1'b1;
		Mem_ALU <=#Ctrl_delay 1'b1;
		WrC <=#Ctrl_delay 1'b1;
		WrCR <=#Ctrl_delay 1'b1;
	end
	else if(Instr[31:26] == 6'b010001)
	begin
		AddrC <=#Dec_delay Instr[25:21];
		AddrA <=#Dec_delay Instr[20:16];
		ALUOp <=#Ctrl_delay 4'b0001;
		ZE_SE <=#Ctrl_delay 1'b1;
		IR_RB <=#Ctrl_delay 1'b0;
		PC_RA <=#Ctrl_delay 1'b1;
		WrC <=#Ctrl_delay 1'b1;
		WrCR <=#Ctrl_delay 1'b1;	
		Mem_ALU <=#Ctrl_delay 1'b1;
	end
	else if(Instr[31:26] == 6'b100001)
	begin
		AddrA <=#Dec_delay Instr[20:16];
		AddrC <=#Dec_delay Instr[25:21];
		PC_RA <=#Ctrl_delay 1'b1;
		ZE_SE <=#Ctrl_delay 1'b0;
		IR_RB <=#Ctrl_delay 1'b0;
		ALUOp <=#Ctrl_delay 4'b0001;
		ALU_PC <=#Ctrl_delay 1'b0;
	end
	else if(Instr[31:26] == 6'b100010)
	begin
		AddrA <=#Dec_delay Instr[20:16];
		AddrB <=#Dec_delay Instr[15:11];
		PC_RA <=#Ctrl_delay 1'b1;
		ZE_SE <=#Ctrl_delay 1'b0;
		IR_RB <=#Ctrl_delay 1'b0;
		ALUOp <=#Ctrl_delay 4'b0001;
		ALU_PC <=#Ctrl_delay 1'b0;
	end
end
always@(MemRdy)
begin
	if(MemRdy == 1'b1)
	begin
		if(Instr[31:26] == 6'b100010)
			Sin_Sout <=#Ctrl_delay 1'b0;
	end
end
	
endmodule

