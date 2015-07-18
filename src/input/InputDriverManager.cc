/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2015 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "InputDriverManager.h"

#include "printutils.h"

InputDriverManager * InputDriverManager::self = 0;

InputDriverManager::InputDriverManager()
{
    currentWindow = 0;
    connect(QApplication::instance(), SIGNAL(focusChanged(QWidget *, QWidget *)), this, SLOT(onFocusChanged(QWidget *, QWidget *)));
}

InputDriverManager::~InputDriverManager()
{

}

InputDriverManager * InputDriverManager::instance()
{
    if (!self) {
        self = new InputDriverManager();
    }
    return self;
}

void InputDriverManager::registerDriver(InputDriver *driver)
{
    this->drivers.push_back(driver);
}

void InputDriverManager::unregisterDriver(InputDriver *driver)
{
    this->drivers.remove(driver);
}

void InputDriverManager::init()
{
    for (drivers_t::iterator it = drivers.begin();it != drivers.end();it++) {
        PRINTB("trying to open %s\n", (*it)->get_name().c_str());
        if ((*it)->open()) {
            PRINTB("using %s\n", (*it)->get_name().c_str());
            break;
        }
    }
}

void InputDriverManager::postEvent(InputEvent *event, bool activeOnly)
{
    QWidget *window = activeOnly ? QApplication::activeWindow() : currentWindow;
    if (window) {
        QCoreApplication::postEvent(window, event);
    }
}

void InputDriverManager::onFocusChanged(QWidget *, QWidget *current)
{
    if (current) {
        currentWindow = dynamic_cast<MainWindow *>(current->window());
    }
}
