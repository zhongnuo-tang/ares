#include <stdio.h>
#include <tinyara/config.h>

#include "lcd_runner.h"

#include "lvgl.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <tinyara/config.h>
#include <tinyara/lcd/lcd_dev.h>
#include <tinyara/rtc.h>

#define LCD_DEV_PATH "/dev/lcd%d"
#define LCD_DEV_PORT 0

/* ====================== CONFIG ====================== */
#define LCD_W CONFIG_LCD_YRES
#define LCD_H CONFIG_LCD_XRES
#define BYTES_PER_PIXEL 2
#define IMG_NAME crabpower

/* ====================== BUFFERS ==================== */
static lv_color_t *buf1 = NULL;
LV_IMG_DECLARE( IMG_NAME );

/* ===================== GLOBALS ===================== */
static int lcd_fd = -1;
static lv_display_t *disp;

void set_lcd_power( int power )
{
    power_test( power );
}

static void lcd_flush_cb( lv_display_t *display, const lv_area_t *area, uint8_t *px_map )
{
    struct lcddev_area_s lcd_area;

    lcd_area.planeno = 0;
    lcd_area.row_start = area->y1;
    lcd_area.row_end = area->y2;
    lcd_area.col_start = area->x1;
    lcd_area.col_end = area->x2;
    lcd_area.stride = ( area->x2 - area->x1 + 1 ) * BYTES_PER_PIXEL;
    lcd_area.data = px_map;

    ioctl( lcd_fd, LCDDEVIO_PUTAREA, (unsigned long)(uintptr_t)&lcd_area );
    usleep( 10000 );

    lv_display_flush_ready( display );
}

void lv_hal_init( void )
{
    struct fb_videoinfo_s vinfo;
    char port[ 20 ];

    /* Open LCD device */
    snprintf( port, sizeof( port ), LCD_DEV_PATH, LCD_DEV_PORT );
    lcd_fd = open( port, O_RDWR | O_SYNC );
    if ( lcd_fd < 0 )
    {
        printf( "ERROR: Failed to open LCD: %d\n", get_errno() );
        return;
    }

    if ( ioctl( lcd_fd, LCDDEVIO_GETVIDEOINFO, (unsigned long)(uintptr_t)&vinfo ) < 0 )
    {
        printf( "ERROR: GETVIDEOINFO failed\n" );
        close( lcd_fd );
        lcd_fd = -1;
        return;
    }

    printf( "LCD resolution: %dx%d\n", vinfo.xres, vinfo.yres );
}

void lv_port_disp_init( void )
{
    lv_init();

    disp = lv_display_create( LCD_W, LCD_H );

    size_t buf_pixels = LCD_W * LCD_H;
    buf1 = malloc( buf_pixels * sizeof( lv_color_t ) );
    if ( !buf1 )
    {
        printf( "LVGL buffer alloc failed\n" );
        return;
    }

    lv_display_set_buffers( disp, buf1, NULL, buf_pixels * sizeof( lv_color_t ), LV_DISPLAY_RENDER_MODE_FULL );

    lv_display_set_flush_cb( disp, lcd_flush_cb );
}

static void anim_x_cb( void *var, int32_t v )
{
    lv_obj_set_x( (lv_obj_t *)var, v );
}

static void anim_size_cb( void *var, int32_t v )
{
    lv_obj_set_size( (lv_obj_t *)var, v, v );
}

/**
 * Create a playback animation
 */
void lcd_draw( void )
{
    lv_obj_t *img = lv_img_create( lv_scr_act() ); // create image object
    lv_img_set_src( img, &IMG_NAME );              // set the crab image
    lv_obj_align( img, LV_ALIGN_LEFT_MID, 10, 0 );

    // Animate horizontally
    lv_anim_t a;
    lv_anim_init( &a );
    lv_anim_set_var( &a, img );
    lv_anim_set_exec_cb( &a, anim_x_cb );
    lv_anim_set_values( &a, 10, LCD_W - lv_obj_get_width( img ) );
    lv_anim_set_duration( &a, 2000 );
    lv_anim_set_playback_time( &a, 2000 );
    lv_anim_set_repeat_count( &a, LV_ANIM_REPEAT_INFINITE );
    lv_anim_start( &a );
}

int task_draw_lcd( int argc, char *argv[] )
{
    /* Initialize LCD & LVGL */
    set_lcd_power( 100 );
    lv_hal_init();
    lv_port_disp_init();

    lcd_draw();

    /* ===== Main LVGL loop ===== */
    while ( 1 )
    {
        lv_tick_inc( 5 );
        lv_timer_handler();
        usleep( 5000 ); // 5 ms
    }

    return 0;
}
