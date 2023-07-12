#include "FUMenu.h"

//フレームのラベルのフォント幅とTcl/Tkの1文字の幅の違いの修正
//1つのラベル全体の文字数から，Tcl/Tkへ渡す文字数の差分(何文字減らすか)
#define FrameWIDTH_SUB 14 

//
//基本ボタンセットの1つのボタンの最小幅の指定．(文字数を指定する)
#define ButtonsMIN_SIZE 8 //

static int unitnum, framenum;
static int fWidth, SCWidth;
static int EWidth, RBWidth, CBWidth, SWidth;

void FUPage::output(ostream& mOut, int& UnitFlag) const
{
	FRSListIterator<FUFrame*> i = mFrameList;
	for (int j = 1; i; i++, j++) {
		mOut << endl;
		mOut << "###Frame" << j << endl;
		i.get()->output(mOut, UnitFlag);
	}
}

void FUPage::outGlobal(int& ui, ostream mOut) const
{
	FRSListIterator<FUFrame*> i = mFrameList;
	for (int j = 1; i; i++, j++) {
		i.get()->outGlobal(ui, mOut);
	}
}

void FUPage::outRegSwitch(int& ui, ostream& mOut) const
{
	FRSListIterator<FUFrame*> i = mFrameList;
	for (int j = 1; i; i++, j++) {
		i.get()->outRegSwitch(ui, mOut);
	}
}

void FUPage::outOutSwitch(int& ui, ostream& mOut) const
{
	FRSListIterator<FUFrame*> i = mFrameList;
	for (int j = 1; i; i++, j++) {
		i.get()->outOutSwitch(ui, mOut);
	}
}

void FUPage::outErrorSwitch(int& ui, ostream& mOut) const
{
	FRSListIterator<FUFrame*> i = mFrameList;
	for (int j = 1; i; i++, j++) {
		i.get()->outErrorSwitch(ui, mOut);
	}
}

void FUPage::outColorSwitch(int& ui, ostream& mOut) const
{
	FRSListIterator<FUFrame*> i = mFrameList;
	for (int j = 1; i; i++, j++) {
		i.get()->outColorSwitch(ui, mOut);
	}
}

void FUPage::outColorIni(int& ui, ostream& mOut) const
{
	FRSListIterator<FUFrame*> i = mFrameList;
	for (int j = 1; i; i++, j++) {
		i.get()->outColorIni(ui, mOut);
	}
}

FUFrame::FUFrame(const FRString& lbl = "")
{
	mLabel = lbl;
}

int FUFrame::scalewidth() const
{
	int sc = 0;
	FRSListIterator<FUUnit*> i = mUnitList;
	for (int j = 1; i; i++, j++, unitnum++) {
		if (sc < i.get()->scwidth(mLabel.size())){
			sc = i.get()->scwidth(mLabel.size());
		}
	}
	return(sc);
}

void FUFrame::output(ostream& mOut, int& UnitFlag) const
{
	FRSListIterator<FUUnit*> x = mUnitList;
	int fnum = 0;
	int sc = 0;
		
	mOut << "frame   .menu.f" << framenum << " -bg $bgcolor" << endl;

	for (int j = 1; x; x++, j++) {
		if (sc < x.get()->scwidth(mLabel.size())){
			sc = x.get()->scwidth(mLabel.size());
		}
	}

	if (sc > 0){
		mOut << "label   .menu.f" << framenum << ".l -text \"" << mLabel << "\" -width " << SCWidth << " -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor" << endl;
	} else {	
		mOut << "label   .menu.f" << framenum << ".l -text \"" << mLabel << "\" -width " << fWidth << " -padx 0 -pady 0 -relief raised -anchor w -fg $fgcolor -bg $bgcolor" << endl;
	}



	mOut << "frame   .menu.f" << framenum << ".right -bd 0 -bg $bgcolor -highlightthickness 0" << endl;

	FRSListIterator<FUUnit*> i = mUnitList;
	for (int j = 1; i; i++, j++, unitnum++) {
		mOut << "##Unit" << j << endl;
		i.get()->output(mOut, UnitFlag);
		fnum += i.get()->DataQuantity();
	}
	mOut << "pack    .menu.f" << framenum << ".l .menu.f" << framenum << ".right -in .menu.f" << framenum << " -side left -fill both -expand 1" << endl;
	framenum += fnum;
//	framenum += mUnitList.numberOfNodes();
}

void FUFrame::outGlobal(int& ui, ostream mOut) const
{
	FRSListIterator<FUUnit*> i = mUnitList;
	for (int j = 1; i; i++, j++) {
		ui++;
		i.get()->outGlobal(ui, mOut);
	}
}

void FUFrame::outRegSwitch(int& ui, ostream& mOut) const
{
	FRSListIterator<FUUnit*> i = mUnitList;
	for (int j = 1; i; i++, j++) {
		ui++;
		i.get()->outRegSwitch(ui, mOut);
	}
}

void FUFrame::outOutSwitch(int& ui, ostream& mOut) const
{
	FRSListIterator<FUUnit*> i = mUnitList;
	for (int j = 1; i; i++, j++) {
		ui++;
		i.get()->outOutSwitch(ui, mOut);
	}
}

void FUFrame::outErrorSwitch(int& ui, ostream& mOut) const
{
	FRSListIterator<FUUnit*> i = mUnitList;
	int fi;
	fi = ui + 1;
	for (int j = 1; i; i++, j++) {
		ui++;
		i.get()->outErrorSwitch(ui, fi, mOut);
	}
}

void FUFrame::outColorSwitch(int& ui, ostream& mOut) const
{
	FRSListIterator<FUUnit*> i = mUnitList;
	for (int j = 1; i; i++, j++) {
		ui++;
		i.get()->outColorSwitch(ui, mOut);
	}
}

void FUFrame::outColorIni(int& ui, ostream& mOut) const
{
	FRSListIterator<FUUnit*> i = mUnitList;
	int fi;
	fi = ui + 1;
	for (int j = 1; i; i++, j++) {
		ui++;
		i.get()->outColorIni(fi, ui, mOut);
	}
}

FUEntry::FUEntry(const FRString& lbl = "") : FUUnit(lbl)
{
	mValue = "0";
	int Esize;
	Esize = lbl.size() - 1;
	//cout << "entrysize=" << Esize << endl;
	if (EWidth < Esize){
		//cout << "Emax=" << Esize << endl;
		EWidth = Esize;
	}
	if (EWidth < 0) EWidth = 0;
}

void FUEntry::output(ostream& mOut, int& UnitFlag) const
{
	UnitFlag = UnitFlag | 0x10;
	mOut << "#Entry" << endl;
	mOut << "frame   .menu.f" << framenum << ".f" << unitnum << " -bg $bgcolor" << endl;
	mOut << "button  .menu.f" << framenum << ".b" << unitnum << " -text \"" << mLabel << "\" -width " << EWidth << " -bd 2 -highlightthickness 0 -padx 0 -pady 0 -command \"MPageSelectEntry " << framenum << " " << unitnum << "\" -fg $fgcolor -bg $bgcolor" << endl;
	mOut << "entry   .menu.f" << framenum << ".e" << unitnum << " -relief ridge -bd 3 -width 13 -highlightthickness 0 -textvariable MPage_Ent" << unitnum << " -fg $fgcolor -bg $bgcolor" << endl;
	mOut << "pack    .menu.f" << framenum << ".b" << unitnum << " .menu.f" << framenum << ".e" << unitnum << " -in .menu.f" << framenum << ".f" << unitnum << " -side left -fill both -expand 1" << endl;
	mOut << "pack    .menu.f" << framenum << ".f" << unitnum << " -in .menu.f" << framenum << ".right -side top -fill both -expand 1" << endl;
	mOut << "set MPage_Ent" << unitnum << " " << mValue << endl;
}

void FUEntry::outGlobal(int& ui, ostream mOut) const
{
		mOut << "	global MPage_Ent" << ui << endl;
}

