module main_module (
	input wire [15:0] SW,
	input wire BTNR,
	input wire BTNC,
	input wire BTNU,
	input wire BTND,
	input wire BTNL,
	input wire CPU_RESETN,
	input wire sys_clk,
	output reg [15:0] LED
);

// === wire/reg declarations ================================
reg [15:0]debounce_sr0;
reg debounce_out0;
reg edge_rise0;
reg edge_fall0;
reg edge_prev0;
reg [0:0] bin_op_result0;
reg [15:0] register_value0;
wire [15:0] number_literal0 = 16'd21845;
reg [15:0] mux_result0;


// === module instances =====================================

// === combination logic ====================================
	always @(*) begin
		// Output LED
		bin_op_result0 = ~CPU_RESETN;
		case (BTNU)
			1'd0: mux_result0 = register_value0;
			1'd1: mux_result0 = number_literal0;
		endcase

		LED = mux_result0;
	end


// === clocked logic ========================================
	always @(posedge sys_clk) begin
		debounce_sr0 <= { debounce_sr0[14:0], BTNC };
	end

	always @(posedge sys_clk) begin
		if (debounce_sr0 == 16'hFFFF)
			debounce_out0 <= 1'b1;
		else if (debounce_sr0 == 16'h0000)
			debounce_out0 <= 1'b0;
	end

	always @(posedge sys_clk) begin
		edge_rise0 <= debounce_out0 & ~edge_prev0;
		edge_fall0 <= ~debounce_out0 & edge_prev0;
		edge_prev0 <= debounce_out0;
	end

	always @(posedge sys_clk) begin
		if (bin_op_result0) 
			register_value0 <= 16'b0;
		else if (edge_rise0 )
			register_value0 <= SW;
	end

endmodule