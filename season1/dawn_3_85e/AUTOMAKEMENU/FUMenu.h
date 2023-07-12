#ifndef FU_MENU
#define FU_MENU

#include <stdio.h>
#include <fstream.h>
#include "FRString.h"
#include "FRSList.h"

class FUFrame;
class FUUnit;

class FUString : public FRString
{
private:
public:
	FUString():FRString() {}
	FUString(const FRString& str):FRString(str) {}
	FUString(const FUString& str):FRString(str.mString) {}
	FUString(const char* str):FRString(str) {}
	FUString(char c):FRString(c) {}
	FUString(int i):FRString() {
		GetBuffer(32);
		sprintf(mString, "%d", i);
		ReleaseBuffer();
	}
	FUString(double d):FRString() {
		GetBuffer(32);
		sprintf(mString, "%f", d);
		ReleaseBuffer();
	}

	~FUString(){}
};

enum FUStringType {
	FUST_STRING,
	FUST_REGEXP,
	FUST_DOUBLE,
	FUST_INT,
	FUST_UINT
};

class FUPage
{
private:
	FRString mName;
public:
	FRSList<FUFrame*> mFrameList;
	
	FUPage(const FRString& name = "") { mName = name; }
	~FUPage() {}

	void append(FUFrame* w){ mFrameList.append(w); }

	void setLabel(const FRString& name = "") { mName = name; }
	const FRString& getLabel() const { return mName; }

//protected:
	void output(ostream&, int&) const;
	void outGlobal(int&, ostream) const;
	void outRegSwitch(int&, ostream&) const;
	void outOutSwitch(int&, ostream&) const;
	void outErrorSwitch(int&, ostream&) const;
	void outColorSwitch(int&, ostream&) const;
	void outColorIni(int&, ostream&) const;
};


class FUFrame
{
private:
	FRString mLabel;
public:
	FRSList<FUUnit*> mUnitList;

	FUFrame() {mLabel = "";}
	FUFrame(const FRString& );
	~FUFrame() {}

	void append(FUUnit* w){ mUnitList.append(w); }
	void setLabel(FRString& str = ""){ mLabel = str; }
	const FRString& getLabel() const { return mLabel; }

//protected:
	int scalewidth() const ;
	void output(ostream&, int&) const;
	void outGlobal(int&, ostream) const;
	void outRegSwitch(int&, ostream&) const;
	void outOutSwitch(int&, ostream&) const;
	void outErrorSwitch(int&, ostream&) const;
	void outColorSwitch(int&, ostream&) const;
	void outColorIni(int&, ostream&) const;
};

class FUUnit
{
protected:
	FRString mLabel;
	FUString mValue;
	FUStringType mType;
public:
	FUUnit(){}
	FUUnit(const FRString& lbl){ mLabel = lbl; }
	virtual ~FUUnit(){}

	void setLabel(const FRString& str) { mLabel = str; }
	const FRString& getLabel() const { return mLabel; }

	const FUString& getValue() const { return mValue; }
	FUStringType getType() const { return mType; }

	virtual void setValue(const FRString& s, FRBoolean regexp = FRFalse)
	{ fprintf(stderr, "Warning! \'setValue(FRString = \"%s\", %d)\' Called no implement method!\n", (const char*)s, regexp);	}
	virtual void setValue(double d)
	{ fprintf(stderr, "Warning! \'setValue(double = %g)\' Called no implement method!\n", d); }
	virtual void setValue(int d)
	{ fprintf(stderr, "Warning! \'setValue(int = %d)\' Called no implement method!\n", d); }
	virtual void setValue(unsigned int d)
	{ fprintf(stderr, "Warning! \'setValue(unsigned int = %d)\' Called no implement method!\n", d); }
	virtual int DataQuantity() const = 0;
	virtual void output(ostream&, int&) const = 0;
	virtual void outGlobal(int&, ostream) const = 0;
	virtual void outRegSwitch(int&, ostream&) const = 0;
	virtual void outOutSwitch(int&, ostream&) const = 0;
	virtual void outErrorSwitch(int&, int, ostream&) const = 0;
	virtual void outColorSwitch(int&, ostream&) const = 0;
	virtual void outColorIni(int, int&, ostream&) const = 0;
	virtual int scwidth(int) const = 0;
};

