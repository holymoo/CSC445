/*
 * @ihm_vision.c
 * @author marc-olivier.dzeukou@parrot.com
 * @date 2007/07/27
 *
 * @author stephane.piskorski@parrot.com
 * @date 2010/10/18
 * ihm vision source file
 *
 */

#include   <pthread.h>
#include   <gtk/gtk.h>

#include <ardrone_api.h>
#ifdef PC_USE_VISION
#    include <Vision/vision_tracker_engine.h>
#endif
#include "ihm/ihm_vision.h"
#include "ihm/ihm.h"
#include "common/mobile_config.h"
#include "ihm/ihm_stages_o_gtk.h"

#include <ardrone_tool/ardrone_tool_configuration.h>
#include <ardrone_tool/ardrone_version.h>

#include <VP_Os/vp_os_print.h>
#include <VP_Os/vp_os_malloc.h>
#include <VP_Os/vp_os_delay.h>
#include <VP_Api/vp_api_supervisor.h>
#include <VLIB/video_codec.h>

#include <ardrone_tool/Academy/academy_stage_recorder.h>
#include <ardrone_tool/Navdata/ardrone_academy_navdata.h>

#ifdef RECORD_RAW_VIDEO
#include <ardrone_tool/Video/video_stage_recorder.h>
extern video_stage_recorder_config_t vrc;
#endif

#if defined(FFMPEG_SUPPORT) && defined(RECORD_FFMPEG_VIDEO)
#include <ardrone_tool/Video/video_stage_ffmpeg_recorder.h>
extern video_stage_ffmpeg_recorder_config_t ffmpeg_vrc;
#endif

#if defined (RECORD_ENCODED_VIDEO)
#include <ardrone_tool/Video/video_stage_encoded_recorder.h>
#endif

#include <ardrone_tool/Video/video_stage_latency_estimation.h>
#include <ardrone_tool/Video/video_stage.h>
#include <ardrone_tool/Video/video_recorder_pipeline.h>

// Added for OpenCV Support
#include "cv.h"
#include "highgui.h" // if you want to display images with OpenCV functions

enum {
  VIDEO_DISPLAYSIZE_FRAME=0,
  STATE_FRAME,
  TRACKING_PARAMETERS_FRAME,
  TRACKING_OPTION_FRAME,
  COMPUTING_OPTION_FRAME,
  VIDEO_STREAM_FRAME,
  VIDEO_BITRATE_FRAME,
  VIDEO_DISPLAY_FRAME,
  VIDEO_INFO_FRAME,
  VIDEO_NAVDATA_FRAME,
  NB_IMAGES_FRAMES
};

enum {
  TRACKING_PARAM_HBOX1=0,
  TRACKING_PARAM_HBOX2,
  TRACKING_PARAMS_HBOX,
  TRACKING_OPTION_HBOX,
  COMPUTING_OPTION_HBOX,
  VIDEO_DISPLAYSIZE_HBOX,
  VIDEO_STREAM_HBOX,
  VIDEO_BITRATE_HBOX,
  VIDEO_DISPLAY_HBOX,
  NB_IMAGES_H_BOXES
};

enum {
  CS_ENTRY=0,
  NB_P_ENTRY,
  LOSS_ENTRY,
  NB_TLG_ENTRY,
  NB_TH_ENTRY,
  SCALE_ENTRY,
  DIST_MAX_ENTRY,
  MAX_DIST_ENTRY,
  NOISE_ENTRY,
  FAKE_ENTRY,
  NB_IMAGES_ENTRIES
};

