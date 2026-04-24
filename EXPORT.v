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
reg [3:0] combiner_out0;
wire [3:0] number_literal0 = 4'd0;
reg splitter_b0_out1, splitter_b1_out1, splitter_b2_out1, splitter_b3_out1;
reg [3:0] register_value0;
reg [3:0] combiner_out1;
reg [3:0] register_value1;
reg [3:0] mux_result0;
reg bin_op_result0;
reg [3:0] mux_result1;
reg splitter_b0_out0, splitter_b1_out0, splitter_b2_out0, splitter_b3_out0;

// ─── combination logic ────────────────────────────────────
	always @(*) begin
		// Output1
		combiner_out0 = {sw3, sw2, sw1, sw0};
		splitter_b0_out1 = number_literal0[0];
		splitter_b1_out1 = number_literal0[1];
		splitter_b2_out1 = number_literal0[2];
		splitter_b3_out1 = number_literal0[3];
		combiner_out1 = {sw3, sw2, sw1, sw0};
	if(btn2)
		mux_result0 = register_value0;
	else
		mux_result0 = register_value1;
		bin_op_result0 = register_value0 ^ register_value1;
	if(btn3)
		mux_result1 = mux_result0;
	else
		mux_result1 = bin_op_result0;
		splitter_b0_out0 = mux_result1[0];
		splitter_b1_out0 = mux_result1[1];
		splitter_b2_out0 = mux_result1[2];
		splitter_b3_out0 = mux_result1[3];
		led0 = splitter_b0_out0;
		// Output2
		led1 = splitter_b1_out0;
		// Output3
		led2 = splitter_b2_out0;
		// Output4
		led3 = splitter_b3_out0;
	end


// ─── clocked logic ────────────────────────────────────────
	always @(posedge sys_clk) begin
		if (splitter_b0_out1) 
			register_value0 <= 4'b0;
		else if (btn1 )
			register_value0 <= combiner_out0;
	end

	always @(posedge sys_clk) begin
		if (splitter_b0_out1) 
			register_value1 <= 4'b0;
		else if (btn0 )
			register_value1 <= combiner_out1;
	end

endmodule