//=====================================================================
// Initialize the display -------------------------------------------
void init_display() {
  static lv_disp_drv_t disp_drv;     // Descriptor of a display driver
  lv_disp_drv_init(&disp_drv);       // Basic initialization
  disp_drv.flush_cb = my_disp_flush; // Set your driver function
  disp_drv.draw_buf = &draw_buf;     // Assign the buffer
  disp_drv.hor_res  = screenWidth;   // horizontal resolution
  disp_drv.ver_res  = screenHeight;  // vertical resolution
  lv_disp_drv_register(&disp_drv);   // Finally register the driver
  lv_disp_set_bg_color(NULL,lv_color_hex(0x0000));// background black
}

//=====================================================================
// Display flushing
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, 
                    lv_color_t *color_p) {
  if (tft.getStartCount()==0){  // Run if not already started
    tft.startWrite();
  } 
  tft.pushImageDMA( area->x1
                  , area->y1
                  , area->x2 - area->x1 + 1
                  , area->y2 - area->y1 + 1
                  , ( lgfx::swap565_t* )&color_p->full);
  lv_disp_flush_ready(disp);
}
//====================================================================
