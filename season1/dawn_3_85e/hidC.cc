//////////////////////
//////  hidC.cc  /////
//////////////////////


#include <fstream.h>
#include <iomanip.h>
#include "toolio.h" 
#include "hid.h" 


// #define DEBUG_HIDC

	//----- constant
const int	DEFAULT_PRECISION       = 8 ;


int  Hid::writePolygonOut(	char*			output_file   ,
				char*			begin_facet   ,
				char			terminal_char ,
				const Parameter& 	parameter	) 				
{
		//----- local
	double	epsilon3d = parameter.epsilon3d ;
	int	status ;
	int	i ;
	char	vertex_file[] = "DDD_VERTEX_FILE_TMP_DDD" ; 
	char	facet_file [] = "DDD_FACET_FILE_TMP_DDD" ; 

//	char*	vertex_file = "DDD_VERTEX_FILE_TMP_DDD" ; 
//	char*	facet_file  = "DDD_FACET_FILE_TMP_DDD" ; 

	ofstream	vertex_out ( vertex_file ) ;	vertex_out.precision( DEFAULT_PRECISION ) ;
	ofstream	facet_out  ( facet_file  ) ;	facet_out.precision ( DEFAULT_PRECISION ) ;
	ofstream	output     ( output_file ) ;	output.precision    ( DEFAULT_PRECISION ) ;
	int		vertex_label ;
	Vector3		center(0.0, 0.0, 0.0 ) ;

		//----- err checking
	if( !vertex_out || !facet_out || !output ) {
		cerr << "WARNING (Hid::outputPolygon2Din3DFormat() ) \n";
		status = 0 ;
		return status ;
	}

		//----- calc center
	center.x = 0.5 * ( parameter.xmin + parameter.xmax ) ;
	center.y = 0.5 * ( parameter.ymin + parameter.ymax ) ;
	center.z = 0.5 * ( parameter.zmin + parameter.zmax ) ;

		//----- header
	vertex_out << "%dawn data format version-PO\n" << endl;

		//----- hint data
	vertex_out << "ObjectOrigin:\t0\t0\t0" << endl;
	vertex_out << "CameraTarget:\t" << parameter.target_x << "\t" << parameter.target_y << "\t" << parameter.target_z << endl;
	vertex_out << "CameraPosition:\t" << parameter.camera_distance << "\t" << parameter.camera_v_angle << "\t" << parameter.camera_h_angle << endl;
	vertex_out << "FocalDistance:\t" << parameter.focal_distance <<  endl;
	vertex_out << "MinPosition:\t"    << parameter.xmin << "\t" << parameter.ymin << "\t" << parameter.zmin << endl;
	vertex_out << "MaxPosition:\t"    << parameter.xmax << "\t" << parameter.ymax << "\t" << parameter.zmax << endl;
	vertex_out << endl;

		//----- other data
	vertex_out << "Tolerance3D:     " << parameter.epsilon3d << endl;
	vertex_out << "Tolerance2D:     " << parameter.epsilon2d << endl;
	vertex_out << endl;

		//----- beginning of vertex block
	vertex_out << "Vertex:"  << endl;

		//----- dummy vertex ( avoid that vertex_labe = 0 ) 
	vertex_out << "0 " << center.x << " " << center.y << " " << center.z << endl;

		//----- beginning of facet block
	facet_out << begin_facet << endl;

		//-----  output each polygon2D in 3D format
	vertex_label =  1 ; 	// initialization :
				//  modified in writingPlanData( polygon2d[i],...)
	for( i = 0 ; i < numPolygon2D ;  i++ ) {

		writePolygonOut(	polygon2d[i]  ,
				vertex_out    ,
				facet_out     ,
				vertex_label  ,			
				terminal_char ,
				epsilon3d      ) ;

	} // for
	vertex_out << endl; 

		//----- close streams
	vertex_out.close();
	facet_out.close();
	output.close();

		//----- make final output and remove temp files
#if defined DEBUG_HIDC
	ToolIO::convineFile( vertex_file, facet_file,  output_file, ToolIO::COPY );
#else
	ToolIO::convineFile( vertex_file, facet_file,  output_file, ToolIO::MOVE );
#endif

		//----- normal return
	status = 1 ;
	return status ;

} // Hid::writePolygonOut(), 1


	//----- Hid::writePolygonOut(), 2
