#include "FUMenu.h"

///////////////////////////////////////////////////
/////  generator --- page --- frame --- unit  /////
///////////////////////////////////////////////////

int main(void)
{
		//----- output stream (tcl script)
	ofstream fp("DAWN_GUI_menu_nogl.body");

		//----- GUI generator
		//.....  Title:  Fukui Renderer DAWN
		//.....  history file: .DAWN.history
		//.....  default file: .DAWN.default
	FUGenerator gen("Fukui Renderer DAWN (GUI ver.1NG) ",fp);
		gen.setDefFile(".DAWN_1");
		
		//------------------// 
		//----- page 1 -----//
		//------------------// 
	FUPage page1("Camera");

		//----- item 1-1 (camera distance; 1 entry)
	FUFrame frame1("  Camera Distance ( AUTO (very large) if non-positive )");
	FUEntry unit1; 
		unit1.setValue("0.0");
	frame1.append(&unit1);

		//----- item 1-2 (camera angle; 2 scales)
	FUFrame frame2("  Camera Angle");

	FUScale unit2("Polar");
		unit2.setValue(19.0) ;
		unit2.setMin(0.0) ;
		unit2.setMax(180.0) ;
		unit2.setStep(0.5) ;
	FUScale unit3("Azimuthal");
		unit3.setValue(71.0) ;
		unit3.setMin(0.0) ;
		unit3.setMax(360.0) ;
		unit3.setStep(0.5) ;

	frame2.append(&unit2);
	frame2.append(&unit3);

		//----- item 1-3 (auto targetting; 2 radio buttons)
		//.....  frame --- radio_button_base(unit4) --- Y/N-buttons
	FUFrame frame3("  Auto Targetting");

	FUBaseRadiobutton unit4;       // Base of radio buttons
		FURadiobutton Yes("Yes","1");  // "YES" botton to output "1"
		FURadiobutton No ("No", "0");  // "NO"  botton to output "0"
		unit4.append(&Yes);            // set "YES" bottun to button base
		unit4.append(&No);             // set "NO"  bottun to button base
		unit4.setValue("1");           // Default output value 
					       // (It must be "1" or "0")
	frame3.append(&unit4);                 // set button base to frame 3

		//----- item 1-4 (targetting point; 3 entries)
	FUFrame frame4("  Default Targetting Point");

	FUEntry unit5("X coord");          
		unit5.setValue("0.0");
	FUEntry unit6("Y coord");
		unit6.setValue("0.0");
	FUEntry unit7("Z coord");
		unit7.setValue("0.0");

	frame4.append(&unit5);
	frame4.append(&unit6);
	frame4.append(&unit7);

		//----- item 1-6 (Magnification; 1 entry)
	FUFrame frame6("  Magnification (1.0 is full size)");
	FUEntry unit9;
		unit9.setValue(0.9);
	frame6.append(&unit9);

		//----- item 4-5 (drawing mode; 5 radio buttons)
	FUFrame frame19("  Viewing Mode");

	FUBaseRadiobutton unit19;

		FURadiobutton Wireframe ("Wireframe","1");
		FURadiobutton WFStored  ("WF-Stored","2");
		FURadiobutton Lambert   ("Surface"  ,"3");
		FURadiobutton Line      ("Line"     ,"4");
		FURadiobutton Phong     ("Surface2" ,"5");

		unit19.append(&Wireframe);
		unit19.append(&WFStored);
		unit19.append(&Lambert);
		unit19.append(&Line);
		unit19.append(&Phong);

		unit19.setValue(1);  // default is "Wireframe"

	frame19.append(&unit19);


		//----- item 5-3 
		//----- item 1-5 (3D tolerance; 1 entry)
	FUFrame frame5("  3D Tolerance");
	FUEntry unit8;
		unit8.setValue(0.001);
	frame5.append(&unit8);

		//----- item 4-1 (display parameters; 2 radio buttons)
	FUFrame frame16("  Display Parameters and Axes");

	FUBaseRadiobutton unit16;         // Base of radio buttons
		unit16.append(&Yes);      // set "YES" bottun to button base
		unit16.append(&No) ;      // set "NO"  bottun to button base
		unit16.setValue("0");     // Default output value 
					  // (It must be "1" or "0")
	frame16.append(&unit16);          // set button base to frame 3

		//----- set frames (1--6) to page 1
	page1.append(&frame1);
	page1.append(&frame2);
	page1.append(&frame3);
	page1.append(&frame4);
	page1.append(&frame6);
	page1.append(&frame19);
	page1.append(&frame5);
	page1.append(&frame16);

		//----- set page 1 to generator
	gen.append(&page1);

		//------------------// 
		//----- page 2 -----//
		//------------------// 
	FUPage	page2("Light") ;

		//----- item 2-1 (Source Light; 1 Color)
	FUFrame frame7("  Source  Light  R G B");
	FUColor unit10;
		unit10.setMin(0.0);
		unit10.setMax(1.0);
		unit10.setRValue(1.0);
		unit10.setGValue(1.0);
		unit10.setBValue(1.0);
	frame7.append(&unit10);

		//----- item 2-2 (Ambient Light; 1 Color)
	FUFrame frame8("  Ambient Light  R G B");
	FUColor unit10B;
		unit10B.setMin(0.0);
		unit10B.setMax(1.0);
		unit10B.setRValue(0.5);
		unit10B.setGValue(0.5);
		unit10B.setBValue(0.5);
	frame8.append(&unit10B);

		//----- item 2-3 (Light Direction; 1 Color)
		//.....  Default light direction is set equal to
		//.....  camera direction.
	FUFrame frame9("  Light Direction");
	frame9.append(&unit2);  // polar
	frame9.append(&unit3);  // azimuthal

		//----- set frames (7--9) to page 2
	page2.append(&frame7);
	page2.append(&frame8);
	page2.append(&frame9);


		//----- set page 2 to generator
	gen.append(&page2);

		//------------------// 
		//----- page 3 -----//
		//------------------// 
	FUPage	 page3("Edge") ;

		//----- item 3-1 (Edge width; 3 entries)
	FUFrame frame10("  Edge Width  (mm, invisible if negative)");

	FUEntry unit12A("ordinary");          
		unit12A.setValue("0.1");
	FUEntry unit12B("outline");
		unit12B.setValue("0.1");
	FUEntry unit12C("auxiliary");
		unit12C.setValue("0.1");

	frame10.append(&unit12A);
	frame10.append(&unit12B);
	frame10.append(&unit12C);

		//----- item 3-2 (Style of aux edge; 3 radio buttons)
	FUFrame frame13("  Style of Auxiliary Edge");

	FUBaseRadiobutton unit13;
		FURadiobutton Solid    ("Solid"     , "1");
		FURadiobutton Dashed   ("Dashed"    , "2");
		FURadiobutton Invisivle("Invisivle" , "3");
		unit13.append(&Solid);
		unit13.append(&Dashed);
		unit13.append(&Invisivle);
		unit13.setValue(3);
	frame13.append(&unit13);

		//----- item 3-2 (Threshhold angle; 1 scale)
	FUFrame frame14("  Crease Angle for Auxiliary Edge");

	FUScale unit14;
		unit14.setValue(70.0);
		unit14.setMin(0.0) ;
		unit14.setMax(180.0) ;
		unit14.setStep(0.5) ;
	frame14.append(&unit14);


		//----- item 3-3 (Line width; 1 entry)
	FUFrame frame15("  Line Width  (mm, minimum 0.0)");
	FUEntry unit12D;          
		unit12D.setValue("0.1");
	frame15.append(&unit12D);

		//----- item 5-1 
	FUFrame frame51("  Haloing");

	FUBaseRadiobutton unit51;        // Base of radio buttons
		unit51.append(&Yes);      
		unit51.append(&No) ;    
		unit51.setValue("0");     // Default output value 
					  // (It must be "1" or "0")
	frame51.append(&unit51);    

		//----- item 5-2 
	FUFrame frame52("  Dashed Edges for Back Faces (WF_Stored mode)");

	FUBaseRadiobutton unit52;        // Base of radio buttons
		unit52.append(&Yes);      
		unit52.append(&No) ;    
		unit52.setValue("1");     // Default output value 
					  // (It must be "1" or "0")
	frame52.append(&unit52);    

		//----- set frames (1--6) to page 1
	page3.append(&frame10);
	page3.append(&frame13);
	page3.append(&frame14);
	page3.append(&frame15);
	page3.append(&frame51);
	page3.append(&frame52);

		//----- set page 3 to generator
	gen.append(&page3);

		//------------------// 
		//----- page 4 -----//
		//------------------// 
	FUPage	page4("Device");


		//----- item 4-3 (device; 5 radio buttons)
	FUFrame frame18("  Device Selection");
	FUBaseRadiobutton unit18;
		FURadiobutton EPS("EPS","1");
		FURadiobutton X("X","2");
		FURadiobutton EPSLS("EPS-LS","3");
		FURadiobutton XLS("X-LS","4");
//		FURadiobutton OpenGL("OpenGL","5");

		unit18.append(&EPS);
		unit18.append(&X);
		unit18.append(&EPSLS);
		unit18.append(&XLS);
//		unit18.append(&OpenGL);

		unit18.setValue(2); // Default is "X"

	frame18.append(&unit18);

		//----- item 4-4 
	FUFrame frame44("  PostScript Previewer (NONE to skip previewing)");
	FUEntry unit44;
		unit44.setValue("gv");
	frame44.append(&unit44);

		//----- item 5-5 
	FUFrame frame55("  Add showpage to EPS files");

	FUBaseRadiobutton unit55;        // Base of radio buttons
		unit55.append(&Yes);      
		unit55.append(&No) ;    
		unit55.setValue("0");     // Default output value 
					  // (It must be "1" or "0")
	frame55.append(&unit55);    


		//----- item 5-6 
	FUFrame frame56("  Append mode (with EPS or EPS-LS)");

	FUBaseRadiobutton unit56;        // Base of radio buttons
		unit56.append(&Yes);      
		unit56.append(&No ) ;    
		unit56.setValue("0");     // Default output value 
					  // (It must be "1" or "0")
	frame56.append(&unit56);    



		//----- set frames (16--19) to page 4
	page4.append(&frame18);
	page4.append(&frame44);
	page4.append(&frame55);
	page4.append(&frame56);

		//----- set page 4 to generator
	gen.append(&page4);


		//---- output 
	gen.output();

		//----- end
	return 0;
}
