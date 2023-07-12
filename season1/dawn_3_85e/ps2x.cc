///////////////////
///// ps2x.cc /////
///////////////////

#include "dev_control.h"
#if defined USE_XWIN


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#include <unistd.h>

#include "ps2x.h"
#include <X11/Xatom.h>

/* X-Window's property.
   GhostScript return this property when finish drawing. */
typedef struct
{
	Window	parent ;
	Atom	GS_DONE ;
	Atom	GS_PAGE ;
} gs_property ;

const int BUFSIZE        = 1024 ;
const char GHOSTSCRIPT[] = "gs" ;
static inline int  ROUND( double x) 
{ return (int)((x) > 0 ? (int)(x + 0.5) : (int)(x - 0.5)) ; }


static void	(*sigchld)(int) ;
static int	gs_pid ;
static int	gs_running = 0 ;

static Display	*cur_dpy ;
static Window	cur_win ;

static char	dpy_env[BUFSIZE] ;
static char	gv_env[BUFSIZE] ;

static void	catch_sigchld( int ) ;
static Bool	gs_event_handler( Display*, XEvent*, char* ) ;


	//-----
PS2X::PS2X()
{
	flag_open = 0 ;
}

	//-----
GSContext PS2X::CreateContext(
	int	orient,
	float	llx,  float lly,
	float	urx,  float ury,
	float	xmag, float ymag )
{
	GSContext gs ;

	if( (gs = (GSContext)malloc(sizeof(GSContext_t))) == NULL)
		return NULL ;

	gs->orient = orient ;
	gs->llx = llx ; gs->lly = lly ;
	gs->urx = urx ; gs->ury = ury ;
	gs->xdpi = xmag * 72.0 ;
	gs->ydpi = ymag * 72.0 ;

	return gs ;
}

	//-----
void PS2X::DestroyContext()
{
	free(ctx) ;
}

	//-----
Bool PS2X::Start(
	Display		*dpy,
	Window		win  )
{
	char	buf[BUFSIZE] ;
	int	fd[2] ;

	if( gs_running )
		return False ;

	sigchld = signal( SIGCHLD, catch_sigchld ) ;

	sprintf( dpy_env, "DISPLAY=%s", XDisplayString(dpy) ) ;
	putenv( dpy_env ) ;

	sprintf( gv_env, "GHOSTVIEW=%d 0", (int)win ) ;
	putenv( gv_env ) ;

	sprintf( buf, "0 %d %d %d %d %d %f %f 0 0 0 0 0 0",
		ctx->orient,
		ROUND(ctx->llx), ROUND(ctx->lly),
		ROUND(ctx->urx), ROUND(ctx->ury),
		ctx->xdpi, ctx->ydpi ) ;

	XChangeProperty( dpy, win, XInternAtom(dpy, "GHOSTVIEW", False),
		XA_STRING, 8, PropModeReplace, (unsigned char*)buf, strlen(buf) ) ;

	XFlush(dpy) ;

	pipe(fd) ;

	if((gs_pid = fork()) < 0)
	{	perror("fork") ;
		exit(1) ;
	}

	if(gs_pid == 0)	/* child */
	{
		close(fd[1]) ;
		close(0) ;		/* std in */
		dup(fd[0]) ;
		close(fd[0]) ;

		if(-1 == (execlp( GHOSTSCRIPT, GHOSTSCRIPT, "-q", "-dNOPAUSE", "-", NULL )))
		{
			perror("gs") ;
			exit(1) ;
		}
	}

	close(fd[0]) ;
	close(1) ;		/* std out */
	dup(fd[1]) ;
	close(fd[1]) ;

	gs_running = 1 ;
	cur_dpy = dpy ;
	cur_win = win ;

	return True ;
}

	//-----
Bool PS2X::End()
{
	XEvent ev ;
	gs_property props ;

	if( ! gs_running )
		return False ;

	fflush(stdout) ;

	props.parent = cur_win ;
	props.GS_DONE = XInternAtom( cur_dpy, "DONE", False ) ;
	props.GS_PAGE = XInternAtom( cur_dpy, "PAGE", False ) ;

	XIfEvent( cur_dpy, &ev, gs_event_handler, (char *)&props ) ;

	signal( SIGCHLD, sigchld ) ;
	kill( gs_pid, SIGTERM ) ;
	wait( NULL ) ;

	gs_running = 0 ;

	return True ;
}

	//-----
void	PS2X::Open ( 
	    Display	*dpy ,
	    Window	win  ,
	    double	xwin_width_pt  , 
	    double	xwin_height_pt  ) 
{
	flag_open = 1 ;

	ctx = this->CreateContext( 0, 0.0, 0.0, xwin_width_pt, xwin_height_pt , 1.0, 1.0 ); 

	if(False == this->Start( dpy , win ) ){	exit(1); }
}

	//----- 
void	PS2X::Close()
{ 
	flag_open = 0 ;
	this->End(); 
	this->DestroyContext() ; 
}


	//-----
static
void catch_sigchld( int catch_sig )
{
	catch_sig++ ; // dummy 
}

	//-----
static
Bool gs_event_handler(
	Display	*dpy,
	XEvent	*event,
	char	*arg )
{
		//-----
	gs_property *props = (gs_property *)arg ;

	if(event->type == ClientMessage	&&
	   event->xclient.window == props->parent &&
	   event->xclient.send_event == True )
	{
		if( event->xclient.message_type == props->GS_DONE ||
		    event->xclient.message_type == props->GS_PAGE )
		   	return True ;
	}

		//----- dummy
	Display* dummy ;  dummy = dpy ;

		//----- return status
	return False ;
}

#endif // USE_XWIN
