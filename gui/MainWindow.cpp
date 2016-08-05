#include "MainWindow.h"

#include <QAction>
#include <QActionGroup>
#include <QCloseEvent>
#include <QMenuBar>
#include <QMessageBox>
#include <QTimer>
#include <QToolBar>
#include <QStackedWidget>

#include "SnapshotPane.h"
#include "ObservationPane.h"
#include "SweepPane.h"
#include "ConfigurationDlg.h"

#include "core.h"
#include "core/Crystal.h"

namespace gui {

//------------------------------------------------------------------------------

MainWindow::MainWindow(const QString &version)
    : QMainWindow()
    , _stackedWdgt(new QStackedWidget)
    , _snapshotModeActn(new QAction("Take &snapshots", this))
    , _observationModeActn(new QAction("Make &observations", this))
    , _sweepModeActn(new QAction("Sweep over &wavelength", this))
    , _configDlg(new ConfigurationDlg(this))
    , _version(version)
    , _snapshotPane(new SnapshotPane(_configDlg->refCrystal()))
    , _observationPane(new ObservationPane(_configDlg->refCrystal()))
    , _sweepPane(new SweepPane(_configDlg->refCrystal()))
{
    // -------------------------------------------------------------------------
    // Central widget
    // -------------------------------------------------------------------------

    _stackedWdgt->addWidget(_snapshotPane);
    _stackedWdgt->addWidget(_observationPane);
    _stackedWdgt->addWidget(_sweepPane);

    setCentralWidget(_stackedWdgt);

    // -------------------------------------------------------------------------
    // Actions
    // -------------------------------------------------------------------------

    auto newAction = new QAction("&New", this);
    auto loadAction = new QAction("&Load", this);
    auto saveAction = new QAction("&Save", this);
    auto saveAsAction = new QAction("Save &as", this);
    connect(newAction, QAction::triggered, this, newSession);
    connect(loadAction, QAction::triggered, this, loadSession);
    connect(saveAction, QAction::triggered, this, saveSession);
    connect(saveAsAction, QAction::triggered, this, saveAsSession);

    auto configureAction = new QAction("&Configure", this);
    configureAction->setIcon(QIcon(":/icons/C-gold-24.png"));
    configureAction->setIconVisibleInMenu(false);
    configureAction->setShortcut(QKeySequence("Alt+C"));
    configureAction->setStatusTip(tr("Switch to configuration mode"));
    connect(configureAction, QAction::triggered, _configDlg, ConfigurationDlg::display);

    auto exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence("Alt+F4"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, &QAction::triggered, this, close);

    auto modeGroup = new QActionGroup(this);
    modeGroup->addAction(_snapshotModeActn);
    modeGroup->addAction(_observationModeActn);
    modeGroup->addAction(_sweepModeActn);

    _snapshotModeActn->setIcon(QIcon(":/icons/S-blue-24.png"));
    _snapshotModeActn->setIconVisibleInMenu(false);
    _snapshotModeActn->setCheckable(true);
    _snapshotModeActn->setShortcut(QKeySequence("Alt+S"));
    _snapshotModeActn->setStatusTip(tr("Switch to snapshot mode"));
    connect(_snapshotModeActn, QAction::triggered, this, switchMode);

    _observationModeActn->setIcon(QIcon(":/icons/O-blue-24.png"));
    _observationModeActn->setIconVisibleInMenu(false);
    _observationModeActn->setCheckable(true);
    _observationModeActn->setShortcut(QKeySequence("Alt+O"));
    _observationModeActn->setStatusTip(tr("Switch to observation mode"));
    connect(_observationModeActn, QAction::triggered, this, switchMode);

    _sweepModeActn->setIcon(QIcon(":/icons/W-blue-24.png"));
    _sweepModeActn->setIconVisibleInMenu(false);
    _sweepModeActn->setCheckable(true);
    _sweepModeActn->setShortcut(QKeySequence("Alt+W"));
    _sweepModeActn->setStatusTip(tr("Switch to wavelength sweeping mode"));
    connect(_sweepModeActn, QAction::triggered, this, switchMode);

    auto cameraStatusAction = new QAction("&Camera status", this);
    connect(cameraStatusAction, QAction::triggered, this, cameraStatus);

    auto releaseNotesAction = new QAction("&Release Notes", this);
    connect(releaseNotesAction, QAction::triggered, this, releaseNotes);

    auto aboutAction = new QAction("&About", this);
    connect(aboutAction, QAction::triggered, this, about);

    // -------------------------------------------------------------------------
    // Menus
    // -------------------------------------------------------------------------

    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(loadAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(configureAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    auto modeMenu = menuBar()->addMenu(tr("&Mode"));
    modeMenu->addAction(_snapshotModeActn);
    modeMenu->addAction(_observationModeActn);
    modeMenu->addAction(_sweepModeActn);

    auto deviceMenu = menuBar()->addMenu(tr("&Devices"));
    deviceMenu->addAction(cameraStatusAction);

    auto helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(releaseNotesAction);
    helpMenu->addAction(aboutAction);

    // -------------------------------------------------------------------------
    // Toolbar
    // -------------------------------------------------------------------------

    auto toolBar = addToolBar(tr("&Mode"));
    toolBar->addAction(_snapshotModeActn);
    toolBar->addAction(_observationModeActn);
    toolBar->addAction(_sweepModeActn);
    toolBar->addSeparator();
    toolBar->addAction(configureAction);

    // -------------------------------------------------------------------------
    // Varia
    // -------------------------------------------------------------------------

    setWindowTitle("NO2 Camera - " + version);
    setWindowIcon(QIcon(":/icons/video-camera-64.png"));
    setFixedSize(sizeHint());
    _snapshotModeActn->setChecked(true);

    connect(_configDlg, ConfigurationDlg::crystalUpdated,
            _snapshotPane, SnapshotPane::recomputeParams);
    if (!_configDlg->isValid())
        QTimer::singleShot(0, this, displayConfigurationDlg);
}

//------------------------------------------------------------------------------

void MainWindow::displayConfigurationDlg()
{
    QMessageBox::warning
        ( this
        , tr("NO2> Camera Command Interface")
        , "<h3>" + tr("Welcome to NO<sub>2</sub> Camera Command Interface") + "</h3>" +
          tr("<p>No valid configuration was found from a previous usage.</p>"
          "<p>Therefore, you need to provide the parameters to use the application.</p>")
        , QMessageBox::Ok);
    _configDlg->display(true);
}

//------------------------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent *event)
{
    _configDlg->persiste();
    _snapshotPane->persiste();
    _observationPane->persiste();
    _sweepPane->persiste();
    QMainWindow::closeEvent(event);
}

//------------------------------------------------------------------------------

void MainWindow::newSession()
{
    qWarning("New session NOT IMPLEMENTED"); //TODO
}

//------------------------------------------------------------------------------

void MainWindow::loadSession()
{
    qWarning("Load session NOT IMPLEMENTED"); //TODO
}

//------------------------------------------------------------------------------

bool MainWindow::saveSession()
{
    qWarning("Save session NOT IMPLEMENTED"); //TODO
    return true;
}

//------------------------------------------------------------------------------

bool MainWindow::saveAsSession()
{
    qWarning("Save as session NOT IMPLEMENTED"); //TODO
    return true;
}


//------------------------------------------------------------------------------

void MainWindow::switchMode()
{
    if (_snapshotModeActn->isChecked())
    {
        _stackedWdgt->setCurrentIndex(0);
    }
    else if (_observationModeActn->isChecked())
    {
        _stackedWdgt->setCurrentIndex(1);
    }
    else if (_sweepModeActn->isChecked())
    {
        _stackedWdgt->setCurrentIndex(2);
    }
    else
    {
        _configDlg->exec();
    }
}

//------------------------------------------------------------------------------

void MainWindow::configure()
{
    _configDlg->exec();
}

//------------------------------------------------------------------------------

void MainWindow::cameraStatus()
{
    qWarning("Save session NOT IMPLEMENTED");
}

//------------------------------------------------------------------------------

void MainWindow::about()
{
    QMessageBox::about
            ( this
            , tr("NO2> Camera Command Interface")
            , tr("<h3>NO<sub>2</sub> Camera Commander</h3>")
            + "<p>" + tr("Version") + ": " + _version + "</p>"
            + tr("<p>Author: Didier Pieroux (didier.pieroux@aeronomie.be)</p>")
            + tr("<p>Copyright 2016 BIRA-IASB</p>")
            + tr("<p>This program is provided AS IS, with NO WARRANTY OF ANY "
                 "KIND.</p>")
              );
}

//------------------------------------------------------------------------------

void MainWindow::coreReady()
{
    //TODO
}

//------------------------------------------------------------------------------

void MainWindow::coreBusy()
{
    //TODO
}

//------------------------------------------------------------------------------

void MainWindow::releaseNotes()
{
    QMessageBox::information
            ( this
            , tr("NO<sub>2</sub> Camera Command Interface")
            , "<h2>" + tr("Release notes") + "</h2>"
            + "<h3>" + tr("Version") + " " + _version + "</h3>"
            + tr("<p>Only the GUI is implemented in this version.</p>"
                 "<p>As a consequence of the lack of logic, the snapshot button"
                 "   remains depressed when clicked. In the final version, it"
                 "   will automatically get released once the snapshot is"
                 "   acquired.</p>"
                )
            , QMessageBox::Ok);
}

//------------------------------------------------------------------------------

bool MainWindow::okToContinue()
{

    if (isWindowModified())
    {
        auto code = QMessageBox::warning
                ( this
                , tr("NO<sub>2</sub> Camera Command Interface")
                , tr("The session has been modified.\n"
                     "Do you want to save your changes?")
                , QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        switch (code)
        {
        case QMessageBox::Yes:
            return saveSession();
        case QMessageBox::No:
            return true;
        case QMessageBox::Cancel:
            return false;
        default:
            Q_UNREACHABLE();
        }
    }
    else
        return true;
}

//------------------------------------------------------------------------------

}
