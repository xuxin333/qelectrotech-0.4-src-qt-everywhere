/*
	Copyright 2006-2015 The QElectroTech Team
	This file is part of QElectroTech.

	QElectroTech is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	QElectroTech is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with QElectroTech.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ELEMENTPROVIDER_H
#define ELEMENTPROVIDER_H

#include <QUuid>
#include <QList>

class QETProject;
class Diagram;
class Element;

/**
  this class can search in the given diagram or project some kind of element
  like 'folio report' or 'master' and return it.
  We can get element element with specific status like 'free'.
*/

class ElementProvider
{
	public:
	ElementProvider(QETProject *prj, Diagram *diagram=0);
	ElementProvider(Diagram *diag);
	QList <Element *> freeElement(const int filter) const;
	QList <Element *> fromUuids(QList <QUuid>) const;
	QList <Element *> find(const int filter) const;

	private:
	QList <Diagram *> diag_list;
};

#endif // ELEMENTPROVIDER_H
