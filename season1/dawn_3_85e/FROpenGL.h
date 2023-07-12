/////////////////////////////
//////////  FROpenGL.h  /////
/////////////////////////////


#if !defined FR_OPENGL_H
#define FR_OPENGL_H

#include "dev_control.h"
#if defined USE_OPEN_GL


#include "box.h"
#include "vector.h"
#include "colortype.h"
#include "linetype.h"
#include "x_window.h"
#include "perspective.h"
#include "GLviewport.h"

#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>

class FROpenGL {
 private:
 	static FROpenGL* Current;

	int doubleBuffer;
	int flag_open;
	GLXContext mCtx;
	X_Window* xwin_p ;

	void MakeMeCurrent();
	Colormap getCmap( XVisualInfo *vi );
	void Wait();
 public:
	FROpenGL(){ doubleBuffer = 1; flag_open = 0; }
	~FROpenGL(){ if( isOpen() ) close(); }
	int	open ( X_Window* xwin_p, char* window_title );
	void	close();
	int	isOpen() const { return flag_open; }
	
	void DrawPolygon( const Vector3* v, size_t n, const ColorType& c );
	void DrawPolygon( const Vector3* v, size_t n, const LineType* lt );
	void DrawPolyline( const Vector3* v, size_t n, const LineType& lt );
	void DrawLine( const Vector3& v1, const Vector3& v2, const LineType& lt );

	void SetCamera( Perspective& camera, const Box3D& bb );
	void SetViewport( int x, int y, int w, int h );
	void Clear( const ColorType& col = ColorType( 0, 0, 0 ) );
	void DepthClear();
	void Flush();
}; // FROpenGL 

#endif // USE_OPEN_GL


//////////////// end of FROpenGL.h
#endif

