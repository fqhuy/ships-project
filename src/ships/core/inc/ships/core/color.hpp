#ifndef __SHIPS_CORE_COLOR_HPP__
#define __SHIPS_CORE_COLOR_HPP__

using namespace std;

namespace Sp {
class ICC_Profile {

public:

	ICC_Profile() {
	}

	ICC_Profile(long ID) {
		ID_ = ID;
	}

private:
	int ID_;
};

class ICC_ProfileRGB: public ICC_Profile {
};

class ICC_ProfileGray: public ICC_Profile {
};

class ColorModel {
public:
	ColorModel();

	virtual ~ColorModel() {
	}
	;
protected:

private:
};

class ColorSpace {
public:
	ColorSpace();
	ColorSpace(int type, int num_components);
	virtual ~ColorSpace()=0;
	static ColorSpace* GetInstance(int color_space);
private:
	int type_;
	int num_components_;
	string* component_names_;

	// Cache of singletons for the predefined color spaces.
	static ColorSpace* sRGB_space_;
	static ColorSpace* XYZ_space_;
	static ColorSpace* PYCC_space_;
	static ColorSpace* GRAY_space_;
	static ColorSpace* LINEAR_RGB_space_;
};

class ICC_ColorSpace: public ColorSpace {
public:
	ICC_ColorSpace();
	ICC_ColorSpace(const ICC_Profile& color_profile);

	virtual ~ICC_ColorSpace() {
	}
	;

	ICC_Profile& GetColorProfile() {
		return color_profile_;
	}
protected:
private:
	ICC_Profile color_profile_;
};

}
#endif
