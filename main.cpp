#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QProcess>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QSettings>
#include <QEvent>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMap>
#include <QTimer>
#include <QDateTime>
#include <QUrl>
#include <QMenu>
#include <QAction>
#include <QComboBox>
#include <QRegularExpression>
#include <QWebEngineView>
#include <QTabWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QInputDialog>
#include <QSlider>
#include <QStyleFactory>

class AppLauncher : public QWidget {
    Q_OBJECT

public:
    AppLauncher(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void handleFilesClick();
    void handleWebAppsClick();
    void handleMusicAppsClick();
    void handleGamingAppsClick();
    void handlePhotoEditingClick();
    void handleMultiPurposeAppsClick();
    void handleInformationClick();
    void searchApplications(const QString &searchText);
    void updateDateTime();
    void openSupportLink();
    void updateSystemInfo();
    void handleSignOut();
    void handleReboot();
    void handleShutdown();
    void handleUpdateSystem();
    void handleChangeVolume();
    void handleOpenTerminal();
    void handlePickMusicClick();
    void handlePlayPauseClick();
    void handleChooseBackgroundClick();
    void openBrowserTab();
    void closeBrowserTab();
    void onVolumeSliderValueChanged(int value);
    void handleScreenshotClick();
    void handleRecordClick();

private:
    QLineEdit *searchBar;
    QWidget *appGridContainer;
    QGridLayout *appGridLayout;
    QLabel *background;
    QPushButton *searchButton;
    QLabel *hoverBox;
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    QMap<QString, QMap<QString, QPair<QString, QString>>> menuMap;
    QLabel *dateTimeLabel;
    QLabel *versionLabel;
    QLabel *hoverTextLabel;
    QLabel *systemInfoLabel;
    QPushButton *systemMenuButton;
    QMenu *systemMenu;
    QPushButton *terminalButton;
    QPushButton *updateButton;
    QPushButton *volumeButton;
    QPushButton *playPauseButton;
    QPushButton *pickMusicButton;
    QPushButton *chooseBackgroundButton;
    QPushButton *screenshotButton;
    QPushButton *recordButton;
    QComboBox *musicDropdown;
    QComboBox *backgroundDropdown;
    QMediaPlayer *musicPlayer;
    QAudioOutput *musicAudioOutput;
    bool isPlaying;
    QTabWidget *browserTabs;
    QVBoxLayout *mainLayout;
    QWidget *mainWidget;
    QPushButton *activeMenuButton;
    QLabel *menuLabel;
    QList<QProcess*> activeProcesses; // Track active processes
    QSlider *volumeSlider;
    QLabel *volumePercentageLabel; // Label to display volume percentage
    bool isRecording;

    QString readImagePathFromFile(const QString &filePath);
    void setBackgroundImage(const QString &imagePath);
    void setSearchButtonIcon();
    void addIconButton(QHBoxLayout *layout, const QString &iconPath, const QString &tooltip, const char *slot);
    void playClickSound();
    void populateMenu(const QString &menuName);
    void addApplication(const QString &name, const QString &exec, const QString &icon = "");
    void launchApplication(const QString &exec);
    void executeBashCommand(const QString &command);
    void loadMusicFiles();
    void loadBackgroundImages();
    void searchSystemApplications(const QString &searchText);
    QString resolveIconPath(const QString &iconName);
    void highlightMenuButton(QPushButton *button);
    void clearMenuButtonHighlights();
    void showNotification(const QString &message);
    void playSound(const QString &soundFile);
};

AppLauncher::AppLauncher(QWidget *parent) : QWidget(parent), isPlaying(false), activeMenuButton(nullptr), isRecording(false) {
    setWindowState(Qt::WindowFullScreen);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mainWidget = new QWidget(this);
    QVBoxLayout *mainWidgetLayout = new QVBoxLayout(mainWidget);
    mainWidgetLayout->setAlignment(Qt::AlignTop);
    mainWidgetLayout->setSpacing(0);
    mainWidgetLayout->setContentsMargins(0, 0, 0, 0);

    // Background image
    background = new QLabel(mainWidget);
    setBackgroundImage("background.txt");
    background->setGeometry(0, 0, width(), height());
    background->lower(); // Ensure the background is at the bottom

    // Top bar layout
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    topBarLayout->setAlignment(Qt::AlignTop);
    topBarLayout->setContentsMargins(10, 10, 10, 0);

    terminalButton = new QPushButton(mainWidget);
    terminalButton->setIcon(QIcon("icons/terminal.png"));
    terminalButton->setIconSize(QSize(32, 32));
    terminalButton->setFixedSize(40, 40);
    terminalButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    terminalButton->setToolTip("Open Terminal");
    connect(terminalButton, &QPushButton::clicked, this, &AppLauncher::handleOpenTerminal);
    topBarLayout->addWidget(terminalButton, 0, Qt::AlignLeft);

    screenshotButton = new QPushButton(mainWidget);
    screenshotButton->setIcon(QIcon("icons/screenshot.png"));
    screenshotButton->setIconSize(QSize(32, 32));
    screenshotButton->setFixedSize(40, 40);
    screenshotButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    screenshotButton->setToolTip("Take Screenshot");
    connect(screenshotButton, &QPushButton::clicked, this, &AppLauncher::handleScreenshotClick);
    topBarLayout->addWidget(screenshotButton, 0, Qt::AlignLeft);

    recordButton = new QPushButton(mainWidget);
    recordButton->setIcon(QIcon("icons/record.png"));
    recordButton->setIconSize(QSize(32, 32));
    recordButton->setFixedSize(40, 40);
    recordButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    recordButton->setToolTip("Record Screen");
    connect(recordButton, &QPushButton::clicked, this, &AppLauncher::handleRecordClick);
    topBarLayout->addWidget(recordButton, 0, Qt::AlignLeft);

    topBarLayout->addStretch();

    updateButton = new QPushButton(mainWidget);
    updateButton->setIcon(QIcon("icons/update.png"));
    updateButton->setIconSize(QSize(32, 32));
    updateButton->setFixedSize(40, 40);
    updateButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    updateButton->setToolTip("Update System");
    connect(updateButton, &QPushButton::clicked, this, &AppLauncher::handleUpdateSystem);
    topBarLayout->addWidget(updateButton, 0, Qt::AlignRight);

    // Volume button and slider
    volumeButton = new QPushButton(mainWidget);
    volumeButton->setIcon(QIcon("icons/sound.png"));
    volumeButton->setIconSize(QSize(32, 32));
    volumeButton->setFixedSize(40, 40);
    volumeButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    volumeButton->setToolTip("Change Volume");
    connect(volumeButton, &QPushButton::clicked, this, &AppLauncher::handleChangeVolume);
    topBarLayout->addWidget(volumeButton, 0, Qt::AlignRight);

    // Volume slider layout below the volume button
    QVBoxLayout *volumeSliderLayout = new QVBoxLayout();
    volumeSliderLayout->setAlignment(Qt::AlignRight);
    volumeSliderLayout->setSpacing(0);
    volumeSliderLayout->setContentsMargins(0, 0, 0, 0);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setSingleStep(10);
    volumeSlider->setPageStep(10);
    volumeSlider->setStyleSheet(
        "QSlider::groove:horizontal { background: gold; height: 10px; border-radius: 5px; } "
        "QSlider::handle:horizontal { background: teal; width: 20px; height: 20px; margin: -5px 0; border-radius: 10px; }"
    );
    volumeSlider->setVisible(false); // Initially hidden
    connect(volumeSlider, &QSlider::valueChanged, this, &AppLauncher::onVolumeSliderValueChanged);

    volumeSliderLayout->addWidget(volumeSlider);
    topBarLayout->addLayout(volumeSliderLayout);

    volumePercentageLabel = new QLabel("100%", mainWidget);
    volumePercentageLabel->setStyleSheet("QLabel { color: gold; font-size: 16px; }");
    volumePercentageLabel->setAlignment(Qt::AlignCenter);
    topBarLayout->addWidget(volumePercentageLabel, 0, Qt::AlignRight);

    systemMenuButton = new QPushButton(mainWidget);
    systemMenuButton->setIcon(QIcon("icons/systemmenu.png"));
    systemMenuButton->setIconSize(QSize(32, 32));
    systemMenuButton->setFixedSize(40, 40);
    systemMenuButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    systemMenuButton->setToolTip("System Menu");
    topBarLayout->addWidget(systemMenuButton, 0, Qt::AlignRight);

    mainWidgetLayout->addLayout(topBarLayout);

    // Date and time label
    dateTimeLabel = new QLabel(mainWidget);
    dateTimeLabel->setStyleSheet("QLabel { color: gold; font-size: 20px; }");
    dateTimeLabel->setAlignment(Qt::AlignCenter);
    mainWidgetLayout->addWidget(dateTimeLabel, 0, Qt::AlignTop | Qt::AlignHCenter);

    // System info label
    systemInfoLabel = new QLabel(mainWidget);
    systemInfoLabel->setStyleSheet("QLabel { color: gold; font-size: 20px; }");
    systemInfoLabel->setAlignment(Qt::AlignCenter);
    mainWidgetLayout->addWidget(systemInfoLabel, 0, Qt::AlignTop | Qt::AlignHCenter);

    // Search bar layout
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    searchLayout->setSpacing(0);
    searchLayout->setContentsMargins(0, 0, 0, 0);

    searchBar = new QLineEdit(mainWidget);
    searchBar->setPlaceholderText("Search for applications...");
    searchBar->setFixedWidth(400);
    searchBar->setStyleSheet("QLineEdit { background-color: #00568f; color: gold; border: 2px solid gray; border-radius: 10px; padding: 5px; font-size: 18px; }");
    searchLayout->addWidget(searchBar);

    searchButton = new QPushButton(mainWidget);
    setSearchButtonIcon();
    searchButton->setIconSize(QSize(32, 32));
    searchButton->setFixedSize(40, 40);
    searchButton->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    searchButton->setToolTip("Open Browser Tab");
    connect(searchButton, &QPushButton::clicked, this, &AppLauncher::openBrowserTab);
    searchLayout->addWidget(searchButton);

    searchButton->raise();
    mainWidgetLayout->addLayout(searchLayout);

    // Bottom icons layout
    QHBoxLayout *bottomIconsLayout = new QHBoxLayout();
    bottomIconsLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    bottomIconsLayout->setSpacing(0);
    bottomIconsLayout->setContentsMargins(0, 0, 0, 0);

    addIconButton(bottomIconsLayout, "icons/files.png", "File Applications", SLOT(handleFilesClick()));
    addIconButton(bottomIconsLayout, "icons/webapps.png", "Web Applications", SLOT(handleWebAppsClick()));
    addIconButton(bottomIconsLayout, "icons/musicapps.png", "Music Applications", SLOT(handleMusicAppsClick()));
    addIconButton(bottomIconsLayout, "icons/gamingapps.png", "Gaming Applications", SLOT(handleGamingAppsClick()));
    addIconButton(bottomIconsLayout, "icons/photo editing.png", "Photo Editing Applications", SLOT(handlePhotoEditingClick()));
    addIconButton(bottomIconsLayout, "icons/settings.png", "Multi Purpose Applications", SLOT(handleMultiPurposeAppsClick()));
    addIconButton(bottomIconsLayout, "icons/information.png", "System Information", SLOT(handleInformationClick()));

    mainWidgetLayout->addLayout(bottomIconsLayout);

    // Menu label
    menuLabel = new QLabel(mainWidget);
    menuLabel->setStyleSheet("QLabel { color: gold; font-size: 20px; }");
    menuLabel->setAlignment(Qt::AlignCenter);
    mainWidgetLayout->addWidget(menuLabel, 0, Qt::AlignTop | Qt::AlignHCenter);

    // Application grid container with scroll area
    QScrollArea *scrollArea = new QScrollArea(mainWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");

    appGridContainer = new QWidget(scrollArea);
    appGridLayout = new QGridLayout(appGridContainer);
    appGridLayout->setAlignment(Qt::AlignCenter);
    appGridLayout->setSpacing(10);
    appGridContainer->setStyleSheet("QWidget { background-color: transparent; }");
    appGridContainer->setVisible(false);

    scrollArea->setWidget(appGridContainer);
    mainWidgetLayout->addWidget(scrollArea);

    // Hover box for tooltips
    hoverBox = new QLabel(mainWidget);
    hoverBox->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 150); color: white; border-radius: 5px; padding: 5px; font-size: 18px; }");
    hoverBox->setAlignment(Qt::AlignCenter);
    hoverBox->setVisible(false);

