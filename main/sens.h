void hx710b_user(void );
void gps_user(void);
void hx710b_init(void);
void gps_init(void);
void sdcard_init(void);
void sdcard_write(float pressure, char *gps);
extern float pressure;
extern char gps_buffer[64];