void FUEntry::outRegSwitch(int& ui, ostream& mOut) const
{
	mOut << "			" << ui << "	{"<< endl;
	mOut << "				if { $MPage_Ent" << ui << " != \"\" } {" << endl;
	mOut << "					set new_data [ concat $new_data $MPage_Ent" << ui << "]" << endl;
	mOut << "				} else {" << endl;
	mOut << "					set new_data [ concat $new_data 0 ]" << endl;
	mOut << "				}" << endl;
	mOut << "			}" << endl;
}

void FUEntry::outOutSwitch(int& ui, ostream& mOut) const
{
	mOut << "			" << ui << "	{set MPage_Ent" << ui << " $i}" << endl;
}

void FUEntry::outErrorSwitch(int& ui, int fi, ostream& mOut) const
{
	mOut << "			" << ui << "	{" << endl;
	mOut << "					if { [IDCHECK $MPage_Ent"<< ui << "] } {" << endl;
	mOut << "						set ErrorNo [concat $ErrorNo \"" << fi << "\"]" << endl;
	mOut << "					}" << endl;
	mOut << "				}" << endl;
}

FUScale::FUScale(const FRString& lbl = "") : FUUnit(lbl)
{
	mMax = 1;
	mMin = 0;
	mStep = 0.1;
	mValue = 0;
	int Ssize;
	Ssize = lbl.size() - 1;
	//cout << "scalesize=" << Ssize << endl;
	if (SWidth < Ssize){
		//cout << "Smax=" << Ssize << endl;
		SWidth = Ssize;
	}
//	//cout << "FrameScale=" << SCWidth << endl;
}

void FUScale::output(ostream& mOut, int& UnitFlag) const
{
	UnitFlag = UnitFlag | 0x8;
	mOut << "#Scale" << endl;
	mOut << "frame   .menu.f" << framenum << ".f" << unitnum << " -relief raised -bd 2 -bg $bgcolor" << endl;
	mOut << "label   .menu.f" << framenum << ".label" << unitnum << " -text \"" << mLabel << "\" -width " << SWidth << " -padx 2 -pady 0 -fg $fgcolor -bg $bgcolor" << endl;
	mOut << "entry   .menu.f" << framenum << ".entry" << unitnum << " -width 5 -relief flat -bd 0 -textvariable scale" << unitnum << " -highlightthickness 0 -fg $fgcolor -bg $bgcolor" << endl;
	mOut << "scale   .menu.f" << framenum << ".scale" << unitnum << " -length 222 -from " << mMin << " -to " << mMax << " -resolution " << mStep << " -orient horizontal -variable scale" << unitnum << " -highlightthickness 0 -showvalue false -fg $fgcolor -bg $bgcolor" << endl;
	mOut << ".menu.f" << framenum << ".scale" << unitnum << " set $scale" << unitnum << endl;
	mOut << "pack    .menu.f" << framenum << ".label" << unitnum << " .menu.f" << framenum << ".entry" << unitnum << " .menu.f" << framenum << ".scale" << unitnum << " -in .menu.f" << framenum << ".f" << unitnum << " -side left -fill both -expand 1" << endl;
	mOut << "pack    .menu.f" << framenum << ".f" << unitnum << " -in .menu.f" << framenum << ".right -side top -fill both -expand 1" << endl;
	mOut << "set     scale" << unitnum << " "<< mValue << endl;
}

void FUScale::outGlobal(int& ui, ostream mOut) const
{
		mOut << "	global scale" << ui << endl;
}

void FUScale::outRegSwitch(int& ui, ostream& mOut) const
{
	mOut << "			" << ui << "	{set new_data [ concat $new_data $scale" << ui << " ]}" << endl;
}

void FUScale::outOutSwitch(int& ui, ostream& mOut) const
{
	mOut << "			" << ui << "	{set scale" << ui << " $i}" << endl;
}

void FUScale::outErrorSwitch(int& ui, int fi, ostream& mOut) const
{
	mOut << "			" << ui << "	{" << endl;
	mOut << "					if { [IDCHECK $scale" << ui << "] } {" << endl;
	mOut << "						set ErrorNo [concat $ErrorNo \"" << fi << "\"]" << endl;
	mOut << "					}" << endl;
	mOut << "				}" << endl;
}

int FUScale::scwidth(int width) const
{
	return(width);
}

void FUBaseRadiobutton::output(ostream& mOut, int& UnitFlag) const
{
	UnitFlag = UnitFlag | 0x4;

	FRSListIterator<FURadiobutton*> i = mRBList;
	int ri = 0;

	mOut << "#Radiobutton" << endl;
	mOut << "frame	.menu.f" << framenum << ".f" << unitnum << " -relief raised -bd 0 -bg $bgcolor" << endl;	
	for (int j = 1; i; i++, j++) {
		ri++;
		if (ri % 2) {
			mOut << "frame   .menu.f" << framenum << ".f" << unitnum << ".frb" << ri << " -bg $bgcolor" << endl;
		}
		i.get()->output(ri, mOut);
	}
	if (ri % 2) {
		mOut << "radiobutton .menu.f" << framenum << ".f" << unitnum << ".r"<< ++ri <<" -text \" \" -variable rb" << unitnum << " -width " << RBWidth << " -relief raised -bd 2 -state disabled -highlightthickness 0 -padx 0 -pady 0  -fg $fgcolor -bg $bgcolor" << endl;
	}
	for(int j = 1; j <= ri; j += 2) {
		mOut << "pack	.menu.f" << framenum << ".f" << unitnum << ".r" << j << " .menu.f" << framenum << ".f" << unitnum << ".r" << j + 1 << " -in .menu.f" << framenum << ".f" << unitnum << ".frb" << j << " -side left -ipadx 2 -ipady 1 -fill both -expand 1" << endl;
		mOut << "pack	.menu.f" << framenum << ".f" << unitnum << ".frb" << j << " -in .menu.f" << framenum << ".f" << unitnum << " -side top -fill both -expand 1" << endl;
	}
	mOut << "pack	.menu.f" << framenum << ".f" << unitnum << " -in .menu.f" << framenum << ".right -side top -fill both -expand 1"<< endl;
	mOut << "set	rb" << unitnum << " "<< mValue << endl;
}

void FUBaseRadiobutton::outGlobal(int& ui, ostream mOut) const
{
		mOut << "	global rb" << ui << endl;
}

void FUBaseRadiobutton::outRegSwitch(int& ui, ostream& mOut) const
{
	mOut << "			" << ui << "	{set new_data [ concat $new_data $rb" << ui << " ]}" << endl;
}

void FUBaseRadiobutton::outOutSwitch(int& ui, ostream& mOut) const
{
	mOut << "			" << ui << "	{set rb" << ui << " $i}" << endl;
}

FURadiobutton::FURadiobutton(const FRString& str = "", const FUString Val = "")
{
	mLabel = str;
	mValue = Val;
	int Rsize;
	Rsize = mLabel.size() - 1;
	//cout << "Radiobuttonsize=" << Rsize << endl;
	if (RBWidth < Rsize){
		//cout << "RBmax=" << Rsize << endl;
		RBWidth = Rsize;
	}
}

void FURadiobutton::output(int ri, ostream& mOut) const
{
	mOut << "radiobutton .menu.f" << framenum << ".f" << unitnum << ".r" << ri << " -text \"" << mLabel << "\" -value " << mValue << " -variable rb" << unitnum << " -width " << RBWidth << " -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg $bgcolor" << endl;
}

int FUBaseCheckbutton::DataQuantity() const
{
		return (mCBList.numberOfNodes());
}