    // Media player for click sounds
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile("sounds/click.mp3"));

    // Predefined menu structure with commands and icons
    menuMap["Files Menu"] = {
        {"Dolphin", {"dolphin", "/usr/share/icons/hicolor/scalable/apps/org.kde.dolphin.svg"}}
    };

    menuMap["Web Apps Menu"] = {
        {"Firefox", {"/usr/lib/firefox/firefox", "/usr/share/icons/hicolor/scalable/apps/firefox.svg"}},
        {"ApexBrowser", {"/usr/bin/apextools/bauh/appimage/installed/apexbrowser/ApexBrowser-Arch-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/apexbrowser/logo.png"}},
        {"Facebook", {"/usr/bin/apextools/bauh/appimage/installed/facebook/Facebook-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/facebook/logo.png"}},
        {"Facebook Messenger", {"/usr/bin/apextools/bauh/appimage/installed/fb/Fb-Messenger-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/fb/logo.png"}},
        {"WhatsApp", {"/usr/bin/apextools/bauh/appimage/installed/whatsapp/WhatsApp-Web-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/whatsapp/logo.png"}},
        {"Telegram", {"/usr/bin/apextools/bauh/appimage/installed/telegram/Telegram-Desktop-x86-64-20240902125404.AppImage", "/usr/bin/apextools/bauh/appimage/installed/telegram/logo.png"}},
        {"Discord", {"/usr/bin/apextools/bauh/appimage/installed/discord/Discord-Arch-Debian-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/discord/logo.png"}},
        {"KDE Pling Store", {"/usr/bin/apextools/bauh/appimage/installed/kde/Kde-Store-Viewer-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/kde/logo.svg"}},
        {"YouTube And Downloader", {"/usr/bin/apextools/bauh/appimage/installed/youtube/youtube-appimage-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/youtube/logo.png"}},
        {"DeepSeek", {"/usr/bin/apextools/bauh/appimage/installed/deepseek/DeepSeek-Arch-qt6-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/deepseek/logo.png"}},
        {"QwenAI", {"/usr/bin/apextools/bauh/appimage/installed/qwenai/QwenAi-Arch-Gtk-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/qwenai/logo.png"}},
        {"Gemini", {"/usr/bin/apextools/bauh/appimage/installed/gemini/Gemini-Arch-Qt6-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/gemini/logo.png"}},
        {"ChatGPT", {"/usr/bin/apextools/bauh/appimage/installed/chatgpt/ChatGpt-Arch-Qt6-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/chatgpt/logo.png"}},
        {"qBittorrent", {"/usr/bin/apextools/bauh/appimage/installed/qbittorrent/qbittorrent-5.0.3_lt20_x86_64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/qbittorrent/logo.png"}}
    };

    menuMap["Music Applications"] = {
        {"VLC", {"/usr/bin/vlc", "/usr/share/icons/breeze/apps/48/vlc.svg"}}
    };

    menuMap["Gaming Applications"] = {
        {"Steam", {"/usr/bin/steam-runtime", "/usr/share/icons/breeze/apps/48/steam.svg"}},
        {"Bottles", {"flatpak run com.usebottles.bottles", "/opt/claudemods-ApexTools/ApexGamester/icons/com.usebottles.bottles.svg"}},
        {"PPSSPP", {"PPSSPPQt", "/usr/share/icons/hicolor/96x96/apps/ppsspp.png"}},
        {"RetroArch", {"retroarch", "com.libretro.RetroArch"}},
        {"RPCS3", {"rpcs3", "/usr/share/icons/hicolor/48x48/apps/rpcs3.png"}},
        {"Waydroid", {"waydroid first-launch", "/usr/share/icons/hicolor/512x512/apps/waydroid.png"}}
    };

    menuMap["Photo Editing Applications"] = {
        {"GIMP", {"gimp-2.10", "/usr/share/icons/breeze/apps/48/gimp.svg"}},
        {"Inkscape", {"inkscape", "/usr/share/icons/breeze-dark/apps/48/inkscape.svg"}},
        {"Gwenview", {"gwenview", "/usr/share/icons/breeze/apps/48/gwenview.svg"}}
    };

    menuMap["Multi Purpose Applications"] = {
        {"Kate", {"kate", "/usr/share/icons/hicolor/scalable/apps/kate.svg"}},
        {"Virt-Manager", {"virt-manager", "/usr/share/icons/breeze/apps/48/virt-manager.svg"}},
        {"gnome-boxes", {"gnome-boxes", "/usr/share/icons/hicolor/scalable/apps/org.gnome.Boxes.svg"}},
        {"Piper", {"piper", "/usr/share/icons/hicolor/scalable/apps/org.freedesktop.Piper.svg"}},
        {"OBS Studio", {"obs", "/usr/share/icons/hicolor/512x512/apps/com.obsproject.Studio.png"}},
        {"Qalculate", {"/usr/bin/apextools/bauh/appimage/installed/qalculate/qalculate-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/qalculate/logo.png"}},
        {"Bauh", {"/usr/bin/apextools/bauh/appimage/installed/bauh/bauh-0.10.7-x86_64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/bauh/logo.svg"}}
    };

    menuMap["System Information"] = {
        {"DNS Manager", {"/usr/bin/apextools/bauh/appimage/installed/dns/", "/usr/bin/apextools/bauh/appimage/installed/dns/logo.png"}},
        {"Stacer", {"/usr/bin/apextools/bauh/appimage/installed/stacer/Stacer-1.1.0-x64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/stacer/logo.png"}},
        {"GParted", {"sudo --preserve-env gparted", "/usr/share/icons/hicolor/scalable/apps/gparted.svg"}},
        {"Arch Mirror Changer", {"/usr/bin/apextools/bauh/appimage/installed/archmirrorchangergui/ArchMirrorChangerGui-x86-64.AppImage", "/usr/bin/apextools/bauh/appimage/installed/archmirrorchangergui/logo.png"}},
        {"System Info Centre", {"/opt/claudemods-ApexTools/ApexGamester/SystemInfo.bin", "/usr/bin/apextools/bauh/appimage/installed/apexbrowser/logo.png"}}
    };

    // Middle button layout
    QHBoxLayout *middleButtonLayout = new QHBoxLayout();
    middleButtonLayout->setAlignment(Qt::AlignCenter);
    middleButtonLayout->setSpacing(10);

    playPauseButton = new QPushButton(mainWidget);
    playPauseButton->setIcon(QIcon("icons/play.png"));
    playPauseButton->setIconSize(QSize(32, 32));
    playPauseButton->setFixedSize(40, 40);
    playPauseButton->setStyleSheet("QPushButton { background-color: transparent; border: 2px solid gold; border-radius: 10px; padding: 5px; } QPushButton:hover { background-color: rgba(255, 215, 0, 50); border: 2px solid gold; }");
    playPauseButton->setToolTip("Play/Pause Music");
    connect(playPauseButton, &QPushButton::clicked, this, &AppLauncher::handlePlayPauseClick);
    middleButtonLayout->addWidget(playPauseButton);

    pickMusicButton = new QPushButton(mainWidget);
    pickMusicButton->setIcon(QIcon("icons/pickmusic.png"));
    pickMusicButton->setIconSize(QSize(32, 32));
    pickMusicButton->setFixedSize(40, 40);
    pickMusicButton->setStyleSheet("QPushButton { background-color: transparent; border: 2px solid gold; border-radius: 10px; padding: 5px; } QPushButton:hover { background-color: rgba(255, 215, 0, 50); border: 2px solid gold; }");
    pickMusicButton->setToolTip("Pick Music");
    connect(pickMusicButton, &QPushButton::clicked, this, &AppLauncher::handlePickMusicClick);
    middleButtonLayout->addWidget(pickMusicButton);

    chooseBackgroundButton = new QPushButton(mainWidget);
    chooseBackgroundButton->setIcon(QIcon("icons/choose.png"));
    chooseBackgroundButton->setIconSize(QSize(32, 32));
    chooseBackgroundButton->setFixedSize(40, 40);
    chooseBackgroundButton->setStyleSheet("QPushButton { background-color: transparent; border: 2px solid gold; border-radius: 10px; padding: 5px; } QPushButton:hover { background-color: rgba(255, 215, 0, 50); border: 2px solid gold; }");
    chooseBackgroundButton->setToolTip("Choose Background");
    connect(chooseBackgroundButton, &QPushButton::clicked, this, &AppLauncher::handleChooseBackgroundClick);
    middleButtonLayout->addWidget(chooseBackgroundButton);

    mainWidgetLayout->addLayout(middleButtonLayout);

    // Dropdown layout for music and background
    QVBoxLayout *dropdownLayout = new QVBoxLayout();
    dropdownLayout->setAlignment(Qt::AlignCenter);
    dropdownLayout->setSpacing(5);

    musicDropdown = new QComboBox(mainWidget);
    backgroundDropdown = new QComboBox(mainWidget);
    dropdownLayout->addWidget(musicDropdown);
    dropdownLayout->addWidget(backgroundDropdown);
    musicDropdown->setVisible(false);
    backgroundDropdown->setVisible(false);

    mainWidgetLayout->addLayout(dropdownLayout);

    // Version label
    versionLabel = new QLabel("Apex Gamester v1.0 build 11-02-2025 ", mainWidget);
    versionLabel->setStyleSheet("QLabel { color: gold; font-size: 20px; text-decoration: underline; font-weight: bold; } QLabel:hover { color: white; text-decoration: none; }");
    versionLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    versionLabel->setCursor(Qt::PointingHandCursor);
    versionLabel->installEventFilter(this);

    // Hover text label
    hoverTextLabel = new QLabel(mainWidget);
    hoverTextLabel->setStyleSheet("QLabel { color: gold; background-color: rgba(0, 86, 143, 200); border-radius: 10px; padding: 10px; font-size: 18px; border: 1px solid gold; }");
    hoverTextLabel->setAlignment(Qt::AlignCenter);
    hoverTextLabel->setVisible(false);

    // Bottom right label layout
    QHBoxLayout *bottomRightLabelLayout = new QHBoxLayout();
    bottomRightLabelLayout->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    bottomRightLabelLayout->addWidget(versionLabel);
    bottomRightLabelLayout->addWidget(hoverTextLabel);
    mainWidgetLayout->addLayout(bottomRightLabelLayout);

    // Music player
    musicPlayer = new QMediaPlayer(this);
    musicAudioOutput = new QAudioOutput(this);
    musicPlayer->setAudioOutput(musicAudioOutput);

    // System info timer
    QTimer *systemInfoTimer = new QTimer(this);
    connect(systemInfoTimer, &QTimer::timeout, this, &AppLauncher::updateSystemInfo);
    systemInfoTimer->start(1000);

    updateSystemInfo();

    // System menu
    systemMenu = new QMenu(this);
    QAction *signOutAction = systemMenu->addAction(QIcon("icons/signout.png"), "Sign Out");
    QAction *rebootAction = systemMenu->addAction(QIcon("icons/reboot.png"), "Reboot");
    QAction *shutdownAction = systemMenu->addAction(QIcon("icons/shutdown.png"), "Shutdown");

    connect(signOutAction, &QAction::triggered, this, &AppLauncher::handleSignOut);
    connect(rebootAction, &QAction::triggered, this, &AppLauncher::handleReboot);
    connect(shutdownAction, &QAction::triggered, this, &AppLauncher::handleShutdown);

    connect(systemMenuButton, &QPushButton::clicked, [this]() {
        systemMenu->exec(systemMenuButton->mapToGlobal(QPoint(0, systemMenuButton->height())));
    });

    // Search bar event filter
    connect(searchBar, &QLineEdit::textChanged, this, &AppLauncher::searchApplications);
    searchBar->installEventFilter(this);

    // Date and time timer
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &AppLauncher::updateDateTime);
    timer->start(1000);

    updateDateTime();

    // Browser tabs
    browserTabs = new QTabWidget(this);
    browserTabs->setVisible(false);
    mainLayout->addWidget(browserTabs);

    // Add main widget to the main layout
    mainLayout->addWidget(mainWidget);

    // Volume slider
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setSingleStep(10);
    volumeSlider->setPageStep(10);
    volumeSlider->setStyleSheet("QSlider::groove:horizontal { background: gold; height: 10px; border-radius: 5px; } QSlider::handle:horizontal { background: teal; width: 20px; height: 20px; margin: -5px 0; border-radius: 10px; }");
    volumeSlider->setVisible(false);
    connect(volumeSlider, &QSlider::valueChanged, this, &AppLauncher::onVolumeSliderValueChanged);
}