enum {
  UPDATE_VISION_PARAMS_BUTTON = 0,
  TZ_KNOWN_BUTTON,
  NO_SE_BUTTON,
  SE2_BUTTON,
  SE3_BUTTON,
  PROJ_OVERSCENE_BUTTON,
  LS_BUTTON,
  FRONTAL_SCENE_BUTTON,
  FLAT_GROUND_BUTTON,
  PICTURE_BUTTON,
  RECORD_BUTTON,
  RAW_CAPTURE_BUTTON,
  ZAPPER_BUTTON,
  FULLSCREEN_BUTTON,
  PAUSE_BUTTON,
  LATENCY_ESTIMATOR_BUTTON,
  CUSTOM_BUTTON,
  NB_IMAGES_BUTTONS,
};

enum {
  RAW_CAPT_FSBUTTON = 0,
  RECORD_LOCAL_FSBUTTON,
  CHANGE_CAM_FSBUTTON,
  FULLSCREEN_FSBUTTON,
  NB_FULL_SCREEN_BUTTON,
};

enum {
  VIDEO_SIZE_LIST=0,
  NB_VIDEO_SIZE_WIDGET
};

enum {
  CODEC_TYPE_LIST=0,
  BITRATE_MODE_LIST,
  MANUAL_BITRATE_ENTRY,
  WIFI_BITRATE_MODE_LIST,
  CODEC_FPS_LIST,
  USB_RECORD_CHECKBOX,
  NB_VIDEO_STREAM_WIDGET
};

char  ihm_ImageTitle[128] = "VISION : Image" ;
char *ihm_ImageFrameCaption[NB_IMAGES_FRAMES]  = {"Video display size",
												  "Vision states",
												  "Tracking parameters",
												  "Tracking options",
												  "Computing options",
												  "Video Stream",
												  "Video Bitrate",
												  "Live Display",
												  "Last decoded picture",
												  "Video stream navdata"};


enum{
  VIDEO_DISPLAYSIZE_CAPTION_FRAMETITLE=0,
  VIDEO_DISPLAYSIZE_CAPTION_SIZES,
  VIDEO_DISPLAYSIZE_CAPTION_INTERP_MODES,
  NB_VIDEO_DISPLAYSIZE_CAPTION
};

char *ihm_ImageVideoSizeCaption[NB_VIDEO_DISPLAYSIZE_CAPTION] = {" Viewport size ","Display size","Interpolation"};

char *ihm_ImageVideoStreamCaption[NB_VIDEO_STREAM_WIDGET] = {" Codec type "," Bitrate control mode ", " Manual target bitrate ","WiFi bitrate","FPS"};


GtkWidget *ihm_ImageVBox, *ihm_ImageVBoxPT, *ihm_ImageHBox[NB_IMAGES_H_BOXES], *displayvbox, *ihm_ImageButton[NB_IMAGES_BUTTONS], *ihm_ImageLabel[NB_IMAGES_ENTRIES],  *ihm_ImageFrames[NB_IMAGES_FRAMES], *ihm_VideoStreamLabel[NB_VIDEO_STREAM_WIDGET],*ihm_ImageVideoSizeLabel[NB_VIDEO_DISPLAYSIZE_CAPTION];

extern GtkWidget *button_show_image,*button_show_image2;
extern mobile_config_t *pcfg;
extern PIPELINE_HANDLE video_pipeline_handle;
/* Vision image var */
GtkLabel *label_vision_values=NULL;
GtkWidget *ihm_ImageWin= NULL, *ihm_ImageEntry[NB_IMAGES_ENTRIES], *ihm_VideoStream_VBox=NULL;

/* For fullscreen video display */
GtkWidget *fullscreen_window = NULL;
GtkWidget *fullscreen_eventbox = NULL;
GtkImage *fullscreen_image = NULL;
GdkScreen *fullscreen = NULL;
GtkWidget *ihm_fullScreenButton[5], *ihm_fullScreenHBox;
GtkWidget *ihm_fullScreenFixedContainer;
GtkWidget *align;
int flag_timer_is_active = 0;
int timer_counter = 0;

