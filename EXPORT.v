module main_module (
	 input wire btn0,
	 input wire btn1,
	 input wire btn2,
	 input wire btn3,
	 input wire sw0,
	 input wire sw1,
	 input wire sw2,
	 input wire sw3,
	 output wire led0,
	 output wire led1,
	 output wire led2,
	 output wire led3
);
wire mux_result0;
assign mux_result0 = sw2 ? btn0 : btn1;
wire bin_op_result0;
nor nor0 (bin_op_result0, mux_result0, sw2);

assign led3 = bin_op_result0;

endmodule