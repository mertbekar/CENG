`timescale 1ns / 1ps
module testbench_ALU(
    );

reg [3:0] A;
reg [3:0] B;
reg [3:0] ALUop;
wire [3:0] C;
wire [3:0] Cond;


ALU INS(A, B, ALUop, C, Cond);

initial 
begin
	$display("Starting Testbehch");
	A=4'b0001;
	B=4'b0010;
	#1; //1
	ALUop=4'b0001;
	$display("Testbench Finished");
	$finish;
end
endmodule
