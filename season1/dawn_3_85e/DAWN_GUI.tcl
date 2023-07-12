#### MULTPAGE GUI MENU ####

set bgcolor #d9d9d9
set fgcolor #000000
set MPage_Count_New 1
set MPage_ch 0
set MPage_Count_Old_Clip 0
set MPage_frame_List1 {1 2 4 5 8 9 10 11 }
set MPage_frame_List2 {12 15 18 }
set MPage_frame_List3 {20 23 24 25 26 27 }
set MPage_frame_List4 {28 29 30 31 }
set MPage_Name_List "{Camera} {Light} {Edge} {Device} "

#Button
frame   .top     -highlightthickness 0 -bg $bgcolor
frame   .top.1   -highlightthickness 0 -bg $bgcolor
frame   .top.2   -highlightthickness 0 -bg $bgcolor
button  .top.1.ok -text "OK" -bd 2 -command { MPageReg history } -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width 8
button  .top.1.cancel -text "Undo" -bd 2 -command MPageCancel -state disabled -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width 8
button  .top.1.def_s -text "Save Default" -bd 2 -command { MPageReg default } -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width 8
button  .top.1.def_l -text "Load Default" -bd 2 -command MPageGetDefault -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width 8
button  .top.1.exit -text "Exit" -bd 2 -command {MPageIniSave ; exit 1 } -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width 8
button  .top.1.help -text "Help" -bd 2 -command {MPageHelp help.txt} -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width 8
menubutton  .top.2.pageview -text "Page Jump" -fg #ff0000 -bg $bgcolor -relief raised -menu .top.2.pageview.menu -highlightthickness 0 -bd 2 -activeforeground #0000ff -width 10
button  .top.2.page  -textvariable MPage_Title_View -width 10 -highlightthickness 0 -fg $fgcolor -bg $bgcolor -state disabled -disabledforeground #000000 -bd 2
button  .top.2.prev -text "<<" -command MPagePrev -state disabled -bd 2 -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width 10
button  .top.2.next -text ">>" -command MPageNext -bd 2 -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width 10
menubutton  .top.2.clip -text "Page Clip" -fg #ff0000 -bg $bgcolor -relief raised -menu .top.2.clip.menu -highlightthickness 0 -bd 2 -activeforeground #0000ff -width 10
pack    .top.1.ok .top.1.cancel .top.1.def_s .top.1.def_l .top.1.exit .top.1.help -side left -padx 0 -pady 0 -fill both -expand 1
pack    .top.2.pageview .top.2.page .top.2.prev .top.2.next .top.2.clip -side left -padx 0 -pady 0 -fill both -expand 1
pack    .top.1 .top.2 -side top -fill both -expand 1
. configure -bg $bgcolor

#Title
wm title	. "Fukui Renderer DAWN (GUI ver.1DI) "
frame	.menu -bg $bgcolor
frame	.menu.main -bg $bgcolor
frame	.menu.clip -bg $bgcolor
frame	.menu.clip.space -height 10 -bg $bgcolor
pack	.menu.clip.space -side top -fill both -expand 1
pack	.menu.main -side top -fill both -expand 1
pack	.top .menu  -side top -padx 1m -pady 5 -fill both -expand 1

####Page1

###Frame1
frame   .menu.f1 -bg $bgcolor
label   .menu.f1.l -text "  Camera Distance ( AUTO (very large) if non-positive )" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f1.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Entry
frame   .menu.f1.f1 -bg $bgcolor
button  .menu.f1.b1 -text "" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 1 1" -fg $fgcolor -bg $bgcolor
entry   .menu.f1.e1 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent1 -fg $fgcolor -bg $bgcolor
pack    .menu.f1.b1 .menu.f1.e1 -in .menu.f1.f1 -side left -fill both -expand 1
pack    .menu.f1.f1 -in .menu.f1.right -side top -fill both -expand 1
set MPage_Ent1 0.0
pack    .menu.f1.l .menu.f1.right -in .menu.f1 -side left -fill both -expand 1

###Frame2
frame   .menu.f2 -bg $bgcolor
label   .menu.f2.l -text "  Camera Angle" -width 34 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f2.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Scale
frame   .menu.f2.f2 -relief raised -bd 2 -bg $bgcolor
label   .menu.f2.label2 -text "Polar" -width 9 -padx 2 -pady 0 -fg $fgcolor -bg $bgcolor
entry   .menu.f2.entry2 -width 5 -relief flat -bd 0 -textvariable scale2 -highlightthickness 0 -fg $fgcolor -bg $bgcolor
scale   .menu.f2.scale2 -length 222 -from 0 -to 180 -resolution 0.5 -orient horizontal -variable scale2 -highlightthickness 0 -showvalue false -fg $fgcolor -bg $bgcolor
.menu.f2.scale2 set $scale2
pack    .menu.f2.label2 .menu.f2.entry2 .menu.f2.scale2 -in .menu.f2.f2 -side left -fill both -expand 1
pack    .menu.f2.f2 -in .menu.f2.right -side top -fill both -expand 1
set     scale2 19.000000
##Unit2
#Scale
frame   .menu.f2.f3 -relief raised -bd 2 -bg $bgcolor
label   .menu.f2.label3 -text "Azimuthal" -width 9 -padx 2 -pady 0 -fg $fgcolor -bg $bgcolor
entry   .menu.f2.entry3 -width 5 -relief flat -bd 0 -textvariable scale3 -highlightthickness 0 -fg $fgcolor -bg $bgcolor
scale   .menu.f2.scale3 -length 222 -from 0 -to 360 -resolution 0.5 -orient horizontal -variable scale3 -highlightthickness 0 -showvalue false -fg $fgcolor -bg $bgcolor
.menu.f2.scale3 set $scale3
pack    .menu.f2.label3 .menu.f2.entry3 .menu.f2.scale3 -in .menu.f2.f3 -side left -fill both -expand 1
pack    .menu.f2.f3 -in .menu.f2.right -side top -fill both -expand 1
set     scale3 71.000000
pack    .menu.f2.l .menu.f2.right -in .menu.f2 -side left -fill both -expand 1

###Frame3
frame   .menu.f4 -bg $bgcolor
label   .menu.f4.l -text "  Auto Targetting" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f4.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Radiobutton
frame	.menu.f4.f4 -relief raised -bd 0 -bg $bgcolor
frame   .menu.f4.f4.frb1 -bg $bgcolor
radiobutton .menu.f4.f4.r1 -text "Yes" -value 1 -variable rb4 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f4.f4.r2 -text "No" -value 0 -variable rb4 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
pack	.menu.f4.f4.r1 .menu.f4.f4.r2 -in .menu.f4.f4.frb1 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f4.f4.frb1 -in .menu.f4.f4 -side top -fill both -expand 1
pack	.menu.f4.f4 -in .menu.f4.right -side top -fill both -expand 1
set	rb4 1
pack    .menu.f4.l .menu.f4.right -in .menu.f4 -side left -fill both -expand 1

