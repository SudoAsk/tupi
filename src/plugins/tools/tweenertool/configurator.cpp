/***************************************************************************
 *   Project TUPI: Magia 2D                                                *
 *   Project Contact: info@maefloresta.com                                 *
 *   Project Website: http://www.maefloresta.com                           *
 *   Project Leader: Gustav Gonzalez <info@maefloresta.com>                *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 *
 *    Gustavo Gonzalez / xtingray                                          *
 *                                                                         *
 *   KTooN's versions:                                                     * 
 *                                                                         *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       *
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Gustav Gonzalez - http://www.maefloresta.com       *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "configurator.h"
#include "tweenmanager.h"
#include "buttonspanel.h"
#include "ktitemtweener.h"
#include "stepsviewer.h"
#include "kttweenerstep.h"
#include "kosd.h"
#include "kradiobuttongroup.h"
#include "kimagebutton.h"
#include "kdebug.h"

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QComboBox>
#include <QBoxLayout>
#include <QHeaderView>
#include <QGraphicsPathItem>
#include <QMenu>
#include <QAction>

struct Configurator::Private
{
    QBoxLayout *layout; 
    QBoxLayout *settingsLayout;
    Settings *settingsPanel;
    TweenManager *tweenManager;
    ButtonsPanel *controlPanel;

    bool selectionDone;

    KImageButton *removeButton;
    KImageButton *editButton;

    Settings::Mode mode; 
    GuiState state;

    int framesTotal;
    int currentFrame;

    KTItemTweener *currentTween;
};

Configurator::Configurator(QWidget *parent) : QFrame(parent), k(new Private)
{
    k->framesTotal = 1;
    k->currentFrame = 0;

    k->mode = Settings::View;
    k->selectionDone = false;
    k->state = Manager;

    k->layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    k->layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QLabel *title = new QLabel(tr("Motion Tween"));
    title->setAlignment(Qt::AlignHCenter);
    title->setFont(QFont("Arial", 8, QFont::Bold));

    k->layout->addWidget(title);

    k->settingsLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    k->settingsLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    k->settingsLayout->setMargin(0);
    k->settingsLayout->setSpacing(0);

    setTweenManagerPanel();
    setButtonsPanel();
    setPropertiesPanel();

    k->layout->addLayout(k->settingsLayout);
    k->layout->addStretch(2);
}

Configurator::~Configurator()
{
    delete k;
}

void Configurator::loadTweenList(QList<QString> tweenList)
{
    k->tweenManager->loadTweenList(tweenList);
    if (tweenList.count() > 0)
        activeButtonsPanel(true); 
}

void Configurator::setPropertiesPanel()
{
    k->settingsPanel = new Settings(this);

    connect(k->settingsPanel, SIGNAL(startingPointChanged(int)), this, SIGNAL(startingPointChanged(int)));

    connect(k->settingsPanel, SIGNAL(clickedSelect()), this, SIGNAL(clickedSelect()));
    connect(k->settingsPanel, SIGNAL(clickedCreatePath()), this, SIGNAL(clickedCreatePath()));

    connect(k->settingsPanel, SIGNAL(clickedApplyTween()), this, SLOT(applyItem()));
    connect(k->settingsPanel, SIGNAL(clickedResetTween()), this, SLOT(closeTweenProperties()));

    k->settingsLayout->addWidget(k->settingsPanel);

    activePropertiesPanel(false);
}

void Configurator::activePropertiesPanel(bool enable)
{
    if (enable)
        k->settingsPanel->show();
    else
        k->settingsPanel->hide();
}

void Configurator::setTweenManagerPanel()
{
    k->tweenManager = new TweenManager(this);

    connect(k->tweenManager, SIGNAL(addNewTween(const QString &)), this, SLOT(addTween(const QString &)));
    connect(k->tweenManager, SIGNAL(editCurrentTween(const QString &)), this, SLOT(editTween()));
    connect(k->tweenManager, SIGNAL(removeCurrentTween(const QString &)), this, SLOT(removeTween(const QString &)));
    connect(k->tweenManager, SIGNAL(updateTweenData(const QString &)), this, SLOT(updateTweenData(const QString &)));

    k->settingsLayout->addWidget(k->tweenManager);

    k->state = Manager;
}

void Configurator::activeTweenManagerPanel(bool enable)
{
    if (enable) 
        k->tweenManager->show();
    else 
        k->tweenManager->hide();

    if (k->tweenManager->listSize() > 0)
        activeButtonsPanel(enable);
}

void Configurator::setButtonsPanel()
{
    kFatal() << "Configurator::setButtonsPanel() - Setting panel!";

    k->controlPanel = new ButtonsPanel(this);
    connect(k->controlPanel, SIGNAL(clickedEditTween()), this, SLOT(editTween()));
    connect(k->controlPanel, SIGNAL(clickedRemoveTween()), this, SLOT(removeTween()));

    k->settingsLayout->addWidget(k->controlPanel);

    activeButtonsPanel(false);
}

void Configurator::activeButtonsPanel(bool enable)
{
    if (enable)
        k->controlPanel->show();
    else
        k->controlPanel->hide();
}

void Configurator::initStartCombo(int framesTotal, int currentFrame)
{
    k->framesTotal = framesTotal;
    k->currentFrame = currentFrame;
    k->settingsPanel->initStartCombo(framesTotal, currentFrame);
}

void Configurator::setStartFrame(int currentIndex)
{
    k->settingsPanel->setStartFrame(currentIndex);
}

int Configurator::startFrame()
{
    return k->settingsPanel->startFrame();
}

int Configurator::startComboSize()
{
    return k->settingsPanel->startComboSize();
}

void Configurator::updateSteps(const QGraphicsPathItem *path)
{
    k->settingsPanel->updateSteps(path);
}

QString Configurator::tweenToXml(int currentFrame, QString &path)
{
    return k->settingsPanel->tweenToXml(currentFrame, path);
}

int Configurator::totalSteps()
{
    return k->settingsPanel->totalSteps();
}

void Configurator::activatePathMode()
{
    //k->options->setCurrentIndex(1);
    //k->stepViewer->cleanRows();
}

void Configurator::activateSelectionMode()
{
    //k->options->setCurrentIndex(0);
}

void Configurator::cleanData()
{
    k->settingsPanel->cleanData();
}

void Configurator::addTween(const QString &name)
{
    kFatal() << "Configurator::addTween() - Adding new Tween!";

    k->mode = Settings::Add;

    activeTweenManagerPanel(false);
    activePropertiesPanel(true);

    k->settingsPanel->setParameters(name, k->framesTotal, k->currentFrame);

    k->state = Properties;

    emit selectionModeOn();
}

void Configurator::editTween()
{
    kFatal() << "Configurator::editTween() - Editing the current Tween!";

    activeTweenManagerPanel(false);

    k->mode = Settings::Edit;
    k->state = Properties;

    k->settingsPanel->setParameters(k->currentTween);
    activePropertiesPanel(true);

    emit editModeOn();
}

void Configurator::closeTweenProperties()
{
    kFatal() << "Configurator::closeTweenProperties() - Tracing mode: " << k->mode;

    if (k->mode == Settings::Add) {
        kFatal() << "Configurator::closeTweenProperties() - removing Tween!";
        k->tweenManager->removeItemFromList();

    } else if (k->mode == Settings::Edit) {
        kFatal() << "Configurator::closeTweenProperties() - edit Mode!";
        closeSettingsPanel();
        k->mode = Settings::View;

        emit clickedResetInterface();
    }

    closeSettingsPanel();
}

void Configurator::removeTween()
{
    QString name = k->tweenManager->currentTweenName();
    k->tweenManager->removeItemFromList();
    removeTween(name);
}

void Configurator::removeTween(const QString &name)
{
    kFatal() << "Configurator::removeTween() - Removing Item!";

    /*
    if (k->tweenManager->listSize() == 0) {
        if (k->state == Buttons) {
            activeButtonsPanel(false);
        }
    }
    */

    emit clickedRemoveTween(name);
}

QString Configurator::currentTweenName() const
{
    return k->tweenManager->currentTweenName();
}

void Configurator::notifySelection(bool flag)
{
    kFatal() << "Configurator::notifySelection() - Updating selection flag: " << flag;

    if (k->mode != Settings::View)
        k->settingsPanel->notifySelection(flag); 
}

void Configurator::closeSettingsPanel()
{
    if (k->state == Properties) {
        activeTweenManagerPanel(true);
        activePropertiesPanel(false);
        k->mode = Settings::View;
    }
}

Settings::Mode Configurator::mode()
{
    return k->mode;
}

void Configurator::applyItem()
{
     k->mode = Settings::Edit;
     emit clickedApplyTween();
}

void Configurator::resetUI()
{
    k->tweenManager->resetUI();
    closeSettingsPanel();

    k->state = Manager;
}

void Configurator::updateTweenData(const QString &name)
{
    //if (k->mode == Settings::View) {
        //kFatal() << "Configurator::updateTweenData() - Item: " << item->text();
        emit getTweenData(name);
    //}
}

void Configurator::setCurretTween(KTItemTweener *currentTween)
{
    kFatal() << "Configurator::setCurretTween() - Tracing VAR";

    k->currentTween = currentTween;
}
