////////////////////////
/////  FROpenGL.cc /////
////////////////////////

#include "dev_control.h"
#if defined USE_OPEN_GL

#include <stdlib.h>
#include <string.h>
#include "unistd.h"
#include "FROpenGL.h"
#include "FRMath.h"

//#define DEBUG_FR_OPENGL

const int opengl_wait = 1;

FROpenGL* FROpenGL::Current = 0 ;


void FROpenGL::MakeMeCurrent()
{
	if( this != Current ){
		glXMakeCurrent( xwin_p->getDisplay(), xwin_p->getWindow(), mCtx );
		Current = this;
	}
}

Colormap FROpenGL::getCmap( XVisualInfo *vi )
{
    Status  status;
    XStandardColormap *standardCmaps;
    Colormap cmap;
    int i, numCmaps;

#if defined DEBUG_FR_OPENGL
    if ( TrueColor != vi->c_class ) 
	cerr << "no support for non-TrueColor visual\n" ;
#endif
    status = XmuLookupStandardColormap ( xwin_p->getDisplay(), vi->screen, vi->visualid,
             vi->depth, XA_RGB_DEFAULT_MAP, False, True );

    if ( status==1 ) {
	status = XGetRGBColormaps ( xwin_p->getDisplay(), RootWindow (xwin_p->getDisplay(), vi->screen ),
                                    &standardCmaps, &numCmaps, XA_RGB_DEFAULT_MAP );
        if ( status==1 )
            for ( i=0; i<numCmaps; ++i )
		if ( standardCmaps[i].visualid == vi->visualid ) {
		    cmap = standardCmaps[i].colormap; XFree ( standardCmaps );
                    return cmap;
		}
    }
    cmap = XCreateColormap ( xwin_p->getDisplay(), RootWindow ( xwin_p->getDisplay(), vi->screen ),
			     vi->visual, AllocNone );
    return cmap;
}

void FROpenGL::Wait()
{
	int flag = 0;
	XEvent event;

	while ( 1 ) {
		do {
			XNextEvent ( xwin_p->getDisplay(), &event );
			switch ( event.type ) {	
				case Expose:
					flag = 1;
					break;
			}
		} while ( XPending ( xwin_p->getDisplay() ) );

		if ( flag == 1) break;
	}

	sleep( opengl_wait );
}

int FROpenGL::open( X_Window* x_p, char* window_title )
{
	XVisualInfo *vi = NULL;

		//----- For HP native compiler 
	int config [11];
	config[0] = GLX_RED_SIZE;
	config[1] =  1;
	config[2] =  GLX_GREEN_SIZE;
	config[3] = 1; 
	config[4] = GLX_BLUE_SIZE;
	config[5] = 1;
	config[6] = GLX_STENCIL_SIZE;
	config[7] = 1;
	config[8] = GLX_DOUBLEBUFFER;
	config[9] = GLX_RGBA;
	config[10]= (int)(None) ;

	XSetWindowAttributes xswa;
	Display* dpy;

	xwin_p = x_p;

	if((dpy = XOpenDisplay(NULL)) == NULL ){
		cerr << "Cannot open display!\n";
		return 0;
	}

	xwin_p->setDisplay( dpy );

	if ( !glXQueryExtension( xwin_p->getDisplay(), NULL, NULL ) ) {
		cerr << "no OpenGL GLX extension is supported!!!\n" ;
		return 0 ; // ERROR 
	}

	if( doubleBuffer ) {
		vi = glXChooseVisual ( xwin_p->getDisplay(), DefaultScreen(xwin_p->getDisplay()), config );
		if ( vi == NULL ){
			doubleBuffer = 0;
			cerr << "this window is not double buffer mode.\n";
		}
	}

	if( !doubleBuffer ) {
		vi = glXChooseVisual ( xwin_p->getDisplay(), DefaultScreen ( xwin_p->getDisplay() ), &config[1] );
		if ( vi == NULL ){
		cerr << "no appropriate RGB visual with depth buffer!!!\n" ;
		return 0 ; // ERROR
		}
	}

//	xswa.colormap = XCreateColormap ( xwin_p->getDisplay(), RootWindow ( xwin_p->getDisplay(), vi->screen ),
//			     vi->visual, AllocNone );
	xswa.colormap = getCmap( vi );
//	XChangeWindowAttributes( xwin_p->getDisplay(), xwin_p->getWindow(), CWColormap, &xswa );

	xswa.border_pixel = 0; xswa.background_pixel = WhitePixel( xwin_p->getDisplay(), 0 );
	xswa.event_mask = ExposureMask | StructureNotifyMask |
                     ButtonPressMask  | ButtonReleaseMask | KeyPressMask;
	xswa.backing_store = Always ;

	xwin_p->setWindow( XCreateWindow( xwin_p->getDisplay(), RootWindow( xwin_p->getDisplay(), vi->screen ),
				300, 300, (int)A4_WIDTH_PT, (int)A4_HEIGHT_PT, 0, vi->depth, InputOutput, vi->visual,
				CWBackingStore | CWBackPixel | CWBorderPixel | CWColormap | CWEventMask, &xswa ));

	XStoreName ( xwin_p->getDisplay(), xwin_p->getWindow(), window_title );
	xwin_p->setOpen();

	XMapWindow( xwin_p->getDisplay(), xwin_p->getWindow() );

//	glXWaitX();

	Wait();

	mCtx = glXCreateContext ( xwin_p->getDisplay(), vi, NULL, GL_TRUE );
	if ( mCtx == NULL ){
		cerr << "can't create rendering context!!!\n" ;
		return 0 ; // ERROR
	}

	flag_open = 1;

	return 1 ; // NORMAL
}

