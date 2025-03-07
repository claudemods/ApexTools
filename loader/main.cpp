#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QPixmap>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>
#include <QHBoxLayout>
#include <QProcess>
#include <QPushButton>
#include <QMessageBox>
#include <QGraphicsEffect> // For glowing effect
#include <QTabWidget>
#include <QTabBar>
#include <QDebug>
#include <QFileInfo>
#include <QMediaPlayer> // For sound playback
#include <QAudioOutput> // For audio output
#include <QFile> // For file operations
#include <QTextStream> // For writing to files

// Shared BackgroundWidget class
class BackgroundWidget : public QWidget {
    Q_OBJECT

public:
    BackgroundWidget(QWidget *parent = nullptr) : QWidget(parent) {
        // Load the background image
        backgroundPixmap = QPixmap("images/paradise.jpg");
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.drawPixmap(0, 0, this->width(), this->height(), backgroundPixmap);
    }

private:
    QPixmap backgroundPixmap;
};

// GlowButton class for hover and glow effects
class GlowButton : public QPushButton {
    Q_OBJECT

public:
    GlowButton(QWidget *parent = nullptr) : QPushButton(parent) {
        // Initialize the glow effect
        glowEffect = new QGraphicsDropShadowEffect(this);
        glowEffect->setBlurRadius(20); // Glow size
        glowEffect->setColor(Qt::yellow); // Glow color
        glowEffect->setOffset(0); // Glow centered around the button
        this->setGraphicsEffect(glowEffect);
        glowEffect->setEnabled(false); // Disable glow by default
    }

protected:
    void enterEvent(QEnterEvent *event) override {
        Q_UNUSED(event);
        glowEffect->setEnabled(true); // Enable glow on hover
    }

    void leaveEvent(QEvent *event) override {
        Q_UNUSED(event);
        glowEffect->setEnabled(false); // Disable glow when not hovering
    }

private:
    QGraphicsDropShadowEffect *glowEffect;
};