void AppLauncher::openBrowserTab() {
    mainWidget->setVisible(false);

    while (browserTabs->count() > 0) {
        QWidget *tab = browserTabs->widget(0);
        browserTabs->removeTab(0);
        delete tab;
    }

    QWidget *tabWidget = new QWidget(this);
    QVBoxLayout *tabLayout = new QVBoxLayout(tabWidget);

    QWebEngineView *webView = new QWebEngineView(tabWidget);
    webView->setUrl(QUrl("https://www.google.com"));

    QHBoxLayout *navLayout = new QHBoxLayout();
    QPushButton *backButton = new QPushButton(QIcon("icons/back.png"), "", tabWidget);
    QPushButton *forwardButton = new QPushButton(QIcon("icons/forward.png"), "", tabWidget);
    QPushButton *refreshButton = new QPushButton(QIcon("icons/refresh.png"), "", tabWidget);
    QPushButton *saveButton = new QPushButton(QIcon("icons/save.png"), "", tabWidget);
    QPushButton *bookmarkButton = new QPushButton(QIcon("icons/bookmark.png"), "", tabWidget);
    QPushButton *hideButton = new QPushButton(QIcon("icons/hide.png"), "", tabWidget);
    QLineEdit *urlBar = new QLineEdit(tabWidget);

    navLayout->addWidget(backButton);
    navLayout->addWidget(forwardButton);
    navLayout->addWidget(refreshButton);
    navLayout->addWidget(urlBar);
    navLayout->addWidget(saveButton);
    navLayout->addWidget(bookmarkButton);
    navLayout->addWidget(hideButton);

    tabLayout->addLayout(navLayout);
    tabLayout->addWidget(webView);
    tabWidget->setLayout(tabLayout);

    browserTabs->addTab(tabWidget, "New Tab");
    browserTabs->setVisible(true);

    connect(backButton, &QPushButton::clicked, webView, &QWebEngineView::back);
    connect(forwardButton, &QPushButton::clicked, webView, &QWebEngineView::forward);
    connect(refreshButton, &QPushButton::clicked, webView, &QWebEngineView::reload);

    connect(urlBar, &QLineEdit::returnPressed, [webView, urlBar]() {
        webView->setUrl(QUrl(urlBar->text()));
    });

    connect(webView, &QWebEngineView::urlChanged, [urlBar](const QUrl &url) {
        urlBar->setText(url.toString());
    });

    connect(saveButton, &QPushButton::clicked, [urlBar]() {
        QFile file("bookmark.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << urlBar->text() << "\n";
            file.close();
        }
    });

    connect(bookmarkButton, &QPushButton::clicked, [urlBar]() {
        QFile file("bookmark.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QStringList bookmarks;
            while (!in.atEnd()) {
                bookmarks << in.readLine();
            }
            file.close();
            QMenu menu;
            for (const QString &bookmark : bookmarks) {
                menu.addAction(bookmark);
            }
            menu.exec(QCursor::pos());
        }
    });

    connect(hideButton, &QPushButton::clicked, this, &AppLauncher::closeBrowserTab);
}

