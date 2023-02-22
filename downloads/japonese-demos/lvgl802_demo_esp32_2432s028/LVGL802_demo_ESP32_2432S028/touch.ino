//===================================================================== 
//Initialize the input device driver
void init_touch() {
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

//===================================================================== 
void my_touchpad_read( lv_indev_drv_t * indev_driver, 
                       lv_indev_data_t * data ){
  uint16_t touchX, touchY;
  bool touched = tft.getTouch( &touchX, &touchY);
  Serial.print(touchX);Serial.print(" ");Serial.println(touchY);
  if(touched){
    if(touchX < screenWidth && touchY < screenHeight){
      data->state = LV_INDEV_STATE_PR;
      data->point.x = touchX;
      data->point.y = touchY;
      Serial.printf("%d,%d\n", touchX, touchY);
    }
    else{ data->state = LV_INDEV_STATE_REL;}
  }
}
//=====================================================================