GtkWidget* video_bitrateEntry;
GtkWidget*  video_bitrateModeList;
GtkWidget*  wifi_bitrateModeList;
GtkWidget*  codecFPSList;
GtkWidget*  codecSlicesList;
GtkWidget*  codecSocketList;
GtkWidget*  decodeLatencyList;
GtkWidget*  usbRecordCheckBox;
//GtkObject * codecFPSList_adj;
GtkWidget * video_sizeList;
GtkWidget * video_interpolationModesList;
GtkWidget * video_codecList;
GtkWidget *video_bitrateButton;
int tab_vision_config_params[10];
int vision_config_options;
int image_vision_window_status, image_vision_window_view;
char label_vision_state_value[32];
extern GtkImage *image;

GtkWidget * video_information = NULL;
GtkWidget * video_information_hbox = NULL;

GtkWidget * video_navdata = NULL;
GtkWidget * video_navdata_hbox = NULL;

#ifdef RECORD_RAW_VIDEO
void ihm_video_recording_callback(video_stage_recorder_config_t *cfg) {
  printf("%s recording %s\n", (cfg->startRec != VIDEO_RECORD_STOP) ? "Started" : "Stopped", cfg->video_filename);
}
#endif

#ifdef RECORD_FFMPEG_VIDEO
void ihm_ffmpeg_video_recording_callback(video_stage_ffmpeg_recorder_config_t *cfg) {
  printf("%s recording %s\n", (cfg->startRec != VIDEO_RECORD_STOP) ? "Started" : "Stopped", cfg->video_filename);
}
#endif

#ifdef RECORD_ENCODED_VIDEO
void ihm_video_encoded_recording_callback(video_stage_encoded_recorder_config_t *cfg) {
  // nothing for now ...
  printf ("callback status %d\n", cfg->startRec);
}
#endif

static void ihm_VideoFullScreenStop(GtkWidget *widget, gpointer data) {
  printf("Quitting fullscreen.\n");
  fullscreen = NULL;
  fullscreen_image = NULL;
  fullscreen_window = NULL;
}

gboolean hide_fullscreen_buttons(gpointer pData) {
  timer_counter--;
  if (timer_counter <= 0) {
    if (GTK_IS_WIDGET(ihm_fullScreenHBox))
      gtk_widget_hide(ihm_fullScreenHBox);
    timer_counter = 0;
  }
  return TRUE;
}

void ihm_VideoFullScreenMouseMove(GtkWidget *widget, gpointer data) {
  timer_counter = 2;
  if (GTK_IS_WIDGET(ihm_fullScreenHBox)) {
    gtk_widget_show(ihm_fullScreenHBox);
  }
}

void ihm_ImageWinDestroy(GtkWidget *widget, gpointer data) {
  image_vision_window_status = WINDOW_CLOSED;
  printf("Destroying the Video window.\n");
  if (fullscreen != NULL) {
    ihm_VideoFullScreenStop(NULL, NULL);
  }
  ihm_VideoStream_VBox = NULL; /* this var. is tested by stage Gtk */
  ihm_ImageWin = NULL;
  video_stage_suspend_thread();
  if (2 <= ARDRONE_VERSION ())
    {
      video_recorder_suspend_thread ();
    }
}

gint ihm_ImageWinDelete(GtkWidget *widget, GdkEvent *event, gpointer data) {
  image_vision_window_status = WINDOW_CLOSED;
  printf("Deleting the Video window.\n");
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button_show_image), FALSE);
  return FALSE;
}

static void ihm_showImage(gpointer pData) {
  //GtkWidget* widget = (GtkWidget*) pData;

  //if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) 
  {
    if (!GTK_IS_WIDGET(ihm_ImageWin)) {
      create_image_window(); // Recreate window if it has been killed
    }
    gtk_widget_show_all(ihm_ImageWin);
    image_vision_window_view = WINDOW_VISIBLE;
    
    vp_os_delay(500);
    video_stage_resume_thread();
    if (2 <= ARDRONE_VERSION ())
      {
        video_recorder_resume_thread ();
      }
  }
}

extern vp_stages_gtk_config_t gtkconf;

