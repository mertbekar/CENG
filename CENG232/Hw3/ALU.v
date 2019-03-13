`timescale 1ns / 1ps
module ALU(
input [3:0] A, 
input [3:0] B, 
input [3:0] ALUop, 
output reg [3:0] C, 
output reg [3:0] Cond // Z,N,C,V
    );
//
//Write your code below
//

reg [4:0] k;
always @ (A or B or ALUop)
begin
	if(ALUop == 4'b0000)
		C = 4'b000;
	else if(ALUop == 4'b0001)
		C = (A + B); 
	else if(ALUop == 4'b0010)
		C = (A - B);
	else if(ALUop == 4'b0011)
		C = -B;
	else if(ALUop == 4'b0100)
		C = (A & B);
	else if(ALUop == 4'b0101)
		C = (A | B);
	else if(ALUop == 4'b0110)
		C = (A ^ B);
	else if(ALUop == 4'b0111)
		C = (!B);
	else if(ALUop == 4'b1001)
		C = B;
	else if(ALUop == 4'b1010)
		C = (A << 1);
	else if(ALUop == 4'b1011)
		C = (A >> 1);
	else if(ALUop == 4'b1100)
	begin
		C[3] = A[2];
		C[2] = A[1];
		C[1] = A[0];
		C[0] = A[3];
	end
	else if(ALUop == 4'b1101) 
	begin
		C[3] = A[0];
		C[2] = A[3];
		C[1] = A[2];
		C[0] = A[1];
	end	
	else if(ALUop == 4'b1110)
		C = A + (0100);
		
	if(C == 4'b0000)
		Cond[3] = 1;
		else
			Cond[3] = 0;
	if(C[3] == 1)
		Cond[2] = 1;
		else
			Cond[2] = 0;
	if(ALUop == 4'b0001)
	begin
		k = (A + B);
		if(k[4] == 1)
			Cond[1] = 1;
		else
			Cond[1] = 0;
	end
	if(ALUop == 4'b1110)
	begin
		k = A + 4'b0100;
		if(k[4] == 1)
			Cond[1] = 1;
		else
			Cond[1]= 0;
	end
	if(ALUop == 4'b0010)
	begin
		if(A < B)
			Cond[1] = 1;
		else
			Cond[1] = 0;
	end
	if(ALUop == 4'b0001)
	begin
		if((A[3] == 0) && (B[3] == 0) && (C[3] == 1))
			Cond[0] = 1;
		else if((A[3] == 1) && (B[3] == 1) && (C[3] == 0))
			Cond[0] = 1;
		else
			Cond[0] = 0;
	end
	if(ALUop == 4'b1110)
	begin
		if((A[3] == 0) && (C[3] == 1))
			Cond[0] = 1;
		else
			Cond[0] = 0;
	end		
	if(ALUop == 4'b0010)
	begin
		if((A[3] == 0) && (B[3] == 1) && (C[3] == 1))
			Cond[0]= 1;
		else if((A[3] == 1) && (B[3] == 0) && (C[3] == 0))
			Cond[0]= 1;
		else
			Cond[0] = 0;
	end
		
end
endmodule