void FROpenGL::close()
{
	MakeMeCurrent();

	glXDestroyContext( xwin_p->getDisplay(), mCtx );
	glXMakeCurrent( xwin_p->getDisplay(), None, NULL );

	flag_open = 0;
}

void FROpenGL::DrawPolygon( const Vector3* v, size_t n, const ColorType& c )
{
	size_t i;

	MakeMeCurrent();
	
	glColor3d( c.getRed(), c.getGreen(), c.getBlue() );
	
	glBegin( GL_POLYGON );
	for( i = 0 ; i < n ; i++ ){
		glVertex3d( v[i].x, v[i].y, v[i].z );
	}
	glEnd();
}

void FROpenGL::DrawPolygon( const Vector3* v, size_t n, const LineType* lt )
{
	MakeMeCurrent();

	size_t i;

	for( i = 0 ; i < n-1 ; i++ ){
		DrawLine( v[i], v[i+1], lt[i] );
	}
	DrawLine( v[n-1], v[0], lt[n-1] );
}

void FROpenGL::DrawPolyline( const Vector3* v, size_t n, const LineType& lt )
{
	MakeMeCurrent();

	size_t i;
	ColorType c = lt.getColor();

	glColor3d( c.getRed(), c.getGreen(), c.getBlue() );

float width = (float)lt.getWidth() * 72.0 / 25.4 ;
width = width * 1.0 ; // Dummy for HP
//cerr << "DEBUG : given LineWidth = " << width << endl;
	glLineWidth( width );

	if( lt.getStyle() != FR_LINE_SOLID ){
		glLineStipple( (int)lt.getDashLength(), 0x3333 );
		glEnable( GL_LINE_STIPPLE );
	} else {
		glDisable( GL_LINE_STIPPLE );
	}

	if( lt.getVisibility() ){
		glBegin( GL_LINES );
		for( i = 0 ; i < n ; i++ ){
			glVertex3d( v[i].x, v[i].y, v[i].z );
		}
		glEnd();
	}
}