###Frame4
frame   .menu.f5 -bg $bgcolor
label   .menu.f5.l -text "  Default Targetting Point" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f5.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Entry
frame   .menu.f5.f5 -bg $bgcolor
button  .menu.f5.b5 -text "X coord" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 5 5" -fg $fgcolor -bg $bgcolor
entry   .menu.f5.e5 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent5 -fg $fgcolor -bg $bgcolor
pack    .menu.f5.b5 .menu.f5.e5 -in .menu.f5.f5 -side left -fill both -expand 1
pack    .menu.f5.f5 -in .menu.f5.right -side top -fill both -expand 1
set MPage_Ent5 0.0
##Unit2
#Entry
frame   .menu.f5.f6 -bg $bgcolor
button  .menu.f5.b6 -text "Y coord" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 5 6" -fg $fgcolor -bg $bgcolor
entry   .menu.f5.e6 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent6 -fg $fgcolor -bg $bgcolor
pack    .menu.f5.b6 .menu.f5.e6 -in .menu.f5.f6 -side left -fill both -expand 1
pack    .menu.f5.f6 -in .menu.f5.right -side top -fill both -expand 1
set MPage_Ent6 0.0
##Unit3
#Entry
frame   .menu.f5.f7 -bg $bgcolor
button  .menu.f5.b7 -text "Z coord" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 5 7" -fg $fgcolor -bg $bgcolor
entry   .menu.f5.e7 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent7 -fg $fgcolor -bg $bgcolor
pack    .menu.f5.b7 .menu.f5.e7 -in .menu.f5.f7 -side left -fill both -expand 1
pack    .menu.f5.f7 -in .menu.f5.right -side top -fill both -expand 1
set MPage_Ent7 0.0
pack    .menu.f5.l .menu.f5.right -in .menu.f5 -side left -fill both -expand 1

###Frame5
frame   .menu.f8 -bg $bgcolor
label   .menu.f8.l -text "  Magnification (1.0 is full size)" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f8.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Entry
frame   .menu.f8.f8 -bg $bgcolor
button  .menu.f8.b8 -text "" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 8 8" -fg $fgcolor -bg $bgcolor
entry   .menu.f8.e8 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent8 -fg $fgcolor -bg $bgcolor
pack    .menu.f8.b8 .menu.f8.e8 -in .menu.f8.f8 -side left -fill both -expand 1
pack    .menu.f8.f8 -in .menu.f8.right -side top -fill both -expand 1
set MPage_Ent8 0.900000
pack    .menu.f8.l .menu.f8.right -in .menu.f8 -side left -fill both -expand 1

###Frame6
frame   .menu.f9 -bg $bgcolor
label   .menu.f9.l -text "  Viewing Mode" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f9.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Radiobutton
frame	.menu.f9.f9 -relief raised -bd 0 -bg $bgcolor
frame   .menu.f9.f9.frb1 -bg $bgcolor
radiobutton .menu.f9.f9.r1 -text "Wireframe" -value 1 -variable rb9 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f9.f9.r2 -text "WF-Stored" -value 2 -variable rb9 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
frame   .menu.f9.f9.frb3 -bg $bgcolor
radiobutton .menu.f9.f9.r3 -text "Surface" -value 3 -variable rb9 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f9.f9.r4 -text "Line" -value 4 -variable rb9 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
frame   .menu.f9.f9.frb5 -bg $bgcolor
radiobutton .menu.f9.f9.r5 -text "Surface2" -value 5 -variable rb9 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f9.f9.r6 -text " " -variable rb9 -width 9 -relief raised -bd 2 -state disabled -highlightthickness 0 -padx 0 -pady 0  -fg $fgcolor -bg $bgcolor
pack	.menu.f9.f9.r1 .menu.f9.f9.r2 -in .menu.f9.f9.frb1 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f9.f9.frb1 -in .menu.f9.f9 -side top -fill both -expand 1
pack	.menu.f9.f9.r3 .menu.f9.f9.r4 -in .menu.f9.f9.frb3 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f9.f9.frb3 -in .menu.f9.f9 -side top -fill both -expand 1
pack	.menu.f9.f9.r5 .menu.f9.f9.r6 -in .menu.f9.f9.frb5 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f9.f9.frb5 -in .menu.f9.f9 -side top -fill both -expand 1
pack	.menu.f9.f9 -in .menu.f9.right -side top -fill both -expand 1
set	rb9 1
pack    .menu.f9.l .menu.f9.right -in .menu.f9 -side left -fill both -expand 1

###Frame7
frame   .menu.f10 -bg $bgcolor
label   .menu.f10.l -text "  3D Tolerance" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f10.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Entry
frame   .menu.f10.f10 -bg $bgcolor
button  .menu.f10.b10 -text "" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 10 10" -fg $fgcolor -bg $bgcolor
entry   .menu.f10.e10 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent10 -fg $fgcolor -bg $bgcolor
pack    .menu.f10.b10 .menu.f10.e10 -in .menu.f10.f10 -side left -fill both -expand 1
pack    .menu.f10.f10 -in .menu.f10.right -side top -fill both -expand 1
set MPage_Ent10 0.001000
pack    .menu.f10.l .menu.f10.right -in .menu.f10 -side left -fill both -expand 1

###Frame8
frame   .menu.f11 -bg $bgcolor
label   .menu.f11.l -text "  Display Parameters and Axes" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f11.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Radiobutton
frame	.menu.f11.f11 -relief raised -bd 0 -bg $bgcolor
frame   .menu.f11.f11.frb1 -bg $bgcolor
radiobutton .menu.f11.f11.r1 -text "Yes" -value 1 -variable rb11 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f11.f11.r2 -text "No" -value 0 -variable rb11 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
pack	.menu.f11.f11.r1 .menu.f11.f11.r2 -in .menu.f11.f11.frb1 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f11.f11.frb1 -in .menu.f11.f11 -side top -fill both -expand 1
pack	.menu.f11.f11 -in .menu.f11.right -side top -fill both -expand 1
set	rb11 0
pack    .menu.f11.l .menu.f11.right -in .menu.f11 -side left -fill both -expand 1

####Page2

