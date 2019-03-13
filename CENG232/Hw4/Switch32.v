`timescale 1ns / 1ps

module Switch32(
input [31:0] A_in,
output reg [31:0] A_out,
output reg [31:0] B_out,
input [31:0] C_in,
input Sel
    );
	 
localparam Switch_delay= 0.5; //500 ps	 

//
//Write your code below
//
always @ (A_in or C_in or Sel)
begin
	if(Sel == 1'b0)
	begin
		#Switch_delay;
		B_out = A_in; 
		end
	else
	begin
		#Switch_delay;
		A_out = C_in;
		end
end
    
endmodule
