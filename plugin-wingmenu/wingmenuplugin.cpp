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

#include "wingmenuplugin.h"
#include "wingmenuconfiguration.h"
#include "wingmenuwidget.h"

#include <QIcon>
#include <QMessageBox>
#include <QTimer>
#include <QWidgetAction>

#include <lxqtglobalkeys.h>

WingMenuPlugin::WingMenuPlugin(const ILXQtPanelPluginStartupInfo& startupInfo)
    : QObject(),
    ILXQtPanelPlugin(startupInfo),
    mShortcut(nullptr),
    mMenu(nullptr),
    mMenuWidget(nullptr),
    mMenuAction(nullptr),
    mMenuFile(QString()),
    mXdgMenu(new XdgMenu),
    mRebuildTimer(new QTimer)
{
    mRebuildTimer->setSingleShot(true);
    mRebuildTimer->setInterval(200);
    auto logDir = settings()->value(QSL("logDir"), QString()).toString();
    mXdgMenu->setEnvironments(QStringList() << QSL("X-LXQT") << QSL("LXQt"));
    mXdgMenu->setLogDir(logDir);
    mWidget = new QToolButton;
    mWidget->setObjectName(QSL("MainMenu"));
    mWidget->setAutoRaise(true);
    mWidget->setMinimumWidth(1);
    mWidget->setMinimumHeight(1);

    connect(mWidget, &QToolButton::clicked, this, &WingMenuPlugin::showHideMenu);
    settingsChanged();
    setupShortcut();
    connect(mXdgMenu, &XdgMenu::changed, mRebuildTimer, QOverload<>::of(&QTimer::start));
    connect(mRebuildTimer, &QTimer::timeout, this, &WingMenuPlugin::rebuildTimeout);
}

WingMenuPlugin::~WingMenuPlugin() {
    delete mRebuildTimer;
    delete mXdgMenu;
}

void WingMenuPlugin::rebuildTimeout()
{
    buildMenu();
    mMenuWidget->settingsChanged();
}

void WingMenuPlugin::buildMenu()
{
    if (nullptr != mMenu)
        mMenu->deleteLater();

    if (nullptr != mMenuWidget)
        mMenuWidget->deleteLater();

    mMenu = new QMenu(mWidget);
    mMenuWidget = new WingMenuWidget(this, mXdgMenu);
    mMenu->setAttribute(Qt::WA_AlwaysShowToolTips);
    mMenu->setAttribute(Qt::WA_TranslucentBackground);
    mMenu->setFocusProxy(mMenuWidget);
    mMenuWidget->ensurePolished();

    mMenuAction = new QWidgetAction(mMenu);
    mMenuAction->setDefaultWidget(mMenuWidget);
    mMenu->addAction(mMenuAction);
    mMenu->ensurePolished();
    connect(mMenuWidget, &WingMenuWidget::hideMenu, this, &WingMenuPlugin::hideMenu);
    connect(mMenu, &QMenu::aboutToHide, mMenuWidget, &WingMenuWidget::onHide);
    connect(mMenu, &QMenu::aboutToShow, mMenuWidget, &WingMenuWidget::onShow);
}

void WingMenuPlugin::settingsChanged()
{
    mShowIcon = settings()->value(QSL("showIcon"), true).toBool();
    mIcon = settings()->value(QSL("icon"), QSL("wing-lxqt")).toString();
    mShowText = settings()->value(QSL("showText"), true).toBool();
    mText = settings()->value(QSL("text"), tr("Menu")).toString();
    auto appLayout = settings()->value(QSL("appLayout"), AppLayout::ListNameAndDescription).value<AppLayout::Layout>();
    auto menuFile = settings()->value(QSL("menuFile"), DEFAULT_MENU_FILE).toString();

    if (!mShowIcon && !mShowText) {
        mWidget->setIcon(QIcon());
        mWidget->setText(QString());
        mWidget->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    }
    else {
        mWidget->show();
        if (mShowIcon && mShowText) {
            mWidget->setIcon(XdgIcon::fromTheme(mIcon, QIcon(mIcon)));
            mWidget->setText(mText);
            mWidget->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextBesideIcon);
        }
        else if (mShowIcon) {
            mWidget->setIcon(XdgIcon::fromTheme(mIcon, QIcon(mIcon)));
            mWidget->setText(QString());

            mWidget->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
        }
        else if (mShowText) {
            mWidget->setIcon(QIcon());
            mWidget->setText(mText);
            mWidget->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextOnly);
        }
    }

    if ((mAppLayout != appLayout) || (mMenuFile != menuFile)) {
        if (mAppLayout != appLayout)
            mAppLayout = appLayout;
        if (mMenuFile != menuFile) {
            mMenuFile = menuFile;
            bool res = mXdgMenu->read(mMenuFile);
            if (!res)
                QMessageBox::warning(nullptr, QSL("Parse error"), mXdgMenu->errorString());
        }
        mRebuildTimer->start();
        return;
    }
    mMenuWidget->settingsChanged();
}

void WingMenuPlugin::setupShortcut()
{
    mShortcut = GlobalKeyShortcut::Client::instance()->addAction(QString{}, QSL("/panel/%1/show_hide").arg(settings()->group()), tr("Show/hide main menu"), this);
    if (mShortcut) {
        connect(mShortcut, &GlobalKeyShortcut::Action::registrationFinished, this, [this] {
            if (mShortcut->shortcut().isEmpty())
                mShortcut->changeShortcut(DEFAULT_SHORTCUT); });
        connect(mShortcut, &GlobalKeyShortcut::Action::activated, this, [this] { showHideMenu(); });
    }
}

void WingMenuPlugin::showMenu()
{
    if (!mMenu)
        return;
    willShowWindow(mMenu);
    mMenu->popup(calculatePopupWindowPos(mMenu->sizeHint()).topLeft());
    mMenuWidget->focusLineEdit();
}

void WingMenuPlugin::hideMenu()
{
    mMenu->hide();
}

void WingMenuPlugin::showHideMenu()
{
    if (!mMenu)
        return;

    if (mMenu->isVisible())
        hideMenu();
    else
        showMenu();
}

void WingMenuPlugin::realign()
{
    mWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QDialog* WingMenuPlugin::configureDialog()
{
    return new WingMenuConfiguration(settings(), mShortcut, mXdgMenu);
}