###Frame1
frame   .menu.f12 -bg $bgcolor
label   .menu.f12.l -text "  Source  Light  R G B" -width 34 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f12.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Color
frame   .menu.f12.f12 -relief raised -bd 2 -bg $bgcolor
frame   .menu.f12.fc12 -bg $bgcolor
frame   .menu.f12.color12 -width 45 -height 45 -relief sunken -bd 2 -bg $bgcolor
frame   .menu.f12.scale12 -bg $bgcolor
frame   .menu.f12.scale12.f1 -bg $bgcolor
label   .menu.f12.scale12.t1 -text " R " -bg $bgcolor -fg $fgcolor
entry   .menu.f12.entred12 -width 5 -relief flat -bd 0 -textvariable r12 -highlightthickness 0 -bg $bgcolor -fg $fgcolor
scale   .menu.f12.scale12.red -length 222 -from 0 -to 1 -orient horizontal -command "MPageSetColor 12 12" -highlightthickness 0 -resolution 0.01 -showvalue false -variable r12 -activebackground #ff0000 -troughcolor #cd0000 -bg $bgcolor -fg $fgcolor
pack    .menu.f12.scale12.t1 .menu.f12.entred12 .menu.f12.scale12.red -in .menu.f12.scale12.f1 -side left -fill both -expand 1
set     r12 1
frame   .menu.f12.scale12.f2 -bg $bgcolor
label   .menu.f12.scale12.t2 -text " G " -bg $bgcolor -fg $fgcolor
entry   .menu.f12.entgreen12 -width 5 -relief flat -bd 0 -textvariable g12 -highlightthickness 0 -bg $bgcolor -fg $fgcolor
scale   .menu.f12.scale12.green -length 222 -from 0 -to 1 -orient horizontal -command "MPageSetColor 12 12" -highlightthickness 0 -resolution 0.01 -showvalue false -variable g12 -activebackground #00ff00 -troughcolor #00cd00 -bg $bgcolor -fg $fgcolor
pack    .menu.f12.scale12.t2 .menu.f12.entgreen12 .menu.f12.scale12.green -in .menu.f12.scale12.f2 -side left -fill both -expand 1
set     g12 1
frame   .menu.f12.scale12.f3 -bg $bgcolor
label   .menu.f12.scale12.t3 -text " B " -bg $bgcolor -fg $fgcolor
entry   .menu.f12.entblue12 -width 5 -relief flat -bd 0 -textvariable b12 -highlightthickness 0 -bg $bgcolor -fg $fgcolor
scale   .menu.f12.scale12.blue -length 222 -from 0 -to 1 -orient horizontal -command "MPageSetColor 12 12" -highlightthickness 0 -resolution 0.01 -showvalue false -variable b12 -activebackground #0000ff -troughcolor #000080 -bg $bgcolor -fg $fgcolor
pack    .menu.f12.scale12.t3 .menu.f12.entblue12 .menu.f12.scale12.blue -in .menu.f12.scale12.f3 -side left -fill both -expand 1
set     b12 1
pack    .menu.f12.color12 -in .menu.f12.fc12 -side top -padx 5 -pady 10 -fill both -expand 1
pack    .menu.f12.scale12.f1 .menu.f12.scale12.f2 .menu.f12.scale12.f3 -in .menu.f12.scale12 -side top -fill both -ipadx 1 -expand 1
pack    .menu.f12.fc12 .menu.f12.scale12 -in .menu.f12.f12 -side left -fill both -expand 1 
pack    .menu.f12.f12 -in .menu.f12.right -side top -fill both -expand 1 
pack    .menu.f12.l .menu.f12.right -in .menu.f12 -side left -fill both -expand 1

###Frame2
frame   .menu.f15 -bg $bgcolor
label   .menu.f15.l -text "  Ambient Light  R G B" -width 34 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f15.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Color
frame   .menu.f15.f15 -relief raised -bd 2 -bg $bgcolor
frame   .menu.f15.fc15 -bg $bgcolor
frame   .menu.f15.color15 -width 45 -height 45 -relief sunken -bd 2 -bg $bgcolor
frame   .menu.f15.scale15 -bg $bgcolor
frame   .menu.f15.scale15.f1 -bg $bgcolor
label   .menu.f15.scale15.t1 -text " R " -bg $bgcolor -fg $fgcolor
entry   .menu.f15.entred15 -width 5 -relief flat -bd 0 -textvariable r15 -highlightthickness 0 -bg $bgcolor -fg $fgcolor
scale   .menu.f15.scale15.red -length 222 -from 0 -to 1 -orient horizontal -command "MPageSetColor 15 15" -highlightthickness 0 -resolution 0.01 -showvalue false -variable r15 -activebackground #ff0000 -troughcolor #cd0000 -bg $bgcolor -fg $fgcolor
pack    .menu.f15.scale15.t1 .menu.f15.entred15 .menu.f15.scale15.red -in .menu.f15.scale15.f1 -side left -fill both -expand 1
set     r15 0.5
frame   .menu.f15.scale15.f2 -bg $bgcolor
label   .menu.f15.scale15.t2 -text " G " -bg $bgcolor -fg $fgcolor
entry   .menu.f15.entgreen15 -width 5 -relief flat -bd 0 -textvariable g15 -highlightthickness 0 -bg $bgcolor -fg $fgcolor
scale   .menu.f15.scale15.green -length 222 -from 0 -to 1 -orient horizontal -command "MPageSetColor 15 15" -highlightthickness 0 -resolution 0.01 -showvalue false -variable g15 -activebackground #00ff00 -troughcolor #00cd00 -bg $bgcolor -fg $fgcolor
pack    .menu.f15.scale15.t2 .menu.f15.entgreen15 .menu.f15.scale15.green -in .menu.f15.scale15.f2 -side left -fill both -expand 1
set     g15 0.5
frame   .menu.f15.scale15.f3 -bg $bgcolor
label   .menu.f15.scale15.t3 -text " B " -bg $bgcolor -fg $fgcolor
entry   .menu.f15.entblue15 -width 5 -relief flat -bd 0 -textvariable b15 -highlightthickness 0 -bg $bgcolor -fg $fgcolor
scale   .menu.f15.scale15.blue -length 222 -from 0 -to 1 -orient horizontal -command "MPageSetColor 15 15" -highlightthickness 0 -resolution 0.01 -showvalue false -variable b15 -activebackground #0000ff -troughcolor #000080 -bg $bgcolor -fg $fgcolor
pack    .menu.f15.scale15.t3 .menu.f15.entblue15 .menu.f15.scale15.blue -in .menu.f15.scale15.f3 -side left -fill both -expand 1
set     b15 0.5
pack    .menu.f15.color15 -in .menu.f15.fc15 -side top -padx 5 -pady 10 -fill both -expand 1
pack    .menu.f15.scale15.f1 .menu.f15.scale15.f2 .menu.f15.scale15.f3 -in .menu.f15.scale15 -side top -fill both -ipadx 1 -expand 1
pack    .menu.f15.fc15 .menu.f15.scale15 -in .menu.f15.f15 -side left -fill both -expand 1 
pack    .menu.f15.f15 -in .menu.f15.right -side top -fill both -expand 1 
pack    .menu.f15.l .menu.f15.right -in .menu.f15 -side left -fill both -expand 1

###Frame3
frame   .menu.f18 -bg $bgcolor
label   .menu.f18.l -text "  Light Direction" -width 34 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f18.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Scale
frame   .menu.f18.f18 -relief raised -bd 2 -bg $bgcolor
label   .menu.f18.label18 -text "Polar" -width 9 -padx 2 -pady 0 -fg $fgcolor -bg $bgcolor
entry   .menu.f18.entry18 -width 5 -relief flat -bd 0 -textvariable scale18 -highlightthickness 0 -fg $fgcolor -bg $bgcolor
scale   .menu.f18.scale18 -length 222 -from 0 -to 180 -resolution 0.5 -orient horizontal -variable scale18 -highlightthickness 0 -showvalue false -fg $fgcolor -bg $bgcolor
.menu.f18.scale18 set $scale18
pack    .menu.f18.label18 .menu.f18.entry18 .menu.f18.scale18 -in .menu.f18.f18 -side left -fill both -expand 1
pack    .menu.f18.f18 -in .menu.f18.right -side top -fill both -expand 1
set     scale18 19.000000
##Unit2
#Scale
frame   .menu.f18.f19 -relief raised -bd 2 -bg $bgcolor
label   .menu.f18.label19 -text "Azimuthal" -width 9 -padx 2 -pady 0 -fg $fgcolor -bg $bgcolor
entry   .menu.f18.entry19 -width 5 -relief flat -bd 0 -textvariable scale19 -highlightthickness 0 -fg $fgcolor -bg $bgcolor
scale   .menu.f18.scale19 -length 222 -from 0 -to 360 -resolution 0.5 -orient horizontal -variable scale19 -highlightthickness 0 -showvalue false -fg $fgcolor -bg $bgcolor
.menu.f18.scale19 set $scale19
pack    .menu.f18.label19 .menu.f18.entry19 .menu.f18.scale19 -in .menu.f18.f19 -side left -fill both -expand 1
pack    .menu.f18.f19 -in .menu.f18.right -side top -fill both -expand 1
set     scale19 71.000000
pack    .menu.f18.l .menu.f18.right -in .menu.f18 -side left -fill both -expand 1

