/********************************************************
*
* Filename: 	ihm_vision.h
*
* Team: 		Beta
*
* Description: 	Header file to a collection of methods 
* 				which are responsible for creating a 
*				window which display camera data from 
*				the drone
*
*********************************************************/

#ifndef _IHM_VISION_H
#define _IHM_VISION_H

#include <gtk-2.0/gtk/gtk.h>
#include <ardrone_tool/Navdata/ardrone_navdata_client.h>

extern char label_vision_state_value[32];
extern GtkLabel *label_vision_values;

void ihm_ImageWinDestroy( GtkWidget *widget, gpointer data );
void create_image_window( void );

// Name: 		navdata_hdvideo_init
// Parameters:	param - ???
// Returns: 	C_RESULT - C_OK
// Description: Creates the gtk window which displays and adjust settings
//				for the video coming from the done
C_RESULT navdata_hdvideo_init( void* param );

// Name: 		navdata_hdvideo_process
// Parameters:	navdata_unpacked_t* - ???
//				navdata - ???
// Returns: 	C_RESULT - C_OK
// Description: ???
C_RESULT navdata_hdvideo_process( const navdata_unpacked_t* const navdata );

// Name: 		navdata_hdvideo_release
// Parameters:	void
// Returns: 	C_RESULT - C_OK
// Description: Clears up the memory associated with creating the display
//				window
C_RESULT navdata_hdvideo_release( void );


#endif // _IHM_VISION_H