void FUBaseCheckbutton::output(ostream& mOut, int& UnitFlag) const
{
	UnitFlag = UnitFlag | 0x2;

	FRSListIterator<FUCheckbutton*> i = mCBList;
	int ci = 0, baseci = unitnum;

	mOut << "#Checkbutton" << endl;
	mOut << "frame	.menu.f" << framenum << ".f" << unitnum << " -relief raised -bd 0 -bg $bgcolor" << endl;	
	for (int j = 1; i; i++, j++) {
		ci++;
		if (ci % 2) {
			mOut << "frame   .menu.f" << framenum << ".f" << baseci << ".fcb" << unitnum << " -bg $bgcolor" << endl;
		}
		i.get()->output(ci, baseci, mOut);
		unitnum++;
	}
	unitnum--;
	if (ci % 2) {
		mOut << "checkbutton .menu.f" << framenum << ".f"<< baseci <<".c" << ++ci << " -text \" \" -variable cb -width " << CBWidth << " -state disabled -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -fg $fgcolor -bg #d9d9d9" << endl;
	}
	for(int j = 1; j < ci; j += 2) {
		mOut << "pack	.menu.f" << framenum << ".f" << baseci  << ".c" << j << " .menu.f" << framenum << ".f" << baseci << ".c" << j +1 << " -in .menu.f" << framenum << ".f" << baseci << ".fcb" << baseci + j - 1 << " -side left -ipadx 2 -ipady 1 -fill both -expand 1" << endl;
		mOut << "pack	.menu.f" << framenum << ".f" << baseci << ".fcb" << baseci + j - 1 << " -in .menu.f" << framenum << ".f" << baseci << " -side top -fill both -expand 1" << endl;
	}
	mOut << "pack	.menu.f" << framenum << ".f" << baseci << " -in .menu.f" << framenum << ".right -side top -fill both -expand 1"<< endl;
}

void FUBaseCheckbutton::outGlobal(int& ui, ostream mOut) const
{
	for (int i = 1; i <= mCBList.numberOfNodes(); i++) {
		mOut << "	global cb" << ui << endl;
		++ui;
	}
	ui--;
}

void FUBaseCheckbutton::outRegSwitch(int& ui, ostream& mOut) const
{
	for (int i = 1; i <= mCBList.numberOfNodes(); i++) {
		mOut << "			" << ui << "	{set new_data [ concat $new_data $cb" << ui << " ]}" << endl;
		++ui;
	}
	ui--;
}

void FUBaseCheckbutton::outOutSwitch(int& ui, ostream& mOut) const
{
	for (int i = 1; i <= mCBList.numberOfNodes(); i++) {
		mOut << "			" << ui << "	{set cb" << ui << " $i}" << endl;
		++ui;
	}
	ui--;
}

void FUBaseCheckbutton::outErrorSwitch(int& ui, int , ostream& ) const
{
		ui += mCBList.numberOfNodes() - 1;
}

void FUBaseCheckbutton::outColorSwitch(int& ui, ostream& ) const
{
		ui += mCBList.numberOfNodes() - 1;
}

void FUBaseCheckbutton::outColorIni(int, int& ui, ostream& ) const
{
		ui += mCBList.numberOfNodes() - 1;
}

void FUCheckbutton::output(int ci,int baseci, ostream& mOut) const
{
	mOut << "checkbutton .menu.f" << framenum << ".f" << baseci << ".c" << ci << " -text \"" << mLabel << "\" -onvalue " << mONValue << " -offvalue " << mOFFValue << " -variable cb" << unitnum << " -width 11 -anchor w -relief raised -bd 2 -highlightthickness 0 -padx 0 -pady 0 -bg #d9d9d9" << endl;
	mOut << "set	cb" << unitnum << " "<< mValue << endl;
}

void FUColor::output(ostream& mOut, int& UnitFlag) const
{
	UnitFlag = UnitFlag | 0x1;
	
	mOut << "#Color" << endl;

	mOut << "frame   .menu.f" << framenum << ".f" << unitnum << " -relief raised -bd 2 -bg $bgcolor" << endl;
	mOut << "frame   .menu.f" << framenum << ".fc" << unitnum << " -bg $bgcolor" << endl;
	mOut << "frame   .menu.f" << framenum << ".color" << unitnum << " -width 45 -height 45 -relief sunken -bd 2 -bg $bgcolor" << endl;
	mOut << "frame   .menu.f" << framenum << ".scale" << unitnum << " -bg $bgcolor" << endl;

	mOut << "frame   .menu.f" << framenum << ".scale" << unitnum << ".f1 -bg $bgcolor" << endl;
	mOut << "label   .menu.f" << framenum << ".scale" << unitnum << ".t1 -text \" R \" -bg $bgcolor -fg $fgcolor" << endl;
	mOut << "entry   .menu.f" << framenum << ".entred" << unitnum << " -width 5 -relief flat -bd 0 -textvariable r" << unitnum << " -highlightthickness 0 -bg $bgcolor -fg $fgcolor" << endl;
	mOut << "scale   .menu.f" << framenum << ".scale" << unitnum << ".red -length 222 -from " << mMin << " -to " << mMax << " -orient horizontal -command \"MPageSetColor " << framenum << " " << unitnum << "\" -highlightthickness 0 -resolution " << mStep << " -showvalue false -variable r" << unitnum << " -activebackground #ff0000 -troughcolor #cd0000 -bg $bgcolor -fg $fgcolor" << endl;
	mOut << "pack    .menu.f" << framenum << ".scale" << unitnum << ".t1 .menu.f" << framenum << ".entred" << unitnum << " .menu.f" << framenum << ".scale" << unitnum << ".red -in .menu.f" << framenum << ".scale" << unitnum << ".f1 -side left -fill both -expand 1" << endl;
	mOut << "set     r" << unitnum << " " << mR << endl;


	mOut << "frame   .menu.f" << framenum << ".scale" << unitnum << ".f2 -bg $bgcolor" << endl;
	mOut << "label   .menu.f" << framenum << ".scale" << unitnum << ".t2 -text \" G \" -bg $bgcolor -fg $fgcolor" << endl;
	mOut << "entry   .menu.f" << framenum << ".entgreen" << unitnum << " -width 5 -relief flat -bd 0 -textvariable g" << unitnum << " -highlightthickness 0 -bg $bgcolor -fg $fgcolor" << endl;
	mOut << "scale   .menu.f" << framenum << ".scale" << unitnum << ".green -length 222 -from " << mMin << " -to " << mMax << " -orient horizontal -command \"MPageSetColor " << framenum << " " << unitnum << "\" -highlightthickness 0 -resolution " << mStep << " -showvalue false -variable g" << unitnum << " -activebackground #00ff00 -troughcolor #00cd00 -bg $bgcolor -fg $fgcolor" << endl;
	mOut << "pack    .menu.f" << framenum << ".scale" << unitnum << ".t2 .menu.f" << framenum << ".entgreen" << unitnum << " .menu.f" << framenum << ".scale" << unitnum << ".green -in .menu.f" << framenum << ".scale" << unitnum << ".f2 -side left -fill both -expand 1" << endl;
	mOut << "set     g" << unitnum << " " << mG << endl;
	
	mOut << "frame   .menu.f" << framenum << ".scale" << unitnum << ".f3 -bg $bgcolor" << endl;
	mOut << "label   .menu.f" << framenum << ".scale" << unitnum << ".t3 -text \" B \" -bg $bgcolor -fg $fgcolor" << endl;
	mOut << "entry   .menu.f" << framenum << ".entblue" << unitnum << " -width 5 -relief flat -bd 0 -textvariable b" << unitnum << " -highlightthickness 0 -bg $bgcolor -fg $fgcolor" << endl;
	mOut << "scale   .menu.f" << framenum << ".scale" << unitnum << ".blue -length 222 -from " << mMin << " -to " << mMax << " -orient horizontal -command \"MPageSetColor " << framenum << " " << unitnum << "\" -highlightthickness 0 -resolution " << mStep << " -showvalue false -variable b" << unitnum << " -activebackground #0000ff -troughcolor #000080 -bg $bgcolor -fg $fgcolor" << endl;
	mOut << "pack    .menu.f" << framenum << ".scale" << unitnum << ".t3 .menu.f" << framenum << ".entblue" << unitnum << " .menu.f" << framenum << ".scale" << unitnum << ".blue -in .menu.f" << framenum << ".scale" << unitnum << ".f3 -side left -fill both -expand 1" << endl;
	mOut << "set     b" << unitnum << " " << mB << endl;
	
	mOut << "pack    .menu.f" << framenum << ".color" << unitnum << " -in .menu.f" << framenum << ".fc" << unitnum << " -side top -padx 5 -pady 10 -fill both -expand 1" << endl;
	mOut << "pack    .menu.f" << framenum << ".scale" << unitnum << ".f1 .menu.f" << framenum << ".scale" << unitnum << ".f2 .menu.f" << framenum << ".scale" << unitnum << ".f3 -in .menu.f" << framenum << ".scale" << unitnum << " -side top -fill both -ipadx 1 -expand 1" << endl;
	mOut << "pack    .menu.f" << framenum << ".fc" << unitnum << " .menu.f" << framenum << ".scale" << unitnum << " -in .menu.f" << framenum << ".f" << unitnum << " -side left -fill both -expand 1 " << endl;
	mOut << "pack    .menu.f" << framenum << ".f" << unitnum << " -in .menu.f" << framenum << ".right -side top -fill both -expand 1 " << endl;

	unitnum += 2;
}

