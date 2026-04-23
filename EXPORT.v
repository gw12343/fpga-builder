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
reg [3:0] counter_out0;
reg splitter_b0_out0, splitter_b1_out0, splitter_b2_out0, splitter_b3_out0;

// ─── combination logic ────────────────────────────────────
	always @(*) begin
		// Output1
		edge_rise0 = debounce_out0 & ~edge_prev0;
		edge_fall0 = ~debounce_out0 & edge_prev0;
		splitter_b0_out0 = counter_out0[0];
		splitter_b1_out0 = counter_out0[1];
		splitter_b2_out0 = counter_out0[2];
		splitter_b3_out0 = counter_out0[3];
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
			counter_out0 <= 4'b0;
		else if (edge_rise0 & sw0 )
			counter_out0 <= counter_out0 + 1;
		else if (edge_rise0 & ~sw0 )
			counter_out0 <= counter_out0 - 1;
		else
			counter_out0 <= counter_out0;
	end

endmodule