class FUEntry : public FUUnit
{
public:
	FUEntry() : FUUnit(""){mValue = 0;}
	FUEntry(const FRString&);
	~FUEntry() {}

	void setValue(const FRString& str, FRBoolean regexp = FRFalse)
	{ mValue = str; if (regexp) mType = FUST_REGEXP; else mType = FUST_STRING; }
	void setValue(double d)	{ mValue = d; mType = FUST_DOUBLE; }
	void setValue(int d) { mValue = d; mType = FUST_INT; }
	void setValue(unsigned int d) { mValue = (signed)d; mType = FUST_UINT; }

protected:
	int DataQuantity() const { return 1;}
	
	void output(ostream&, int&) const;
	void outGlobal(int&, ostream) const;
	void outRegSwitch(int&, ostream&) const;
	void outOutSwitch(int&, ostream&) const;
	void outErrorSwitch(int&, int, ostream&) const;
	void outColorSwitch(int&, ostream&) const{}
	void outColorIni(int, int&, ostream&) const{}
	int scwidth(int dumy) const { return 0;dumy = 0;}
};

class FUScale : public FUUnit
{
private:
	double mMax, mMin, mStep;
public:
	FUScale() : FUUnit(""){ mMax = 1; mMin = 0; mStep = 0.1; mValue = 0; }
	FUScale(const FRString& );

	~FUScale() { }

	void setValue(double val) { mValue = val; }

	void setMax(double m) { mMax = m; }
	void setMin(double m) { mMin = m; }
	void setStep(double m) { mStep = m; }
	
	double getMax() { return mMax; }
	double getMin() { return mMin; }
	double getStep() { return mStep; }

protected:
	int DataQuantity() const { return 1;}

	void output(ostream&, int&) const;
	void outUnitHeader(ostream&) const;
	void outGlobal(int&, ostream) const;
	void outRegSwitch(int&, ostream&) const;
	void outOutSwitch(int&, ostream&) const;
	void outErrorSwitch(int&, int, ostream&) const;
	void outColorSwitch(int&, ostream&) const{}
	void outColorIni(int, int&, ostream&) const{}
	int scwidth(int) const ;
};

class FURadiobutton
{
protected:
	FUString mValue;
	FRString mLabel;
public:
	FURadiobutton(const FRString& , const FUString );
	~FURadiobutton() {}

	void setValue(const FUString& Val) { mValue = Val; }
	const FUString& getValue() const { return mValue; }
	void setLabel(const FRString& str) { mLabel = str; }
	const FRString& getLabel() const { return mLabel; }
//protected:
	void output(int, ostream&) const;
};

class FUBaseRadiobutton : public FUUnit
{
public:
	FRSList<FURadiobutton*> mRBList;

	FUBaseRadiobutton() {}
	~FUBaseRadiobutton() {}

	void setValue(const FUString& Val) {mValue = Val;}
	void append(FURadiobutton* w){ mRBList.append(w); }

protected:
	int DataQuantity() const { return 1;}
	void output(ostream&, int&) const;
	void outGlobal(int&, ostream) const;
	void outRegSwitch(int&, ostream&) const;
	void outOutSwitch(int&, ostream&) const;
	void outErrorSwitch(int&, int, ostream&) const {}
	void outColorSwitch(int&, ostream&) const{}
	void outColorIni(int, int&, ostream&) const{}
	int scwidth(int dumy) const { return 0;dumy = 0;}
};

class FUCheckbutton
{
protected:
	FUString mValue;
	FRString mLabel;
	FUString mONValue, mOFFValue;
public:
	FUCheckbutton(const FUString& str = "",const FUString Val = "",const FUString ONVal = "1",const FUString OFFVal = "0")
	{ mLabel = str; mValue = Val; mONValue = ONVal; mOFFValue = OFFVal; }
	~FUCheckbutton() {}