void AppLauncher::closeBrowserTab() {
    browserTabs->setVisible(false);
    mainWidget->setVisible(true);
}

bool AppLauncher::eventFilter(QObject *obj, QEvent *event) {
    if (obj == searchBar && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            executeBashCommand(searchBar->text());
            return true;
        }
    }

    if (event->type() == QEvent::Enter) {
        QPushButton *button = qobject_cast<QPushButton*>(obj);
        if (button) {
            hoverBox->setText(button->toolTip());
            hoverBox->move(button->mapToGlobal(QPoint(0, -30)).x(), button->mapToGlobal(QPoint(0, -30)).y());
            hoverBox->adjustSize();
            hoverBox->setVisible(true);

            button->setStyleSheet("QPushButton { background-color: rgba(255, 255, 255, 50); border: 2px solid gold; border-radius: 10px; }");
        } else if (obj == versionLabel) {
            hoverTextLabel->setText("Support the creator Aaron Dsouza");
            hoverTextLabel->move(versionLabel->mapToParent(QPoint(0, -hoverTextLabel->height() - 10)));
            hoverTextLabel->adjustSize();
            hoverTextLabel->setVisible(true);

            QTimer::singleShot(3000, [this]() {
                hoverTextLabel->setVisible(false);
            });
        }
    } else if (event->type() == QEvent::Leave) {
        hoverBox->setVisible(false);

        QPushButton *button = qobject_cast<QPushButton*>(obj);
        if (button) {
            button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
        }

        if (obj == versionLabel) {
            hoverTextLabel->setVisible(false);
        }
    }

    if (obj == versionLabel && event->type() == QEvent::MouseButtonPress) {
        openSupportLink();
        return true;
    }

    return QWidget::eventFilter(obj, event);
}