####Page3

###Frame1
frame   .menu.f20 -bg $bgcolor
label   .menu.f20.l -text "  Edge Width  (mm, invisible if negative)" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f20.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Entry
frame   .menu.f20.f20 -bg $bgcolor
button  .menu.f20.b20 -text "ordinary" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 20 20" -fg $fgcolor -bg $bgcolor
entry   .menu.f20.e20 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent20 -fg $fgcolor -bg $bgcolor
pack    .menu.f20.b20 .menu.f20.e20 -in .menu.f20.f20 -side left -fill both -expand 1
pack    .menu.f20.f20 -in .menu.f20.right -side top -fill both -expand 1
set MPage_Ent20 0.1
##Unit2
#Entry
frame   .menu.f20.f21 -bg $bgcolor
button  .menu.f20.b21 -text "outline" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 20 21" -fg $fgcolor -bg $bgcolor
entry   .menu.f20.e21 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent21 -fg $fgcolor -bg $bgcolor
pack    .menu.f20.b21 .menu.f20.e21 -in .menu.f20.f21 -side left -fill both -expand 1
pack    .menu.f20.f21 -in .menu.f20.right -side top -fill both -expand 1
set MPage_Ent21 0.1
##Unit3
#Entry
frame   .menu.f20.f22 -bg $bgcolor
button  .menu.f20.b22 -text "auxiliary" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 20 22" -fg $fgcolor -bg $bgcolor
entry   .menu.f20.e22 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent22 -fg $fgcolor -bg $bgcolor
pack    .menu.f20.b22 .menu.f20.e22 -in .menu.f20.f22 -side left -fill both -expand 1
pack    .menu.f20.f22 -in .menu.f20.right -side top -fill both -expand 1
set MPage_Ent22 0.1
pack    .menu.f20.l .menu.f20.right -in .menu.f20 -side left -fill both -expand 1

###Frame2
frame   .menu.f23 -bg $bgcolor
label   .menu.f23.l -text "  Style of Auxiliary Edge" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f23.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Radiobutton
frame	.menu.f23.f23 -relief raised -bd 0 -bg $bgcolor
frame   .menu.f23.f23.frb1 -bg $bgcolor
radiobutton .menu.f23.f23.r1 -text "Solid" -value 1 -variable rb23 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f23.f23.r2 -text "Dashed" -value 2 -variable rb23 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
frame   .menu.f23.f23.frb3 -bg $bgcolor
radiobutton .menu.f23.f23.r3 -text "Invisivle" -value 3 -variable rb23 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f23.f23.r4 -text " " -variable rb23 -width 9 -relief raised -bd 2 -state disabled -highlightthickness 0 -padx 0 -pady 0  -fg $fgcolor -bg $bgcolor
pack	.menu.f23.f23.r1 .menu.f23.f23.r2 -in .menu.f23.f23.frb1 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f23.f23.frb1 -in .menu.f23.f23 -side top -fill both -expand 1
pack	.menu.f23.f23.r3 .menu.f23.f23.r4 -in .menu.f23.f23.frb3 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f23.f23.frb3 -in .menu.f23.f23 -side top -fill both -expand 1
pack	.menu.f23.f23 -in .menu.f23.right -side top -fill both -expand 1
set	rb23 3
pack    .menu.f23.l .menu.f23.right -in .menu.f23 -side left -fill both -expand 1

###Frame3
frame   .menu.f24 -bg $bgcolor
label   .menu.f24.l -text "  Crease Angle for Auxiliary Edge" -width 34 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f24.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Scale
frame   .menu.f24.f24 -relief raised -bd 2 -bg $bgcolor
label   .menu.f24.label24 -text "" -width 9 -padx 2 -pady 0 -fg $fgcolor -bg $bgcolor
entry   .menu.f24.entry24 -width 5 -relief flat -bd 0 -textvariable scale24 -highlightthickness 0 -fg $fgcolor -bg $bgcolor
scale   .menu.f24.scale24 -length 222 -from 0 -to 180 -resolution 0.5 -orient horizontal -variable scale24 -highlightthickness 0 -showvalue false -fg $fgcolor -bg $bgcolor
.menu.f24.scale24 set $scale24
pack    .menu.f24.label24 .menu.f24.entry24 .menu.f24.scale24 -in .menu.f24.f24 -side left -fill both -expand 1
pack    .menu.f24.f24 -in .menu.f24.right -side top -fill both -expand 1
set     scale24 70.000000
pack    .menu.f24.l .menu.f24.right -in .menu.f24 -side left -fill both -expand 1

###Frame4
frame   .menu.f25 -bg $bgcolor
label   .menu.f25.l -text "  Line Width  (mm, minimum 0.0)" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f25.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Entry
frame   .menu.f25.f25 -bg $bgcolor
button  .menu.f25.b25 -text "" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 25 25" -fg $fgcolor -bg $bgcolor
entry   .menu.f25.e25 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent25 -fg $fgcolor -bg $bgcolor
pack    .menu.f25.b25 .menu.f25.e25 -in .menu.f25.f25 -side left -fill both -expand 1
pack    .menu.f25.f25 -in .menu.f25.right -side top -fill both -expand 1
set MPage_Ent25 0.1
pack    .menu.f25.l .menu.f25.right -in .menu.f25 -side left -fill both -expand 1

###Frame5
frame   .menu.f26 -bg $bgcolor
label   .menu.f26.l -text "  Haloing" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f26.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Radiobutton
frame	.menu.f26.f26 -relief raised -bd 0 -bg $bgcolor
frame   .menu.f26.f26.frb1 -bg $bgcolor
radiobutton .menu.f26.f26.r1 -text "Yes" -value 1 -variable rb26 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f26.f26.r2 -text "No" -value 0 -variable rb26 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
pack	.menu.f26.f26.r1 .menu.f26.f26.r2 -in .menu.f26.f26.frb1 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f26.f26.frb1 -in .menu.f26.f26 -side top -fill both -expand 1
pack	.menu.f26.f26 -in .menu.f26.right -side top -fill both -expand 1
set	rb26 0
pack    .menu.f26.l .menu.f26.right -in .menu.f26 -side left -fill both -expand 1

