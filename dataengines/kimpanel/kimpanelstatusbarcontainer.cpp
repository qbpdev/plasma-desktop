/***************************************************************************
 *   Copyright (C) 2011 by CSSlayer <wengxt@gmail.com>                     *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "kimpanelstatusbarcontainer.h"
#include "kimpanelservice.h"
#include "kimpanelagent.h"
#include "kimpaneldataengine.h"

KimpanelStatusBarContainer::KimpanelStatusBarContainer(QObject* parent, PanelAgent* panelAgent):
    DataContainer(parent),
    m_panelAgent(panelAgent)
{
    connect(m_panelAgent, &PanelAgent::updateProperty, this, &KimpanelStatusBarContainer::updateProperty);
    connect(m_panelAgent, &PanelAgent::registerProperties, this, &KimpanelStatusBarContainer::registerProperties);
    connect(m_panelAgent, &PanelAgent::execMenu, this, &KimpanelStatusBarContainer::execMenu);
    connect(m_panelAgent, &PanelAgent::execDialog, this, &KimpanelStatusBarContainer::execDialog);
}

Plasma::Service* KimpanelStatusBarContainer::service(QObject* parent)
{
    KimpanelService *controller = new KimpanelService(parent, QLatin1String(STATUSBAR_SOURCE_NAME), m_panelAgent);
    connect(this, &Plasma::DataContainer::updateRequested,
            controller, &KimpanelService::enableKimpanelOperations);
    return controller;
}

void KimpanelStatusBarContainer::updateProperty(const KimpanelProperty& property)
{
    int i = 0;
    for (i = 0; i < m_props.size(); i ++) {
        if (m_props[i].key == property.key) {
            m_props[i] = property;
            QList<QVariant> varList;
            Q_FOREACH(const KimpanelProperty & prop, m_props) {
                varList << prop.toMap();
            }
            setData(QStringLiteral("Properties"), varList);
            checkForUpdate();
            break;
        }
    }
}

void KimpanelStatusBarContainer::registerProperties(const QList< KimpanelProperty >& props)
{
    m_props = props;
    QList<QVariant> varList;
    Q_FOREACH(const KimpanelProperty & prop, m_props) {
        varList << prop.toMap();
    }
    setData(QStringLiteral("Properties"), varList);
    checkForUpdate();
}

void KimpanelStatusBarContainer::execMenu(const QList< KimpanelProperty >& props)
{
    QList<QVariant> varList;
    Q_FOREACH(const KimpanelProperty & prop, props) {
        varList << prop.toMap();
    }
    QVariantMap map;
    map[QStringLiteral("props")] = varList;
    map[QStringLiteral("timestamp")] = QDateTime::currentMSecsSinceEpoch();
    setData(QStringLiteral("Menu"), map);
    checkForUpdate();
}

void KimpanelStatusBarContainer::execDialog(const KimpanelProperty& prop)
{
    Q_UNUSED(prop)
}
