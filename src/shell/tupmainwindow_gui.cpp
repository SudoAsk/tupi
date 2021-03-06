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
 *   the Free Software Foundation; either version 2 of the License, or     *
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

#include "tupmainwindow.h"
#include "tupapplication.h"

#include "tcommandhistory.h"
#include "toolview.h"
#include "tviewbutton.h"
// #include "taudioplayer.h"

/**
 * This class implements the Tupi GUI
 * Here is where all the Tupi GUI components are defined 
 * @author David Cuadrado 
*/

/**
 * @if english
 * This method includes the widgets around the border of the main window
 * @endif
 * @if spanish
 * Este metodo incluye los componentes alrededor del borde la ventana principal
 * @endif
*/

void TupMainWindow::createGUI()
{
    TAction *hideAction = new TAction(QPixmap(THEME_DIR + "icons/hide_top_panel.png"), tr("Hide top panels"), QKeySequence(tr("Alt")),
                                      this, SLOT(hideTopPanels()), m_actionManager);
    m_actionManager->insert(hideAction, "hideaction", "file");
    addSpecialButton(hideAction);

    // Adding the color palette to the left side of the interface 
    m_colorPalette = new TupColorPalette;
    colorView = addToolView(m_colorPalette, Qt::LeftDockWidgetArea, Animation, "Color Palette", QKeySequence(tr("Shift+P")));
    //colorView->setShortcut(QKeySequence(tr("Shift+P")));

    m_actionManager->insert(colorView->toggleViewAction(), "show_palette");
    addToPerspective(colorView->toggleViewAction(), Animation);

    connectWidgetToPaintArea(m_colorPalette);

    // Adding the pen parameters widget to the left side of the interface 

    m_brushWidget = new TupBrushWidget;
    penView = addToolView(m_brushWidget, Qt::LeftDockWidgetArea, Animation, "Pen", QKeySequence(tr("Shift+B")));
    m_actionManager->insert(penView->toggleViewAction(), "show_pen");
    addToPerspective(penView->toggleViewAction(), Animation);

    connectWidgetToPaintArea(m_brushWidget);

    // Adding the objects library widget to the left side of the interface

    m_libraryWidget = new TupLibraryWidget(this);
    m_libraryWidget->setLibrary(m_projectManager->project()->library());

    libraryView = addToolView(m_libraryWidget, Qt::LeftDockWidgetArea, Animation, "Library", QKeySequence(tr("Shift+L")));
    m_actionManager->insert(libraryView->toggleViewAction(), "show_library");
    addToPerspective(libraryView->toggleViewAction(), Animation);

    new TAction(QPixmap(THEME_DIR + "icons/bitmap.png"), tr("Bitmap"), QKeySequence(tr("Alt+B")), m_libraryWidget, SLOT(importBitmapGroup()),
		m_actionManager, "importBitmap");

    new TAction(QPixmap(THEME_DIR + "icons/bitmap_array.png"), tr("Bitmap Sequence"), QKeySequence(tr("Alt+Shift+B")), 
		m_libraryWidget, SLOT(importBitmapSequence()), m_actionManager, "importBitmapSequence");

    new TAction(QPixmap(THEME_DIR + "icons/svg.png"), tr("SVG File"), QKeySequence(tr("Alt+S")), m_libraryWidget, SLOT(importSvgGroup()),
		m_actionManager, "importSvg");

    new TAction(QPixmap(THEME_DIR + "icons/svg_array.png"), tr("SVG Sequence"), QKeySequence(tr("Alt+Shift+S")), m_libraryWidget, 
		SLOT(importSvgSequence()), m_actionManager, "importSvgSequence");

    //new TAction(QPixmap(), tr("Audio File..."), QKeySequence(), m_libraryWidget, SLOT(importSound()),
    //            m_actionManager, "importAudioFile");

    // SQA: Temporary code
    //m_actionManager->enable("importSvg", false);
    //m_actionManager->enable("importSvgArray", false);

    connectWidgetToManager(m_libraryWidget);
    connectWidgetToLocalManager(m_libraryWidget);

    // Adding the scenes widget to the right side of the interface

    m_scenes = new TupScenesWidget;
    scenesView = addToolView(m_scenes, Qt::RightDockWidgetArea, Animation, "Scenes Manager", QKeySequence(tr("Shift+C")));
    m_actionManager->insert(scenesView->toggleViewAction(), "show_scenes");
    addToPerspective(scenesView->toggleViewAction(), Animation);

    connectWidgetToManager(m_scenes);
    connectWidgetToLocalManager(m_scenes);

    // Adding the exposure sheet to the right side of the interface
    m_exposureSheet = new TupExposureSheet(this, m_projectManager->project());
    // connect(m_exposureSheet, SIGNAL(newPerspective(int)), this, SLOT(changePerspective(int)));

    exposureView = addToolView(m_exposureSheet, Qt::RightDockWidgetArea, Animation, "Exposure Sheet", QKeySequence(tr("Shift+E")));
    m_actionManager->insert(exposureView->toggleViewAction(), "show_exposure");
    addToPerspective(exposureView->toggleViewAction(), Animation);

    connectWidgetToManager(m_exposureSheet);
    connectWidgetToLocalManager(m_exposureSheet);

    // Adding the time line widget to the bottom side of the interface
    m_timeLine = new TupTimeLine(m_projectManager->project());
    connect(m_timeLine, SIGNAL(newPerspective(int)), this, SLOT(changePerspective(int)));

    timeView = addToolView(m_timeLine, Qt::BottomDockWidgetArea, Animation, "Time Line", QKeySequence(tr("Shift+T")));
    m_actionManager->insert(timeView->toggleViewAction(), "show_timeline");
    addToPerspective(timeView->toggleViewAction(), Animation);

    connectWidgetToManager(m_timeLine);
    connectWidgetToLocalManager(m_timeLine);

/* SQA: Debug visual component has been removed
#if defined(QT_GUI_LIB) && defined(K_DEBUG) && defined(Q_OS_LINUX)
    QDesktopWidget desktop;
    m_debug = new TupDebugWidget(this, desktop.screenGeometry().width());
    debugView = addToolView(m_debug, Qt::BottomDockWidgetArea, Animation, "Debug Term", QKeySequence(tr("Shift+D")));
    m_actionManager->insert(debugView->toggleViewAction(), "show_debug");
    addToPerspective(debugView->toggleViewAction(), Animation);
#endif
*/

    // Adding the script editor to the bottom side, if kinas was enabled
    /*
#ifdef ENABLE_KINAS
    KinasWidget *m_scriptEditor = new KinasWidget;
    addToolView(m_scriptEditor, Qt::BottomDockWidgetArea, Animation, "Tupi Script", QKeySequence(tr("Shift+K"));
#endif
    */

    enableToolViews(false);
}