void FUColor::outGlobal(int& ui, ostream mOut) const
{
	mOut << "	global r" << ui << " g" << ui << " b" << ui << endl;
	ui += 2;
}

void FUColor::outRegSwitch(int& ui, ostream& mOut) const
{
	mOut << "			" << ui << "	{set new_data [ concat $new_data $r" << ui << " ]}" << endl;
	mOut << "			" << ui + 1 << "	{set new_data [ concat $new_data $g" << ui << " ]}" << endl;
	mOut << "			" << ui + 2 << "	{set new_data [ concat $new_data $b" << ui << " ]}" << endl;
	ui += 2;
}

void FUColor::outOutSwitch(int& ui, ostream& mOut) const
{
	mOut << "			" << ui << "	{set r" << ui << " $i}" << endl;
	mOut << "			" << ui + 1 << "	{set g" << ui << " $i}" << endl;
	mOut << "			" << ui + 2 << "	{set b" << ui << " $i}" << endl;
	ui += 2;
}

void FUColor::outErrorSwitch(int& ui, int fi, ostream& mOut) const
{
	mOut << "			" << ui << "	{" << endl;
	mOut << "					if { [IDCHECK $r" << ui << "] } {" << endl;
	mOut << "						set ErrorNo [concat $ErrorNo \"" << fi << "\"]" << endl;
	mOut << "					}" << endl;
	mOut << "				}" << endl;

	mOut << "			" << ui + 1 << "	{" << endl;
	mOut << "					if { [IDCHECK $g" << ui << "] } {" << endl;
	mOut << "						set ErrorNo [concat $ErrorNo \"" << fi << "\"]" << endl;
	mOut << "					}" << endl;
	mOut << "				}" << endl;

	mOut << "			" << ui  + 2 << "	{" << endl;
	mOut << "					if { [IDCHECK $b" << ui << "] } {" << endl;
	mOut << "						set ErrorNo [concat $ErrorNo \"" << fi << "\"]" << endl;
	mOut << "					}" << endl;
	mOut << "				}" << endl;
	ui += 2;
}

void FUColor::outColorSwitch(int& ui, ostream& mOut) const
{
		double dcolor;
		dcolor = 1.0 / mMax;

		mOut << "		" << ui << "	{" << endl;
		mOut << "			global r" << ui << " g" << ui << " b" << ui << "" << endl;
    	mOut << "			if { [IDCHECK $r" << ui << "] || [IDCHECK $g" << ui << "] || [IDCHECK $b" << ui << "] } {" << endl;
    	mOut << "				MPageErrorOut \"" << ui << "\"" << endl;
	    mOut << "				return" << endl;
	    mOut << "			}" << endl;
    	mOut << "				set red [ expr " << dcolor << " * $r" << ui << " * 255 ]" << endl;
	    mOut << "				set green [ expr " << dcolor << " * $g" << ui << " * 255 ]" << endl;
    	mOut << "				set blue [ expr " << dcolor << " * $b" << ui << " * 255 ] " << endl;
	    mOut << "			}" << endl;
		ui += 2;
}

void FUColor::outColorIni(int fi ,int& ui, ostream& mOut) const
{
	mOut << "		MPageSetColor " << fi << " " << ui << " 0" << endl;
	ui += 2;
}

int FUColor::scwidth(int width) const
{
	return(width);
}

