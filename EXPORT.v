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
reg splitter_b0_out0, splitter_b1_out0, splitter_b2_out0, splitter_b3_out0;

// ─── combination logic ────────────────────────────────────
	always @(*) begin
		// Output1
		combiner_out0 = {btn3, btn2, btn1, btn0};
		splitter_b0_out0 = combiner_out0[0];
		splitter_b1_out0 = combiner_out0[1];
		splitter_b2_out0 = combiner_out0[2];
		splitter_b3_out0 = combiner_out0[3];
		led0 = splitter_b0_out0;
		// Output2
		led1 = splitter_b1_out0;
		// Output3
		led2 = splitter_b2_out0;
		// Output4
		led3 = splitter_b3_out0;
	end


// ─── clocked logic ────────────────────────────────────────
endmodule