/**
 * @if english
 * This method defines the main menu for the application
 * @endif
 * @if spanish
 * Este metodo define el menu principal para la aplicacion
 * @endif
*/

void TupMainWindow::setupMenu()
{
    // Setting up the file menu
    setupFileActions();

    // Menu File	
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_actionManager->find("new_project"));
    m_fileMenu->addAction(m_actionManager->find("open_project"));

    // SQA: This code has been disabled temporary
    // m_fileMenu->addAction(m_actionManager->find("opennetproject"));
    // m_fileMenu->addAction(m_actionManager->find("exportprojectserver"));

    // Adding Option Open Recent	
    m_recentProjectsMenu = new QMenu(tr("Open Recent"), this);

    TCONFIG->beginGroup("General");
    QString files = TCONFIG->value("Recents").toString();
    QStringList recents = files.split(';');
    if (files.isEmpty())
        recents.clear();
    updateOpenRecentMenu(m_recentProjectsMenu, recents);	
    m_fileMenu->addMenu(m_recentProjectsMenu);

    // Adding Options save, save as, close, export, import palettes and exit	
    m_fileMenu->addAction(m_actionManager->find("save_project"));

    m_fileMenu->addAction(m_actionManager->find("save_project_as"));
    m_fileMenu->addAction(m_actionManager->find("close_project"));

    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_actionManager->find("export"));

    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_actionManager->find("Exit"));

    // Setting up the Preferences menu
    setPreferencesAction();
    m_settingsMenu = menuBar()->addMenu(tr("&Edit"));

    m_settingsMenu->addAction(m_actionManager->find("preferences"));
    // Temporary out while SQA is done
    // m_actionManager->enable("preferences", false);

    // Temporary out while SQA is done
    // Setting up the insert menu
    // setupInsertActions();
    // Menu Insert
    m_insertMenu = menuBar()->addMenu(tr("&Import"));

    // Adding Options import bitmap and import audio file
    m_insertMenu->addAction(m_actionManager->find("importBitmap"));
    m_insertMenu->addAction(m_actionManager->find("importBitmapSequence"));
    m_insertMenu->addAction(m_actionManager->find("importSvg"));
    m_insertMenu->addAction(m_actionManager->find("importSvgSequence"));
    // m_insertMenu->addAction(m_actionManager->find("importAudioFile"));

    m_insertMenu->addSeparator();
    m_insertMenu->addAction(m_actionManager->find("importGimpPalettes"));

    // SQA: Action disabled while Library module is fixed
    m_insertMenu->addAction(m_actionManager->find("importPapagayoLipSync"));

    // Setting up the window menu
    m_windowMenu = menuBar()->addMenu(tr("&Window"));

    // Adding Options show debug, palette, pen, library, timeline, scenes, exposure, help
    m_windowMenu->addAction(m_actionManager->find("show_palette"));
    m_windowMenu->addAction(m_actionManager->find("show_pen"));
    m_windowMenu->addAction(m_actionManager->find("show_library"));
    m_windowMenu->addAction(m_actionManager->find("show_timeline"));
    m_windowMenu->addAction(m_actionManager->find("show_scenes"));
    m_windowMenu->addAction(m_actionManager->find("show_exposure"));

