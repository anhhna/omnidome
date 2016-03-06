/* Copyright (c) 2014-2016 "Omnidome" by cr8tr
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
#ifndef OMNI_SERIALIZATION_NODE_H_
#define OMNI_SERIALIZATION_NODE_H_

#include <memory>
#include <map>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QVariant>

namespace omni {
    namespace serialization {
        class Node {
        public:

        private:
            QByteArray data_;
            std::map<QString,std::unique_ptr<Node>> children_;
        }
    }
}

#endif /* OMNI_SERIALIZATION_NODE_H_ */
