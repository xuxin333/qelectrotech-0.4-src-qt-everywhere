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
#include "elementinfowidget.h"
#include "ui_elementinfowidget.h"
#include "qetapp.h"
#include "changeelementinformationcommand.h"
#include "diagram.h"
#include "elementinfopartwidget.h"
#include "element.h"

/**
 * @brief ElementInfoWidget::ElementInfoWidget
 * Constructor
 * @param elmt element to edit information
 * @param parent parent widget
 */
ElementInfoWidget::ElementInfoWidget(Element *elmt, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ElementInfoWidget),
	element_(elmt),
	elmt_info(elmt->elementInformations())
{
	ui->setupUi(this);
	buildInterface();
	fillInfo();
}

/**
 * @brief ElementInfoWidget::~ElementInfoWidget
 * Destructor
 */
ElementInfoWidget::~ElementInfoWidget()
{
	qDeleteAll(eipw_list);
	delete ui;
}

/**
 * @brief ElementInfoWidget::apply
 * Apply the new information with a new undo command (got with method associatedUndo)
 * pushed to the stack of element project.
 * Return true if new info change, else false.
 */
bool ElementInfoWidget::apply() {
	if (QUndoCommand *undo = associatedUndo()) {
		element_ -> diagram() -> undoStack().push(undo);
		return true;
	}
	return false;
}

/**
 * @brief ElementInfoWidget::associatedUndo
 * If the edited info is different of the actual element info,
 * return a QUndoCommand with the change.
 * If no change return NULL;
 * @return
 */
QUndoCommand* ElementInfoWidget::associatedUndo() const {
	DiagramContext new_info;
	DiagramContext old_info = element_ -> elementInformations();

	foreach (ElementInfoPartWidget *eipw, eipw_list) {
			//add value only if they're something to store
		if (!eipw->text().isEmpty())
			new_info.addValue(eipw->key(),
						eipw->text(),
						eipw->mustShow());
	}

	if (old_info != new_info) {
		return (new ChangeElementInformationCommand(element_, old_info, new_info));
	}
	return NULL;
}

/**
 * @brief ElementInfoWidget::buildInterface
 * Build the widget
 */
void ElementInfoWidget::buildInterface() {
	foreach (QString str, QETApp::elementInfoKeys()) {
		ElementInfoPartWidget *eipw = new ElementInfoPartWidget(str, QETApp::elementTranslatedInfoKey(str), this);
		ui->scroll_vlayout->addWidget(eipw);
		eipw_list << eipw;
	}
}

/**
 * @brief ElementInfoWidget::fillInfo
 * fill information fetch in elmt_info to the
 * corresponding line edit
 */
void ElementInfoWidget::fillInfo() {
	foreach (ElementInfoPartWidget *eipw, eipw_list) {

		eipw -> setText (elmt_info[eipw->key()].toString());
		eipw -> setShow (elmt_info.keyMustShow(eipw->key()));

		//If the current eipw is for label or comment and the text is empty
		//we force the checkbox to ckecked
		if (eipw -> key() == "label" || eipw -> key() == "comment") {
			if (elmt_info[eipw->key()].toString().isEmpty())
				eipw->setShow(true);
		}
		else //< for other eipw we hide the checkbox
			eipw->setHideShow(true);
	}
}
