/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt-project.org/
 *
 * Copyright: 2022 LXQt team
 *
 * Authors:
 *   elviosak <https://github.com/elviosak>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "applicationsview.h"

#include <QMenu>
#include <QProxyStyle>
#include <QScrollBar>
#include <QStandardItemModel>
#include <XdgDesktopFile>
#include <XdgIcon>

#include <QDebug>

class SingleActivateStyle : public QProxyStyle
{
public:
    using QProxyStyle::QProxyStyle;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override
    {
        if (hint == QStyle::SH_ItemView_ActivateItemOnSingleClick)
            return 1;
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};

ApplicationsView::ApplicationsView(int iconSize, AppLayout::Layout appLayout, QWidget* parent)
    : QListView(parent), mIconSize(iconSize),
    mAppLayout(appLayout),
    mDelegate(new ItemDescriptionDelegate),
    mDidDrag(false)
{
    setMouseTracking(true);
    setDragDropMode(QListView::DragOnly);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(ScrollMode::ScrollPerPixel);
    setDefaultDropAction(Qt::MoveAction);
    setIconSize(QSize(mIconSize, mIconSize));
    setAppLayout(mAppLayout);
    setItemDelegate(mDelegate);
}

void ApplicationsView::setAppLayout(AppLayout::Layout layout)
{
    int w = 32;
    switch (layout) {
    case AppLayout::ListNameAndDescription:
        setIconSize(QSize(mIconSize, mIconSize));
        setGridSize(QSize());
        setViewMode(QListView::ViewMode::ListMode);
        setWordWrap(false);
        w = mIconSize + fontMetrics().averageCharWidth() * 30 + 4;
        break;
    case AppLayout::ListNameOnly:
        setIconSize(QSize(mIconSize, mIconSize));
        setGridSize(QSize());
        setViewMode(QListView::ViewMode::ListMode);
        setWordWrap(false);
        w = mIconSize + fontMetrics().averageCharWidth() * 30 + 4;
        break;
    case AppLayout::Icons:
        QSize grid;
        auto fm = fontMetrics();
        QSize icon(mIconSize * 2, mIconSize * 2);
        setIconSize(icon);
        int textWidth = fm.averageCharWidth() * 13;
        int textHeight = fm.lineSpacing() * 3;
        grid.setWidth(qMax(icon.width(), textWidth) + 4); // a margin of 2 px for selection rect
        grid.setHeight(icon.height() + textHeight + 8);   // a margin of 2 px for icon and 2px for text
        setGridSize(grid);
        setViewMode(QListView::ViewMode::IconMode);
        setWordWrap(true);
        w = grid.width() * 3 + verticalScrollBar()->sizeHint().width() + 16;
        setMovement(QListView::Movement::Static);
        break;
    }

    setMinimumWidth(w);
    mAppLayout = layout;
    mDelegate->setAppLayout(layout);
}

void ApplicationsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        mDragStartPosition = event->pos();
        mDidDrag = false;
    }

    QListView::mousePressEvent(event);
}

void ApplicationsView::mouseReleaseEvent(QMouseEvent* e)
{
    if ((e->button() == Qt::LeftButton) && (e->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance() && !mDidDrag) {

        auto index = indexAt(mDragStartPosition);
        auto index2 = indexAt(e->pos());
        if (index == index2)
            emit indexClicked(index);
    }
}

void ApplicationsView::dragEnterEvent(QDragEnterEvent* e)
{
    QAbstractItemView::dragEnterEvent(e);
}
void ApplicationsView::dragMoveEvent(QDragMoveEvent* e)
{
    QAbstractItemView::dragMoveEvent(e);
}
void ApplicationsView::dragLeaveEvent(QDragLeaveEvent* e)
{
    QAbstractItemView::dragLeaveEvent(e);
}
void ApplicationsView::dropEvent(QDropEvent* e)
{
    QAbstractItemView::dropEvent(e);
}

void ApplicationsView::keyPressEvent(QKeyEvent* e) {
    // ignore everything and let event propagate to WingMenuMenu and then sent to WingMenuWidget
    // everything is handled in WingMenuWidget::onKeyPressed 
    e->ignore();
}

void ApplicationsView::mouseMoveEvent(QMouseEvent* event)
{
    // hover
    if (event->buttons() == Qt::NoButton) {
        auto index = indexAt(event->pos());
        if (index.isValid() && !selectedIndexes().contains(index)) {
            setCurrentIndex(index);
        }
        return;
    }
    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;
    mDidDrag = true;
    auto index = indexAt(mDragStartPosition);
    if (!index.isValid())
        return;

    QMimeData* mimeData = new QMimeData();

    QList<QUrl> urls;
    urls << QUrl::fromLocalFile(index.data(Qt::UserRole + 3).toString());

    mimeData->setUrls(urls);

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction | Qt::LinkAction);
}
