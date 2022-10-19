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

#pragma once

#include <QStyledItemDelegate>

#include "common.h"

class ItemDescriptionDelegate : public QStyledItemDelegate
{
    using QStyledItemDelegate::QStyledItemDelegate;
public:
    void setAppLayout(AppLayout::Layout);

    AppLayout::Layout mAppLayout;
private:
    void paint(QPainter* painter,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
