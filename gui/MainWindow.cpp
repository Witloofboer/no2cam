#include "MainWindow.h"

#include <QAction>
#include <QActionGroup>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QGroupBox>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTimer>
#include <QThread>
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

#include "core/Manager.h"
#include "core/Crystal.h"

//------------------------------------------------------------------------------

static void init_resource()
{
    Q_INIT_RESOURCE(resources);
}

//------------------------------------------------------------------------------

namespace gui {

//------------------------------------------------------------------------------

MainWindow::MainWindow(core::Crystal *crystal,
                       core::Manager *coreInstance,
                       const QString &subversion,
                       const QString &devicesNotes)
    : QMainWindow()
    , _coreManager(coreInstance)
    , _configDlg(new ConfigurationDlg(this, crystal))
    , _stackedWdgt(new QStackedWidget)
    , _selectFolderActn(new QAction("&Select data folder"))
    , _snapshotModeActn(new QAction("Take &snapshots", this))
    , _observationModeActn(new QAction("Make &observations", this))
    , _sweepModeActn(new QAction("Sweep over &wavelength", this))
    , _configParamActn(new QAction("&Configure", this))
    , _dataFolder()
    , _version(tr("0.3.0")
               + (subversion.isEmpty()
                  ? QString("")
                  : QString(" (%1)").arg(subversion)))
    , _devicesNotes(devicesNotes)
    , _snapshotPane(new SnapshotParameterPane(crystal))
    , _observationPane(new ObservationParameterPane)
    , _sweepPane(new SweepParameterPane)
    , _cameraBtnBox(new CameraBtnBox)
    , _sessionEdit(new LineEdit)
    , _snapshot(new SnapshotWidget)
    , _histogram(new HistogramWidget)
    , _infoWdgt(new QLabel)
    , _temperatureWdgt(new QLabel)
    , _infoT(new QTimer)
{
    init_resource();

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

    auto lineEditBox = new QGroupBox;
    lineEditBox->setLayout(lineEditLayout);

    leftLayout->addWidget(_stackedWdgt);
    leftLayout->addWidget(lineEditBox);
    leftLayout->addWidget(_cameraBtnBox);

    _cameraBtnBox->setStyleSheet("margin-bottom: 0px");

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

    auto topLayout = new QHBoxLayout;

    topLayout->addLayout(leftLayout);
    topLayout->addWidget(snapshotBox);
    topLayout->addWidget(histogramBox);

    // -------------------------------------------------------------------------
    // Info box
    // -------------------------------------------------------------------------

    auto bottomLayout = new QHBoxLayout;

    auto infoBox = new QGroupBox;
    auto infoBoxLayout = new QHBoxLayout;
    infoBoxLayout->addWidget(_infoWdgt);
    infoBoxLayout->addStretch();
    infoBox->setLayout(infoBoxLayout);
    infoBox->setTitle("Info");

    auto temperatureBox = new QGroupBox;
    auto temperatureBoxLayout = new QHBoxLayout;
    temperatureBoxLayout->addWidget(_temperatureWdgt);
    temperatureBox->setLayout(temperatureBoxLayout);
    temperatureBox->setTitle("T [°C]");

    bottomLayout->addWidget(infoBox);
    bottomLayout->addWidget(temperatureBox);


    _infoT->setSingleShot(true);
    _infoT->setInterval(4000);
    connect(_infoT, QTimer::timeout, this, onInfoTimer);

    statusBar()->hide();

    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    auto *centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


    // -------------------------------------------------------------------------
    // Actions
    // -------------------------------------------------------------------------

    connect(_selectFolderActn, QAction::triggered, this, onSelectFolder);

    _configParamActn->setIcon(QIcon(":/icons/C-gold-24.png"));
    _configParamActn->setIconVisibleInMenu(false);
    _configParamActn->setShortcut(QKeySequence("Alt+C"));
    _configParamActn->setStatusTip(tr("Switch to configuration mode"));
    connect(_configParamActn, QAction::triggered, _configDlg, ConfigurationDlg::onDisplay);

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
    connect(_snapshotModeActn, QAction::triggered, this, onSwitchMode);

    _observationModeActn->setIcon(QIcon(":/icons/O-blue-24.png"));
    _observationModeActn->setIconVisibleInMenu(false);
    _observationModeActn->setCheckable(true);
    _observationModeActn->setShortcut(QKeySequence("Alt+O"));
    _observationModeActn->setStatusTip(tr("Switch to observation mode"));
    connect(_observationModeActn, QAction::triggered, this, onSwitchMode);

    _sweepModeActn->setIcon(QIcon(":/icons/W-blue-24.png"));
    _sweepModeActn->setIconVisibleInMenu(false);
    _sweepModeActn->setCheckable(true);
    _sweepModeActn->setShortcut(QKeySequence("Alt+W"));
    _sweepModeActn->setStatusTip(tr("Switch to wavelength sweeping mode"));
    connect(_sweepModeActn, QAction::triggered, this, onSwitchMode);

    auto releaseNotesAction = new QAction("&Release Notes", this);
    connect(releaseNotesAction, QAction::triggered, this, onDisplayReleaseNotes);

    auto aboutAction = new QAction("&About", this);
    connect(aboutAction, QAction::triggered, this, onAbout);

    // -------------------------------------------------------------------------
    // Menus
    // -------------------------------------------------------------------------

    auto fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(_selectFolderActn);
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

    setWindowIcon(QIcon(":/icons/video-camera-64.png"));
    setFixedSize(sizeHint());
    _snapshotModeActn->setChecked(true);

    connect(_configDlg, ConfigurationDlg::parametersUpdated,
            this, onParametersUpdated);
    if (!_configDlg->isValid())
        QTimer::singleShot(0, this, onDisplayConfigurationDlg); //TODO emit instead of timer?

    connect(coreInstance, core::Manager::updateApplicationReadiness, this, onUpdateApplicationReadiness);
    connect(coreInstance, core::Manager::displaySnapshot, this, onDisplaySnapshot);

    connect(_snapshotPane, BaseParameterPane::parametersChanged, this, refreshBtns);
    connect(_observationPane, BaseParameterPane::parametersChanged, this, refreshBtns);
    connect(_sweepPane, BaseParameterPane::parametersChanged, this, refreshBtns);

    connect(_snapshotPane, SnapshotParameterPane::opticalSnapshot,
            coreInstance, core::Manager::onOpticalSnapshot);
    connect(_snapshotPane, SnapshotParameterPane::acousticSnapshot,
            coreInstance, core::Manager::onAcousticSnapshot);
    connect(_observationPane, ObservationParameterPane::observationRequested,
            coreInstance, core::Manager::onObservation);
    connect(_sweepPane, SweepParameterPane::sweepRequested,
            coreInstance, core::Manager::onSweep);

    connect(this, onStop, coreInstance, core::Manager::onStop);
    connect(this, shutdownRequested, coreInstance, core::Manager::onShutdown);
    connect(this, temperaturePeriodUpdated, coreInstance, core::Manager::onTemperaturePeriodUpdated);
    connect(_cameraBtnBox, CameraBtnBox::started, this, onStart);
    connect(_cameraBtnBox, CameraBtnBox::stopped, this, onStop);

    connect(coreInstance, core::Manager::fileCreationError, this, onFileCreationError);
    connect(coreInstance, core::Manager::fileWritingError, this, onFileWritingError);

    connect(coreInstance, core::Manager::updateTemperature,
            this, onUpdateTemperature);

    connect(coreInstance, core::Manager::displayInformation,
            this, onDisplayInformation);
    refreshBtns();
    restore();

    onParametersUpdated();
}

//------------------------------------------------------------------------------

void MainWindow::onStart(bool burst, bool record)
{
    if (record)
    {
        QDir dir(_dataFolder);
        bool ok=dir.mkpath(".");

        if (!ok)
        {
            QMessageBox::critical
            ( this
              , tr("NO2 Camera: Operation cancelled")
              , QString(tr("<p>The data folder %1 does not exist and can not be "
                           "created.</p>"
                           "<p>Please use another data directory or enable its "
                           "creation before re-attempting the recording.</p>")
                       ).arg(_dataFolder)
              , QMessageBox::Ok);
            onUpdateApplicationReadiness(true);
            return;
        }
    }

    onUpdateApplicationReadiness(false);
    currentPane()->start(burst,
                         record,
                         _configDlg->stabilisingTime(),
                         _sessionEdit->text(),
                         _dataFolder);
}

//------------------------------------------------------------------------------

void MainWindow::onUpdateApplicationReadiness(bool isAppReady)
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

void MainWindow::onDisplaySnapshot()
{
    _snapshot->update();
    _histogram->update(_snapshot->histogramData());
}

//------------------------------------------------------------------------------

void MainWindow::onDisplayInformation(QString msg)
{
    _infoWdgt->setText(msg);
    _infoT->start();
}

//------------------------------------------------------------------------------

void MainWindow::onInfoTimer()
{
    _infoWdgt->setText("");
}

//------------------------------------------------------------------------------

void MainWindow::onDisplayConfigurationDlg()
{
    QMessageBox::warning
    ( this
      , tr("NO2 Camera Command Interface")
      , tr("<h3> Welcome to NO<sub>2</sub> Camera Command Interface</h3>"
           "<p>No valid configuration was found from a previous usage.</p>"
           "<p>Therefore, you need to provide the parameters to use the application.</p>")
      , QMessageBox::Ok);
    _configDlg->onDisplay(true);
}

//------------------------------------------------------------------------------

void MainWindow::onParametersUpdated()
{
    _snapshotPane->recomputeParams();
    emit temperaturePeriodUpdated(_configDlg->temperaturePeriod());
}

//------------------------------------------------------------------------------

void MainWindow::onUpdateTemperature(double temperature)
{
    _snapshotPane->updateTemperature(temperature);
    _temperatureWdgt->setText(QString("%1").arg(temperature, 5, 'f', 2));
}

//------------------------------------------------------------------------------

void MainWindow::onFileCreationError(QString datafolder,
                                     QString filename)
{
    QMessageBox::critical
    ( this
      , tr("NO2 Camera: Operation cancelled")
      , QString(tr("<p><b>Failure to open file in data folder.</b></p>"
                   "<p><b>File:</b> %1<br/><b>Folder:</b> %2</p>"
                   "<p>Hint: Check the folder access rights.</p>"
                  )).arg(filename).arg(datafolder)
      , QMessageBox::Ok);
}

//------------------------------------------------------------------------------

void MainWindow::onFileWritingError(QString datafolder,
                                    QString filename)
{
    QMessageBox::critical
    ( this
      , tr("NO2 Camera: Operation cancelled")
      , QString(tr("<p><b>Failure to dump snapshot in file.</b></p>"
                   "<p><b>File:</b> %1<br/><b>Folder:</b> %2</p>"
                   "<p>Hint: check the space left on the device and your usage"
                   "quota.</p>"
                  )).arg(filename).arg(datafolder)
      , QMessageBox::Ok);
}

//------------------------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent *event)
{
    auto coreThread = _coreManager->thread();
    // It is necessary to record the actual core thread as _coreManager is
    // moved back to the main application thread upon exiting. Depending on
    // race conditions, coreThread->wait() could be executed on the main thread.

    emit shutdownRequested();
    persiste();
    _configDlg->persiste();
    _snapshotPane->persiste();
    _observationPane->persiste();
    _sweepPane->persiste();
    coreThread->wait();

    QMainWindow::closeEvent(event);
}

//------------------------------------------------------------------------------

void MainWindow::onSwitchMode()
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


void MainWindow::onSelectFolder()
{
    _dataFolder = QFileDialog::getExistingDirectory
                  (this,
                   tr("Select data folder"),
                   _dataFolder,
                   QFileDialog::ShowDirsOnly
                  );
    refreshWindowTitle();
}

//------------------------------------------------------------------------------

void MainWindow::onConfigure()
{
    _configDlg->exec();
}

//------------------------------------------------------------------------------

void MainWindow::onAbout()
{
    QMessageBox::about
    ( this
      , tr("NO2 Camera Command Interface")
      , tr("<h3>NO<sub>2</sub> Camera Command Interface</h3>"
           "<p>Version: %1</p>"
           "<p>Author: Didier Pieroux (didier.pieroux@aeronomie.be)</p>"
           "<p>Copyright 2016-2017 BIRA-IASB</p>"
           "<p>THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY "
           "KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE "
           "WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE "
           "AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT "
           "HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, "
           "WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
           "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER "
           "DEALINGS IN THE SOFTWARE.</p>"
          ).arg(_version));
}

//------------------------------------------------------------------------------

void MainWindow::onDisplayReleaseNotes()
{
    QMessageBox::information
    ( this
      , tr("NO2 Camera Command Interface")
      , tr("<h2>NO<sub>2</sub> Camera Command Interface</h2>"
           "<h3> Version: %1 </h3>"
           "<h3>Functionalities</h3>"
           "%2"
           "<h3>Devices</h3>"
           "%3"
          )
      .arg(_version)
      .arg("<p>All initially foreseen functionalities are implemented.</p>")
      .arg(_devicesNotes)
      , QMessageBox::Ok);
}

//------------------------------------------------------------------------------

BaseParameterPane *MainWindow::currentPane()
{
    return dynamic_cast<BaseParameterPane *>(_stackedWdgt->currentWidget());
}

//------------------------------------------------------------------------------

static const char *dataFolderLbl = "data folder";

void MainWindow::persiste()
{
    qDebug("Persisting main window parameters");

    QSettings settings;

    settings.setValue(dataFolderLbl, _dataFolder);
}

//------------------------------------------------------------------------------

void MainWindow::restore()
{
    qDebug("Restoring main window parameters");

    QSettings settings;

    _dataFolder = settings.value(dataFolderLbl, "").toString();
    if (_dataFolder.isEmpty())
        _dataFolder = QDir::homePath()+"/no2cam";

    refreshWindowTitle();
}

//------------------------------------------------------------------------------

void MainWindow::refreshWindowTitle()
{
    setWindowTitle("NO2 Camera " + _version +
                   " - "+ QDir::toNativeSeparators(_dataFolder));
}

//------------------------------------------------------------------------------

}
