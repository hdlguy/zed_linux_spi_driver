module top
   (DDR_addr,
    DDR_ba,
    DDR_cas_n,
    DDR_ck_n,
    DDR_ck_p,
    DDR_cke,
    DDR_cs_n,
    DDR_dm,
    DDR_dq,
    DDR_dqs_n,
    DDR_dqs_p,
    DDR_odt,
    DDR_ras_n,
    DDR_reset_n,
    DDR_we_n,
    FIXED_IO_ddr_vrn,
    FIXED_IO_ddr_vrp,
    FIXED_IO_mio,
    FIXED_IO_ps_clk,
    FIXED_IO_ps_porb,
    FIXED_IO_ps_srstb,
    spi_rtl_io0_io,
    spi_rtl_io1_io,
    spi_rtl_ss_io);
  inout [14:0]DDR_addr;
  inout [2:0]DDR_ba;
  inout DDR_cas_n;
  inout DDR_ck_n;
  inout DDR_ck_p;
  inout DDR_cke;
  inout DDR_cs_n;
  inout [3:0]DDR_dm;
  inout [31:0]DDR_dq;
  inout [3:0]DDR_dqs_n;
  inout [3:0]DDR_dqs_p;
  inout DDR_odt;
  inout DDR_ras_n;
  inout DDR_reset_n;
  inout DDR_we_n;
  inout FIXED_IO_ddr_vrn;
  inout FIXED_IO_ddr_vrp;
  inout [53:0]FIXED_IO_mio;
  inout FIXED_IO_ps_clk;
  inout FIXED_IO_ps_porb;
  inout FIXED_IO_ps_srstb;
  inout spi_rtl_io0_io;
  inout spi_rtl_io1_io;
  inout [0:0]spi_rtl_ss_io;

  wire [14:0]DDR_addr;
  wire [2:0]DDR_ba;
  wire DDR_cas_n;
  wire DDR_ck_n;
  wire DDR_ck_p;
  wire DDR_cke;
  wire DDR_cs_n;
  wire [3:0]DDR_dm;
  wire [31:0]DDR_dq;
  wire [3:0]DDR_dqs_n;
  wire [3:0]DDR_dqs_p;
  wire DDR_odt;
  wire DDR_ras_n;
  wire DDR_reset_n;
  wire DDR_we_n;
  wire FIXED_IO_ddr_vrn;
  wire FIXED_IO_ddr_vrp;
  wire [53:0]FIXED_IO_mio;
  wire FIXED_IO_ps_clk;
  wire FIXED_IO_ps_porb;
  wire FIXED_IO_ps_srstb;
  wire spi_rtl_io0_i;
  wire spi_rtl_io0_io;
  wire spi_rtl_io0_o;
  wire spi_rtl_io0_t;
  wire spi_rtl_io1_i;
  wire spi_rtl_io1_io;
  wire spi_rtl_io1_o;
  wire spi_rtl_io1_t;
  wire [0:0]spi_rtl_ss_i_0;
  wire [0:0]spi_rtl_ss_io_0;
  wire [0:0]spi_rtl_ss_o_0;
  wire spi_rtl_ss_t;

  IOBUF spi_rtl_io0_iobuf
       (.I(spi_rtl_io0_o),
        .IO(spi_rtl_io0_io),
        .O(spi_rtl_io0_i),
        .T(spi_rtl_io0_t));
  IOBUF spi_rtl_io1_iobuf
       (.I(spi_rtl_io1_o),
        .IO(spi_rtl_io1_io),
        .O(spi_rtl_io1_i),
        .T(spi_rtl_io1_t));
  IOBUF spi_rtl_ss_iobuf_0
       (.I(spi_rtl_ss_o_0),
        .IO(spi_rtl_ss_io[0]),
        .O(spi_rtl_ss_i_0),
        .T(spi_rtl_ss_t));
  system system_i
       (.DDR_addr(DDR_addr),
        .DDR_ba(DDR_ba),
        .DDR_cas_n(DDR_cas_n),
        .DDR_ck_n(DDR_ck_n),
        .DDR_ck_p(DDR_ck_p),
        .DDR_cke(DDR_cke),
        .DDR_cs_n(DDR_cs_n),
        .DDR_dm(DDR_dm),
        .DDR_dq(DDR_dq),
        .DDR_dqs_n(DDR_dqs_n),
        .DDR_dqs_p(DDR_dqs_p),
        .DDR_odt(DDR_odt),
        .DDR_ras_n(DDR_ras_n),
        .DDR_reset_n(DDR_reset_n),
        .DDR_we_n(DDR_we_n),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
        .spi_rtl_io0_i(spi_rtl_io0_i),
        .spi_rtl_io0_o(spi_rtl_io0_o),
        .spi_rtl_io0_t(spi_rtl_io0_t),
        .spi_rtl_io1_i(spi_rtl_io1_i),
        .spi_rtl_io1_o(spi_rtl_io1_o),
        .spi_rtl_io1_t(spi_rtl_io1_t),
        .spi_rtl_ss_i(spi_rtl_ss_i_0),
        .spi_rtl_ss_o(spi_rtl_ss_o_0),
        .spi_rtl_ss_t(spi_rtl_ss_t));
endmodule
