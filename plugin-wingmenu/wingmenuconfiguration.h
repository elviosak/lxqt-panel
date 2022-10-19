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

#include <QAbstractButton>
#include <QDialog>
#include <QStandardItemModel>

#include "../panel/ilxqtpanelplugin.h"
#include "../panel/pluginsettings.h"

class XdgDesktopFile;
class XdgMenu;

namespace Ui
{
    class WingMenuConfiguration;
};

namespace GlobalKeyShortcut
{
    class Action;
}

class WingMenuConfiguration : public QDialog
{
    Q_OBJECT
public:
    explicit WingMenuConfiguration(
        PluginSettings& settings,
        GlobalKeyShortcut::Action* shortcut,
        XdgMenu* xdgMenu,
        QWidget* parent = nullptr);

    explicit WingMenuConfiguration(
        PluginSettings* settings,
        GlobalKeyShortcut::Action* shortcut,
        XdgMenu* xdgMenu,
        QWidget* parent = nullptr) : WingMenuConfiguration(
            *settings,
            shortcut,
            xdgMenu,
            parent) {};

    ~WingMenuConfiguration() = default;

    PluginSettings& settings() const;

protected slots:
    void loadSettings() const;
    void dialogButtonsAction(QAbstractButton* btn);

private:
    Ui::WingMenuConfiguration* ui;
    PluginSettings& mSettings;
    XdgMenu* mXdgMenu;
    GlobalKeyShortcut::Action* mShortcut;
    QStandardItemModel* mLeaveActionsModel;
    QString mDesktopFilesDir;

    void globalShortcutChanged(const QString& oldShortcut, const QString& newShortcut);
    void shortcutChanged(const QString& value);
    void shortcutReset();
    void chooseMenuFile();

    void actionActivated(const QModelIndex& index);
    QStandardItem* createItem(const QString& fileName) const;
    void copyDesktopFile(const QString& fileName);
    void loadLeaveActions() const;
    void saveLeaveActions();
    void customizeLeave(bool customize);
    void loadFromMenu();
    void addDesktopFile();
    void newAction();
    void editAction();
    void upAction();
    void downAction();
    void removeAction();
    void openEditDialog(const QString& fileName = QString());
    void saveDesktopFile(const QString& name, const QString& icon, const QString& exec, const QString& fileName);
    QString newFileName();
};
