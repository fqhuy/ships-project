#include <ships/core/core.hpp>

namespace Sp {

    ColorModel::ColorModel() {
    }

    ColorSpace::ColorSpace(int type, int num_components) {
        type_ = type;
        num_components_ = num_components;
    }

    ColorSpace* ColorSpace::GetInstance(int color_space) {
        ColorSpace* cp;
        /*    switch (color_space) {
            case CS_sRGB:
                if (sRGB_space_ == NULL) {
                    ICC_Profile p = ICC_Profile.GetInstance(CS_sRGB);
                    sRGB_space_ = new ICC_ColorSpace (p);
                }
                cp = sRGB_space_;
                break;
            default:
                break;
            }
         */
        return cp;
    }

    /*
    ICC_ColorSpace::ICC_ColorSpace() {}
    ICC_ColorSpace::ICC_ColorSpace(const ICC_Profile& color_profile) {}
     */
    /*ICC_Profile& ICC_Profile::GetInstance(int color_space) {
        ICC_Profile p = NULL;
        switch (color_space) {
            case ColorSpace::CS_sRGB:
                break;
        }
    }
     */
}
