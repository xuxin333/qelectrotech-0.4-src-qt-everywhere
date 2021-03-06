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
#include "reportelement.h"
#include "elementtextitem.h"
#include "diagramposition.h"
#include "qetproject.h"
#include "diagram.h"

ReportElement::ReportElement(const ElementsLocation &location, QString link_type,QGraphicsItem *qgi, int *state) :
	CustomElement(location, qgi, state),
	m_text_field (NULL)
{	
		/*
		 * Get text tagged label. This is work for report
		 * create after the revision 3559.
		 * for report create before, we take the first text field
		 * because report haven't got a text field tagged label
		 */
	m_text_field = taggedText("label");
	if (!m_text_field && !texts().isEmpty())
		m_text_field = texts().first();
	if (m_text_field)
		m_text_field -> setNoEditable();

	link_type == "next_report"? link_type_=NextReport : link_type_=PreviousReport;
	link_type == "next_report"? inverse_report=PreviousReport : inverse_report=NextReport;
}

ReportElement::~ReportElement() {
	unlinkAllElements();
}

/**
 * @brief ReportElement::linkToElement
 * Link this element to the other element
 * @param elmt
 * element to be linked with this
 */
void ReportElement::linkToElement(Element * elmt)
{
	if (!diagram() && !elmt -> diagram())
	{
		qDebug() << "ReportElement : linkToElement : Unable to link this or element to link isn't in a diagram";
		return;
	}

		//ensure elmt isn't already linked
	bool i = true;
	if (!this -> isFree())
	{
		if (connected_elements.first() == elmt) i = false;
	}

		//ensure elmt is an inverse report of this element
	if ((elmt->linkType() == inverse_report) && i)
	{
		unlinkAllElements();
		connected_elements << elmt;

		connect(elmt,                   SIGNAL( xChanged() ),                                       this, SLOT( updateLabel()     ));
		connect(elmt,                   SIGNAL( yChanged() ),                                       this, SLOT( updateLabel()     ));
		connect(diagram(),              SIGNAL( reportPropertiesChanged(QString) ),                 this, SLOT( setLabel(QString) ));
		connect(diagram() -> project(), SIGNAL( projectDiagramsOrderChanged(QETProject*,int,int) ), this, SLOT( updateLabel()     ));

		label_ = diagram() -> defaultReportProperties();
		updateLabel();

		elmt -> linkToElement(this);
	}
}

/**
 * @brief ReportElement::unLinkAllElements
 * Unlink all of the element in the QList connected_elements
 */
void ReportElement::unlinkAllElements()
{
	if (!isFree()){
		QList <Element *> tmp_elmt = connected_elements;

		foreach(Element *elmt, connected_elements)
		{
			disconnect(elmt, SIGNAL(xChanged()), this, SLOT(updateLabel()));
			disconnect(elmt, SIGNAL(yChanged()), this, SLOT(updateLabel()));
			disconnect(diagram()->project(), SIGNAL(projectDiagramsOrderChanged(QETProject*,int,int)), this, SLOT(updateLabel()));
		}
		connected_elements.clear();
		updateLabel();

		foreach(Element *elmt, tmp_elmt)
		{
			elmt -> setHighlighted(false);
			elmt -> unlinkAllElements();
		}
	}
}
/**
 * @brief ReportElement::unlinkElement
 *unlink the specified element.
 *for reportelement, they must be only one linked element, so we call
 *unlinkAllElements for clear the connected_elements list.
 * @param elmt
 */
void ReportElement::unlinkElement(Element *elmt) {
	Q_UNUSED (elmt);
	unlinkAllElements();
}

/**
 * @brief ReportElement::setLabel
 * Set new label and call updatelabel
 * @param label new label
 */
void ReportElement::setLabel(QString label) {
	label_ = label;
	updateLabel();
}

/**
 * @brief ReportElement::updateLabel
 * Update the displayed label.
 * ie the folio and position of the linked folio report
 */
void ReportElement::updateLabel()
{
	if (!m_text_field) return;

	if (!connected_elements.isEmpty())
	{
		Element *elmt = connected_elements.at(0);
		QString label = label_;
		label.replace("%f", QString::number(elmt->diagram()->folioIndex()+1));
		label.replace("%c", QString::number(elmt->diagram() -> convertPosition(elmt -> scenePos()).number()));
		label.replace("%l", elmt->diagram() -> convertPosition(elmt -> scenePos()).letter());
		m_text_field -> setPlainText(label);
	}
	else
	{
		m_text_field -> setPlainText("/");
	}
}
