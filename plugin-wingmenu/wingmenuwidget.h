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

#include <QAction>
#include <QActionGroup>
#include <QDomElement>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QStackedWidget>
#include <QStandardItemModel>
#include <QStringList>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include <XdgAction>
#include <XdgDesktopFile>
#include <XdgIcon>
#include <XdgMenu>

#include "common.h"

class WingMenuPlugin;
class ApplicationsView;
class ItemDescriptionDelegate;
class MyModel;

class WingMenuWidget : public QWidget
{
    Q_OBJECT
public:
    enum DataType {
        Comment = Qt::UserRole + 1,
        Category = Qt::UserRole + 2,
        DesktopFile = Qt::UserRole + 3,
        SearchString = Qt::UserRole + 4,
        Exec = Qt::UserRole + 5
    };
    Q_ENUM(DataType);

    WingMenuWidget(WingMenuPlugin* plugin, XdgMenu* xdgMenu, QWidget* parent = nullptr);
    ~WingMenuWidget() = default;

    void onShow();
    void onHide();
    void focusLineEdit();
    void settingsChanged();

private:
    WingMenuPlugin* mPlugin;
    int mIconSize;
    QHBoxLayout* mRootBox;
    QVBoxLayout* mMainBox;
    QHBoxLayout* mMenuBox;
    QLineEdit* mSearchEdit;
    QWidget* mSideWidget;
    QVBoxLayout* mSideBox;
    QSpacerItem* mSideSpacer;
    QWidget* mCategoryWidget;
    QVBoxLayout* mCategoryBox;
    QStackedWidget* mApplicationsStack;
    ApplicationsView* mApplicationsView;
    QStandardItemModel* mApplicationsModel;
    QSortFilterProxyModel* mProxyModel;
    ApplicationsView* mFavoritesView;
    QStandardItemModel* mFavoritesModel;
    QActionGroup* mSideGroup;
    QActionGroup* mCategoryGroup;
    QTimer* mHoverTimer;
    QAction* mHoveredAction;
    QLabel* mFavoritesInfo;

    QStringList mLeaveActions;
    QStringList mOtherActions;
    QString mLastCategory;

    bool mCategoryLeft;
    bool mSearchBottom;
    bool mSidebarLeft;
    bool mReverseSidebar;
    AppLayout::Layout mAppLayout;
    double mButtonMultiplier;

    QStringList mFavoritesList;
    bool mAskFavoriteRemove;
    bool mSwitchOnHover;
    int mHoverDelay;
    bool mCustomizeLeave;
    QStringList mCustomActions;

    XdgMenu* mXdgMenu;
    QDomElement mXml;
    QString mLogDir;
    QString mMenuFile;

    QString mCurrentWidget;

    void focusCategoryWidget();
    void sendKeyToApplicationsList(Qt::Key key);
    bool eventFilter(QObject* watched, QEvent* event) override;
    void indexActivated(const QModelIndex& index);
    void showCustomMenu(const QPoint& pos);
    void buildMenu();
    void connectSignals();
    QToolButton* createSideButton(const XdgDesktopFile& df);
    void buildSideButtons();
    void parseSideActions(const QDomElement& xml);
    QString escape(QString string);
    void addMenu(const QDomElement& xml);

    void addCategoryButton(const QIcon& icon, const QString& title, const QString& name);
    void addItem(const QDomElement& xml, const QString& category);
    void addItem(const XdgDesktopFile& df, const QString& category);
    void setItemToolTip(QStandardItem* item);

    void removeSpaces(QLayout* layout);
    void removeSpaces(QWidget* widget);
    void filterApplications(DataType type, const QString& filterString);
    void sideActionTriggered(QAction* action);
    void sideActionHovered(QAction* action);
    void uncheckSideActions();
    void categoryActionTriggered(QAction* action);
    void categoryActionHovered(QAction* action);
    void hoverTimeout();
    void addItemToFavorites(const XdgDesktopFile& df);
    void saveFavoritesList();
    void removeFromFavorites(const QModelIndex& index);
    void favoritesRowRemoved();
    void moveFavoriteUp(int index);
    void moveFavoriteDown(int index);
protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* keyEvent) override;
signals:
    void hideMenu();
};