void FUGenerator::outputHeader() const
{
	unitnum = 1;
	framenum = 1;
	mOut << "#### MULTPAGE GUI MENU ####" << endl;
	mOut << endl;
	mOut << "set bgcolor " << mBgColor << endl;
	mOut << "set fgcolor " << mFgColor << endl;
	mOut << "set MPage_Count_New 1" << endl;
	mOut << "set MPage_ch 0" << endl;
	mOut << "set MPage_Count_Old_Clip 0" << endl;
//	mOut << "set MPage_Color_End 1" << endl;
}
void FUGenerator::outputButtons() const
{
	int btwidth;
	btwidth = (fWidth + EWidth ) / 6;
	cout << btwidth << endl;
	if (btwidth < ButtonsMIN_SIZE) btwidth = ButtonsMIN_SIZE;
	cout << btwidth << endl;
	mOut << endl;
	mOut << "#Button" << endl;
	mOut << "frame   .top     -highlightthickness 0 -bg $bgcolor" << endl;
	mOut << "frame   .top.1   -highlightthickness 0 -bg $bgcolor" << endl;
	mOut << "frame   .top.2   -highlightthickness 0 -bg $bgcolor" << endl;
	mOut << "button  .top.1.ok -text \"OK\" -bd 2 -command { MPageReg history } -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width " << btwidth << endl;
	mOut << "button  .top.1.cancel -text \"Undo\" -bd 2 -command MPageCancel -state disabled -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width " << btwidth << endl;
	mOut << "button  .top.1.def_s -text \"Save Default\" -bd 2 -command { MPageReg default } -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width " << btwidth << endl;
	mOut << "button  .top.1.def_l -text \"Load Default\" -bd 2 -command MPageGetDefault -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width " << btwidth << endl;
	mOut << "button  .top.1.exit -text \"Exit\" -bd 2 -command {MPageIniSave ; exit 1 } -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width " << btwidth << endl;
	mOut << "button  .top.1.help -text \"Help\" -bd 2 -command {MPageHelp help.txt} -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width " << btwidth << endl;
	mOut << "menubutton  .top.2.pageview -text \"Page Jump\" -fg #ff0000 -bg $bgcolor -relief raised -menu .top.2.pageview.menu -highlightthickness 0 -bd 2 -activeforeground #0000ff -width 10" << endl;
	mOut << "button  .top.2.page  -textvariable MPage_Title_View -width 10 -highlightthickness 0 -fg $fgcolor -bg $bgcolor -state disabled -disabledforeground #000000 -bd 2" << endl;
	mOut << "button  .top.2.prev -text \"<<\" -command MPagePrev -state disabled -bd 2 -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width 10" << endl;
	mOut << "button  .top.2.next -text \">>\" -command MPageNext -bd 2 -highlightthickness 0 -fg $fgcolor -bg $bgcolor -width 10" << endl;
	mOut << "menubutton  .top.2.clip -text \"Page Clip\" -fg #ff0000 -bg $bgcolor -relief raised -menu .top.2.clip.menu -highlightthickness 0 -bd 2 -activeforeground #0000ff -width 10" << endl;
	mOut << "pack    .top.1.ok .top.1.cancel .top.1.def_s .top.1.def_l .top.1.exit .top.1.help -side left -padx 0 -pady 0 -fill both -expand 1" << endl;
	mOut << "pack    .top.2.pageview .top.2.page .top.2.prev .top.2.next .top.2.clip -side left -padx 0 -pady 0 -fill both -expand 1" << endl;
	mOut << "pack    .top.1 .top.2 -side top -fill both -expand 1" << endl;
	mOut << ". configure -bg $bgcolor" << endl;
}
void FUGenerator::outputTitle() const
{
	mOut << endl;
	mOut << "#Title" << endl;
	mOut << "wm title	. \"" << mName << "\"" << endl;
	mOut << "frame	.menu -bg $bgcolor" << endl;
	mOut << "frame	.menu.main -bg $bgcolor" << endl;
	mOut << "frame	.menu.clip -bg $bgcolor" << endl;
	mOut << "frame	.menu.clip.space -height 10 -bg $bgcolor" << endl;
	mOut << "pack	.menu.clip.space -side top -fill both -expand 1" << endl;
	mOut << "pack	.menu.main -side top -fill both -expand 1" << endl;
	mOut << "pack	.top .menu  -side top -padx 1m -pady 5 -fill both -expand 1" << endl;
	
}
void FUGenerator::outputIni() const
{
	mOut << endl;
	mOut << "###initialize" << endl;
	mOut << "set fp [ open " << mDefFile << ".history { WRONLY CREAT } ]" << endl; 
	mOut << "close $fp" << endl;
	mOut << "set fp [ open " << mDefFile << ".default { WRONLY CREAT } ]" << endl; 
	mOut << "if { [ file size " << mDefFile << ".default ] == 0 } {" << endl;
	mOut << "	MPageReg default" << endl;
//	mOut << "	MPageIniSave" << endl;
	mOut << "}" << endl;
	mOut << "close $fp" << endl;
	mOut << "MPageCancel" << endl;
	mOut << "MPageJump 1" << endl;
	mOut << "MPageIniLoad" << endl;
}
void FUGenerator::outputPrev() const
{
	mOut << endl;
	mOut << "proc MPagePrev {} {" << endl;
	mOut << "	global MPage_Count_New MPage_Count_Old_Clip" << endl;
	mOut << "	set MPage_Count_W $MPage_Count_New" << endl;
	mOut << "	incr MPage_Count_W -1" << endl;
	mOut << "	if { $MPage_Count_Old_Clip == $MPage_Count_W } {" << endl;
	mOut << "		incr MPage_Count_W -1" << endl;
	mOut << "	}" << endl;
	mOut << "	MPageJump $MPage_Count_W" << endl;
	mOut << "}" << endl;
}
void FUGenerator::outputNext() const
{
	mOut << endl;
	mOut << "proc MPageNext {} {" << endl;
	mOut << "	global MPage_Count_New MPage_Count_Old_Clip" << endl;
	mOut << "	set MPage_Count_W $MPage_Count_New" << endl;
	mOut << "	incr MPage_Count_W" << endl;
	mOut << "	if { $MPage_Count_Old_Clip == $MPage_Count_W } {" << endl;
	mOut << "		incr MPage_Count_W" << endl;
	mOut << "	}" << endl;
	mOut << "	MPageJump $MPage_Count_W" << endl;
	mOut << "}" << endl;
}
void FUGenerator::outputJump(int pnum) const
{
	int i;
	mOut << endl;
	mOut << "proc MPageJump { JPage } {" << endl;
	mOut << "	global MPage_Count_New MPage_Count_Old MPage_Count_Old_Clip" << endl;
	mOut << "	global MPage_Title_View MPage_Name_List MPage_Last" << endl;
	for (i = 1; i <= pnum; i++){
		mOut << "	global MPage_frame_List"<< i << endl;
	}
	mOut << "	set MPage_Count_Old $MPage_Count_New" << endl;
	mOut << "	set MPage_Count_New $JPage" << endl;
	mOut << "	.top.2.prev configure -state normal" << endl;
	mOut << "	.top.2.next configure -state normal" << endl;
	mOut << "	if { $MPage_Count_New <= 1 } {" << endl;
	mOut << "		.top.2.prev configure -state disabled" << endl;
	mOut << "	} elseif { $MPage_Count_New >= $MPage_Last } {" << endl;
	mOut << "		.top.2.next configure -state disabled" << endl;
	mOut << "	}" << endl;
	mOut << "	if { $MPage_Count_Old_Clip == 1 && $MPage_Count_New == 2} {" << endl;
	mOut << "		.top.2.prev configure -state disabled" << endl;
	mOut << "	}" << endl;
	mOut << "	if { $MPage_Count_Old_Clip == $MPage_Last && $MPage_Count_New == [expr $MPage_Last - 1]} {" << endl;
	mOut << "		.top.2.next configure -state disabled" << endl;
	mOut << "	}" << endl;
	mOut << "	set n [expr $MPage_Count_New - 1]" << endl;
	mOut << "	if { [lindex $MPage_Name_List $n] == \"NoPageName\" } {" << endl;
	mOut << "		set MPage_Title_View page$MPage_Count_New/$MPage_Last" << endl;
	mOut << "	} else { " << endl;
	mOut << "		set MPage_Title_View \"page$MPage_Count_New/$MPage_Last [lindex $MPage_Name_List $n]\"" << endl;
	mOut << "	}" << endl;
	mOut << "	switch $MPage_Count_Old {" << endl;
	for (i = 1; i <= pnum; i++){
		mOut << "		" << i << "	{  foreach fr $MPage_frame_List" << i << " { pack forget .menu.f$fr } }" << endl;
	}
	mOut << "		default {puts \"error-->proc MPageJump(1)\"; exit }" << endl;
	mOut << "	}" << endl;
	mOut << "	switch $MPage_Count_New {" << endl;
	for (i = 1; i <= pnum; i++){
		mOut << "		" << i << "	{  foreach fr $MPage_frame_List" << i << " { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.main} }" << endl;
	}
	mOut << "		default {puts \"error-->proc MPageJump(2)\"; exit }" << endl;
	mOut << "	}" << endl;
	mOut << "} " << endl;
}
void FUGenerator::outputClip(int pnum) const
{
	int i;
	
	mOut << endl;
    mOut << "proc MPageClip {} {" << endl;
    mOut << "	global MPage_Count_New MPage_Count_Old MPage_Count_Old_Clip rbc" << endl;
    mOut << "	global MPage_Title_View MPage_Name_List MPage_Last MPage_Name_title" << endl;

	for (i = 1; i <= pnum; i++){
		mOut << "	global MPage_frame_List"<< i << endl;
	}

    mOut << "	if { $rbc == \"y\" } {" << endl;
    mOut << "		if { $MPage_Count_Old_Clip != 0 } {" << endl;
    mOut << "			switch $MPage_Count_Old_Clip {" << endl;
	for (i = 1; i <= pnum; i++){
		mOut << "				" << i << "	{  foreach fr $MPage_frame_List" << i << " { pack forget .menu.f$fr } }" << endl;
	}
	mOut << "				default {puts \"error-->proc MPageClip(1)\"; exit }" << endl;
    mOut << "			}" << endl;
    mOut << "			.top.2.pageview.menu entryconfigure [expr $MPage_Count_Old_Clip - 1] -state normal" << endl;
    mOut << "		}" << endl;

    mOut << "		set MPage_Count_Old_Clip $MPage_Count_New" << endl;
    mOut << "		switch $MPage_Count_New {" << endl;

	for (i = 1; i <= pnum; i++){
		mOut << "			" << i << "	{  foreach fr $MPage_frame_List" << i << " { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.clip} }" << endl;
	}
	
	mOut << "			default {puts \"error-->proc MPageClip(2)\"; exit }" << endl;
    mOut << "		}" << endl;

    mOut << "		if { $MPage_Count_New == $MPage_Last } {" << endl;
    mOut << "			incr MPage_Count_New -1" << endl;
    mOut << "		} else {" << endl;
    mOut << "			incr MPage_Count_New" << endl;
    mOut << "		}" << endl;
		
    mOut << "		set n [expr $MPage_Count_New - 1]" << endl;
    mOut << "			if { [lindex $MPage_Name_List $n] == \"NoPageName\" } {" << endl;
    mOut << "			set MPage_Title_View page$MPage_Count_New/$MPage_Last" << endl;
    mOut << "		} else {" << endl;
    mOut << "			set MPage_Title_View \"page$MPage_Count_New/$MPage_Last [lindex $MPage_Name_List $n]\"" << endl;
    mOut << "		}" << endl;

    mOut << "		switch $MPage_Count_New {" << endl;
	for (i = 1; i <= pnum; i++){
		mOut << "			" << i << "	{  foreach fr $MPage_frame_List" << i << " { pack .menu.f$fr -side top -fill both -expand 1 -in .menu.main} }" << endl;
	}
	mOut << "			default {puts \"error-->proc MPageClip(3)\"; exit }" << endl;
    mOut << "		}" << endl;
    mOut << "		pack .menu.clip -side top -fill both -expand 1" << endl;
    mOut << "		.top.2.clip.menu entryconfigure 1 -state normal" << endl;
    mOut << "		.top.2.prev configure -state normal" << endl;
    mOut << "		.top.2.next configure -state normal" << endl;
		
    mOut << "		if { $MPage_Count_New <= 1 } {" << endl;
    mOut << "			.top.2.prev configure -state disabled" << endl;
    mOut << "		} elseif { $MPage_Count_New >= $MPage_Last } {" << endl;
    mOut << "			.top.2.next configure -state disabled" << endl;
    mOut << "		}" << endl;
	
    mOut << "		if { $MPage_Count_Old_Clip == 1 && $MPage_Count_New == 2} {" << endl;
    mOut << "			.top.2.prev configure -state disabled" << endl;
    mOut << "		}" << endl;
    mOut << "		if { $MPage_Count_Old_Clip == $MPage_Last && $MPage_Count_New == [expr $MPage_Last - 1]} {" << endl;
    mOut << "			.top.2.next configure -state disabled" << endl;
    mOut << "		}" << endl;
    mOut << "		.top.2.pageview.menu entryconfigure [expr $MPage_Count_Old_Clip - 1] -state disabled" << endl;
    mOut << "	} else {" << endl;
    mOut << "		switch $MPage_Count_Old_Clip {" << endl;
	for (i = 1; i <= pnum; i++){
		mOut << "			" << i << "	{  foreach fr $MPage_frame_List" << i << " { pack forget .menu.f$fr } }" << endl;
	}
    mOut << "			default {puts \"error-->proc MPageClip(4)\"; exit }" << endl;
    mOut << "		}" << endl;
		
    mOut << "		pack forget .menu.clip" << endl;
    mOut << "		if { $MPage_Count_Old_Clip == 1 || $MPage_Count_Old_Clip == $MPage_Last} {" << endl; 
    mOut << "			.top.2.prev configure -state normal" << endl;
    mOut << "			.top.2.next configure -state normal" << endl;
    mOut << "		}" << endl;
    mOut << "		.top.2.pageview.menu entryconfigure [expr $MPage_Count_Old_Clip - 1] -state normal" << endl;
    mOut << "		set MPage_Count_Old_Clip 0" << endl;
    mOut << "		.top.2.clip.menu entryconfigure 1 -state disabled" << endl;
    mOut << "	}" << endl;
    mOut << "}" << endl;
}
void FUGenerator::outputMenuButton() const
{
	mOut << endl;
	mOut << "#menubutton" << endl;
	mOut << "menu .top.2.pageview.menu -tearoff 0 -bg $bgcolor" << endl;
	mOut << "	set MPage_Last 0" << endl;
	mOut << "	foreach p $MPage_Name_List {" << endl;
	mOut << "		incr MPage_Last" << endl;
	mOut << "		.top.2.pageview.menu add radiobutton -label \"$MPage_Last $p\" -indicatoron 0	-variable MPage_Count_W -value $MPage_Last -command {MPageJump $MPage_Count_W}" << endl;
	mOut << "	}" << endl;
	mOut << "set rbc n" << endl;
	mOut << "menu .top.2.clip.menu -tearoff 0 -bg $bgcolor" << endl;
	mOut << "	.top.2.clip.menu add radiobutton -label \"     ON     \" -value y -variable rbc -command { MPageClip }" << endl;
	mOut << "	.top.2.clip.menu add radiobutton -label \"     OFF    \" -value n -variable rbc -command { MPageClip }" << endl;
	mOut << "	.top.2.clip.menu entryconfigure 1 -state disabled" << endl;
	mOut << "	if { $MPage_Last <= 1 } {" << endl;
	mOut << "		.top.2.prev configure -state disabled" << endl;
	mOut << "		.top.2.next configure -state disabled" << endl;
	mOut << "		.top.2.pageview configure -state disabled -disabledforeground #000000" << endl;
	mOut << "	}" << endl;
}

