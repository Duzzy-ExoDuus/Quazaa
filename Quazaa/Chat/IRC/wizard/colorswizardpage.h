/*
* Copyright (C) 2008-2011 J-P Nurmi jpnurmi@gmail.com
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef COLORSWIZARDPAGE_H
#define COLORSWIZARDPAGE_H

#include "ui_colorswizardpage.h"
#include "quazaasettings.h"

#include <QComboBox>
#include <QStyledItemDelegate>

enum Columns
{
	Message,
	Color
};

// SVG color keyword names provided by the World Wide Web Consortium
static const QStringList COLORS = QStringList()
	<< "aliceblue" << "antiquewhite" << "aqua" << "aquamarine" << "azure" << "beige" << "bisque"
	<< "black" << "blanchedalmond" << "blue" << "blueviolet" << "brown" << "burlywood" << "cadetblue"
	<< "chartreuse" << "chocolate" << "coral" << "cornflowerblue" << "cornsilk" << "crimson" << "cyan"
	<< "darkblue" << "darkcyan" << "darkgoldenrod" << "darkgray" << "darkgreen" << "darkgrey"
	<< "darkkhaki" << "darkmagenta" << "darkolivegreen" << "darkorange" << "darkorchid" << "darkred"
	<< "darksalmon" << "darkseagreen" << "darkslateblue" << "darkslategray" << "darkslategrey"
	<< "darkturquoise" << "darkviolet" << "deeppink" << "deepskyblue" << "dimgray" << "dimgrey"
	<< "dodgerblue" << "firebrick" << "floralwhite" << "forestgreen" << "fuchsia" << "gainsboro"
	<< "ghostwhite" << "gold" << "goldenrod" << "gray" << "grey" << "green" << "greenyellow"
	<< "honeydew" << "hotpink" << "indianred" << "indigo" << "ivory" << "khaki" << "lavender"
	<< "lavenderblush" << "lawngreen" << "lemonchiffon" << "lightblue" << "lightcoral" << "lightcyan"
	<< "lightgoldenrodyellow" << "lightgray" << "lightgreen" << "lightgrey" << "lightpink"
	<< "lightsalmon" << "lightseagreen" << "lightskyblue" << "lightslategray" << "lightslategrey"
	<< "lightsteelblue" << "lightyellow" << "lime" << "limegreen" << "linen" << "magenta"
	<< "maroon" << "mediumaquamarine" << "mediumblue" << "mediumorchid" << "mediumpurple"
	<< "mediumseagreen" << "mediumslateblue" << "mediumspringgreen" << "mediumturquoise"
	<< "mediumvioletred" << "midnightblue" << "mintcream" << "mistyrose" << "moccasin"
	<< "navajowhite" << "navy" << "oldlace" << "olive" << "olivedrab" << "orange" << "orangered"
	<< "orchid" << "palegoldenrod" << "palegreen" << "paleturquoise" << "palevioletred"
	<< "papayawhip" << "peachpuff" << "peru" << "pink" << "plum" << "powderblue" << "purple" << "red"
	<< "rosybrown" << "royalblue" << "saddlebrown" << "salmon" << "sandybrown" << "seagreen"
	<< "seashell" << "sienna" << "silver" << "skyblue" << "slateblue" << "slategray" << "slategrey"
	<< "snow" << "springgreen" << "steelblue" << "tan" << "teal" << "thistle" << "tomato"
	<< "turquoise" << "violet" << "wheat" << "white" << "whitesmoke" << "yellow" << "yellowgreen";

class ColorItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	ColorItemDelegate(QObject* parent = 0) : QStyledItemDelegate(parent)
	{
	}

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		Q_UNUSED(option);
		if (index.column() == Color)
		{
			QComboBox* comboBox = new QComboBox(parent);
			comboBox->addItems(COLORS);
			int i = 0;
			foreach (const QString& color, COLORS)
				comboBox->setItemData(i++, QColor(color), Qt::DecorationRole);
			connect(comboBox, SIGNAL(activated(int)), this, SIGNAL(colorChanged()));
			return comboBox;
		}
		return 0;
	}

	void setEditorData(QWidget* editor, const QModelIndex& index) const
	{
		if (index.column() == Color)
		{
			QComboBox* comboBox = static_cast<QComboBox*>(editor);
			int i = comboBox->findText(index.data().toString());
			comboBox->setCurrentIndex(i);
		}
	}

	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
	{
		if (index.column() == Color)
		{
			QComboBox* comboBox = static_cast<QComboBox*>(editor);
			model->setData(index, comboBox->currentText());
			model->setData(index, QColor(comboBox->currentText()), Qt::DecorationRole);
		}
	}

signals:
	void colorChanged();
};

class ColorsWizardPage : public QWizardPage
{
	Q_OBJECT

public:
	ColorsWizardPage(QWidget* parent = 0);

	QHash<int, QString> colors() const;
	void setColors(const QHash<int, QString>& colors);

private:
	Ui::ColorsWizardPage ui;
	ColorItemDelegate *m_oColorsDelegate;

signals:
	void settingsChanged();
};

#endif // COLORSWIZARDPAGE_H
