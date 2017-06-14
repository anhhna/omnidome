/* Copyright (c) 2014-2015 "Omnidome" by Michael Winkelmann
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

#include <omni/mapping/Interface.h>

#include <omni/input/List.h>
#include <omni/util.h>
#include <omni/serialization/PropertyMap.h>
#include <omni/visual/Scene.h>
#include <omni/visual/Shader.h>

namespace omni {
    namespace mapping {
        Interface::Interface(QObject* parent) :
          QObject(parent),
          transform_(new AffineTransform(this))
        {
            // Disable scaling and translation by default
            transform_->setTranslationEnabled(false);
        }

        Interface::~Interface()
        {}

        void Interface::initialize()
        {
            /// Make shader
            shader_.reset(new QOpenGLShaderProgram());
            shader_->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                             vertexShaderSourceCode());
            shader_->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                             fragmentShaderSourceCode());
            shader_->link();
        }

        void Interface::bind() {
            if (!shader_) initialize();

            if (shader_)
            {
                shader_->bind();
                shader_->setUniformValue("flip_horizontal", flipHorizontal_);
                shader_->setUniformValue("flip_vertical",   flipVertical_);

                if (isUVW()) {
                  float _size = scene_ ? scene_->size() : 1.0;
                    shader_->setUniformValue("bound_to_canvas", boundToCanvas_);
                    shader_->setUniformValue("matrix",          matrix());
                    shader_->setUniformValue("offset",          _size*matrix().column(3).toVector3D());
                }
            }
        }

        visual::Scene const* Interface::scene() const {
          return scene_;
        }

        void Interface::setScene(Scene const* _scene) {
          scene_ = _scene;
        }

        void Interface::bind(input::Interface const* _input, OutputMode _outputMode, bool _grayscale)
        {
            bind();
            if (shader_)
            {
                shader_->setUniformValue("output_mode",
                                                        util::enumToInt(
                                             _outputMode));

                shader_->setUniformValue("texture",   0);
                shader_->setUniformValue("texture_size",QVector2D(_input->width(),_input->height()));
                shader_->setUniformValue("gray_output", _grayscale);
                shader_->setUniformValue("transparency",GLfloat(1.0));
            }
        }

        void Interface::bind(input::Interface const* _input, float _transparency) {
            bind();
            if (shader_)
            {
                shader_->setUniformValue("output_mode",
                                                        util::enumToInt(OutputMode::MAPPED_INPUT));
                shader_->setUniformValue("texture",   0);
                shader_->setUniformValue("texture_size",QVector2D(_input->width(),_input->height()));
                shader_->setUniformValue("gray_output", false);
                shader_->setUniformValue("transparency",_transparency);
            }
        }

        void Interface::release()
        {
            if (shader_) shader_->release();
        }

        /// Return ref to affine transform
        AffineTransform* Interface::transform() const {
            return transform_;
        }

        QMatrix4x4 Interface::matrix() const {
            return transform_->matrix();
        }

        QString Interface::vertexShaderSourceCode() const
        {
            return visual::ShaderCompiler::compile(":/shaders/mapping/common.vert");
        }

        QString Interface::fragmentShaderSourceCode() const
        {
            return
                visual::ShaderCompiler::compile(":/shaders/mapping/Template.frag") +
                visual::ShaderCompiler::compile(QString(
                                    ":/shaders/mapping/") + getTypeId().str() +
                                ".frag");
        }

        OMNI_PROPERTY_RW_IMPL(Interface,bool,flipHorizontal,setFlipHorizontal)
        OMNI_PROPERTY_RW_IMPL(Interface,bool,flipVertical,setFlipVertical)
        OMNI_PROPERTY_RW_IMPL(Interface,bool,boundToCanvas,setBoundToCanvas)
    }
}
