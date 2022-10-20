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

#include "common.h"

#include "../panel/ilxqtpanelplugin.h"
#include "../panel/pluginsettings.h"


#include <QAction>
#include <QDialog>
#include <QMenu>
#include <QObject>
#include <QString>
#include <QToolButton>
#include <QWidget>
#include <QWidgetAction>
#include <QKeyEvent>
#include <XdgMenu>

namespace GlobalKeyShortcut
{
    class Action;
}
class WingMenuWidget;

class WingMenuPlugin : public QObject, public ILXQtPanelPlugin
{
    Q_OBJECT
public:
    WingMenuPlugin(const ILXQtPanelPluginStartupInfo& startupInfo);
    ~WingMenuPlugin();

    virtual QWidget* widget() override { return mWidget; }
    virtual QString themeId() const override
    {
        return QStringLiteral("WingMenuPlugin");
    }

    virtual Flags flags() const override { return HaveConfigDialog; }
    QDialog* configureDialog() override;

    bool isSeparate() const override { return true; }
    bool isExpandable() const override { return false; }

    void realign() override;
    void showMenu();
    void hideMenu();
    const GlobalKeyShortcut::Action* shortcut() { return mShortcut; };

private:
    QToolButton* mWidget;
    GlobalKeyShortcut::Action* mShortcut;
    QMenu* mMenu;
    WingMenuWidget* mMenuWidget;
    QWidgetAction* mMenuAction;
    AppLayout::Layout mAppLayout;
    int mButtonSize;
    QString mMenuFile;
    XdgMenu* mXdgMenu;
    QTimer* mRebuildTimer;

    bool mShowIcon;
    QString mIcon;
    bool mShowText;
    QString mText;

    void settingsChanged() override;
    void setupShortcut();

    void showHideMenu();
    void buildMenu();
    void rebuildTimeout();
};

// ***************************************************

class WingMenuLibrary : public QObject, public ILXQtPanelPluginLibrary
{
    Q_OBJECT
        Q_PLUGIN_METADATA(IID "lxqt.org/Panel/PluginInterface/3.0")
        Q_INTERFACES(ILXQtPanelPluginLibrary)
public:
    ILXQtPanelPlugin* instance(const ILXQtPanelPluginStartupInfo& startupInfo) const
    {
        return new WingMenuPlugin(startupInfo);
    }
};