void AppLauncher::resizeEvent(QResizeEvent *event) {
    setBackgroundImage(readImagePathFromFile("background.txt"));
    QWidget::resizeEvent(event);
}

void AppLauncher::closeEvent(QCloseEvent *event) {
    // Terminate all active processes
    for (QProcess *process : activeProcesses) {
        process->terminate();
        process->waitForFinished();
        process->deleteLater();
    }
    activeProcesses.clear();

    // Accept the close event
    event->accept();
}

void AppLauncher::handleFilesClick() {
    playClickSound();
    clearMenuButtonHighlights();
    activeMenuButton = qobject_cast<QPushButton*>(sender());
    highlightMenuButton(activeMenuButton);
    menuLabel->setText("File Applications");
    populateMenu("Files Menu");
}

void AppLauncher::handleWebAppsClick() {
    playClickSound();
    clearMenuButtonHighlights();
    activeMenuButton = qobject_cast<QPushButton*>(sender());
    highlightMenuButton(activeMenuButton);
    menuLabel->setText("Web Applications");
    populateMenu("Web Apps Menu");
}

void AppLauncher::handleMusicAppsClick() {
    playClickSound();
    clearMenuButtonHighlights();
    activeMenuButton = qobject_cast<QPushButton*>(sender());
    highlightMenuButton(activeMenuButton);
    menuLabel->setText("Music Applications");
    populateMenu("Music Applications");
}

void AppLauncher::handleGamingAppsClick() {
    playClickSound();
    clearMenuButtonHighlights();
    activeMenuButton = qobject_cast<QPushButton*>(sender());
    highlightMenuButton(activeMenuButton);
    menuLabel->setText("Gaming Applications");
    populateMenu("Gaming Applications");
}