/* SQA: Debug visual component has been removed
#if defined(QT_GUI_LIB) && defined(K_DEBUG) && defined(Q_OS_LINUX)
    m_windowMenu->addAction(m_actionManager->find("show_debug"));
#endif
*/

    m_windowMenu->addSeparator();

    // Setup perspective menu
    m_viewMenu = new QMenu(tr("Modules"), this);
    QActionGroup *group = new QActionGroup(this);
    group->setExclusive(true);

    // Adding Option Animation
    QAction *drawingPerspective = new QAction(tr("Animation"), this);
    drawingPerspective->setIcon(QPixmap(THEME_DIR + "icons/animation_mode.png")); 
    drawingPerspective->setIconVisibleInMenu(true);
    drawingPerspective->setShortcut(QKeySequence("Ctrl+1"));
    drawingPerspective->setData(Animation);
    group->addAction(drawingPerspective);

    // Adding Option Player 
    QAction *animationPerspective = new QAction(tr("Player"), this);
    animationPerspective->setIcon(QPixmap(THEME_DIR + "icons/play_small.png"));
    animationPerspective->setIconVisibleInMenu(true);
    animationPerspective->setShortcut(QKeySequence("Ctrl+2"));
    animationPerspective->setData(Player);
    group->addAction(animationPerspective);

    // Adding Option News 
    QAction *newsPerspective = new QAction(tr("News"), this);
    newsPerspective->setIcon(QPixmap(THEME_DIR + "icons/news_mode.png"));
    newsPerspective->setIconVisibleInMenu(true);
    newsPerspective->setShortcut(QKeySequence("Ctrl+3"));
    newsPerspective->setData(News);
    group->addAction(newsPerspective);

    m_viewMenu->addActions(group->actions());
    connect(group, SIGNAL(triggered(QAction *)), this, SLOT(changePerspective(QAction *)));
    menuBar()->addMenu(m_viewMenu);
	
    // Setting up the help menu
    setupHelpActions();
    m_helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(m_helpMenu);
    m_helpMenu->addAction(m_actionManager->find("help"));
    m_helpMenu->addAction(m_actionManager->find("tip_of_day"));
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_actionManager->find("about_tupi"));

    setMenuItemsContext(false);

    // SQA: Temporary code
    // menuBar()->setVisible(false);
}

void TupMainWindow::setMenuItemsContext(bool flag)
{
    m_actionManager->enable("save_project", flag);
    m_actionManager->enable("save_project_as", flag);
    m_actionManager->enable("close_project", flag);
    m_actionManager->enable("hideaction", flag);
    m_actionManager->enable("export", flag);
    m_actionManager->enable("importBitmap", flag);

    // m_settingsMenu->setEnabled(flag);
    m_insertMenu->setEnabled(flag);
    m_windowMenu->setEnabled(flag);
    m_viewMenu->setEnabled(flag);
}

/**
 * @if english
 * This method defines the actions for the options in the menu File
 * @endif
 * @if spanish
 * Este metodo define las acciones para las opciones del menu Archivo
 * @endif
*/