###Frame6
frame   .menu.f27 -bg $bgcolor
label   .menu.f27.l -text "  Dashed Edges for Back Faces (WF_Stored mode)" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f27.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Radiobutton
frame	.menu.f27.f27 -relief raised -bd 0 -bg $bgcolor
frame   .menu.f27.f27.frb1 -bg $bgcolor
radiobutton .menu.f27.f27.r1 -text "Yes" -value 1 -variable rb27 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f27.f27.r2 -text "No" -value 0 -variable rb27 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
pack	.menu.f27.f27.r1 .menu.f27.f27.r2 -in .menu.f27.f27.frb1 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f27.f27.frb1 -in .menu.f27.f27 -side top -fill both -expand 1
pack	.menu.f27.f27 -in .menu.f27.right -side top -fill both -expand 1
set	rb27 1
pack    .menu.f27.l .menu.f27.right -in .menu.f27 -side left -fill both -expand 1

####Page4

###Frame1
frame   .menu.f28 -bg $bgcolor
label   .menu.f28.l -text "  Device Selection" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f28.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Radiobutton
frame	.menu.f28.f28 -relief raised -bd 0 -bg $bgcolor
frame   .menu.f28.f28.frb1 -bg $bgcolor
radiobutton .menu.f28.f28.r1 -text "EPS" -value 1 -variable rb28 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f28.f28.r2 -text "EPS-LS" -value 3 -variable rb28 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
pack	.menu.f28.f28.r1 .menu.f28.f28.r2 -in .menu.f28.f28.frb1 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f28.f28.frb1 -in .menu.f28.f28 -side top -fill both -expand 1
pack	.menu.f28.f28 -in .menu.f28.right -side top -fill both -expand 1
set	rb28 1
pack    .menu.f28.l .menu.f28.right -in .menu.f28 -side left -fill both -expand 1

###Frame2
frame   .menu.f29 -bg $bgcolor
label   .menu.f29.l -text "  PostScript Previewer (NONE to skip previewing)" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f29.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Entry
frame   .menu.f29.f29 -bg $bgcolor
button  .menu.f29.b29 -text "" -width 9 -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command "MPageSelectEntry 29 29" -fg $fgcolor -bg $bgcolor
entry   .menu.f29.e29 -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent29 -fg $fgcolor -bg $bgcolor
pack    .menu.f29.b29 .menu.f29.e29 -in .menu.f29.f29 -side left -fill both -expand 1
pack    .menu.f29.f29 -in .menu.f29.right -side top -fill both -expand 1
set MPage_Ent29 gsview32
pack    .menu.f29.l .menu.f29.right -in .menu.f29 -side left -fill both -expand 1

###Frame3
frame   .menu.f30 -bg $bgcolor
label   .menu.f30.l -text "  Add showpage to EPS files" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f30.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Radiobutton
frame	.menu.f30.f30 -relief raised -bd 0 -bg $bgcolor
frame   .menu.f30.f30.frb1 -bg $bgcolor
radiobutton .menu.f30.f30.r1 -text "Yes" -value 1 -variable rb30 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f30.f30.r2 -text "No" -value 0 -variable rb30 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
pack	.menu.f30.f30.r1 .menu.f30.f30.r2 -in .menu.f30.f30.frb1 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f30.f30.frb1 -in .menu.f30.f30 -side top -fill both -expand 1
pack	.menu.f30.f30 -in .menu.f30.right -side top -fill both -expand 1
set	rb30 0
pack    .menu.f30.l .menu.f30.right -in .menu.f30 -side left -fill both -expand 1

###Frame4
frame   .menu.f31 -bg $bgcolor
label   .menu.f31.l -text "  Append mode (with EPS or EPS-LS)" -width 41 -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor
frame   .menu.f31.right -bd 0 -bg $bgcolor -highlightthickness 0
##Unit1
#Radiobutton
frame	.menu.f31.f31 -relief raised -bd 0 -bg $bgcolor
frame   .menu.f31.f31.frb1 -bg $bgcolor
radiobutton .menu.f31.f31.r1 -text "Yes" -value 1 -variable rb31 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
radiobutton .menu.f31.f31.r2 -text "No" -value 0 -variable rb31 -width 9 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor
pack	.menu.f31.f31.r1 .menu.f31.f31.r2 -in .menu.f31.f31.frb1 -side left -ipadx 2 -ipady 1 -fill both -expand 1
pack	.menu.f31.f31.frb1 -in .menu.f31.f31 -side top -fill both -expand 1
pack	.menu.f31.f31 -in .menu.f31.right -side top -fill both -expand 1
set	rb31 0
pack    .menu.f31.l .menu.f31.right -in .menu.f31 -side left -fill both -expand 1

#menubutton
menu .top.2.pageview.menu -tearoff 0 -bg $bgcolor
	set MPage_Last 0
	foreach p $MPage_Name_List {
		incr MPage_Last
		.top.2.pageview.menu add radiobutton -label "$MPage_Last $p" -indicatoron 0	-variable MPage_Count_W -value $MPage_Last -command {MPageJump $MPage_Count_W}
	}
set rbc n
menu .top.2.clip.menu -tearoff 0 -bg $bgcolor
	.top.2.clip.menu add radiobutton -label "     ON     " -value y -variable rbc -command { MPageClip }
	.top.2.clip.menu add radiobutton -label "     OFF    " -value n -variable rbc -command { MPageClip }
	.top.2.clip.menu entryconfigure 1 -state disabled
	if { $MPage_Last <= 1 } {
		.top.2.prev configure -state disabled
		.top.2.next configure -state disabled
		.top.2.pageview configure -state disabled -disabledforeground #000000
	}

###Procedures

proc MPageSelectEntry { fno eno } {
	.menu.f${fno}.e${eno} delete 0 end
	focus .menu.f${fno}.e${eno}
}

