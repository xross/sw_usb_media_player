# Dependencies shared by the player and its target fixtures.
set(APP_DEPENDENT_MODULES "lib_logging(3.3.2)"
    "git@github.com:xross/lib_xuh(2d5f5b4b36bd0675e7e04f3368a78c6e108abfb3)"
    "https://github.com/xmos/lib_fatfs(7793c9c5a7df6ed184f785568bc7a9a86b234d0b)")
set(XUH_JPEG_DEPENDENCY "git@github.com:xmos/lib_jpeg(b515df327d287e3444e8d3cc01642d65d90691ae)")
set(XUH_SPI_DEPENDENCY "lib_spi(4.0.0)")
# lib_i2s 6.0.1 declares lib_xassert 4.3.1 but only uses assert(), which 5.x
# keeps. lib_xassert is named first so that the player builds against 5.x.
set(XUH_I2S_DEPENDENCY "lib_xassert(5.0.0)" "lib_i2s(6.0.1)")
set(XUH_SW_PLL_DEPENDENCY "lib_sw_pll(2.5.0)")
set(XUH_MPEG_DEPENDENCY "git@github.com:xross/lib_mpeg(7032989b40a2aa969480df5ec539a87fd585f66b)")