void TupMainWindow::setupFileActions()
{
    TAction *newProject = new TAction(QPixmap(THEME_DIR + "icons/new.png"), tr("New Project"), QKeySequence(tr("Ctrl+N")),
				      this, SLOT(newProject()), m_actionManager);
    newProject->setStatusTip(tr("Open new project"));
    m_actionManager->insert(newProject, "new_project", "file");

    TAction *openFile = new TAction(QPixmap(THEME_DIR + "icons/open.png"), tr("Open Project"), QKeySequence(tr("Ctrl+O")), 
				    this, SLOT(openProject()), m_actionManager);
    m_actionManager->insert(openFile, "open_project", "file");
    openFile->setStatusTip(tr("Load existent project"));

    // SQA: This code has been disabled temporary
    /*
    TAction *openNetFile = new TAction(QPixmap(THEME_DIR + "icons/net_document.png"), tr("Open Project From Server..."), 
				       tr(""), this, SLOT(openProjectFromServer()), m_actionManager);
    m_actionManager->insert(openNetFile, "opennetproject", "file");

    TAction *importNetFile = new TAction(QPixmap(THEME_DIR + "icons/import_project.png"), tr("Export Project To Server..."), tr(""), this, 
					 SLOT(importProjectToServer()), m_actionManager);
    m_actionManager->insert(importNetFile, "exportprojectserver", "file");
    */

    TAction *save = new TAction(QPixmap(THEME_DIR + "icons/save.png"), tr( "Save Project" ),
				QKeySequence(tr("Ctrl+S")), this, SLOT(saveProject()), m_actionManager);
    m_actionManager->insert(save, "save_project", "file");
    save->setStatusTip(tr("Save current project in current location"));

    TAction *saveAs = new TAction(QPixmap(THEME_DIR + "icons/save_as.png"), tr("Save Project As..."),
				  QKeySequence(tr("Ctrl+Shift+S")), this, SLOT(saveAs()), m_actionManager);
    saveAs->setStatusTip(tr("Open dialog box to save current project in any location"));
    m_actionManager->insert(saveAs, "save_project_as", "file");

    TAction *close = new TAction(QPixmap(THEME_DIR + "icons/close.png"), tr("Cl&ose Project"), QKeySequence(tr("Ctrl+W")),
				 this, SLOT(closeProject()), m_actionManager);
    close->setStatusTip(tr("Close active project"));
    m_actionManager->insert(close, "close_project", "file");

    // Import Palette action

    TAction *importPalette = new TAction(QPixmap(THEME_DIR + "icons/import.png"), tr("&Import GIMP Palettes"),
					 QKeySequence(tr("Shift+G")), this, SLOT(importPalettes()), m_actionManager);
    importPalette->setStatusTip(tr("Import palettes"));
    m_actionManager->insert(importPalette, "importGimpPalettes", "file");

    TAction *importPapagayo = new TAction(QPixmap(THEME_DIR + "icons/papagayo.png"), tr("&Import Papagayo Lip-sync"),
                                         QKeySequence(tr("Alt+P")), this, SLOT(importPapagayoLipSync()), m_actionManager);
    importPapagayo->setStatusTip(tr("Import Papagayo lip-sync"));
    m_actionManager->insert(importPapagayo, "importPapagayoLipSync", "file");

    // Export Project action
    TAction *exportProject = new TAction(QPixmap(THEME_DIR + "icons/export.png"), tr("&Export Project"), QKeySequence(tr("Ctrl+R")),
                                         this, SLOT(exportProject()), m_actionManager);
    exportProject->setStatusTip(tr("Export project to several video formats"));
    m_actionManager->insert(exportProject, "export", "file");

    // Exit action
    TAction *exit = new TAction(QPixmap(THEME_DIR + "icons/exit.png"), tr("Quit"), QKeySequence(tr("Ctrl+Q")),
                                qApp, SLOT(closeAllWindows()), m_actionManager);

    exit->setStatusTip(tr("Close application"));
    m_actionManager->insert(exit, "exit", "file");

    // when the last window is closed, the application should quit
    connect(qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));
}

/**
 * @if english
 * This method defines the actions for the options in the menu Settings
 * @endif
 * @if spanish
 * Este metodo define las acciones para las opciones del menu Configuracion
 * @endif
*/

void TupMainWindow::setPreferencesAction()
{
    TAction *preferences = new TAction(QPixmap(THEME_DIR + "icons/properties.png"), tr("Pr&eferences..."), 
                                        QKeySequence(tr("Ctrl+P")), this, SLOT(preferences()),
                                        m_actionManager, "preferences");
    preferences->setStatusTip(tr("Opens the preferences dialog box"));
}

/**
 * @if english
 * This method defines the actions for the options in the menu Help
 * @endif
 * @if spanish
 * Este metodo define las acciones para las opciones del menu Ayuda
 * @endif
*/

void TupMainWindow::setupHelpActions()
{
    helpAction = new TAction(QPixmap(THEME_DIR + "icons/help_mode.png"), tr("Online Help Content"), QKeySequence(tr("F1")),
                 this, SLOT(showHelp()), m_actionManager, "help");
    helpAction->setEnabled(false);

    new TAction(QPixmap(THEME_DIR + "icons/tip.png"), tr("Tip Of The Day"), QKeySequence(tr("Ctrl+T")),
                this, SLOT(showTipDialog()), m_actionManager, "tip_of_day");
    new TAction(QPixmap(THEME_DIR + "icons/about.png"), tr("About Tupi"), QKeySequence(tr("Ctrl+K")), 
                this, SLOT(aboutTupi()), m_actionManager, "about_tupi");
}

