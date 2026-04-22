module main_module (
	 input wire btn0,
	 input wire btn1,
	 input wire btn2,
	 input wire btn3,
	 input wire sw0,
	 input wire sw1,
	 input wire sw2,
	 input wire sw3,
	input wire sys_clk,
	 output reg led0,
	 output reg led1,
	 output reg led2,
	 output reg led3
);

// ─── wire/reg declarations ────────────────────────────────
reg [3:0]debounce_sr0;
reg debounce_out0;
reg edge_rise0;
reg edge_fall0;
reg edge_prev0;
reg bin_op_result0;
reg dff_out0;
reg bin_op_result1;
reg dff_out1;

// ─── combination logic ────────────────────────────────────
	always @(*) begin
		// Output1
		edge_rise0 = debounce_out0 & ~edge_prev0;
		edge_fall0 = ~debounce_out0 & edge_prev0;
		bin_op_result0 = ~dff_out0;
		led0 = dff_out0;
		// Output2
		bin_op_result1 = ~dff_out1;
		led1 = dff_out1;
	end


// ─── clocked logic ────────────────────────────────────────
	always @(posedge sys_clk) begin
		debounce_sr0 <= { debounce_sr0[2:0], btn0 };
	end

	always @(posedge sys_clk) begin
		if (debounce_sr0 == 4'b1111)
			debounce_out0 <= 1'b1;
		else if (debounce_sr0 == 4'b0000)
			debounce_out0 <= 1'b0;
	end

	always @(posedge sys_clk) begin
		edge_prev0 <= debounce_out0;
	end

	always @(posedge sys_clk) begin
		if (btn1)
			dff_out0 <= 1'b0;
		else if (edge_fall0)
			dff_out0 <= bin_op_result0;
	end

	always @(posedge sys_clk) begin
		if (btn1)
			dff_out1 <= 1'b0;
		else if (edge_rise0)
			dff_out1 <= bin_op_result1;
	end

endmodule