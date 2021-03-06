﻿#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include "TrayMenu.h"

class SystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SystemTrayIcon(QObject *parent = nullptr);
    explicit SystemTrayIcon(const QIcon &icon, QObject *parent = nullptr);

signals:
    void sigShow();
    void sigPick();
    void sigQuit();

private:
    void initTrayMenu();
    void initConnect();

    TrayMenu *trayMenu = nullptr;
};

#endif // SYSTEMTRAYICON_H
