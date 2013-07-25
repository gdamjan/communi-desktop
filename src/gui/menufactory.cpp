/*
* Copyright (C) 2008-2013 The Communi Project
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

#include "menufactory.h"
#include "messageview.h"
#include "userlistview.h"
#include "sessiontreeitem.h"
#include "sessiontreewidget.h"
#include "session.h"
#include <IrcCommand>
#include <IrcChannel>

MenuFactory::MenuFactory(QObject* parent) : QObject(parent)
{
}

MenuFactory::~MenuFactory()
{
}

class UserViewMenu : public QMenu
{
    Q_OBJECT

public:
    UserViewMenu(const QString& user, MessageView* view) :
        QMenu(view), user(user), view(view)
    {
    }

private slots:
    void onWhoisTriggered()
    {
        IrcCommand* command = IrcCommand::createWhois(user);
        view->session()->sendCommand(command);
    }

    void onQueryTriggered()
    {
        QMetaObject::invokeMethod(view, "queried", Q_ARG(QString, user));
    }

private:
    QString user;
    MessageView* view;
};

QMenu* MenuFactory::createUserViewMenu(const QString& user, MessageView* view)
{
    UserViewMenu* menu = new UserViewMenu(user, view);
    menu->addAction(tr("Whois"), menu, SLOT(onWhoisTriggered()));
    menu->addAction(tr("Query"), menu, SLOT(onQueryTriggered()));
    return menu;
}

class UserListMenu : public QMenu
{
    Q_OBJECT

public:
    UserListMenu(UserListView* listView) : QMenu(listView), listView(listView)
    {
    }

private slots:
    void onWhoisTriggered()
    {
        QAction* action = qobject_cast<QAction*>(sender());
        if (action) {
            IrcCommand* command = IrcCommand::createWhois(action->data().toString());
            listView->session()->sendCommand(command);
        }
    }

    void onQueryTriggered()
    {
        QAction* action = qobject_cast<QAction*>(sender());
        if (action)
            QMetaObject::invokeMethod(listView, "queried", Q_ARG(QString, action->data().toString()));
    }

    void onModeTriggered()
    {
        QAction* action = qobject_cast<QAction*>(sender());
        if (action) {
            QStringList params = action->data().toStringList();
            IrcCommand* command = IrcCommand::createMode(listView->channel()->title(), params.at(1), params.at(0));
            listView->session()->sendCommand(command);
        }
    }

    void onKickTriggered()
    {
        QAction* action = qobject_cast<QAction*>(sender());
        if (action) {
            IrcCommand* command = IrcCommand::createKick(listView->channel()->title(), action->data().toString());
            listView->session()->sendCommand(command);
        }
    }

    void onBanTriggered()
    {
        QAction* action = qobject_cast<QAction*>(sender());
        if (action) {
            IrcCommand* command = IrcCommand::createMode(listView->channel()->title(), "+b", action->data().toString() + "!*@*");
            listView->session()->sendCommand(command);
        }
    }

private:
    UserListView* listView;
};

QMenu* MenuFactory::createUserListMenu(const QString& prefix, const QString& name, UserListView* listView)
{
    UserListMenu* menu = new UserListMenu(listView);

    QAction* action = 0;
    action = menu->addAction(tr("Whois"), menu, SLOT(onWhoisTriggered()));
    action->setData(name);

    action = menu->addAction(tr("Query"), menu, SLOT(onQueryTriggered()));
    action->setData(name);

    menu->addSeparator();

    if (prefix.contains("@")) {
        action = menu->addAction(tr("Deop"), menu, SLOT(onModeTriggered()));
        action->setData(QStringList() << name << "-o");
    } else {
        action = menu->addAction(tr("Op"), menu, SLOT(onModeTriggered()));
        action->setData(QStringList() << name << "+o");
    }

    if (prefix.contains("+")) {
        action = menu->addAction(tr("Devoice"), menu, SLOT(onModeTriggered()));
        action->setData(QStringList() << name << "-v");
    } else {
        action = menu->addAction(tr("Voice"), menu, SLOT(onModeTriggered()));
        action->setData(QStringList() << name << "+v");
    }

    menu->addSeparator();

    action = menu->addAction(tr("Kick"), menu, SLOT(onKickTriggered()));
    action->setData(name);

    action = menu->addAction(tr("Ban"), menu, SLOT(onBanTriggered()));
    action->setData(name);

    return menu;
}

class SessionTreeMenu : public QMenu
{
    Q_OBJECT

public:
    SessionTreeMenu(SessionTreeItem* item, SessionTreeWidget* tree) :
        QMenu(tree), item(item), tree(tree)
    {
    }

private slots:
    void onEditSession()
    {
        QMetaObject::invokeMethod(tree, "editSession", Q_ARG(IrcSession*, item->session()));
    }

    void onNamesTriggered()
    {
        IrcCommand* command = IrcCommand::createNames(item->text(0));
        item->session()->sendCommand(command);
    }

    void onWhoisTriggered()
    {
        IrcCommand* command = IrcCommand::createWhois(item->text(0));
        item->session()->sendCommand(command);
    }

    void onJoinTriggered()
    {
        IrcCommand* command = IrcCommand::createJoin(item->text(0));
        item->session()->sendCommand(command);
    }

    void onPartTriggered()
    {
        IrcCommand* command = IrcCommand::createPart(item->text(0));
        item->session()->sendCommand(command);
    }

    void onCloseTriggered()
    {
        QMetaObject::invokeMethod(tree, "closeItem", Q_ARG(SessionTreeItem*, item));
    }

private:
    SessionTreeItem* item;
    SessionTreeWidget* tree;
};

QMenu* MenuFactory::createSessionTreeMenu(SessionTreeItem* item, SessionTreeWidget* tree)
{
    bool active = item->session()->isActive();
    SessionTreeMenu* menu = new SessionTreeMenu(item, tree);
    if (!item->parent()) {
        Session* session = qobject_cast<Session*>(item->session()); // TODO
        if (session && session->isReconnecting())
            menu->addAction(tr("Stop"), item->session(), SLOT(stopReconnecting()));
        else if (active)
            menu->addAction(tr("Disconnect"), item->session(), SLOT(quit()));
        else
            menu->addAction(tr("Reconnect"), item->session(), SLOT(reconnect()));
        menu->addAction(tr("Edit"), menu, SLOT(onEditSession()))->setEnabled(!active);
        menu->addSeparator();
    } else if (active){
        bool channel = !item->text(0).isEmpty() && IrcSessionInfo(item->session()).channelTypes().contains(item->text(0).at(0));
        if (channel) {
            if (item->view()->isActive()) {
                menu->addAction(tr("Names"), menu, SLOT(onNamesTriggered()));
                menu->addAction(tr("Part"), menu, SLOT(onPartTriggered()));
            } else {
                menu->addAction(tr("Join"), menu, SLOT(onJoinTriggered()));
            }
        } else {
            menu->addAction(tr("Whois"), menu, SLOT(onWhoisTriggered()));
        }
        menu->addSeparator();
    }
    menu->addAction(tr("Close"), menu, SLOT(onCloseTriggered()));
    return menu;
}

#include "menufactory.moc"
