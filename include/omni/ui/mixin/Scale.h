/* Copyright (c) 2014-2015 "Omnidome" by cr8tr
 * Dome Mapping Projection Software (http://omnido.me).
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OMNI_UI_MIXIN_SCALE_H_
#define OMNI_UI_MIXIN_SCALE_H_

#include <map>

namespace omni {
    namespace ui {
        namespace mixin {
        /// Scales a set of sliders and sets a common unit suffix
        template<typename SLIDER, typename SCALAR = float>
        class Scale {
        public:
            typedef SLIDER slider_type;
            typedef SCALAR scalar_type;

            /// Set scale and apply to sliders
            virtual void setScale(float _scale) {
                float _oldScale = scale_;
                scale_ = _scale;

                for (auto& _slider_info : sliders_) {
                    auto& _slider = _slider_info.first;
                    auto& _info = _slider_info.second;
                    auto _value = _slider->value() / _oldScale * scale_;
                    _slider->setRange(_info.min_ * scale_, _info.max_ * scale_);
                    _slider->setValue(_value);
                    _slider->setSingleStep(_info.singleStep_ * scale_);
                    _slider->setPageStep(_info.pageStep_ * scale_);
                    _slider->setPrecision(getPrecision(scale_));
                }
            }

            /// Return scale value
            float scale() const {
                return scale_;
            }

            QString suffix() const {
                return suffix_;
            }

            void setSuffix(QString const& _suffix) {
                suffix_ = _suffix;
                for (auto& _slider_info : sliders_) {
                    _slider_info->setSuffix(suffix_);
                }
            }

            void registerScaledSlider(slider_type* _slider) {
                if (sliders_.count(_slider)) return;
                sliders_[_slider] = SliderInfo(_slider->minimum(),_slider->maximum(),
                    _slider->pageStep(),_slider->singleStep());
            }

            void unregisterScaledSlider(slider_type* _slider) {
                if (!sliders_.count(_slider)) return;
                sliders_.erase(_slider);
            }

            static int getPrecision(float _scale) {
                if (_scale <= 0.1) {
                    return 4;
                }
                if (_scale <= 1) {
                    return 3;
                }
                if (_scale <= 10) {
                    return 2;
                }
                if (_scale <= 100) {
                    return 1;
                }
                return 2;
            }

        private:
            struct SliderInfo {
                SliderInfo() {}
                SliderInfo(float _min, float _max, float _pageStep, float _singleStep) :
                    min_(_min),
                    max_(_max),
                    pageStep_(_pageStep),
                    singleStep_(_singleStep) {}

                float min_, max_;
                float pageStep_, singleStep_;
            };

            QString suffix_ = "m";
            int precision_ = 2;
            float scale_ = 1.0;
            std::map<slider_type*,SliderInfo> sliders_;
        };
    }
    }
}

#endif /* OMNI_UI_MIXIN_SCALE_H_ */
