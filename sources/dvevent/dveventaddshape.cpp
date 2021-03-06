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
#include "dveventaddshape.h"
#include "diagramview.h"
#include "qetshapeitem.h"
#include "diagram.h"
#include "diagramcommands.h"
#include <QMouseEvent>

/**
 * @brief DVEventAddShape::DVEventAddShape
 * Default constructor
 * @param dv, the diagram view where operate this event
 * @param shape_type, the shape type to draw
 */
DVEventAddShape::DVEventAddShape(DiagramView *dv, QetShapeItem::ShapeType shape_type) :
	DVEventInterface(dv),
	m_shape_type (shape_type),
	m_shape_item (NULL),
	m_help_horiz (NULL),
	m_help_verti (NULL)
{
	m_dv -> setContextMenuPolicy(Qt::NoContextMenu);
}

/**
 * @brief DVEventAddShape::~DVEventAddShape
 */
DVEventAddShape::~DVEventAddShape()
{
	if (m_running || m_abort)
	{
		m_diagram -> removeItem(m_shape_item);
		delete m_shape_item;
	}
	delete m_help_horiz;
	delete m_help_verti;
	m_dv -> setContextMenuPolicy(Qt::DefaultContextMenu);
}

/**
 * @brief DVEventAddShape::mousePressEvent
 * Action when mouse is pressed
 * @param event : event of mouse press
 * @return : true if this event is managed, otherwise false
 */
bool DVEventAddShape::mousePressEvent(QMouseEvent *event)
{
	if (!m_dv->isInteractive() && m_diagram->isReadOnly()) return false;

	QPointF pos = m_dv->mapToScene(event->pos());

		//Action for left mouse click
	if (event -> button() == Qt::LeftButton)
	{
			//Create shape item
		if (!m_shape_item)
		{
			m_shape_item = new QetShapeItem(pos, pos, m_shape_type);
			m_diagram -> addItem (m_shape_item);
			m_running = true;
			return true;
		}

			//If current item isn't a polyline, add it with an undo command
		if (m_shape_type != QetShapeItem::Polyline)
		{
			m_shape_item -> setP2 (pos);
			m_diagram -> undoStack().push (new AddItemCommand<QetShapeItem *> (m_shape_item, m_diagram));
			m_shape_item = NULL; //< set to NULL for create new shape at next left clic
		}
			//Else add a new point to polyline
		else
		{
			m_shape_item -> setNextPoint (pos);
		}

		return true;
	}

	if (event -> button() == Qt::RightButton)
		return true;

	return false;
}

/**
 * @brief DVEventAddShape::mouseMoveEvent
 * Action when mouse move
 * @param event : event of mouse move
 * @return : true if this event is managed, otherwise false
 */
bool DVEventAddShape::mouseMoveEvent(QMouseEvent *event)
{
	updateHelpCross(event->pos());
	if (!m_running) return false;

	if (m_shape_item && event -> buttons() == Qt::NoButton)
	{
		m_shape_item -> setP2 (m_dv -> mapToScene (event -> pos()));
		return true;
	}

	return false;
}

/**
 * @brief DVEventAddShape::mouseReleaseEvent
 * Action when mouse button is released
 * @param event : event of mouse release
 * @return : true if this event is managed, otherwise false
 */
bool DVEventAddShape::mouseReleaseEvent(QMouseEvent *event)
{
	if (event -> button() == Qt::RightButton)
	{
			//If shape is created, we manage right click
		if (m_shape_item)
		{
				//Shape is a polyline and have three points or more we just remove the last point
			if (m_shape_type == QetShapeItem::Polyline && (m_shape_item -> pointsCount() >= 3) )
			{
				m_shape_item -> removePoints();
				m_shape_item -> setP2(m_dv -> mapToScene (event -> pos())); //Set the new last point under the cursor
				return true;
			}

				//For other case, we remove item from scene
			m_diagram -> removeItem(m_shape_item);
			delete m_shape_item;
			m_shape_item = NULL;
			return true;
		}

			//Else (no shape), we set to false the running status
			//for indicate to the owner of this event that everything is done
		m_running = false;
		return true;
	}

	return false;
}

/**
 * @brief DVEventAddShape::mouseDoubleClickEvent
 * @param event
 * @return
 */
bool DVEventAddShape::mouseDoubleClickEvent(QMouseEvent *event)
{
		//If current item is a polyline, add it with an undo command
	if (m_shape_item && m_shape_type == QetShapeItem::Polyline && event -> button() == Qt::LeftButton)
	{
		m_shape_item -> setP2 (m_dv -> mapToScene (event -> pos()));
		m_diagram -> undoStack().push (new AddItemCommand<QetShapeItem *> (m_shape_item, m_diagram));
		m_shape_item = NULL; //< set to NULL for create new shape at next left clic
		return true;
	}

	return false;
}

/**
 * @brief DVEventAddShape::updateHelpCross
 * Create and update the position of the cross to help user for draw new shape
 * @param event
 */
void DVEventAddShape::updateHelpCross(const QPoint &p)
{
		//If line isn't created yet, we create it.
	if (!m_help_horiz || !m_help_verti)
	{
		QPen pen;
		pen.setWidthF(0.4);
		pen.setCosmetic(true);
		pen.setColor(Qt::darkGray);

			//Add +5 for each line, because the topleft of diagram isn't draw at position (0:0) but (5:5)
		if (!m_help_horiz)
		{
			m_help_horiz = new QGraphicsLineItem(m_diagram -> border_and_titleblock.rowsHeaderWidth() + 5, 0,
												 m_diagram -> border_and_titleblock.diagramWidth() + 5, 0,
												 0, m_diagram);
			m_help_horiz->setPen(pen);
		}

		if (!m_help_verti)
		{
			m_help_verti = new QGraphicsLineItem(0, m_diagram -> border_and_titleblock.columnsHeaderHeight() + 5,
												 0, m_diagram -> border_and_titleblock.diagramHeight() + 5,
												 0, m_diagram);
			m_help_verti->setPen(pen);
		}
	}

		//Update the position of the cross
	QPointF point = Diagram::snapToGrid(m_dv->mapToScene(p));

	m_help_horiz->setY(point.y());
	m_help_verti->setX(point.x());
}