proc MPageSetColor {fnumber unumber dummy} {
	switch $unumber {
		12	{
			global r12 g12 b12
			if { [IDCHECK $r12] || [IDCHECK $g12] || [IDCHECK $b12] } {
				MPageErrorOut "12"
				return
			}
				set red [ expr 1 * $r12 * 255 ]
				set green [ expr 1 * $g12 * 255 ]
				set blue [ expr 1 * $b12 * 255 ] 
			}
		15	{
			global r15 g15 b15
			if { [IDCHECK $r15] || [IDCHECK $g15] || [IDCHECK $b15] } {
				MPageErrorOut "15"
				return
			}
				set red [ expr 1 * $r15 * 255 ]
				set green [ expr 1 * $g15 * 255 ]
				set blue [ expr 1 * $b15 * 255 ] 
			}
		default {puts "error-->proc MPageSetColor"; exit"}
	}
        regexp {^[0-9]+} $red red 
        regexp {^[0-9]+} $green green 
        regexp {^[0-9]+} $blue blue
        .menu.f$fnumber.color$unumber config -bg [ format #%02x%02x%02x $red $green $blue ] 
}

proc MPageCancel {} {
	set MPage_History ""
	set n 1
	set fd [ open .DAWN_1.history r ]
	while { ! [ eof $fd ] } {
		set line [ gets $fd ]
		set MPage_History [ concat $MPage_History $line ]
	}
	MPageOutput $MPage_History
		MPageSetColor 12 12 0
		MPageSetColor 15 15 0
	close $fd
	MPageErrorCheck
	if { ! [ file size .DAWN_1.history ] == 0 } {
		.top.1.cancel configure -state normal 
	}
}

proc MPageReg { object } {
	global MPage_Ent1
	global scale2
	global scale3
	global rb4
	global MPage_Ent5
	global MPage_Ent6
	global MPage_Ent7
	global MPage_Ent8
	global rb9
	global MPage_Ent10
	global rb11
	global r12 g12 b12
	global r15 g15 b15
	global scale18
	global scale19
	global MPage_Ent20
	global MPage_Ent21
	global MPage_Ent22
	global rb23
	global scale24
	global MPage_Ent25
	global rb26
	global rb27
	global rb28
	global MPage_Ent29
	global rb30
	global rb31
	if { [MPageErrorCheck] } {
		return
	}
	set new_data ""
	set n 1 
	while { $n < 32 } {
		switch $n { 
			1	{
				if { $MPage_Ent1 != "" } {
					set new_data [ concat $new_data $MPage_Ent1]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			2	{set new_data [ concat $new_data $scale2 ]}
			3	{set new_data [ concat $new_data $scale3 ]}
			4	{set new_data [ concat $new_data $rb4 ]}
			5	{
				if { $MPage_Ent5 != "" } {
					set new_data [ concat $new_data $MPage_Ent5]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			6	{
				if { $MPage_Ent6 != "" } {
					set new_data [ concat $new_data $MPage_Ent6]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			7	{
				if { $MPage_Ent7 != "" } {
					set new_data [ concat $new_data $MPage_Ent7]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			8	{
				if { $MPage_Ent8 != "" } {
					set new_data [ concat $new_data $MPage_Ent8]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			9	{set new_data [ concat $new_data $rb9 ]}
			10	{
				if { $MPage_Ent10 != "" } {
					set new_data [ concat $new_data $MPage_Ent10]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			11	{set new_data [ concat $new_data $rb11 ]}
			12	{set new_data [ concat $new_data $r12 ]}
			13	{set new_data [ concat $new_data $g12 ]}
			14	{set new_data [ concat $new_data $b12 ]}
			15	{set new_data [ concat $new_data $r15 ]}
			16	{set new_data [ concat $new_data $g15 ]}
			17	{set new_data [ concat $new_data $b15 ]}
			18	{set new_data [ concat $new_data $scale18 ]}
			19	{set new_data [ concat $new_data $scale19 ]}
			20	{
				if { $MPage_Ent20 != "" } {
					set new_data [ concat $new_data $MPage_Ent20]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			21	{
				if { $MPage_Ent21 != "" } {
					set new_data [ concat $new_data $MPage_Ent21]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			22	{
				if { $MPage_Ent22 != "" } {
					set new_data [ concat $new_data $MPage_Ent22]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			23	{set new_data [ concat $new_data $rb23 ]}
			24	{set new_data [ concat $new_data $scale24 ]}
			25	{
				if { $MPage_Ent25 != "" } {
					set new_data [ concat $new_data $MPage_Ent25]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			26	{set new_data [ concat $new_data $rb26 ]}
			27	{set new_data [ concat $new_data $rb27 ]}
			28	{set new_data [ concat $new_data $rb28 ]}
			29	{
				if { $MPage_Ent29 != "" } {
					set new_data [ concat $new_data $MPage_Ent29]
				} else {
					set new_data [ concat $new_data 0 ]
				}
			}
			30	{set new_data [ concat $new_data $rb30 ]}
			31	{set new_data [ concat $new_data $rb31 ]}
			default {puts "error-->proc MPageReg"; exit}
		}
		incr n
	}
	set fd [ open .DAWN_1.${object} w ]
	foreach i $new_data {
		puts $fd $i
	}
	close $fd
	MPageIniSave
	if { [ regexp history ${object} ] } {
		exit 
	} else { 
		.top.1.def_l configure -state normal
	}
}

proc MPageOutput { data } {
	global MPage_Ent1
	global scale2
	global scale3
	global rb4
	global MPage_Ent5
	global MPage_Ent6
	global MPage_Ent7
	global MPage_Ent8
	global rb9
	global MPage_Ent10
	global rb11
	global r12 g12 b12
	global r15 g15 b15
	global scale18
	global scale19
	global MPage_Ent20
	global MPage_Ent21
	global MPage_Ent22
	global rb23
	global scale24
	global MPage_Ent25
	global rb26
	global rb27
	global rb28
	global MPage_Ent29
	global rb30
	global rb31
	set n 1
	foreach i $data {
		switch $n {
			1	{set MPage_Ent1 $i}
			2	{set scale2 $i}
			3	{set scale3 $i}
			4	{set rb4 $i}
			5	{set MPage_Ent5 $i}
			6	{set MPage_Ent6 $i}
			7	{set MPage_Ent7 $i}
			8	{set MPage_Ent8 $i}
			9	{set rb9 $i}
			10	{set MPage_Ent10 $i}
			11	{set rb11 $i}
			12	{set r12 $i}
			13	{set g12 $i}
			14	{set b12 $i}
			15	{set r15 $i}
			16	{set g15 $i}
			17	{set b15 $i}
			18	{set scale18 $i}
			19	{set scale19 $i}
			20	{set MPage_Ent20 $i}
			21	{set MPage_Ent21 $i}
			22	{set MPage_Ent22 $i}
			23	{set rb23 $i}
			24	{set scale24 $i}
			25	{set MPage_Ent25 $i}
			26	{set rb26 $i}
			27	{set rb27 $i}
			28	{set rb28 $i}
			29	{set MPage_Ent29 $i}
			30	{set rb30 $i}
			31	{set rb31 $i}
			32 { break }
			default {puts "error-->proc MPageOutput"; exit}
		}
		incr n
	}
}

proc MPageGetDefault {} {
	set def ""
	set n 1
	set cklist [ glob -nocomplain .DAWN_1.default ]
	if { [ lsearch -exact $cklist .DAWN_1.default ] != -1 } {
		set fd [ open .DAWN_1.default r ]
		while { ! [ eof $fd ] } {
			set line [ gets $fd ]
			set def [ concat $def $line ]
		}
		MPageOutput $def
		MPageSetColor 12 12 0
		MPageSetColor 15 15 0
		close $fd
		MPageErrorCheck
	} else {
		set in_fd [ open .DAWN_1.history r ]
		set out_fd [ open .DAWN_1.default w ]
		puts $out_fd [ read $in_fd ]
		close $in_fd
		close $out_fd
		MPageCancel
	}
}

proc MPagePrev {} {
	global MPage_Count_New MPage_Count_Old_Clip
	set MPage_Count_W $MPage_Count_New
	incr MPage_Count_W -1
	if { $MPage_Count_Old_Clip == $MPage_Count_W } {
		incr MPage_Count_W -1
	}
	MPageJump $MPage_Count_W
}

proc MPageNext {} {
	global MPage_Count_New MPage_Count_Old_Clip
	set MPage_Count_W $MPage_Count_New
	incr MPage_Count_W
	if { $MPage_Count_Old_Clip == $MPage_Count_W } {
		incr MPage_Count_W
	}
	MPageJump $MPage_Count_W
}

proc MPageJump { JPage } {
	global MPage_Count_New MPage_Count_Old MPage_Count_Old_Clip
	global MPage_Title_View MPage_Name_List MPage_Last
	global MPage_frame_List1
	global MPage_frame_List2
	global MPage_frame_List3
	global MPage_frame_List4
	set MPage_Count_Old $MPage_Count_New
	set MPage_Count_New $JPage
	.top.2.prev configure -state normal
	.top.2.next configure -state normal
	if { $MPage_Count_New <= 1 } {
		.top.2.prev configure -state disabled
	} elseif { $MPage_Count_New >= $MPage_Last } {
		.top.2.next configure -state disabled
	}
	if { $MPage_Count_Old_Clip == 1 && $MPage_Count_New == 2} {
		.top.2.prev configure -state disabled
	}
	if { $MPage_Count_Old_Clip == $MPage_Last && $MPage_Count_New == [expr $MPage_Last - 1]} {
		.top.2.next configure -state disabled
	}
	set n [expr $MPage_Count_New - 1]
	if { [lindex $MPage_Name_List $n] == "NoPageName" } {
		set MPage_Title_View page$MPage_Count_New/$MPage_Last
	} else { 
		set MPage_Title_View "page$MPage_Count_New/$MPage_Last [lindex $MPage_Name_List $n]"
	}
	switch $MPage_Count_Old {
		1	{  foreach fr $MPage_frame_List1 { pack forget .menu.f$fr } }
		2	{  foreach fr $MPage_frame_List2 { pack forget .menu.f$fr } }
		3	{  foreach fr $MPage_frame_List3 { pack forget .menu.f$fr } }
		4	{  foreach fr $MPage_frame_List4 { pack forget .menu.f$fr } }
		default {puts "error-->proc MPageJump(1)"; exit }
	}
	switch $MPage_Count_New {
		1	{  foreach fr $MPage_frame_List1 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.main} }
		2	{  foreach fr $MPage_frame_List2 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.main} }
		3	{  foreach fr $MPage_frame_List3 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.main} }
		4	{  foreach fr $MPage_frame_List4 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.main} }
		default {puts "error-->proc MPageJump(2)"; exit }
	}
} 

proc MPageClip {} {
	global MPage_Count_New MPage_Count_Old MPage_Count_Old_Clip rbc
	global MPage_Title_View MPage_Name_List MPage_Last MPage_Name_title
	global MPage_frame_List1
	global MPage_frame_List2
	global MPage_frame_List3
	global MPage_frame_List4
	if { $rbc == "y" } {
		if { $MPage_Count_Old_Clip != 0 } {
			switch $MPage_Count_Old_Clip {
				1	{  foreach fr $MPage_frame_List1 { pack forget .menu.f$fr } }
				2	{  foreach fr $MPage_frame_List2 { pack forget .menu.f$fr } }
				3	{  foreach fr $MPage_frame_List3 { pack forget .menu.f$fr } }
				4	{  foreach fr $MPage_frame_List4 { pack forget .menu.f$fr } }
				default {puts "error-->proc MPageClip(1)"; exit }
			}
			.top.2.pageview.menu entryconfigure [expr $MPage_Count_Old_Clip - 1] -state normal
		}
		set MPage_Count_Old_Clip $MPage_Count_New
		switch $MPage_Count_New {
			1	{  foreach fr $MPage_frame_List1 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.clip} }
			2	{  foreach fr $MPage_frame_List2 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.clip} }
			3	{  foreach fr $MPage_frame_List3 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.clip} }
			4	{  foreach fr $MPage_frame_List4 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.clip} }
			default {puts "error-->proc MPageClip(2)"; exit }
		}
		if { $MPage_Count_New == $MPage_Last } {
			incr MPage_Count_New -1
		} else {
			incr MPage_Count_New
		}
		set n [expr $MPage_Count_New - 1]
			if { [lindex $MPage_Name_List $n] == "NoPageName" } {
			set MPage_Title_View page$MPage_Count_New/$MPage_Last
		} else {
			set MPage_Title_View "page$MPage_Count_New/$MPage_Last [lindex $MPage_Name_List $n]"
		}
		switch $MPage_Count_New {
			1	{  foreach fr $MPage_frame_List1 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.main} }
			2	{  foreach fr $MPage_frame_List2 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.main} }
			3	{  foreach fr $MPage_frame_List3 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.main} }
			4	{  foreach fr $MPage_frame_List4 { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.main} }
			default {puts "error-->proc MPageClip(3)"; exit }
		}
		pack .menu.clip -side top -fill both -expand 1
		.top.2.clip.menu entryconfigure 1 -state normal
		.top.2.prev configure -state normal
		.top.2.next configure -state normal
		if { $MPage_Count_New <= 1 } {
			.top.2.prev configure -state disabled
		} elseif { $MPage_Count_New >= $MPage_Last } {
			.top.2.next configure -state disabled
		}
		if { $MPage_Count_Old_Clip == 1 && $MPage_Count_New == 2} {
			.top.2.prev configure -state disabled
		}
		if { $MPage_Count_Old_Clip == $MPage_Last && $MPage_Count_New == [expr $MPage_Last - 1]} {
			.top.2.next configure -state disabled
		}
		.top.2.pageview.menu entryconfigure [expr $MPage_Count_Old_Clip - 1] -state disabled
	} else {
		switch $MPage_Count_Old_Clip {
			1	{  foreach fr $MPage_frame_List1 { pack forget .menu.f$fr } }
			2	{  foreach fr $MPage_frame_List2 { pack forget .menu.f$fr } }
			3	{  foreach fr $MPage_frame_List3 { pack forget .menu.f$fr } }
			4	{  foreach fr $MPage_frame_List4 { pack forget .menu.f$fr } }
			default {puts "error-->proc MPageClip(4)"; exit }
		}
		pack forget .menu.clip
		if { $MPage_Count_Old_Clip == 1 || $MPage_Count_Old_Clip == $MPage_Last} {
			.top.2.prev configure -state normal
			.top.2.next configure -state normal
		}
		.top.2.pageview.menu entryconfigure [expr $MPage_Count_Old_Clip - 1] -state normal
		set MPage_Count_Old_Clip 0
		.top.2.clip.menu entryconfigure 1 -state disabled
	}
}

proc MPageErrorOut ErrorNo {
	global MPage_frame_List1
	global MPage_frame_List2
	global MPage_frame_List3
	global MPage_frame_List4
	set Errorlist ""
	foreach Errno $ErrorNo {
		set nb 0
		foreach fr $MPage_frame_List1 {
			incr nb 
			if { $Errno == $fr } {
				set work "page1 ERROR  item$nb\n"
				set Errorlist $Errorlist$work
			}
		}
		set nb 0
		foreach fr $MPage_frame_List2 {
			incr nb 
			if { $Errno == $fr } {
				set work "page2 ERROR  item$nb\n"
				set Errorlist $Errorlist$work
			}
		}
		set nb 0
		foreach fr $MPage_frame_List3 {
			incr nb 
			if { $Errno == $fr } {
				set work "page3 ERROR  item$nb\n"
				set Errorlist $Errorlist$work
			}
		}
		set nb 0
		foreach fr $MPage_frame_List4 {
			incr nb 
			if { $Errno == $fr } {
				set work "page4 ERROR  item$nb\n"
				set Errorlist $Errorlist$work
			}
		}
	}
	MPageDialog {ERROR} $Errorlist
}

proc MPageErrorCheck {} {
	global MPage_Ent1
	global scale2
	global scale3
	global rb4
	global MPage_Ent5
	global MPage_Ent6
	global MPage_Ent7
	global MPage_Ent8
	global rb9
	global MPage_Ent10
	global rb11
	global r12 g12 b12
	global r15 g15 b15
	global scale18
	global scale19
	global MPage_Ent20
	global MPage_Ent21
	global MPage_Ent22
	global rb23
	global scale24
	global MPage_Ent25
	global rb26
	global rb27
	global rb28
	global MPage_Ent29
	global rb30
	global rb31
	set ErrorNo ""
	set n 1
	while { $n < 32 } {
		switch $n {
			1	{
					if { [IDCHECK $MPage_Ent1] } {
						set ErrorNo [concat $ErrorNo "1"]
					}
				}
			2	{
					if { [IDCHECK $scale2] } {
						set ErrorNo [concat $ErrorNo "2"]
					}
				}
			3	{
					if { [IDCHECK $scale3] } {
						set ErrorNo [concat $ErrorNo "2"]
					}
				}
			5	{
					if { [IDCHECK $MPage_Ent5] } {
						set ErrorNo [concat $ErrorNo "5"]
					}
				}
			6	{
					if { [IDCHECK $MPage_Ent6] } {
						set ErrorNo [concat $ErrorNo "5"]
					}
				}
			7	{
					if { [IDCHECK $MPage_Ent7] } {
						set ErrorNo [concat $ErrorNo "5"]
					}
				}
			8	{
					if { [IDCHECK $MPage_Ent8] } {
						set ErrorNo [concat $ErrorNo "8"]
					}
				}
			10	{
					if { [IDCHECK $MPage_Ent10] } {
						set ErrorNo [concat $ErrorNo "10"]
					}
				}
			12	{
					if { [IDCHECK $r12] } {
						set ErrorNo [concat $ErrorNo "12"]
					}
				}
			13	{
					if { [IDCHECK $g12] } {
						set ErrorNo [concat $ErrorNo "12"]
					}
				}
			14	{
					if { [IDCHECK $b12] } {
						set ErrorNo [concat $ErrorNo "12"]
					}
				}
			15	{
					if { [IDCHECK $r15] } {
						set ErrorNo [concat $ErrorNo "15"]
					}
				}
			16	{
					if { [IDCHECK $g15] } {
						set ErrorNo [concat $ErrorNo "15"]
					}
				}
			17	{
					if { [IDCHECK $b15] } {
						set ErrorNo [concat $ErrorNo "15"]
					}
				}
			18	{
					if { [IDCHECK $scale18] } {
						set ErrorNo [concat $ErrorNo "18"]
					}
				}
			19	{
					if { [IDCHECK $scale19] } {
						set ErrorNo [concat $ErrorNo "18"]
					}
				}
			20	{
					if { [IDCHECK $MPage_Ent20] } {
						set ErrorNo [concat $ErrorNo "20"]
					}
				}
			21	{
					if { [IDCHECK $MPage_Ent21] } {
						set ErrorNo [concat $ErrorNo "20"]
					}
				}
			22	{
					if { [IDCHECK $MPage_Ent22] } {
						set ErrorNo [concat $ErrorNo "20"]
					}
				}
			24	{
					if { [IDCHECK $scale24] } {
						set ErrorNo [concat $ErrorNo "24"]
					}
				}
			25	{
					if { [IDCHECK $MPage_Ent25] } {
						set ErrorNo [concat $ErrorNo "25"]
					}
				}
			29	{
					if { [IDCHECK $MPage_Ent29] } {
						set ErrorNo [concat $ErrorNo "29"]
					}
				}
			default {}
		}
		incr n
	}
	if { $ErrorNo != "" } {
		MPageErrorOut $ErrorNo
		return 1
	} else {
		return 0
	}
}
proc IDCHECK String {
	return 0
}

proc MPageHelp { helpfile args } {
	set HFile [ open $helpfile {RDONLY CREAT} ]
	if { [ file size $helpfile ] == 0 } {
		MPageDialog {Read Me File Error} "Can not open $helpfile"
	} else {
		set Comment [ read $HFile ]
		set Font [ lindex $args 0 ]
		if { $Font !=  } {
			MPageDialog {Read Me} $Comment $Font
		} else {
			MPageDialog {Read Me} $Comment
		}
	}
	close $HFile
}

proc MPageDialog { Name Comment args } {
	global bgcolor
	if { [winfo exists .dialog] } {
		destroy .dialog
	}
	toplevel .dialog
	wm title .dialog $Name
	set Font [ lindex $args 0 ]
	frame .dialog.top -relief raised -bd 2 -bg $bgcolor
	frame .dialog.bottom -relief raised -bd 2 -bg $bgcolor
	if { $Font == "" } {
		text .dialog.top.text -wrap word -height 15 -width 25 -yscrollcommand ".dialog.top.scroll set" -highlightthickness 0
	} else {
		text .dialog.top.text -wrap word -height 15 -width 25 -yscrollcommand ".dialog.top.scroll set" -font $Font -highlightthickness 0
	}
	.dialog.top.text insert end $Comment
	scrollbar .dialog.top.scroll -command ".dialog.top.text yview" -highlightthickness 0
	if { $Font == "" } {
		button .dialog.bottom.button -text OK -command { destroy .dialog } -highlightthickness 0
	} else {
		button .dialog.bottom.button -text OK -command { destroy .dialog } -font $Font -highlightthickness 0
	}
	pack .dialog.top -side top -fill both -expand 1
	pack .dialog.bottom -side bottom -fill x
	pack .dialog.top.scroll	-side right -fill y
	pack .dialog.top.text -side left -fill both -expand 1
	pack .dialog.bottom.button -side left -expand 1 -padx 10 -pady 5 -ipadx 5 -ipady 2
}

proc MPageIniLoad {} {
	global rbc
	set inidata ""
	set fd [ open .DAWN_1.default r ]
	while { ! [ eof $fd ] } {
		set line [gets $fd]
		set inidata [concat $inidata $line]
	}
	close $fd
	set rbc [lindex $inidata 32]
	if {$rbc == "y"} {
		MPageJump [lindex $inidata 33]
		MPageClip
	}
	MPageJump [lindex $inidata 31]
}

proc MPageIniSave {} {
	global MPage_Count_New MPage_Count_Old_Clip rbc
	set inidata ""
	set n 0
	set fd [ open .DAWN_1.default r ]
	while { ! [ eof $fd ] } {
		incr n
		if { $n < 32 } {
			set line [gets $fd]
			set inidata [concat $inidata $line]
		} else {
			break
		}
	}
	close $fd
	set inidata [concat $inidata $MPage_Count_New]
	set inidata [concat $inidata $rbc]
	if {$rbc == "y"} {
		set inidata [concat $inidata $MPage_Count_Old_Clip]
	}
	set fd [ open .DAWN_1.default w ] 
	foreach i $inidata { 
		puts $fd $i 
	} 
	close $fd
}

###initialize
set fp [ open .DAWN_1.history { WRONLY CREAT } ]
close $fp
set fp [ open .DAWN_1.default { WRONLY CREAT } ]
if { [ file size .DAWN_1.default ] == 0 } {
	MPageReg default
}
close $fp
MPageCancel
MPageJump 1
MPageIniLoad