// Custom ImageButton class for MainMenu
class ImageButton : public QWidget {
    Q_OBJECT

public:
    ImageButton(const QString& imagePath, const QString& labelText, QWidget *parent = nullptr)
    : QWidget(parent), imagePath(imagePath), labelText(labelText) {
        // Create layout
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setAlignment(Qt::AlignCenter);

        // Create image label
        imageLabel = new QLabel(this);
        QPixmap pixmap(imagePath);
        imageLabel->setPixmap(pixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(imageLabel);

        // Create text label
        QLabel *textLabel = new QLabel(labelText, this);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setStyleSheet("font-size: 20px; color: gold;");
        layout->addWidget(textLabel);

        // Initialize glow effect
        glowEffect = new QGraphicsDropShadowEffect(this);
        glowEffect->setBlurRadius(20); // Glow size
        glowEffect->setColor(Qt::yellow); // Glow color
        glowEffect->setOffset(0); // Glow centered around the image
        imageLabel->setGraphicsEffect(glowEffect);
        glowEffect->setEnabled(false); // Disable glow by default

        // Set initial size (larger by default)
        scaleImage(largeSize);

        // Initialize media player for hover sound
        mediaPlayer = new QMediaPlayer(this);
        audioOutput = new QAudioOutput(this);
        mediaPlayer->setAudioOutput(audioOutput);
        mediaPlayer->setSource(QUrl::fromLocalFile("/opt/claudemods-ApexTools/sounds/hover.mp3"));
        audioOutput->setVolume(50); // Set volume (0-100)
    }

signals:
    void clicked();

protected:
    void enterEvent(QEnterEvent *event) override {
        Q_UNUSED(event);
        glowEffect->setEnabled(true); // Enable glow on hover
        scaleImage(largeSizeHover); // Enlarge image further on hover
        mediaPlayer->play(); // Play hover sound
    }

    void leaveEvent(QEvent *event) override {
        Q_UNUSED(event);
        glowEffect->setEnabled(false); // Disable glow when not hovering
        scaleImage(largeSize); // Restore to default larger size
        mediaPlayer->stop(); // Stop hover sound
    }

    void mousePressEvent(QMouseEvent *event) override {
        Q_UNUSED(event);
        emit clicked(); // Emit clicked signal
    }

private:
    void scaleImage(const QSize& size) {
        QPixmap pixmap(imagePath);
        QPixmap scaledPixmap = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
    }

    QString imagePath;
    QString labelText;
    QLabel *imageLabel;
    QGraphicsDropShadowEffect *glowEffect;
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;

    // Define sizes for default and hover states
    QSize largeSize = QSize(200, 200); // Default larger size
    QSize largeSizeHover = QSize(400, 400); // Size when hovered
};

// LoadingWindow class
class LoadingWindow : public QWidget {
    Q_OBJECT

public:
    LoadingWindow(QTabWidget *tabWidget, QWidget *parent = nullptr)
    : QWidget(parent), currentImageIndex(0), tabWidget(tabWidget) {
        // Set the window to full-screen
        this->setWindowState(Qt::WindowFullScreen);

        // Create the background widget
        BackgroundWidget *backgroundWidget = new BackgroundWidget(this);
        backgroundWidget->setGeometry(0, 0, this->width(), this->height());

        // Create a layout for the overlay content
        QVBoxLayout *overlayLayout = new QVBoxLayout(this);
        overlayLayout->setAlignment(Qt::AlignCenter);

        // Add images
        imageLayout = new QHBoxLayout();

        imageLabel1 = new QLabel(this);
        imageLabel2 = new QLabel(this);
        imageLabel3 = new QLabel(this);

        // Load images
        pixmaps.append(QPixmap("images/gamester.png"));
        pixmaps.append(QPixmap("images/isocreator.png"));
        pixmaps.append(QPixmap("images/recovery.png"));

        // Scale images initially
        scaleImages();

        // Apply gold glow effect to the enlarged image
        applyGlowEffect();

        imageLabel1->setPixmap(pixmaps[0]);
        imageLabel2->setPixmap(pixmaps[1]);
        imageLabel3->setPixmap(pixmaps[2]);

        imageLayout->addWidget(imageLabel1);
        imageLayout->addWidget(imageLabel2);
        imageLayout->addWidget(imageLabel3);
        overlayLayout->addLayout(imageLayout);

        // Add loading text
        QLabel *loadingText = new QLabel("Loading Apex Tools", this);
        loadingText->setAlignment(Qt::AlignCenter);
        loadingText->setStyleSheet("font-size: 40px; color: teal;");
        overlayLayout->addWidget(loadingText);

        // Add progress bar
        progressBar = new QProgressBar(this);
        progressBar->setRange(0, 100);
        progressBar->setValue(0);

        // Customize the progress bar
        progressBar->setStyleSheet(
            "QProgressBar {"
            "   border: 2px solid grey;"
            "   border-radius: 5px;"
            "   background-color: #f0f0f0;"
            "   height: 10px;"
            "   width: 300px;"
            "}"
            "QProgressBar::chunk {"
            "   background-color: teal;"
            "}"
        );

        overlayLayout->addWidget(progressBar, 0, Qt::AlignHCenter);

        // Add the version label in the bottom-right corner
        QLabel *versionLabel = new QLabel("Apex Tools v1.0 Build 11-02-2025", this);
        versionLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);
        versionLabel->setStyleSheet(
            "font-size: 20px; color: gold; padding: 10px;" // Gold text with padding
        );

        // Create a container widget for the version label
        QWidget *versionContainer = new QWidget(this);
        QHBoxLayout *versionLayout = new QHBoxLayout(versionContainer);
        versionLayout->setContentsMargins(0, 0, 10, 10); // Add margins to avoid touching the edges
        versionLayout->addStretch(); // Add a stretch to push the label to the right
        versionLayout->addWidget(versionLabel, 0, Qt::AlignRight | Qt::AlignBottom);

        // Add the version container to the main layout
        overlayLayout->addStretch(); // Add a stretch to push everything to the top
        overlayLayout->addWidget(versionContainer);

        // Set up timer for loading bar
        loadingTimer = new QTimer(this);
        connect(loadingTimer, &QTimer::timeout, this, &LoadingWindow::updateProgress);
        loadingTimer->start(100); // 100ms interval

        // Set up timer for image animation
        QTimer *animationTimer = new QTimer(this);
        connect(animationTimer, &QTimer::timeout, this, &LoadingWindow::updateImageAnimation);
        animationTimer->start(3500); // Change image every 3.5 seconds
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        // Resize the background widget and scale images when the window is resized
        QWidget::resizeEvent(event);
        if (BackgroundWidget *backgroundWidget = findChild<BackgroundWidget *>()) {
            backgroundWidget->setGeometry(0, 0, this->width(), this->height());
        }
        scaleImages();
        applyGlowEffect(); // Reapply glow effect after resizing
    }

private slots:
    void updateProgress() {
        // Increment progress bar value
        int value = progressBar->value() + 1;
        progressBar->setValue(value);

        // If progress is complete, check for InstalledVersion.txt
        if (value >= 100) {
            loadingTimer->stop();

            // Check if InstalledVersion.txt exists
            if (checkInstalledVersion()) {
                // File exists, switch to MainMenu
                tabWidget->setCurrentIndex(1);
            } else {
                // File does not exist, display a message and launch ArchInstaller
                QMessageBox::information(this, "Apex Tools Not Installed",
                                         "Claudemods Apex Tools are not installed. Launching ArchInstaller to install the tools...");

                // Launch ArchInstaller script
                launchArchInstaller();

                // Create InstalledVersion.txt and write version information
                createInstalledVersionFile();
            }
        }
    }