/**
 * @if english
 * This method defines the Toolbar and its actions
 * @endif
 * @if spanish
 * Este metodo define la barra de herramientas y sus acciones
 * @endif
*/

void TupMainWindow::setupToolBar()
{
    mainToolBar = new QToolBar(tr("Actions Bar"), this);
    mainToolBar->setIconSize(QSize(22, 22));
    addToolBar(Qt::TopToolBarArea, mainToolBar);

    mainToolBar->addAction(m_actionManager->find("new_project"));
    mainToolBar->addAction(m_actionManager->find("open_project"));

    // SQA: This code has been disabled temporary
    // mainToolBar->addAction(m_actionManager->find("opennetproject"));

    mainToolBar->addAction(m_actionManager->find("save_project"));
    mainToolBar->addAction(m_actionManager->find("save_project_as"));
    mainToolBar->addAction(m_actionManager->find("close_project"));
}

/**
 * @if english
 * This method updates the entries in the recent menu option
 * @endif
 * @if spanish
 * Este metodo actualiza las entradas en la opcion "Recientes" del menu
 * @endif
*/

void TupMainWindow::updateOpenRecentMenu(QMenu *menu, QStringList recents)
{
    menu->clear();
    m_recentProjects.clear();

    if (recents.count() == 0) {
        m_recentProjectsMenu->setEnabled(false);
        return;
    } else {
        if (recents.count() > 5) {
            QStringList list; 
            list << recents.mid(0, 4);
            recents = list;
        }
    }

    int i = 0;
    QAction *action[5];
    foreach (QString recent, recents) {
             m_recentProjects << recent;
             action[i] = new QAction(QPixmap(THEME_DIR + "icons/recent_files.png"), recent, this); 
             action[i]->setIconVisibleInMenu(true);
             menu->addAction(action[i]);
             connect(action[i], SIGNAL(triggered()), this, SLOT(openRecentProject()));
             i++;
    }

    if (!m_recentProjectsMenu->isEnabled())
        m_recentProjectsMenu->setEnabled(true);
}

/**
 * @if english
 * This method changes the perspective view according the events
 * @endif
 * @if spanish
 * Este metodo cambia la vista de perspectiva de acuerdo a los eventos
 * @endif
*/

void TupMainWindow::changePerspective(QAction *action)
{
    int perspective = action->data().toInt();

    // Animation or Player perspective
    if (perspective == Animation || perspective == Player) {
        setCurrentTab(perspective - 1);
    } else if (perspective == News) { // News perspective
               setCurrentTab(2);
    } 

    action->setChecked(true);
}

void TupMainWindow::changePerspective(int index)
{
    if (index == 4) {
        setCurrentTab(1);
        cameraWidget->doPlay();
    } else {
        setCurrentTab(index);
    }
}

void TupMainWindow::setUndoRedoActions()
{
    QAction *undo = new QAction(QIcon(THEME_DIR + "icons/undo.png"), tr("Undo"), this);
    undo->setIconVisibleInMenu(true);
    undo->setShortcut(QKeySequence(tr("Ctrl+Z")));
    connect(undo, SIGNAL(triggered()), m_projectManager, SLOT(undo()));

    QAction *redo = new QAction(QIcon(THEME_DIR + "icons/redo.png"), tr("Redo"), this);
    redo->setIconVisibleInMenu(true);
    redo->setShortcut(QKeySequence(tr("Ctrl+Y")));
    connect(redo, SIGNAL(triggered()), m_projectManager, SLOT(redo()));

    kApp->insertGlobalAction(undo, "undo");
    kApp->insertGlobalAction(redo, "redo");
}

void TupMainWindow::importPapagayoLipSync()
{
    animationTab->importPapagayoLipSync();
}

void TupMainWindow::hideTopPanels()
{
    if (m_projectManager->isOpen()) {
        if (mainToolBar->isVisible()) {
            m_actionManager->find("hideaction")->setIcon(QIcon(QPixmap(THEME_DIR + "icons/show_top_panel.png")));
            menuBar()->setVisible(false);
            mainToolBar->setVisible(false);
        } else {
            m_actionManager->find("hideaction")->setIcon(QIcon(QPixmap(THEME_DIR + "icons/hide_top_panel.png")));
            menuBar()->setVisible(true);
            mainToolBar->setVisible(true);
        }
    }
}