int  Hid::writePolygonOut(	const  Polygon2D&	poly2d        ,
				ofstream&		vertex_out    ,
				ofstream&		facet_out    ,
				int&			vertex_label  ,			
				char			terminal_char ,
				double			epsilon3d      )
{
		//----- local
	int		i ;
	int		num_vertex      = poly2d.numVertex ;
	Vector3*	normal_vector_p = &(poly2d.parent3D_p->normalVector) ;
	Vector3*	P_on_p          = poly2d.parent3D_p->vertex[0] ;
	Vector3*	vertex_position = new Vector3 [ num_vertex ];
	int*		sgn_of_vertex_label = new int [ num_vertex ];
	Polygon2D	poly2d_tmp ; poly2d_tmp = poly2d ;

		//----- err check
	if( !vertex_position || !sgn_of_vertex_label ) {
		cerr << "ERROR: (Hid::writePolygonOut(), 1)\n";
		exit(1);
	}

		//----- 2D ---> 3D
	for( i = 0 ; i < num_vertex ; i++ ) {
		int	status_tmp = \
		this->point2DToPoint3D(	poly2d_tmp.vertex2D[i]  , 
					*normal_vector_p    ,
					*P_on_p             ,
					&vertex_position[i] ,
					epsilon3d            ) ;

		if( !status_tmp ) {
#if defined DEBUG_HIDC
			cerr << "WARNING: (Hid::writePolygonOut(), 2 )\n";
#endif
				//----- skip this vertex				
			int j, j_plus ;
			for( j = i ; j < (num_vertex - 2) ; j++ ) {
				j_plus = j+1 ; if( j_plus == num_vertex ) { j_plus = 0 ; }
				poly2d_tmp.edgeType[j] = poly2d_tmp.edgeType[j_plus ] ;
				poly2d_tmp.vertex2D[j] = poly2d_tmp.vertex2D[j_plus ] ;
			}

			i--; num_vertex--;
		} 

	} // for

		//----- output obtained 3-dim polygon
	if( num_vertex >=3 ) 
	{
			//----- initialize sign of vertex label to plus
		for( i = 0 ; i < num_vertex ; i++) {
			sgn_of_vertex_label[i] = 1 ;
		}

			//----- set invisible edge 
		for( i = 0 ; i < num_vertex ; i++) {
			if( poly2d_tmp.edgeType[i]->getVisibility() == 0 ) {
				int i_plus = i + 1 ;
				if( i_plus == num_vertex ) { i_plus = 0 ; }
				sgn_of_vertex_label[i_plus] = -1 ;
			}
		}

			//----- output
		for( i = 0 ; i < num_vertex ; i++) {

				//----- output to vertex block
			vertex_out << vertex_label  << " " ;
			vertex_out << (vertex_position[i].x) << " " ;
			vertex_out << (vertex_position[i].y) << " " ;
			vertex_out << (vertex_position[i].z) << endl;

				//----- output to facet block
			facet_out << (sgn_of_vertex_label[i] * vertex_label )  << " " ;

				//----- increment vertex_label if current vertex is a new one
			vertex_label++ ;

		} // i

			//----- end of one facet
		facet_out << terminal_char ;
		facet_out << "{ " << ( poly2d_tmp.color->getRed()   ) << " " ;
		facet_out         << ( poly2d_tmp.color->getGreen() ) << " " ;
		facet_out         << ( poly2d_tmp.color->getBlue () ) << " }" << endl;

	} // if( num_vertex > 0 )

		//----- delete dynamical memory
	delete  [] vertex_position ;
	delete  [] sgn_of_vertex_label ;
	
		//----- return status
	return num_vertex ;

} // Hid::writePolygonOut(), 2
