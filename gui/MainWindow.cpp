#include "MainWindow.h"

#include <QAction>
#include <QActionGroup>
#include <QCloseEvent>
#include <QGroupBox>
#include <QMenuBar>
#include <QMessageBox>
#include <QStackedWidget>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>

#include "CameraBtnBox.h"
#include "SnapshotParameterPane.h"
#include "ObservationParameterPane.h"
#include "SweepParameterPane.h"
#include "ConfigurationDlg.h"
#include "HistogramWidget.h"
#include "SnapshotWidget.h"
#include "tooling.h"

#include "core.h"
#include "core/Crystal.h"

namespace gui {

//------------------------------------------------------------------------------

MainWindow::MainWindow(core::Crystal *crystal,
                       core::AbstractCrysTempProbe *crysTempProb,
                       core::Core *coreInstance,
                       const QString &version)
    : QMainWindow()
    , _coreInstance(coreInstance)
    , _configDlg(new ConfigurationDlg(this, crystal))
    , _stackedWdgt(new QStackedWidget)
    , _snapshotModeActn(new QAction("Take &snapshots", this))
    , _observationModeActn(new QAction("Make &observations", this))
    , _sweepModeActn(new QAction("Sweep over &wavelength", this))
    , _configParamActn(new QAction("&Configure", this))
    , _version(version)
    , _snapshotPane(new SnapshotParameterPane(crystal, crysTempProb))
    , _observationPane(new ObservationParameterPane)
    , _sweepPane(new SweepParameterPane)
    , _cameraBtnBox(new CameraBtnBox)
    , _sessionEdit(new LineEdit)
    , _snapshot(new SnapshotWidget)
    , _histogram(new HistogramWidget)
{
    // -------------------------------------------------------------------------
    // Central widget
    // -------------------------------------------------------------------------

    // Left part
    auto leftLayout = new QVBoxLayout;
    _stackedWdgt->addWidget(_snapshotPane);
    _stackedWdgt->addWidget(_observationPane);
    _stackedWdgt->addWidget(_sweepPane);

    auto lineEditLayout = new QHBoxLayout;
    lineEditLayout->addWidget(new QLabel("Session:"));
    lineEditLayout->addWidget(_sessionEdit);

    leftLayout->addWidget(_stackedWdgt);
    leftLayout->addWidget(_cameraBtnBox);
    leftLayout->addLayout(lineEditLayout);


    // Middle part
    auto snapshotLayout = new QVBoxLayout;
    snapshotLayout->addWidget(_snapshot);

    auto snapshotBox = new QGroupBox(tr("Snapshot"));
    snapshotBox->setLayout(snapshotLayout);

    // Right part
    auto histogramBoxLayout = new QVBoxLayout;
    histogramBoxLayout->addWidget(_histogram);

    auto histogramBox = new QGroupBox(tr("Histogram"));
    histogramBox->setLayout(histogramBoxLayout);

    auto mainLayout = new QHBoxLayout;

    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(snapshotBox);
    mainLayout->addWidget(histogramBox);

    auto *centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


    // -------------------------------------------------------------------------
    // Actions
    // -------------------------------------------------------------------------

    _configParamActn->setIcon(QIcon(":/icons/C-gold-24.png"));
    _configParamActn->setIconVisibleInMenu(false);
    _configParamActn->setShortcut(QKeySequence("Alt+C"));
    _configParamActn->setStatusTip(tr("Switch to configuration mode"));
    connect(_configParamActn, QAction::triggered, _configDlg, ConfigurationDlg::display);

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

    auto releaseNotesAction = new QAction("&Release Notes", this);
    connect(releaseNotesAction, QAction::triggered, this, releaseNotes);

    auto aboutAction = new QAction("&About", this);
    connect(aboutAction, QAction::triggered, this, about);

    // -------------------------------------------------------------------------
    // Menus
    // -------------------------------------------------------------------------

    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(_configParamActn);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    auto modeMenu = menuBar()->addMenu(tr("&Mode"));
    modeMenu->addAction(_snapshotModeActn);
    modeMenu->addAction(_observationModeActn);
    modeMenu->addAction(_sweepModeActn);

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
    toolBar->addAction(_configParamActn);

    // -------------------------------------------------------------------------
    // Varia
    // -------------------------------------------------------------------------

    setWindowTitle("NO2 Camera - " + version);
    setWindowIcon(QIcon(":/icons/video-camera-64.png"));
    setFixedSize(sizeHint());
    _snapshotModeActn->setChecked(true);

    connect(_configDlg, ConfigurationDlg::crystalUpdated,
            _snapshotPane, SnapshotParameterPane::recomputeParams);
    if (!_configDlg->isValid())
        QTimer::singleShot(0, this, displayConfigurationDlg);

    connect(coreInstance, core::Core::ready, this, updateState);
    connect(coreInstance, core::Core::snapshotAvailable, this, updateSnapshot);

    connect(_snapshotPane, BaseParameterPane::parametersChanged, this, refreshBtns);
    connect(_observationPane, BaseParameterPane::parametersChanged, this, refreshBtns);
    connect(_sweepPane, BaseParameterPane::parametersChanged, this, refreshBtns);

    connect(_snapshotPane, SnapshotParameterPane::spectralSnapshot,
            coreInstance, core::Core::spectralSnapshot);
    connect(_snapshotPane, SnapshotParameterPane::acousticSnapshot,
            coreInstance, core::Core::acousticSnapshot);
    connect(_observationPane, ObservationParameterPane::observationRequested,
            coreInstance, core::Core::observation);
    connect(_sweepPane, SweepParameterPane::sweepRequested,
            coreInstance, core::Core::sweep);

    connect(this, stopped, coreInstance, core::Core::stop);
    connect(_cameraBtnBox, CameraBtnBox::started, this, start);
    connect(_cameraBtnBox, CameraBtnBox::stopped, this, stopped);

    refreshBtns();
}

//------------------------------------------------------------------------------

void MainWindow::start(bool burst, bool record)
{
    updateState(false);
    currentPane()->start(burst,
                         record,
                         _configDlg->stabilisationTime(),
                         _sessionEdit->text());
}

//------------------------------------------------------------------------------

void MainWindow::updateState(bool isAppReady)
{
    _snapshotModeActn->setEnabled(isAppReady);
    _observationModeActn->setEnabled(isAppReady);
    _sweepModeActn->setEnabled(isAppReady);
    _configParamActn->setEnabled(isAppReady);
    _snapshotPane->updateState(isAppReady);
    _observationPane->updateState(isAppReady);
    _sweepPane->updateState(isAppReady);
    _cameraBtnBox->updateState(isAppReady);
    _sessionEdit->setEnabled(isAppReady);
}

//------------------------------------------------------------------------------

void MainWindow::updateSnapshot()
{
    _snapshot->update();
    _histogram->update(_snapshot->histogramData());
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

void MainWindow::switchMode()
{
    if (_snapshotModeActn->isChecked())
    {
        _stackedWdgt->setCurrentIndex(0);
        refreshBtns();
    }
    else if (_observationModeActn->isChecked())
    {
        _stackedWdgt->setCurrentIndex(1);
        refreshBtns();
    }
    else if (_sweepModeActn->isChecked())
    {
        _stackedWdgt->setCurrentIndex(2);
        refreshBtns();
    }
    else
    {
        _configDlg->exec();
    }
}

//------------------------------------------------------------------------------

void MainWindow::refreshBtns()
{
    _cameraBtnBox->enableBtns(currentPane()->areParametersValid(), true);
}

//------------------------------------------------------------------------------

void MainWindow::configure()
{
    _configDlg->exec();
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

void MainWindow::releaseNotes()
{
    QMessageBox::information
    ( this
      , tr("NO<sub>2</sub> Camera Command Interface")
      , "<h2>" + tr("Release notes of the version") + " " + _version + "</h2>"
      + "<h3>" + tr("Implemented functionalities") + "</h3>"

      + "<p> This version contains a full implementation of the GUI (with the "
      + "exception of the data folder selection) and the business logic "
      + "required by the  GUI.</p>"

      + "<h3>" + tr("Missing functionalities") + "</h3>"

      + "<p> All devices (camera, electronic boards, temperator sensor) are "
      + "mockups and there is no mechanism yet to store the snapshot data on "
      + "disk.</p>"
      , QMessageBox::Ok);
}

//------------------------------------------------------------------------------

BaseParameterPane *MainWindow::currentPane()
{
    return dynamic_cast<BaseParameterPane *>(_stackedWdgt->currentWidget());
}

//------------------------------------------------------------------------------

}