static void ihm_changeVideoInterpolation(GtkComboBox *widget, gpointer data) {
  gint pos;
  pos = gtk_combo_box_get_active( widget );
 
  switch(pos)
  {
    case 0:   printf("Setting GTK display interpolation : Nearest neighbour\n"); gtkconf.gdk_interpolation_mode = GDK_INTERP_NEAREST;  break;
    case 1:   printf("Setting GTK display interpolation : Tiles\n");             gtkconf.gdk_interpolation_mode = GDK_INTERP_TILES;    break;
    case 2:   printf("Setting GTK display interpolation : Bilinear\n");          gtkconf.gdk_interpolation_mode = GDK_INTERP_BILINEAR; break;
    case 3:   printf("Setting GTK display interpolation : Hyperbolic\n");        gtkconf.gdk_interpolation_mode = GDK_INTERP_HYPER;    break;
      
    default: /*nada*/ break;
  }
}

extern int video_stage_decoder_fakeLatency;

void update_vision( void )
{
  if (ihm_ImageWin != NULL && GTK_IS_WIDGET(ihm_ImageWin)) {
    if (image_vision_window_view == WINDOW_VISIBLE) {

      // Vision state refresh
      if (label_vision_values != NULL && GTK_IS_LABEL(label_vision_values))
        gtk_label_set_label(label_vision_values, label_vision_state_value);
      if (ihm_ImageWin != NULL && GTK_IS_WIDGET(ihm_ImageWin))
        gtk_widget_show_all(ihm_ImageWin);
    }
  }
}

// Name:        create_image_window
// Parameters:  void
// Returns:     void
// Description: Creates the window that houses the image data
void create_image_window( void )
{

  // Image main window
  ihm_ImageWin = gtk_window_new( GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width(GTK_CONTAINER(ihm_ImageWin), 10);
  gtk_window_set_title(GTK_WINDOW(ihm_ImageWin), ihm_ImageTitle);
  gtk_signal_connect(GTK_OBJECT(ihm_ImageWin), "destroy", G_CALLBACK(ihm_ImageWinDestroy), NULL );

  /* Set the callback for the checkbox inside the main application window */
  //g_signal_connect(G_OBJECT(button_show_image), "clicked", G_CALLBACK(ihm_showImage), (gpointer) ihm_ImageWin);
  g_signal_connect(G_OBJECT(button_show_image2), "clicked", G_CALLBACK(ihm_showImage), (gpointer) ihm_ImageWin);
}


C_RESULT navdata_hdvideo_init( void* param )
{
  return C_OK;
}

C_RESULT navdata_hdvideo_process( const navdata_unpacked_t* const navdata )
{
	char buffer[1024];

	snprintf(buffer,sizeof(buffer)-1,"Storage FIFO: %d packets - %d kbytes %s\nUSB key : size %d kbytes - free %d kbytes",
			navdata->navdata_hdvideo_stream.storage_fifo_nb_packets,
			navdata->navdata_hdvideo_stream.storage_fifo_size,
			(navdata->navdata_hdvideo_stream.hdvideo_state&NAVDATA_HDVIDEO_STORAGE_FIFO_IS_FULL)?
					"- FULL":
					"",
			navdata->navdata_hdvideo_stream.usbkey_size,
			navdata->navdata_hdvideo_stream.usbkey_freespace);

	gdk_threads_enter();

	if (video_information){
			gtk_label_set_text((GtkLabel *)video_navdata,(const gchar*)buffer);
			gtk_label_set_justify((GtkLabel *)video_navdata,GTK_JUSTIFY_LEFT);
	    }

	gdk_threads_leave();

	return C_OK;
}

// Name:        navadata_hdvideo_release
// Parameters:  void
// Return:      C_Result - C_OK always
// Description: Releases all the memory allocated during the 
//              processing of the video
C_RESULT navdata_hdvideo_release( void )
{
  return C_OK;
}


