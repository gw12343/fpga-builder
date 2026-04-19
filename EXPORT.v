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
reg     debounce_out0;
wire number_literal0 = 1'b1;
reg bin_op_result0;
reg dff_out0;

// ─── combination logic ────────────────────────────────────
	always @(*) begin

bin_op_result0 = dff_out0 ^ number_literal0;

led0 = dff_out0;



	end

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
		if (btn1)
			dff_out0 <= 1'b0;
		else if (debounce_out0)
			dff_out0 <= bin_op_result0;
	end

endmodule