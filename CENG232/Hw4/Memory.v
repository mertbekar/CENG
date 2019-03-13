`timescale 1ns / 1ps
module Memory(
input [23:0] Addr,
input Length, //byte:0 vs. word:1
input Rd, 
input Wr, 
input Enable,
output reg Rdy,
input [31:0] DataIn, 
output reg [31:0] DataOut
);

localparam Tmem= 8; //8 ns
localparam Td = 0.2; //200 ps


//
reg[7:0]memo[0:16777215];
initial
begin
Rdy = 1;
end
always@(Addr, Wr, Rd,Enable,DataIn,Length)
begin
	if(Enable == 1)
	begin
		if(Rdy == 1)
		begin
		Rdy = 0;
			if(Rd == 1)
			begin
				if(Length == 0)
				begin
					#Td;				
					//DataOut[31:8] = 24'b000000000000000000000000;
					DataOut= memo[Addr];
					#(Tmem-Td);
				end
				else
				begin
					#Td;
					DataOut[7:0] =memo[{Addr[23:2],2'b00}];
					DataOut[15:8] =memo[{Addr[23:2],2'b01}];
					DataOut[23:16] =memo[{Addr[23:2],2'b10}];
					DataOut[31:24] =memo[{Addr[23:2],2'b11}];
					#(Tmem-Td);
				end
			end
			if(Wr == 1)
			begin
				if(Length == 0)
				begin
					#Td;
					memo[Addr] = DataIn;
					#(Tmem-Td);
				end
				else
				begin
					#Td;
					memo[{Addr[23:2],2'b00}] = DataIn[7:0];
					memo[{Addr[23:2],2'b01}] = DataIn[15:8];
					memo[{Addr[23:2],2'b10}] = DataIn[23:16];
					memo[{Addr[23:2],2'b11}] = DataIn[31:24];
					#(Tmem-Td);
				end
			end
		end
		Rdy =1;
	end
end


//Write your code below
//


endmodule
	
	

