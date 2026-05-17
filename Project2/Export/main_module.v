module main_module (
	input wire [15:0] SW,
	input wire BTNR,
	input wire BTNC,
	input wire BTNU,
	input wire BTND,
	input wire BTNL,
	input wire CPU_RESETN,
	input wire sys_clk,
	output reg [15:0] LED,
	output reg [6:0] SEG,
	output reg DP,
	output reg [7:0] AN,
	output reg VGA_HS,
	output reg VGA_VS,
	output reg [3:0] VGA_R,
	output reg [3:0] VGA_G,
	output reg [3:0] VGA_B
);

// === wire/reg declarations ================================
wire [0:0] number_literal0 = 1'd1;
reg [0:0] bin_op_result0;
wire [3:0] vga_out_custom_out_color_r0;
wire [3:0] vga_out_custom_out_color_g0;
wire [3:0] vga_out_custom_out_color_b0;
wire [0:0] vga_out_custom_out_VS0;
wire [0:0] vga_out_custom_out_HS0;
wire [3:0] number_literal1 = 4'd15;
reg [3:0] sub_out0;
reg sub_carry_out0;
reg [3:0] mux_result0;


// === module instances =====================================
vga_out custom_node0 (
	.sys_clk(sys_clk),
	.enable(number_literal0),
	.reset(bin_op_result0),
	.color_r(vga_out_custom_out_color_r0),
	.color_g(vga_out_custom_out_color_g0),
	.color_b(vga_out_custom_out_color_b0),
	.VS(vga_out_custom_out_VS0),
	.HS(vga_out_custom_out_HS0)
);

// === combination logic ====================================
	always @(*) begin
		// Output VGA_HS
		bin_op_result0 = ~CPU_RESETN;
		VGA_HS = vga_out_custom_out_HS0;
		// Output VGA_VS
		VGA_VS = vga_out_custom_out_VS0;
		// Output VGA_R
		{sub_carry_out0, sub_out0} = {1'b0, number_literal1} - {1'b0, vga_out_custom_out_color_r0};
		case (BTNU)
			1'd0: mux_result0 = vga_out_custom_out_color_r0;
			1'd1: mux_result0 = sub_out0;
		endcase

		VGA_R = mux_result0;
		// Output VGA_B
		VGA_B = vga_out_custom_out_color_b0;
		// Output VGA_G
		VGA_G = vga_out_custom_out_color_g0;
	end


// === clocked logic ========================================
endmodule