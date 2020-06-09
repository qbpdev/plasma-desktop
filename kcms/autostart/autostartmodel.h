/***************************************************************************
 *   Copyright (C) 2020 by Méven Car <meven.car@enioka.com>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA          *
 ***************************************************************************/
#ifndef AUTOSTARTMODEL_H
#define AUTOSTARTMODEL_H

#include <QAbstractListModel>

#include <KService>

struct AutostartEntry;

class AutostartModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AutostartModel(QObject *parent = nullptr);

    enum Roles {
        Name = Qt::DisplayRole,
        Command = Qt::UserRole + 1,
        Enabled,
        Source,
        FileName,
        OnlyInPlasma,
        IconName
    };

    enum AutostartEntrySource {
        XdgAutoStart = 0,
        XdgScripts = 1,
        PlasmaShutdown = 2,
        PlasmaStart = 3,
    };
    Q_ENUM(AutostartEntrySource)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool reloadEntry(const QModelIndex &index, const QString &fileName);

    Q_INVOKABLE void removeEntry(int row);
    Q_INVOKABLE void editApplication(int row);
    Q_INVOKABLE void addScript(const QUrl &path, AutostartEntrySource kind);
    Q_INVOKABLE void showApplicationDialog();

    void load();

Q_SIGNALS:
    void error(const QString &message);

private:
    void addApplication(const KService::Ptr &service);
    void loadScriptsFromDir(const QString &subDir, AutostartEntrySource kind);
    QString XdgAutoStartPath() const;

    QVector<AutostartEntry> m_entries;
    int m_lastApplication = 0;
    int m_lastLoginScript = 0;
};

struct AutostartEntry
{
    QString name; // Human readable name or local script name
    QString command; // exec or original .sh file
    AutostartModel::AutostartEntrySource source;
    bool enabled;
    QString fileName; // the file backing the entry
    bool onlyInPlasma;
    QString iconName;
};
Q_DECLARE_TYPEINFO(AutostartEntry, Q_MOVABLE_TYPE);

#endif // AUTOSTARTMODEL_H