void FUGenerator::outputCancel(int UnitFlag) const
{
	FRSListIterator<FUPage*> page;
	int ui = 0;

	mOut << endl;
    mOut << "proc MPageCancel {} {" << endl;
    mOut << "	set MPage_History \"\"" << endl;
    mOut << "	set n 1" << endl;
    mOut << "	set fd [ open " << mDefFile << ".history r ]" << endl;
    mOut << "	while { ! [ eof $fd ] } {" << endl;
    mOut << "		set line [ gets $fd ]" << endl;
    mOut << "		set MPage_History [ concat $MPage_History $line ]" << endl;
    mOut << "	}" << endl;
    mOut << "	MPageOutput $MPage_History" << endl;

	if ( UnitFlag & 0x1 ) {
		for (page = mPageList, ui = 0; page ; page++) {
			page.get()->outColorIni(ui,mOut);
		}
	}
	
	mOut << "	close $fd" << endl;
    mOut << "	MPageErrorCheck" << endl;
    mOut << "	if { ! [ file size " << mDefFile << ".history ] == 0 } {" << endl; 
    mOut << "		.top.1.cancel configure -state normal " << endl;
    mOut << "	}" << endl;
    mOut << "}" << endl;
}

void FUGenerator::outputSelectEntry() const
{
	mOut << endl;
	mOut << "proc MPageSelectEntry { fno eno } {" << endl;
	mOut << "	.menu.f${fno}.e${eno} delete 0 end" << endl;
	mOut << "	focus .menu.f${fno}.e${eno}" << endl;
	mOut << "}" << endl;

}

