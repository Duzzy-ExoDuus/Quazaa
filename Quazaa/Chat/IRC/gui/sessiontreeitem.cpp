/*
* Copyright (C) 2008-2013 J-P Nurmi <jpnurmi@gmail.com>
* Copyright (C) 2010-2013 SmokeX <smokexjc@gmail.com>
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

#include "sessiontreeitem.h"
#include "sessiontreewidget.h"

#include "quazaasettings.h"

SessionTreeItem::SessionTreeItem(WidgetIrcMessageView* view, QTreeWidget* parent) : QTreeWidgetItem(parent)
{
    d.view = view;
    d.alerted = false;
    d.highlighted = false;
    setText(0, view->receiver());
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled);
}

SessionTreeItem::SessionTreeItem(WidgetIrcMessageView* view, QTreeWidgetItem* parent) : QTreeWidgetItem(parent)
{
    d.view = view;
    d.alerted = false;
    d.highlighted = false;
    setText(0, view->receiver());
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
}

SessionTreeItem::~SessionTreeItem()
{
    if (SessionTreeItem* p = static_cast<SessionTreeItem*>(parent())) {
        p->d.alertedChildren.remove(this);
        p->d.highlightedChildren.remove(this);
    }

    if (SessionTreeWidget* tw = static_cast<SessionTreeWidget*>(treeWidget())) {
        tw->d.alertedItems.remove(this);
        tw->d.resetedItems.remove(this);
    }
}

Session* SessionTreeItem::session() const
{
    return d.view->session();
}

WidgetIrcMessageView* SessionTreeItem::view() const
{
    return d.view;
}

SessionTreeItem* SessionTreeItem::findChild(const QString& name) const
{
    for (int i = 0; i < childCount(); ++i)
        if (child(i)->text(0).compare(name, Qt::CaseInsensitive) == 0)
            return static_cast<SessionTreeItem*>(child(i));
    return 0;
}

QVariant SessionTreeItem::data(int column, int role) const
{
    if (role == Qt::ForegroundRole) {
        SessionTreeWidget* tw = static_cast<SessionTreeWidget*>(treeWidget());
        if (!d.view->isActive())
            return tw->statusColor(SessionTreeWidget::Inactive);
        if (d.alerted || (!isExpanded() && !d.alertedChildren.isEmpty()))
            return tw->currentAlertColor();
        if (d.highlighted || (!isExpanded() && !d.highlightedChildren.isEmpty()))
            return tw->statusColor(SessionTreeWidget::Highlight);
    }
    if (role == Qt::BackgroundRole) {
        return QColor(quazaaSettings.Chat.Colors.value(IrcColorType::Background));
    }
    if (role == Qt::DecorationRole) {
        switch (column)
            case 0:
            {
                switch (d.view->viewType()) {
                    case WidgetIrcMessageView::ServerView:
                        return QIcon(":/Resource/Network/Network.png");
                    case WidgetIrcMessageView::ChannelView:
                        return QIcon(":/Resource/Chat/Friends.png");
                    case WidgetIrcMessageView::QueryView:
                        return QIcon(":/Resource/Chat/Chat.png");
            }
        }
    }
    return QTreeWidgetItem::data(column, role);
}

bool SessionTreeItem::isAlerted() const
{
    return d.alerted;
}

void SessionTreeItem::setAlerted(bool alerted)
{
    if (d.alerted != alerted) {
        d.alerted = alerted;
        if (SessionTreeItem* p = static_cast<SessionTreeItem*>(parent())) {
            if (alerted)
                p->d.alertedChildren.insert(this);
            else
                p->d.alertedChildren.remove(this);
            if (!p->isExpanded())
                p->emitDataChanged();
        }
        emitDataChanged();
    }
}

bool SessionTreeItem::isHighlighted() const
{
    return d.highlighted;
}

void SessionTreeItem::setHighlighted(bool highlighted)
{
    if (d.highlighted != highlighted) {
        d.highlighted = highlighted;
        if (SessionTreeItem* p = static_cast<SessionTreeItem*>(parent())) {
            if (highlighted)
                p->d.highlightedChildren.insert(this);
            else
                p->d.highlightedChildren.remove(this);
            if (!p->isExpanded())
                p->emitDataChanged();
        }
        emitDataChanged();
    }
}

bool SessionTreeItem::operator<(const QTreeWidgetItem& other) const
{
    Q_ASSERT(parent() && other.parent());
    QVariantHash state = static_cast<SessionTreeWidget*>(treeWidget())->d.state;
    QStringList receivers = state.value(parent()->text(0)).toStringList();
    const int a = receivers.indexOf(text(0));
    const int b = receivers.indexOf(other.text(0));
    if (a == -1 && b != -1)
        return false;
    if (a != -1 && b == -1)
        return true;
    return a < b;
}
