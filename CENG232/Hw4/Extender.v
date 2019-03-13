`timescale 1ns / 1ps
module Extender(
input [31:0] X_in,
input ZE_SE,
output reg [31:0] X_out
    );

localparam Ext_delay= 0.5; //500 ps

//
//Write your code below
//
always @ (X_in or ZE_SE)
begin
//Sign extension replicates the most significant
// bit loaded into the remaining bits. 

		if(ZE_SE == 1'b0)
		begin 
			#Ext_delay;
			X_out[0] = X_in[0];
			X_out[1] = X_in[1];
			X_out[2] = X_in[2];
			X_out[3] = X_in[3];
			X_out[4] = X_in[4];
			X_out[5] = X_in[5];
			X_out[6] = X_in[6];
			X_out[7] = X_in[7];
			X_out[8] = X_in[8];
			X_out[9] = X_in[9];
			X_out[10] = X_in[10];
			X_out[11] = X_in[11];
			X_out[12] = X_in[12];
			X_out[13] = X_in[13];
			X_out[14] = X_in[14];
			X_out[15] = X_in[15];
			X_out[16] = 0;
			X_out[17] = 0;
			X_out[18] = 0;
			X_out[19] = 0;
			X_out[20] = 0;
			X_out[21] = 0;
			X_out[22] = 0;
			X_out[23] = 0;
			X_out[24] = 0;
			X_out[25] = 0;
			X_out[26] = 0;
			X_out[27] = 0;
			X_out[28] = 0;
			X_out[29] = 0;
			X_out[30] = 0;
			X_out[31] = 0;
		end 
			 
		else
		begin 
			#Ext_delay;
			X_out[0] = X_in[0];
			X_out[1] = X_in[1];
			X_out[2] = X_in[2];
			X_out[3] = X_in[3];
			X_out[4] = X_in[4];
			X_out[5] = X_in[5];
			X_out[6] = X_in[6];
			X_out[7] = X_in[7];
			X_out[8] = X_in[8];
			X_out[9] = X_in[9];
			X_out[10] = X_in[10];
			X_out[11] = X_in[11];
			X_out[12] = X_in[12];
			X_out[13] = X_in[13];
			X_out[14] = X_in[14];	
			X_out[15] = X_in[15];
			X_out[16] = X_in[0];
			X_out[17] = X_in[1];
			X_out[18] = X_in[2];
			X_out[19] = X_in[3];
			X_out[20] = X_in[4];
			X_out[21] = X_in[5];
			X_out[22] = X_in[6];
			X_out[23] = X_in[7];
			X_out[24] = X_in[8];
			X_out[25] = X_in[9];
			X_out[26] = X_in[10];
			X_out[27] = X_in[11];
			X_out[28] = X_in[12];
			X_out[29] = X_in[13];
			X_out[30] = X_in[14];
			X_out[31] = X_in[15];
			
		end 	
	end

endmodule