void AppLauncher::handlePhotoEditingClick() {
    playClickSound();
    clearMenuButtonHighlights();
    activeMenuButton = qobject_cast<QPushButton*>(sender());
    highlightMenuButton(activeMenuButton);
    menuLabel->setText("Photo Editing Applications");
    populateMenu("Photo Editing Applications");
}

void AppLauncher::handleMultiPurposeAppsClick() {
    playClickSound();
    clearMenuButtonHighlights();
    activeMenuButton = qobject_cast<QPushButton*>(sender());
    highlightMenuButton(activeMenuButton);
    menuLabel->setText("Multi Purpose Applications");
    populateMenu("Multi Purpose Applications");
}

void AppLauncher::handleInformationClick() {
    playClickSound();
    clearMenuButtonHighlights();
    activeMenuButton = qobject_cast<QPushButton*>(sender());
    highlightMenuButton(activeMenuButton);
    menuLabel->setText("System Information");
    populateMenu("System Information");
}

void AppLauncher::updateDateTime() {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeString = currentDateTime.toString("dd/MM/yyyy hh:mm:ss");
    dateTimeLabel->setText(dateTimeString);
}

void AppLauncher::openSupportLink() {
    // Open the support link in the browser widget
    openBrowserTab(); // Ensure the browser widget is visible
    QWebEngineView *webView = qobject_cast<QWebEngineView*>(browserTabs->currentWidget()->layout()->itemAt(1)->widget());
    if (webView) {
        webView->setUrl(QUrl("https://www.paypal.com/paypalme/claudemods?country.x=GB&locale"));
    }
}

QString AppLauncher::readImagePathFromFile(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString path = in.readLine().trimmed();
        file.close();
        return path;
    } else {
        qDebug() << "Failed to open file:" << filePath;
        return QString();
    }
}

void AppLauncher::setBackgroundImage(const QString &imagePath) {
    QPixmap bgImage(imagePath);
    if (!bgImage.isNull()) {
        background->setPixmap(bgImage.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        background->setGeometry(0, 0, width(), height());
    } else {
        qDebug() << "Failed to load background image from path:" << imagePath;
    }
}

void AppLauncher::setSearchButtonIcon() {
    QString iconPath = "icons/search.png";
    if (QFile::exists(iconPath)) {
        QIcon icon(iconPath);
        searchButton->setIcon(icon);
    } else {
        qDebug() << "Icon not found at:" << iconPath;
    }
}

void AppLauncher::addIconButton(QHBoxLayout *layout, const QString &iconPath, const QString &tooltip, const char *slot) {
    QPushButton *button = new QPushButton(this);
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(64, 64));
    button->setFixedSize(80, 80); // Fixed: Removed extra ')'
    button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
    button->setToolTip(tooltip);
    connect(button, SIGNAL(clicked()), this, slot);
    button->installEventFilter(this);
    layout->addWidget(button);
}

void AppLauncher::playClickSound() {
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mediaPlayer->stop();
    }
    mediaPlayer->play();
}

QString AppLauncher::resolveIconPath(const QString &iconName) {
    // Check if the icon path is already a valid file
    if (QFile::exists(iconName)) {
        return iconName;
    }

    // Fallback to system-wide icon directories
    QStringList iconPaths = {
        "/usr/share/icons",
        "/usr/share/pixmaps",
        "/usr/local/share/icons",
        QStandardPaths::locate(QStandardPaths::GenericDataLocation, "icons", QStandardPaths::LocateDirectory)
    };

    for (const QString &path : iconPaths) {
        QDir iconDir(path);
        QStringList iconFiles = iconDir.entryList(QStringList() << iconName + ".*", QDir::Files);
        if (!iconFiles.isEmpty()) {
            return iconDir.filePath(iconFiles.first());
        }
    }

    // If no icon is found, return a default icon or an empty string
    return ":/icons/default.png"; // Replace with a default icon path if available
}

void AppLauncher::populateMenu(const QString &menuName) {
    // Clear the grid layout
    QLayoutItem *item;
    while ((item = appGridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QMap<QString, QPair<QString, QString>> apps = menuMap[menuName];
    int row = 0, col = 0;
    for (const QString &appName : apps.keys()) {
        QString exec = apps[appName].first;
        QString iconPath = resolveIconPath(apps[appName].second); // Resolve the icon path

        // Create a container widget for the icon and label
        QWidget *appWidget = new QWidget(appGridContainer);
        QVBoxLayout *appLayout = new QVBoxLayout(appWidget);
        appLayout->setAlignment(Qt::AlignCenter);
        appLayout->setSpacing(5);

        // Create the icon button
        QPushButton *appButton = new QPushButton(appWidget);
        appButton->setIcon(QIcon(iconPath));
        appButton->setIconSize(QSize(64, 64));
        appButton->setFixedSize(80, 80); // Fixed: Removed extra ')'
        appButton->setStyleSheet("QPushButton { background-color: transparent; border: none; } QPushButton:hover { background-color: rgba(255, 255, 255, 50); border: 2px solid gold; border-radius: 10px; }");
        connect(appButton, &QPushButton::clicked, [this, exec]() { launchApplication(exec); });

        // Create the application name label
        QLabel *appLabel = new QLabel(appName, appWidget);
        appLabel->setAlignment(Qt::AlignCenter);
        appLabel->setStyleSheet("QLabel { color: gold; font-size: 16px; }");

        // Add the icon and label to the layout
        appLayout->addWidget(appButton);
        appLayout->addWidget(appLabel);

        // Add the container widget to the grid
        appGridLayout->addWidget(appWidget, row, col);
        col++;
        if (col >= 5) { // Adjust the number of columns as needed
            col = 0;
            row++;
        }
    }

    // Ensure the app grid is visible and doesn't overlap with other elements
    appGridContainer->setVisible(true);
    appGridContainer->raise(); // Bring the grid to the front
}

void AppLauncher::addApplication(const QString &name, const QString &exec, const QString &icon) {
    QWidget *appWidget = new QWidget(appGridContainer);
    QVBoxLayout *appLayout = new QVBoxLayout(appWidget);
    appLayout->setAlignment(Qt::AlignCenter);
    appLayout->setSpacing(5);

    QPushButton *appButton = new QPushButton(appWidget);
    appButton->setIcon(QIcon(icon));
    appButton->setIconSize(QSize(64, 64));
    appButton->setFixedSize(80, 80); // Fixed: Removed extra ')'
    appButton->setStyleSheet("QPushButton { background-color: transparent; border: none; } QPushButton:hover { background-color: rgba(255, 255, 255, 50); border: 2px solid gold; border-radius: 10px; }");
    connect(appButton, &QPushButton::clicked, [this, exec]() { launchApplication(exec); });

    QLabel *appLabel = new QLabel(name, appWidget);
    appLabel->setAlignment(Qt::AlignCenter);
    appLabel->setStyleSheet("QLabel { color: gold; font-size: 16px; }");

    appLayout->addWidget(appButton);
    appLayout->addWidget(appLabel);

    int row = appGridLayout->rowCount();
    int col = appGridLayout->columnCount();
    if (col >= 5) {
        col = 0;
        row++;
    }
    appGridLayout->addWidget(appWidget, row, col);
}

void AppLauncher::launchApplication(const QString &exec) {
    playSound("sounds/choice.mp3"); // Play choice sound when selecting an application

    QProcess *process = new QProcess(this);
    process->start("bash", QStringList() << "-c" << "hyprctl dispatch workspace 3 && " + exec);

    // Connect the finished signal to handle application closure
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this, process](int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/) {
                // Execute hyprctl dispatch workspace 2 after the app closes
                QProcess::startDetached("bash", QStringList() << "-c" << "hyprctl dispatch workspace 2");

                // Clean up the process
                activeProcesses.removeOne(process);
                process->deleteLater();
            });

    // Add the process to the active processes list
    activeProcesses.append(process);
}

