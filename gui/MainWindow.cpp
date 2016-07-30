#include "MainWindow.h"

#include <QAction>
#include <QActionGroup>
#include <QCloseEvent>
#include <QMenuBar>
#include <QMessageBox>
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

MainWindow::MainWindow(const QString &version, QWidget *parent)
    : QMainWindow(parent)
    , stackedWidget(new QStackedWidget)
    , snapshotModeAction(new QAction("Take &snapshots", this))
    , observationModeAction(new QAction("Make &observations", this))
    , sweepModeAction(new QAction("Sweep over &wavelength", this))
    , configurationDlg(new ConfigurationDlg(this))
    , version(version)
    , snapshotPane(new SnapshotPane(configurationDlg))
{
    // -------------------------------------------------------------------------
    // Central widget
    // -------------------------------------------------------------------------

    stackedWidget->addWidget(snapshotPane);
    stackedWidget->addWidget(new ObservationPane());
    stackedWidget->addWidget(new SweepingPane());

    setCentralWidget(stackedWidget);

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
    connect(configureAction, QAction::triggered, configurationDlg, ConfigurationDlg::display);

    auto exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence("Alt+F4"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, &QAction::triggered, this, close);

    auto modeGroup = new QActionGroup(this);
    modeGroup->addAction(snapshotModeAction);
    modeGroup->addAction(observationModeAction);
    modeGroup->addAction(sweepModeAction);

    snapshotModeAction->setIcon(QIcon(":/icons/S-blue-24.png"));
    snapshotModeAction->setIconVisibleInMenu(false);
    snapshotModeAction->setCheckable(true);
    snapshotModeAction->setShortcut(QKeySequence("Alt+S"));
    snapshotModeAction->setStatusTip(tr("Switch to snapshot mode"));
    connect(snapshotModeAction, QAction::triggered, this, switchMode);

    observationModeAction->setIcon(QIcon(":/icons/O-blue-24.png"));
    observationModeAction->setIconVisibleInMenu(false);
    observationModeAction->setCheckable(true);
    observationModeAction->setShortcut(QKeySequence("Alt+O"));
    observationModeAction->setStatusTip(tr("Switch to observation mode"));
    connect(observationModeAction, QAction::triggered, this, switchMode);

    sweepModeAction->setIcon(QIcon(":/icons/W-blue-24.png"));
    sweepModeAction->setIconVisibleInMenu(false);
    sweepModeAction->setCheckable(true);
    sweepModeAction->setShortcut(QKeySequence("Alt+W"));
    sweepModeAction->setStatusTip(tr("Switch to wavelength sweeping mode"));
    connect(sweepModeAction, QAction::triggered, this, switchMode);

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
    modeMenu->addAction(snapshotModeAction);
    modeMenu->addAction(observationModeAction);
    modeMenu->addAction(sweepModeAction);

    auto deviceMenu = menuBar()->addMenu(tr("&Devices"));
    deviceMenu->addAction(cameraStatusAction);

    auto helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(releaseNotesAction);
    helpMenu->addAction(aboutAction);

    // -------------------------------------------------------------------------
    // Toolbar
    // -------------------------------------------------------------------------

    auto toolBar = addToolBar(tr("&Mode"));
    toolBar->addAction(snapshotModeAction);
    toolBar->addAction(observationModeAction);
    toolBar->addAction(sweepModeAction);
    toolBar->addSeparator();
    toolBar->addAction(configureAction);

    // -------------------------------------------------------------------------
    // Varia
    // -------------------------------------------------------------------------

    setWindowTitle("NO2 Camera - " + version);
    setWindowIcon(QIcon(":/icons/video-camera-64.png"));
    setFixedSize(sizeHint());
    snapshotModeAction->setChecked(true);
    connect(configurationDlg, ConfigurationDlg::parametersUpdated,
            snapshotPane, SnapshotPane::refreshParameters);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    snapshotPane->persiste(); // TODO also for the other panes.
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
    if (snapshotModeAction->isChecked())
    {
        stackedWidget->setCurrentIndex(0);
    }
    else if (observationModeAction->isChecked())
    {
        stackedWidget->setCurrentIndex(1);
    }
    else if (sweepModeAction->isChecked())
    {
        stackedWidget->setCurrentIndex(2);
    }
    else
    {
        configurationDlg->exec();
    }
}

//------------------------------------------------------------------------------

void MainWindow::configure()
{
    configurationDlg->exec();
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
            (this,
             tr("About NO2_CAM"),
             tr("<h3>NO<sub>2</sub> Camera Control Software</h3>") +
             "<p>" + tr("Version") + ": " + version + "</p>" +
             tr("<p>Author: Didier Pieroux (didier.pieroux@aeronomie.be)</p>") +
             tr("<p>Copyright 2016 BIRA-IASB</p>") +
             tr("<p>This program is provided AS IS, with NO WARRANTY OF ANY "
                "KIND.</p>")
             );
}

//------------------------------------------------------------------------------

void MainWindow::releaseNotes()
{
    QMessageBox::information
            (this,
             tr("NO2_CAM"),
             "<h2>" + tr("Release notes") + "</h2>" +
             "<h3>" + tr("Version") + " " + version + "</h3>" +
             tr("<p>Only the GUI is implemented in this version.</p>"
                "<p>As a consequence of the lack of logic, the snapshot button"
                "   remains depressed when clicked. In the final version, it"
                "   will automatically get released once the snapshot is"
                "   acquired.</p>"
                ),
             QMessageBox::Ok);
}

//------------------------------------------------------------------------------

bool MainWindow::okToContinue()
{

    if (isWindowModified())
    {
        auto code = QMessageBox::warning
                (this,
                 tr("NO2_CAM"),
                 tr("The session has been modified.\n"
                    "Do you want to save your changes?"),
                 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
                 );

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