void FUGenerator::outputSetColor() const
{
	FRSListIterator<FUPage*> page;
	int ui = 0;

	mOut << endl;
	mOut << "proc MPageSetColor {fnumber unumber dummy} {" << endl;
	mOut << "	switch $unumber {" << endl;

	for (page = mPageList, ui = 0; page ; page++) {
		page.get()->outColorSwitch(ui,mOut);
	}

	mOut << "		default {puts \"error-->proc MPageSetColor\"; exit\"}" << endl;
    mOut << "	}" << endl;
    mOut << "        regexp {^[0-9]+} $red red " << endl;
    mOut << "        regexp {^[0-9]+} $green green " << endl;
    mOut << "        regexp {^[0-9]+} $blue blue" << endl;
    mOut << "        .menu.f$fnumber.color$unumber config -bg [ format #%02x%02x%02x $red $green $blue ] " << endl;
    mOut << "}" << endl;
}
void FUGenerator::outputIniLoad() const
{
	mOut << endl;
    mOut << "proc MPageIniLoad {} {" << endl;
    mOut << "	global rbc" << endl;
    mOut << "	set inidata \"\"" << endl;
    mOut << "	set fd [ open " << mDefFile << ".default r ]" << endl;
    mOut << "	while { ! [ eof $fd ] } {" << endl;
    mOut << "		set line [gets $fd]" << endl;
    mOut << "		set inidata [concat $inidata $line]" << endl;
    mOut << "	}" << endl;
    mOut << "	close $fd" << endl;
    mOut << "	set rbc [lindex $inidata " << unitnum << "]" << endl;
    mOut << "	if {$rbc == \"y\"} {" << endl;
    mOut << "		MPageJump [lindex $inidata " << unitnum + 1 << "]" << endl;
    mOut << "		MPageClip" << endl;
    mOut << "	}" << endl;
    mOut << "	MPageJump [lindex $inidata " << unitnum - 1<< "]" << endl;
    mOut << "}" << endl;
}
void FUGenerator::outputIniSave() const
{
	mOut << endl;
    mOut << "proc MPageIniSave {} {" << endl;
    mOut << "	global MPage_Count_New MPage_Count_Old_Clip rbc" << endl;
    mOut << "	set inidata \"\"" << endl;
    mOut << "	set n 0" << endl;
    mOut << "	set fd [ open " << mDefFile << ".default r ]" << endl;
    mOut << "	while { ! [ eof $fd ] } {" << endl;
    mOut << "		incr n" << endl;
    mOut << "		if { $n < " << unitnum  << " } {" << endl;
    mOut << "			set line [gets $fd]" << endl;
    mOut << "			set inidata [concat $inidata $line]" << endl;
    mOut << "		} else {" << endl;
    mOut << "			break" << endl;
    mOut << "		}" << endl;
    mOut << "	}" << endl;
    mOut << "	close $fd" << endl;
    mOut << "	set inidata [concat $inidata $MPage_Count_New]" << endl;
    mOut << "	set inidata [concat $inidata $rbc]" << endl;
    mOut << "	if {$rbc == \"y\"} {" << endl;
    mOut << "		set inidata [concat $inidata $MPage_Count_Old_Clip]" << endl;
    mOut << "	}" << endl;
    mOut << "	set fd [ open " << mDefFile << ".default w ] " << endl;
    mOut << "	foreach i $inidata { " << endl;
    mOut << "		puts $fd $i " << endl;
    mOut << "	} " << endl;
    mOut << "	close $fd" << endl;
    mOut << "}" << endl;
}
void FUGenerator::outputReg() const
{
	FRSListIterator<FUPage*> page;
	int ui = 0;

	mOut << endl;
	mOut << "proc MPageReg { object } {" << endl; 

	for (page = mPageList; page ; page++) {
		page.get()->outGlobal(ui,mOut);
	}

//	mOut << "        global r10 g10 b10" << endl; 

	mOut << "	if { [MPageErrorCheck] } {" << endl; 
	mOut << "		return" << endl; 
	mOut << "	}" << endl; 
	mOut << "	set new_data \"\"" << endl;  
	mOut << "	set n 1 " << endl; 

	mOut << "	while { $n < " << unitnum << " } {" << endl;  
	mOut << "		switch $n { " << endl; 

	for (page = mPageList, ui = 0; page ; page++) {
		page.get()->outRegSwitch(ui,mOut);
	}

//                        2       { set new_data [ concat $new_data $scale2 ] } 
//                        4       { set new_data [ concat $new_data $rb4 ] } 
//                        13      { set new_data [ concat $new_data $r13 ] } 
//                        14      { set new_data [ concat $new_data $g13 ] } 
//                        15      { set new_data [ concat $new_data $b13 ] } 
	mOut << "			default {puts \"error-->proc MPageReg\"; exit}" << endl; 
	mOut << "		}" << endl; 
    mOut << "		incr n" << endl;  
    mOut << "	}" << endl; 
	mOut << "	set fd [ open " << mDefFile << ".${object} w ]" << endl;  
    mOut << "	foreach i $new_data {" << endl; 
    mOut << "		puts $fd $i" << endl; 
    mOut << "	}" << endl; 
    mOut << "	close $fd" << endl; 
    mOut << "	MPageIniSave" << endl; 
    mOut << "	if { [ regexp history ${object} ] } {" << endl;  
    mOut << "		exit " << endl; 
    mOut << "	} else { " << endl; 
    mOut << "		.top.1.def_l configure -state normal" << endl; 
    mOut << "	}" << endl; 
    mOut << "}" << endl; 
}
void FUGenerator::outputOutput() const
{
	FRSListIterator<FUPage*> page;
	int ui = 0;

	mOut << endl;
	mOut << "proc MPageOutput { data } {" << endl;

	for (page = mPageList; page ; page++) {
		page.get()->outGlobal(ui,mOut);
	}

	mOut << "	set n 1" << endl;
	mOut << "	foreach i $data {" << endl;
	mOut << "		switch $n {" << endl;

	for (page = mPageList, ui = 0; page ; page++) {
		page.get()->outOutSwitch(ui,mOut);
	}

	mOut << "			" << ui+ 1 << " { break }" << endl;
	mOut << "			default {puts \"error-->proc MPageOutput\"; exit}" << endl;
	mOut << "		}" << endl;
	mOut << "		incr n" << endl;
	mOut << "	}" << endl;
	mOut << "}" << endl;
}
void FUGenerator::outputHelp() const
{
	mOut << endl;
	mOut << "proc MPageHelp { helpfile args } {" << endl;
	mOut << "	set HFile [ open $helpfile {RDONLY CREAT} ]" << endl;
	mOut << "	if { [ file size $helpfile ] == 0 } {" << endl;
	mOut << "		MPageDialog {Read Me File Error} \"Can not open $helpfile\"" << endl;
	mOut << "	} else {" << endl;
	mOut << "		set Comment [ read $HFile ]" << endl;
	mOut << "		set Font [ lindex $args 0 ]" << endl;
	mOut << "		if { $Font != "" } {" << endl;
	mOut << "			MPageDialog {Read Me} $Comment $Font" << endl;
	mOut << "		} else {" << endl;
	mOut << "			MPageDialog {Read Me} $Comment" << endl;
	mOut << "		}" << endl;
	mOut << "	}" << endl;
	mOut << "	close $HFile" << endl;
	mOut << "}" << endl;
}
void FUGenerator::outputGetDefault(int UnitFlag) const
{
	FRSListIterator<FUPage*> page;
	int ui = 0;

	mOut << endl;
	mOut << "proc MPageGetDefault {} {" << endl; 
	mOut << "	set def \"\"" << endl; 
	mOut << "	set n 1" << endl; 
	mOut << "	set cklist [ glob -nocomplain " << mDefFile << ".default ]" << endl;  
	mOut << "	if { [ lsearch -exact $cklist " << mDefFile << ".default ] != -1 } {" << endl;  
	mOut << "		set fd [ open " << mDefFile << ".default r ]" << endl; 
	mOut << "		while { ! [ eof $fd ] } {" << endl; 
	mOut << "			set line [ gets $fd ]" << endl; 
	mOut << "			set def [ concat $def $line ]" << endl; 
	mOut << "		}" << endl; 
	mOut << "		MPageOutput $def" << endl;

	if ( UnitFlag & 0x1 ) {
		for (page = mPageList, ui = 0; page ; page++) {
			page.get()->outColorIni(ui,mOut);
		}
	}

	mOut << "		close $fd" << endl; 
	mOut << "		MPageErrorCheck" << endl; 
	mOut << "	} else {" << endl; 
	mOut << "		set in_fd [ open " << mDefFile << ".history r ]" << endl;  
	mOut << "		set out_fd [ open " << mDefFile << ".default w ]" << endl; 
	mOut << "		puts $out_fd [ read $in_fd ]" << endl; 
	mOut << "		close $in_fd" << endl; 
	mOut << "		close $out_fd" << endl; 
	mOut << "		MPageCancel" << endl; 
	mOut << "	}" << endl; 
	mOut << "}" << endl; 
}
void FUGenerator::outputErrorOut(int pnum) const
{
	int i;
	mOut << endl;
	mOut << "proc MPageErrorOut ErrorNo {" << endl;
	for (i = 1; i <= pnum; i++){
		mOut << "	global MPage_frame_List" << i << endl;
	}
	mOut << "	set Errorlist \"\"" << endl;
	mOut << "	foreach Errno $ErrorNo {" << endl;

	for (i = 1; i <= pnum; i++){
		mOut << "		set nb 0" << endl;
		mOut << "		foreach fr $MPage_frame_List" << i << " {" << endl;
		mOut << "			incr nb " << endl;
		mOut << "			if { $Errno == $fr } {" << endl;
		mOut << "				set work \"page" << i << " ERROR  item$nb\\n\"" << endl;
		mOut << "				set Errorlist $Errorlist$work" << endl;
		mOut << "			}" << endl;
		mOut << "		}" << endl;
	}
	mOut << "	}" << endl;
	mOut << "	MPageDialog {ERROR} $Errorlist" << endl;
	mOut << "}" << endl;
}