void AppLauncher::executeBashCommand(const QString &command) {
    QProcess::startDetached("bash", QStringList() << "-c" << command);
}

void AppLauncher::searchApplications(const QString &searchText) {
    // Clear the grid layout
    QLayoutItem *item;
    while ((item = appGridLayout->takeAt(0)) != nullptr) {
        delete item->widget(); // Delete the widget
        delete item; // Delete the layout item
    }

    // Hide the grid if the search bar is empty
    if (searchText.isEmpty()) {
        appGridContainer->setVisible(false);
        return;
    }

    // Search predefined menus
    for (const QString &menuName : menuMap.keys()) {
        QMap<QString, QPair<QString, QString>> apps = menuMap[menuName];
        for (const QString &appName : apps.keys()) {
            if (appName.contains(searchText, Qt::CaseInsensitive)) {
                QString exec = apps[appName].first;
                QString icon = resolveIconPath(apps[appName].second);
                addApplication(appName, exec, icon);
            }
        }
    }

    // Search system applications in /usr/share/applications
    searchSystemApplications(searchText);

    // Ensure the app grid is visible
    appGridContainer->setVisible(true);
    appGridContainer->raise(); // Bring the grid to the front
}

void AppLauncher::searchSystemApplications(const QString &searchText) {
    QDir applicationsDir("/usr/share/applications");
    QStringList desktopFiles = applicationsDir.entryList(QStringList() << "*.desktop", QDir::Files);

    for (const QString &desktopFile : desktopFiles) {
        QFile file(applicationsDir.filePath(desktopFile));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString name, exec, icon;
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.startsWith("Name=")) {
                    name = line.mid(5);
                } else if (line.startsWith("Exec=")) {
                    exec = line.mid(5);
                } else if (line.startsWith("Icon=")) {
                    icon = line.mid(5);
                }
            }
            file.close();

            if (name.contains(searchText, Qt::CaseInsensitive)) {
                addApplication(name, exec, resolveIconPath(icon));
            }
        }
    }
}

void AppLauncher::updateSystemInfo() {
    QProcess cpuProcess;
    cpuProcess.start("sh", QStringList() << "-c" << "top -bn1 | grep 'Cpu(s)' | sed 's/.*, *\\([0-9.]*\\)%* id.*/\\1/' | awk '{print 100 - $1}'");
    cpuProcess.waitForFinished();
    QString cpuUsage = cpuProcess.readAllStandardOutput().trimmed() + "% CPU";

    QProcess ramProcess;
    ramProcess.start("sh", QStringList() << "-c" << "free -m | awk '/Mem:/ {print $3 \" MB / \" $2 \" MB\"}'");
    ramProcess.waitForFinished();
    QString ramUsage = ramProcess.readAllStandardOutput().trimmed();

    QProcess driveProcess;
    driveProcess.start("sh", QStringList() << "-c" << "df -h / | awk '/\\// {print $3 \" / \" $2}'");
    driveProcess.waitForFinished();
    QString driveUsage = driveProcess.readAllStandardOutput().trimmed();

    systemInfoLabel->setText("CPU: " + cpuUsage + " | RAM: " + ramUsage + " | Drive: " + driveUsage);
}

void AppLauncher::handleSignOut() {
    bool ok;
    QString password = QInputDialog::getText(this, "Sign Out", "Enter sudo password:", QLineEdit::Password, "", &ok);
    if (ok && !password.isEmpty()) {
        playSound("sounds/shutdown.mp3"); // Play shutdown sound
        showNotification("Signing out...");

        QProcess process;
        process.start("bash", QStringList() << "-c" << "echo " + password + " | sudo -S gnome-session-quit --no-prompt");
        process.waitForFinished();
    }
}

void AppLauncher::handleReboot() {
    bool ok;
    QString password = QInputDialog::getText(this, "Reboot", "Enter sudo password:", QLineEdit::Password, "", &ok);
    if (ok && !password.isEmpty()) {
        playSound("sounds/shutdown.mp3"); // Play shutdown sound
        showNotification("Rebooting...");

        QProcess process;
        process.start("bash", QStringList() << "-c" << "echo " + password + " | sudo -S reboot");
        process.waitForFinished();
    }
}

void AppLauncher::handleShutdown() {
    bool ok;
    QString password = QInputDialog::getText(this, "Shutdown", "Enter sudo password:", QLineEdit::Password, "", &ok);
    if (ok && !password.isEmpty()) {
        playSound("sounds/shutdown.mp3"); // Play shutdown sound
        showNotification("Shutting down...");

        QProcess process;
        process.start("bash", QStringList() << "-c" << "echo " + password + " | sudo -S shutdown now");
        process.waitForFinished();
    }
}

