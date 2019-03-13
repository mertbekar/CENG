`timescale 1ns / 1ps

module Reg_File(
input [4:0] AddrA, 
input [4:0] AddrB, 
input [4:0] AddrC, 
output reg [31:0] DataA, 
output reg [31:0] DataB, 
input [31:0] DataC, 
input WrC,
input Reset,
input Clk
    );

localparam RF_delay = 4; //4 ns

//
reg[31:0] regis[0:31];
always@(AddrA or AddrB)
begin
	if(Reset == 1)
	begin
		regis[0] = 0;
		regis[1] = 0;
		regis[2] = 0;
		regis[3] = 0;
		regis[4] = 0;
		regis[5] = 0;
		regis[6] = 0;
		regis[7] = 0;
		regis[8] = 0;
		regis[9] = 0;
		regis[10] = 0;
		regis[11] = 0;
		regis[12] = 0;
		regis[13] = 0;
		regis[14] = 0;
		regis[15] = 0;
		regis[16] = 0;
		regis[17] = 0;
		regis[18] = 0;
		regis[19] = 0;
		regis[20] = 0;
		regis[21] = 0;
		regis[22] = 0;
		regis[23] = 0;
		regis[24] = 0;
		regis[25] = 0;
		regis[26] = 0;
		regis[27] = 0;
		regis[28] = 0;
		regis[29] = 0;
		regis[30] = 0;
		regis[31] = 0;
		end
	if(Reset == 0)
	begin
		#RF_delay;
		DataA = regis[AddrA];
		DataB = regis[AddrB];
	end
end

always@(Clk or WrC)
begin
	if(Reset == 1)
	begin
			regis[0] = 0;
			regis[1] = 0;
			regis[2] = 0;
			regis[3] = 0;
			regis[4] = 0;
			regis[5] = 0;
			regis[6] = 0;
			regis[7] = 0;
			regis[8] = 0;
			regis[9] = 0;
			regis[10] = 0;
			regis[11] = 0;
			regis[12] = 0;
			regis[13] = 0;
			regis[14] = 0;
			regis[15] = 0;
			regis[16] = 0;
			regis[17] = 0;
			regis[18] = 0;
			regis[19] = 0;
			regis[20] = 0;
			regis[21] = 0;
			regis[22] = 0;
			regis[23] = 0;
			regis[24] = 0;
			regis[25] = 0;
			regis[26] = 0;
			regis[27] = 0;
			regis[28] = 0;
			regis[29] = 0;
			regis[30] = 0;
			regis[31] = 0;
			
		end
	if(Clk)
	begin
		if(Reset == 0)
		begin
			if(WrC == 1)
			begin
				#RF_delay;
				regis[AddrC] = DataC;
				end
			end
		end
	end
		
//Write your code below
//

endmodule