    void updateImageAnimation() {
        // Cycle through images
        currentImageIndex = (currentImageIndex + 1) % pixmaps.size();
        scaleImages();
        applyGlowEffect(); // Reapply glow effect when the image changes
    }

private:
    bool checkInstalledVersion() {
        // Check if InstalledVersion.txt exists
        QFileInfo fileInfo("/opt/claudemods-ApexTools/InstalledVersion.txt");
        return fileInfo.exists();
    }

    void launchArchInstaller() {
        // Launch ArchInstaller script
        QProcess *process = new QProcess(this);
        process->start("bash", QStringList() << "-c" << "hyprctl dispatch workspace 3 && /usr/bin/apexinstallgui");

        // Handle process completion
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitStatus == QProcess::CrashExit || exitCode != 0) {
                QMessageBox::critical(this, "Error", "Failed to launch ArchInstaller.");
            }
            process->deleteLater();
        });
    }

    void createInstalledVersionFile() {
        // Create and write to InstalledVersion.txt
        QFile file("/opt/claudemods-ApexTools/InstalledVersion.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "claudemods ApexTools v1.0 Build 11-02-2025";
            file.close();
        } else {
            QMessageBox::critical(this, "Error", "Failed to create InstalledVersion.txt.");
        }
    }

    void scaleImages() {
        // Get screen dimensions
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int screenHeight = screenGeometry.height();

        // Calculate image dimensions
        int largeImageHeight = screenHeight * 0.8; // 80% of screen height for the large image
        int smallImageHeight = screenHeight * 0.2; // 20% of screen height for the small images

        // Scale all images
        for (int i = 0; i < pixmaps.size(); ++i) {
            QPixmap scaledPixmap = pixmaps[i].scaled(
                i == currentImageIndex ? largeImageHeight : smallImageHeight,
                i == currentImageIndex ? largeImageHeight : smallImageHeight,
                Qt::KeepAspectRatio, Qt::SmoothTransformation
            );

            if (i == 0) {
                imageLabel1->setPixmap(scaledPixmap);
            } else if (i == 1) {
                imageLabel2->setPixmap(scaledPixmap);
            } else if (i == 2) {
                imageLabel3->setPixmap(scaledPixmap);
            }
        }
    }

    void applyGlowEffect() {
        // Remove glow effect from all labels
        if (imageLabel1->graphicsEffect()) imageLabel1->graphicsEffect()->setEnabled(false);
        if (imageLabel2->graphicsEffect()) imageLabel2->graphicsEffect()->setEnabled(false);
        if (imageLabel3->graphicsEffect()) imageLabel3->graphicsEffect()->setEnabled(false);

        // Apply glow effect to the currently enlarged image
        QGraphicsDropShadowEffect *glowEffect = new QGraphicsDropShadowEffect(this);
        glowEffect->setBlurRadius(20); // Glow size
        glowEffect->setColor(Qt::yellow); // Glow color
        glowEffect->setOffset(0); // Glow centered around the image

        if (currentImageIndex == 0) {
            imageLabel1->setGraphicsEffect(glowEffect);
        } else if (currentImageIndex == 1) {
            imageLabel2->setGraphicsEffect(glowEffect);
        } else if (currentImageIndex == 2) {
            imageLabel3->setGraphicsEffect(glowEffect);
        }
    }

    QHBoxLayout *imageLayout;
    QLabel *imageLabel1;
    QLabel *imageLabel2;
    QLabel *imageLabel3;
    QList<QPixmap> pixmaps;
    int currentImageIndex;
    QProgressBar *progressBar;
    QTimer *loadingTimer;
    QTabWidget *tabWidget; // Reference to the tab widget
};

