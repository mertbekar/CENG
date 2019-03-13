`timescale 1ns / 1ps
module Mux32(
input [31:0] A_in,
input [31:0] B_in,
input Sel,
output reg [31:0] Data_out 
    );
//
//Write your code below
//

always@(posedge Sel or posedge A_in or posedge B_in)
begin
	if (Sel == 1'b1)
		Data_out = B_in;
	else
		Data_out = A_in;
end
endmodule