void FROpenGL::DrawLine( const Vector3& v1, const Vector3& v2, const LineType& lt )
{
	MakeMeCurrent();

	ColorType c = lt.getColor();

	glColor3d( c.getRed(), c.getGreen(), c.getBlue() );

float width = (float)lt.getWidth() * 72.0 / 25.4 ;
width = width * 1.0 ; // Dummy for HP
//cerr << "DEBUG : given LineWidth = " << width << endl;
	glLineWidth( width );

	if( lt.getStyle() != FR_LINE_SOLID ){
		glLineStipple( (int)lt.getDashLength(), 0x3333 );
		glEnable( GL_LINE_STIPPLE );
	} else {
		glDisable( GL_LINE_STIPPLE );
	}

	if( lt.getVisibility() ){
		glBegin( GL_LINES );
		glVertex3d( v1.x, v1.y, v1.z );
		glVertex3d( v2.x, v2.y, v2.z );
		glEnd();	
	}
}

void FROpenGL::SetCamera( Perspective& camera, const Box3D& bb )
{
	MakeMeCurrent();

	Vector3 eye,target,up; // eye_position, target_point, and up_vector
	Vector3 eye_vector   ; // eye_vector
	
	eye  = camera.getCameraPosition() ;
	target = camera.getTarget();

	char UP_DIRECTION = 'Y'; // initiallization

	if( getenv( "DAWN_UP_DIRECTION" ) != NULL ) {
		if(	!strcmp(getenv( "DAWN_UP_DIRECTION" ),"z") || \
			!strcmp(getenv( "DAWN_UP_DIRECTION" ),"Z")      )
		{
			UP_DIRECTION = 'Z' ;
		}
	} 

	//	up.x = -eye.x * eye.z ;
	//	up.y = -eye.y * eye.z ;
	//	up.z = eye.x * eye.x + eye.y * eye.y ;
	switch (UP_DIRECTION) {
	 case 'y': case 'Y':
		up(0.0, 1.0, 0.0 );
		break ;
	 case 'z': case 'Z':
		up(0.0, 0.0, 1.0 );
		break ;
	 default: // 'Y'
		up(0.0, 1.0, 0.0 );
		break ;
	}
	up = up.normalize() ;

		//----- error recovery for eye_vector // up_vector
	eye_vector = camera.getEyeVector();
	eye_vector = eye_vector.normalize();
	if( fabs(up * eye_vector) > 0.999 ) { 
	  double x = up.x, y = up.y , z = up.z ;
	  up(y, x, z) ; 
	  // e.g.  if up // eye_vec // y-axis, up // x-axis
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// CERN
	//gluLookAt( eye.x, eye.y, eye.z, 
	//			target.x, target.y, target.z,
	//				up.x, up.y, up.z );
	gluLookAt( eye.x    , eye.y    , eye.z    , 
		   target.x , target.y , target.z ,
		   up.x     , up.y     , up.z      );	


	double near, far, fovy, eps = 1e+0;

	// CERN
	//	near = (eye - target ).getLength();
	//	far = near;
	Vector3 center ;
	center = bb.getCenter() ;
	near   = camera.eyeZ( center );
	far = near ;

	Vector3 corner; double length;

	for( int i = 0; i < 8; i ++ ){

		ToolBox3D::GetCorner( i, bb, corner ) ;		
//		corner = bb.getCorner( i );


		length = camera.eyeZ( corner );
		if( near > length ) near = length;
		if( far < length ) far = length;
	}

	fovy = 2.0*atan2(0.5, camera.getFocalDistance() )*180.0/FR_D_PI/GL_VIEWPORT_ASPECT;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( fovy, GL_VIEWPORT_ASPECT, near-eps, far+eps );

	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );

}

void FROpenGL::SetViewport( int x, int y, int w, int h )
{
	MakeMeCurrent();
	glViewport( x, y, w, h );
}

void FROpenGL::Clear( const ColorType& col )
{
	MakeMeCurrent();
	glClearColor( col.getRed(), col.getGreen(), col.getBlue(), 1.0 );
	glClear( GL_COLOR_BUFFER_BIT );
}

void FROpenGL::DepthClear()
{
	MakeMeCurrent();
	glEnable( GL_DEPTH_TEST );
	glClear( GL_DEPTH_BUFFER_BIT );
}

void FROpenGL::Flush()
{
	if ( doubleBuffer ) glXSwapBuffers( xwin_p->getDisplay(), xwin_p->getWindow() );
	else glFlush();
}

#endif // USE_OPEN_GL
