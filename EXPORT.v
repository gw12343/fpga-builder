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
wire number_literal0 = 1'b0;
wire mux_result0;
assign mux_result0 = number_literal0 ? btn3 : sw3;

assign led3 = mux_result0;

endmodule