void FUGenerator::outputErrorCheck() const
{
	FRSListIterator<FUPage*> page;
	int ui = 0;
	mOut << endl;
	mOut << "proc MPageErrorCheck {} {" << endl;

	for (page = mPageList; page ; page++) {
		page.get()->outGlobal(ui,mOut);
	}

	mOut << "	set ErrorNo \"\"" << endl;
	mOut << "	set n 1" << endl;
	mOut << "	while { $n < " << unitnum << " } {" << endl;
	mOut << "		switch $n {" << endl;

	for (page = mPageList, ui = 0; page ; page++) {
		page.get()->outErrorSwitch(ui,mOut);
	}

	mOut << "			default {}" << endl;
//	mOut << "			default {puts \"error-->proc MPageErrorCheck\"; exit}" << endl;
	mOut << "		}" << endl;
	mOut << "		incr n" << endl;
	mOut << "	}" << endl;
	mOut << "	if { $ErrorNo != \"\" } {" << endl;
	mOut << "		MPageErrorOut $ErrorNo" << endl;
	mOut << "		return 1" << endl;
	mOut << "	} else {" << endl;
	mOut << "		return 0" << endl;
	mOut << "	}" << endl;
	mOut << "}" << endl;
}

void FUGenerator::outputDialog() const
{
	mOut << endl;
	mOut << "proc MPageDialog { Name Comment args } {" << endl;
	mOut << "	global bgcolor" << endl;
	mOut << "	if { [winfo exists .dialog] } {" << endl;
	mOut << "		destroy .dialog" << endl;
	mOut << "	}" << endl;
	mOut << "	toplevel .dialog" << endl;
	mOut << "	wm title .dialog $Name" << endl;

	mOut << "	set Font [ lindex $args 0 ]" << endl;

	mOut << "	frame .dialog.top -relief raised -bd 2 -bg $bgcolor" << endl;
	mOut << "	frame .dialog.bottom -relief raised -bd 2 -bg $bgcolor" << endl;

	mOut << "	if { $Font == \"\" } {" << endl;
	mOut << "		text .dialog.top.text -wrap word -height 15 -width 25 -yscrollcommand \".dialog.top.scroll set\" -highlightthickness 0" << endl;
	mOut << "	} else {" << endl;
	mOut << "		text .dialog.top.text -wrap word -height 15 -width 25 -yscrollcommand \".dialog.top.scroll set\" -font $Font -highlightthickness 0" << endl;
	mOut << "	}" << endl;
	mOut << "	.dialog.top.text insert end $Comment" << endl;

	mOut << "	scrollbar .dialog.top.scroll -command \".dialog.top.text yview\" -highlightthickness 0" << endl;

	mOut << "	if { $Font == \"\" } {" << endl;
	mOut << "		button .dialog.bottom.button -text OK -command { destroy .dialog } -highlightthickness 0" << endl;
	mOut << "	} else {" << endl;
	mOut << "		button .dialog.bottom.button -text OK -command { destroy .dialog } -font $Font -highlightthickness 0" << endl;
	mOut << "	}" << endl;
	mOut << "	pack .dialog.top -side top -fill both -expand 1" << endl;
	mOut << "	pack .dialog.bottom -side bottom -fill x" << endl;

	mOut << "	pack .dialog.top.scroll	-side right -fill y" << endl;
	mOut << "	pack .dialog.top.text -side left -fill both -expand 1" << endl;
	mOut << "	pack .dialog.bottom.button -side left -expand 1 -padx 10 -pady 5 -ipadx 5 -ipady 2" << endl;
	mOut << "}" << endl;
}
void FUGenerator::outputIDCheck() const
{
	mOut << "proc IDCHECK String {" << endl;
//////////////// by Tanaka
//	mOut << "	if { [regexp {^[+-]?[0-9]+$} $String] || [regexp {^[+-]?[0-9]+\\.$} $String] || [regexp {^[+-]?[0-9]*\\.[0-9]+$} $String] || [regexp {^[+-]?[0-9]+\\.?[eE][+-]?[0-9]+$} $String] || [regexp {^[+-]?[0-9]*\\.[0-9]+[eE][+-]?[0-9]+$} $String] } {" << endl;
//	mOut << "		return 0" << endl;
//	mOut << "	} else {" << endl;
//	mOut << "		return 1" << endl;
//	mOut << "	}" << endl;
/////////////////////////////////////

	mOut << "	return 0" << endl;
	mOut << "}" << endl;
}

int FUGenerator::scalewidth() const
{
	int pagenum = mPageList.numberOfNodes();
	int width, scmax = 0;
	if (pagenum == 0) { //error
		cerr << "error: no page data." << endl;
	} else {
		FRSListIterator<FUPage*> page;
		for (page = mPageList; page; page++) {
			FRSListIterator<FUFrame*> frame = page.get()->mFrameList;
			for ( ; frame; frame++) {
				width = frame.get()->scalewidth();
				//cout << "frame.get()->scalewidth()" << width << endl; 
				if(scmax < width){
					//cout << "SCMax=" << width << endl;
					scmax = width;
				}
			}
		}
	}
	return(scmax);
}


void FUGenerator::output() const
{
	int pagenum = mPageList.numberOfNodes();
		
	if (pagenum == 0) { //error
		cerr << "error: no page data." << endl;
	} else {
		int pn = 1, fnum = 1, pnum = 0, UnitFlag = 0;
//		int texnum, utexnum;
		int texnum;
		SCWidth = scalewidth();		
		outputHeader(); //header(tcl)
			
		FRSListIterator<FUPage*> page;
		fWidth = 0;
		for (page = mPageList; page; page++) {
			FRSListIterator<FUFrame*> frame = page.get()->mFrameList;
			for ( ; frame; frame++) {
//				FRSListIterator<FUUnit*> unit = frame.get()->mUnitList;
				texnum = frame.get()->getLabel().size() - 1;
				//cout << "width=" << texnum << "  ";
				if(fWidth < texnum){
					//cout << "max=" << texnum << "  ";
					fWidth = texnum;
				}
//					utexnum =unit.get()->getLabel().size() - 1;
//					cout << "uwidth=" << utexnum << endl;
//				}
			}
		}
		fWidth -= FrameWIDTH_SUB;
		if (fWidth < 0) fWidth = 0;
//		uWidth -= UnitWIDTHSUB;
//		if (uWidth < 0) fWidth = 0;
		for (page = mPageList; page; page++, pn++) {
			FRSListIterator<FUFrame*> frame = page.get()->mFrameList;
			mOut << "set MPage_frame_List" << pn << " {";
			for ( ; frame; frame++) {
				FRSListIterator<FUUnit*> unit = frame.get()->mUnitList;
				mOut << fnum << " ";
				for ( ; unit; unit++) {
					fnum += unit.get()->DataQuantity();
//					fnum += frame.get()->mUnitList.numberOfNodes();
				}
			}
			mOut << "}" << endl;
		}
			
		mOut << "set MPage_Name_List \"";
		for (page = mPageList; page; page++) {
			mOut << "{" << page.get()->getLabel() << "} ";
		}
		mOut << "\"" << endl;
		
		outputButtons();
		outputTitle();
		for (page = mPageList; page ; page++) {
			pnum++;
			mOut << endl;
			mOut << "####Page" << pnum <<endl;
			page.get()->output(mOut,UnitFlag);
		}
		outputMenuButton();
		mOut << endl;
		mOut << "###Procedures" << endl;
			
		if ( UnitFlag & 0x10 ) {
			outputSelectEntry();	//entry
		}
		if ( UnitFlag & 0x1 ) {
			outputSetColor();		//color
		}
		outputCancel(UnitFlag);
		outputReg();
		outputOutput();
		outputGetDefault(UnitFlag);
		outputPrev();
		outputNext();
		outputJump(pnum);
		outputClip(pnum);
		outputErrorOut(pnum);
		outputErrorCheck();
		outputIDCheck();		//IDerrorcheck
		outputHelp();
		outputDialog();
		outputIniLoad();
		outputIniSave();
		outputIni();
	}
}