// MainMenu class
class MainMenu : public QWidget {
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr) : QWidget(parent) {
        // Set the window to full-screen
        this->setWindowState(Qt::WindowFullScreen);

        // Create the background widget
        BackgroundWidget *backgroundWidget = new BackgroundWidget(this);

        // Create a layout for the overlay content
        QVBoxLayout *overlayLayout = new QVBoxLayout(backgroundWidget);
        overlayLayout->setAlignment(Qt::AlignCenter);

        // Add "Apex Tools" title
        QLabel *titleLabel = new QLabel("Apex Tools", backgroundWidget);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size: 40px; color: gold;");
        overlayLayout->addWidget(titleLabel);

        // Add "Click An Option Below" text
        QLabel *instructionLabel = new QLabel("Click An Option Below", backgroundWidget);
        instructionLabel->setAlignment(Qt::AlignCenter);
        instructionLabel->setStyleSheet("font-size: 30px; color: gold;");
        overlayLayout->addWidget(instructionLabel);

        // Create a horizontal layout for the buttons
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(20);

        // Create image buttons
        ImageButton *button1 = new ImageButton("images/gamester.png", "Apex Gamester", backgroundWidget);
        ImageButton *button2 = new ImageButton("images/isocreator.png", "Iso Creator Gui", backgroundWidget);
        ImageButton *button3 = new ImageButton("images/recovery.png", "Apex Recovery", backgroundWidget);

        // Connect button clicks to actions
        connect(button1, &ImageButton::clicked, this, &MainMenu::launchApexGamester);
        connect(button2, &ImageButton::clicked, this, &MainMenu::launchIsoCreator);
        connect(button3, &ImageButton::clicked, this, &MainMenu::launchApexRecovery);

        // Add buttons to the layout
        buttonLayout->addWidget(button1, 0, Qt::AlignCenter);
        buttonLayout->addWidget(button2, 0, Qt::AlignCenter);
        buttonLayout->addWidget(button3, 0, Qt::AlignCenter);

        // Add the button layout to the main layout
        overlayLayout->addLayout(buttonLayout);

        // Add a spacer to push the version label to the bottom
        overlayLayout->addStretch();

        // Add the version label in the bottom-right corner
        QLabel *versionLabel = new QLabel("Apex Tools v1.0 Build 07-03-2025", backgroundWidget);
        versionLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);
        versionLabel->setStyleSheet("font-size: 20px; color: gold; padding: 10px;");

        // Create a container widget for the version label
        QWidget *versionContainer = new QWidget(backgroundWidget);
        QHBoxLayout *versionLayout = new QHBoxLayout(versionContainer);
        versionLayout->setContentsMargins(0, 0, 0, 0);
        versionLayout->addStretch();
        versionLayout->addWidget(versionLabel, 0, Qt::AlignRight | Qt::AlignBottom);

        // Add the version container to the main layout
        overlayLayout->addWidget(versionContainer);

        // Set the background widget as the main widget of the MainMenu
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0); // Remove margins
        mainLayout->addWidget(backgroundWidget);
    }

private slots:
    void launchApexGamester() {
        QProcess *process = new QProcess(this);
        process->start("bash", QStringList() << "-c" << "/usr/bin/claudemods-ApexTools/ApexGamester/launchgamester.sh");
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitStatus == QProcess::CrashExit || exitCode != 0) {
                QMessageBox::critical(this, "Error", "Failed to start Apex Gamester.");
            }
            process->deleteLater();
        });
    }

    void launchIsoCreator() {
        QProcess *process = new QProcess(this);
        process->start("bash", QStringList() << "-c" << "hyprctl dispatch workspace 2 && /usr/bin/apexisocreatorgui");
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitStatus == QProcess::CrashExit || exitCode != 0) {
                QMessageBox::critical(this, "Error", "Failed to start Iso Creator.");
            }
            process->deleteLater();
        });
    }

    void launchApexRecovery() {
        QProcess *process = new QProcess(this);
        process->start("bash", QStringList() << "-c" << "/opt/claudemods-ApexTools/commands/launchrecovery.sh");
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitStatus == QProcess::CrashExit || exitCode != 0) {
                QMessageBox::critical(this, "Error", "Failed to start Apex Recovery.");
            }
            process->deleteLater();
        });
    }
};

// Main function
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create a tab widget
    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->setWindowTitle("Apex Tools");

    // Hide the tab bar
    tabWidget->tabBar()->hide();

    // Create the LoadingWindow and add it as the first tab
    LoadingWindow *loadingWindow = new LoadingWindow(tabWidget);
    tabWidget->addTab(loadingWindow, ""); // Empty tab name

    // Create the MainMenu and add it as the second tab
    MainMenu *mainMenu = new MainMenu();
    tabWidget->addTab(mainMenu, ""); // Empty tab name

    // Show the tab widget
    tabWidget->setWindowState(Qt::WindowFullScreen);
    tabWidget->show();

    return app.exec();
}

#include "main.moc"