	void setValue(const FUString& Val) {mValue = Val;}
	const FUString& getValue() const { return mValue; }
	void setLabel(const FRString& str) { mLabel = str; }
	const FRString& getLabel() const { return mLabel; }

	void setONValue(const FUString& ONVal) {mONValue = ONVal;}
	void setOFFValue(const FUString& OFFVal) {mOFFValue = OFFVal;}
	FUString& getONValue(){return mONValue;}
	FUString& getOFFValue(){return mOFFValue;}

	void output(int, int, ostream&) const;
};

class FUBaseCheckbutton : public FUUnit
{
public:
	FRSList<FUCheckbutton*> mCBList;

	FUBaseCheckbutton() {}
	~FUBaseCheckbutton() {}

	void append(FUCheckbutton* w){ mCBList.append(w); }

protected:
	int DataQuantity() const;

	void output(ostream&, int&) const;
	void outGlobal(int&, ostream) const;
	void outRegSwitch(int&, ostream&) const;
	void outOutSwitch(int&, ostream&) const;

	void outErrorSwitch(int&, int, ostream&) const;
	void outColorSwitch(int& , ostream&) const;
	void outColorIni(int, int& , ostream&) const;
	int scwidth(int dumy) const { return 0;dumy = 0;}
};

class FUColor : public FUUnit
{
private:
	double mR, mG, mB, mMax, mMin, mStep;
public:
	FUColor()
	{ mR = 0; mG = 0; mB = 0; mMax = 1; mMin = 0; mStep = 0.01;}

	~FUColor() {}

	void setRValue(double val) { mR = val; }
	void setGValue(double val) { mG = val; }
	void setBValue(double val) { mB = val; }

	void setMax(double m) { mMax = m; }
	void setMin(double m) { mMin = m; }
	void setStep(double m) { mStep = m; }
	
	double getMax() { return mMax; }
	double getMin() { return mMin; }
	double getStep() { return mStep; }

	double getRValue() { return mR;}
	double getGValue() { return mG;}
	double getBValue() { return mB;}
	
protected:
	int DataQuantity() const { return 3;}

	void output(ostream&, int&) const;
	void outGlobal(int&, ostream) const;
	void outRegSwitch(int&, ostream&) const;
	void outOutSwitch(int&, ostream&) const;
	void outErrorSwitch(int&, int, ostream&) const;
	void outColorSwitch(int&, ostream&) const;
	void outColorIni(int, int&, ostream&) const;
	int scwidth(int) const;
};

class FUGenerator
{
private:
	FRSList<FUPage*> mPageList;
	FUString mFgColor, mBgColor, mDefFile;
	FUString mName;
	ostream& mOut;
public:
	FUGenerator(const FUString& name = "", ostream& out = cout) :mOut(out)
	{ mName = name; mFgColor = "#000000"; mBgColor = "#d9d9d9";
	  mDefFile = ".Def";}
	~FUGenerator(){ }

	void setFgColor(const FUString& str) { mFgColor = str; } // fore ground color set
	void setBgColor(const FUString& str) { mBgColor = str; } // back ground color set
	void setDefFile(const FUString& str) { mDefFile = str; } // ?.history ?.default filename set

	void append(FUPage* w){ mPageList.append(w); }
	void output() const;

protected:
	int scalewidth() const;
	void outputHeader() const;
	void outputButtons() const;
	void outputTitle() const;
	void outputIni() const;
	void outputPrev() const;
	void outputNext() const;
	void outputJump(int) const;
	void outputClip(int) const;
	void outputMenuButton() const;
	void outputCancel(int) const;
	void outputSelectEntry() const;
	void outputSetColor() const;
	void outputIniLoad() const;
	void outputIniSave() const;
	void outputReg() const;
	void outputOutput() const;
	void outputGetDefault(int) const;
	void outputErrorOut(int) const;
	void outputErrorCheck() const;
	void outputDialog() const;
	void outputHelp() const;
	void outputIDCheck() const;
};
#endif