void AppLauncher::handleUpdateSystem() {
    QString hyprlandCommand = QString("hyprctl dispatch workspace 3 && konsole -e sudo pacman -Sy && sudo pacman -Syu");
    QProcess *process = new QProcess(this);
    process->start("bash", QStringList() << "-c" << hyprlandCommand);

    // Connect the finished signal to handle application closure
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this, process](int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/) {
                // Execute hyprctl dispatch workspace 2 after the update process finishes
                QProcess::startDetached("bash", QStringList() << "-c" << "hyprctl dispatch workspace 2");

                // Clean up the process
                activeProcesses.removeOne(process);
                process->deleteLater();
            });

    // Add the process to the active processes list
    activeProcesses.append(process);
}

void AppLauncher::handleChangeVolume() {
    volumeSlider->setVisible(!volumeSlider->isVisible());
}

void AppLauncher::onVolumeSliderValueChanged(int value) {
    // Snap to multiples of 10
    int snappedValue = (value + 5) / 10 * 10;
    volumeSlider->setValue(snappedValue);

    // Update the volume percentage label
    volumePercentageLabel->setText(QString::number(snappedValue) + "%");

    // Execute the volume change command
    executeBashCommand("pamixer --set-volume " + QString::number(snappedValue));
}

void AppLauncher::handleOpenTerminal() {
    QString hyprlandCommand = QString("hyprctl dispatch workspace 3 && konsole");
    QProcess::startDetached("bash", QStringList() << "-c" << hyprlandCommand);
}

void AppLauncher::handlePickMusicClick() {
    musicDropdown->setVisible(!musicDropdown->isVisible());

    if (musicDropdown->isVisible()) {
        loadMusicFiles();
    }
}

void AppLauncher::loadMusicFiles() {
    musicDropdown->clear();
    QDir musicDir("/opt/claudemods-ApexTools/ApexGamester/media");
    QStringList mp3Files = musicDir.entryList(QStringList() << "*.mp3", QDir::Files);
    for (const QString &file : mp3Files) {
        musicDropdown->addItem(file);
    }
    if (mp3Files.isEmpty()) {
        QMessageBox::information(this, "No Music", "No .mp3 files found in the sounds directory.");
    }
}

void AppLauncher::handleChooseBackgroundClick() {
    backgroundDropdown->setVisible(!backgroundDropdown->isVisible());

    if (backgroundDropdown->isVisible()) {
        loadBackgroundImages();
    }
}

void AppLauncher::loadBackgroundImages() {
    backgroundDropdown->clear();
    QDir backgroundDir("/opt/claudemods-ApexTools/ApexGamester/backgrounds");
    QStringList imageFiles = backgroundDir.entryList(QStringList() << "*.png" << "*.jpg" << "*.jpeg", QDir::Files);
    for (const QString &file : imageFiles) {
        backgroundDropdown->addItem(file);
    }
    if (imageFiles.isEmpty()) {
        QMessageBox::information(this, "No Backgrounds", "No image files found in the backgrounds directory.");
    } else {
        connect(backgroundDropdown, QOverload<int>::of(&QComboBox::activated), this, [this](int index) {
            QString selectedImage = backgroundDropdown->itemText(index);
            QString imagePath = "/opt/claudemods-ApexTools/ApexGamester/backgrounds/" + selectedImage;

            QFile file("background.txt");
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << imagePath;
                file.close();
            }

            setBackgroundImage(imagePath);
        });
    }
}

void AppLauncher::handlePlayPauseClick() {
    QString selectedMusic = musicDropdown->currentText();
    if (selectedMusic.isEmpty()) {
        QMessageBox::warning(this, "No Music Selected", "Please select a music file to play.");
        return;
    }
    QString musicPath = "/opt/claudemods-ApexTools/ApexGamester/media/" + selectedMusic;
    if (isPlaying) {
        musicPlayer->pause();
        playPauseButton->setIcon(QIcon("icons/play.png"));
        isPlaying = false;
    } else {
        musicPlayer->setSource(QUrl::fromLocalFile(musicPath));
        musicPlayer->play();
        playPauseButton->setIcon(QIcon("icons/pause.png"));
        isPlaying = true;
    }
}

void AppLauncher::highlightMenuButton(QPushButton *button) {
    if (button) {
        button->setStyleSheet("QPushButton { background-color: rgba(255, 215, 0, 100); border: 2px solid gold; border-radius: 10px; }");
    }
}

void AppLauncher::clearMenuButtonHighlights() {
    for (QObject *child : mainWidget->children()) {
        QPushButton *button = qobject_cast<QPushButton*>(child);
        if (button) {
            button->setStyleSheet("QPushButton { background-color: transparent; border: none; }");
        }
    }
}

void AppLauncher::showNotification(const QString &message) {
    QMessageBox msgBox;
    msgBox.setStyleSheet("QMessageBox { background-color: gold; color: teal; font-size: 18px; }");
    msgBox.setText(message);
    msgBox.exec();
}

void AppLauncher::playSound(const QString &soundFile) {
    QMediaPlayer *soundPlayer = new QMediaPlayer(this);
    QAudioOutput *soundOutput = new QAudioOutput(this);
    soundPlayer->setAudioOutput(soundOutput);
    soundPlayer->setSource(QUrl::fromLocalFile(soundFile));
    soundPlayer->play();
}

void AppLauncher::handleScreenshotClick() {
    QMessageBox::information(this, "Screenshot", "Press OK and click a window to take a screenshot.");
    executeBashCommand("hyprshot -m window");
}

void AppLauncher::handleRecordClick() {
    if (isRecording) {
        // Stop recording
        executeBashCommand("pkill ffmpeg");
        isRecording = false;
        recordButton->setIcon(QIcon("icons/record.png"));
        recordButton->setToolTip("Record Screen");
        QMessageBox::information(this, "Recording Saved", "Video saved in " + QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    } else {
        // Start recording
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Record Screen", "Do you want to record your screen and mic?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            isRecording = true;
            recordButton->setIcon(QIcon("icons/pauserecord.png"));
            recordButton->setToolTip("Pause Recording");
            QString outputFile = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/recording.mp4";
            executeBashCommand("ffmpeg -f x11grab -video_size 1920x1080 -framerate 30 -i :0.0 -f pulse -i default -c:v libx264 -preset ultrafast -c:a aac " + outputFile);
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    AppLauncher launcher;
    launcher.show();
    return app.exec();
}

#include "main.moc"
