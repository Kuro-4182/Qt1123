#include "mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QImageReader>
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QScrollArea>
#include <QStatusBar>
#include <QStyle>
#include <QToolBar>
#include <QTransform>
#include <QInputDialog>
#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createCentralWidget();
    createActions();
    createMenus();
    createToolBar();
    updateActionsState();

    resize(960, 640);
    setWindowTitle(tr("Qt 影像處理程式 v1"));
    statusBar()->showMessage(tr("就緒：請使用 Open Image 載入圖片"));
}
\
    void MainWindow::createCentralWidget(){
    m_imageLabel = new QLabel(this);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setText(tr("尚未載入圖片\n請點選 File -> Open Image 或工具列圖示開啟圖片"));
    m_imageLabel->setMinimumSize(320, 240);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidget(m_imageLabel);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setAlignment(Qt::AlignCenter);

    setCentralWidget(m_scrollArea);
}

void MainWindow::createActions(){

    m_openAction = new QAction(this);
    m_openAction->setText(tr("Open Image"));
    m_openAction->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    m_openAction->setShortcut(QKeySequence::Open); // Ctrl+O
    m_openAction->setStatusTip(tr("開啟圖片檔案"));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openImage);

    m_saveAction = new QAction(this);
    m_saveAction->setText(tr("Save"));
    m_saveAction->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    m_saveAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    m_saveAction->setStatusTip(tr("儲存目前編輯的圖片 (Ctrl+S)"));
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveImage);

    m_exitAction = new QAction(this);
    m_exitAction->setText(tr("Exit"));
    m_exitAction->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("離開程式"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);

    m_rotateAction = new QAction(this);
    m_rotateAction->setText(tr("Rotate 90° CW"));
    m_rotateAction->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    m_rotateAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    m_rotateAction->setStatusTip(tr("將圖片順時針旋轉 90 度"));
    connect(m_rotateAction, &QAction::triggered, this, &MainWindow::rotateClockwise90);

    m_zoomInAction = new QAction(this);
    m_zoomInAction->setText(tr("Zoom In"));
    m_zoomInAction->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    m_zoomInAction->setStatusTip(tr("放大預覽"));
    connect(m_zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);

    m_zoomOutAction = new QAction(this);
    m_zoomOutAction->setText(tr("Zoom Out"));
    m_zoomOutAction->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    m_zoomOutAction->setStatusTip(tr("縮小預覽"));
    connect(m_zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);

    m_resetViewAction = new QAction(this);
    m_resetViewAction->setText(tr("Reset Zoom (100%)"));
    m_resetViewAction->setIcon(style()->standardIcon(QStyle::SP_RestoreDefaultsButton));
    m_resetViewAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    m_resetViewAction->setStatusTip(tr("重設縮放比例為 100%"));
    connect(m_resetViewAction, &QAction::triggered, this, &MainWindow::resetView);

    m_grayscaleAction = new QAction(this);
    m_grayscaleAction->setText(tr("Grayscale"));
    m_grayscaleAction->setIcon(style()->standardIcon(QStyle::SP_DesktopIcon));
    m_grayscaleAction->setStatusTip(tr("將圖片轉為灰階"));
    connect(m_grayscaleAction, &QAction::triggered, this, &MainWindow::convertToGrayscale);

    m_brightnessAction = new QAction(this);
    m_brightnessAction->setText(tr("Brightness..."));
    m_brightnessAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    m_brightnessAction->setStatusTip(tr("調整圖片明亮度 (-100 ~ +100)"));
    connect(m_brightnessAction, &QAction::triggered, this, &MainWindow::adjustBrightness);

}

void MainWindow::createMenus(){

    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_imageMenu = menuBar()->addMenu(tr("&Image"));
    m_imageMenu->addAction(m_rotateAction);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_zoomInAction);
    m_editMenu->addAction(m_zoomOutAction);
    m_editMenu->addAction(m_resetViewAction);

    m_imageMenu->addAction(m_rotateAction);
    m_imageMenu->addAction(m_grayscaleAction);
    m_imageMenu->addAction(m_brightnessAction);

}

void MainWindow::createToolBar(){

    m_mainToolBar = addToolBar(tr("Main ToolBar"));
    m_mainToolBar->setMovable(false);

    m_mainToolBar->addAction(m_openAction);
    m_mainToolBar->addAction(m_saveAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_rotateAction);
    m_mainToolBar->addAction(m_zoomInAction);
    m_mainToolBar->addAction(m_zoomOutAction);
    m_mainToolBar->addAction(m_resetViewAction);

    m_mainToolBar->addAction(m_rotateAction);
    m_mainToolBar->addAction(m_grayscaleAction);
    m_mainToolBar->addAction(m_brightnessAction);

}

void MainWindow::openImage(){

    const QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Image"),
        QString(),
        tr("Images (*.png *.jpg *.jpeg *.bmp);;All Files (*)")
        );

    if (fileName.isEmpty()) {
        return;
    }

    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage loaded = reader.read();

    if (loaded.isNull()) {
        QMessageBox::warning(this, tr("開啟失敗"), tr("無法載入圖片：%1").arg(reader.errorString()));
        return;
    }

    m_currentImage = loaded;
    m_currentFilePath = fileName;
    m_scaleFactor = 1.0;

    updateImageDisplay();
    updateActionsState();
    statusBar()->showMessage(tr("已載入：%1 (%2 x %3)")
                                 .arg(m_currentFilePath)
                                 .arg(m_currentImage.width())
                                 .arg(m_currentImage.height()));

    m_baseImage = loaded;
    m_currentImage = m_baseImage;
    m_brightness = 0;

}

void MainWindow::saveImage(){

    if (m_currentImage.isNull()) {
        return;
    }

    const QString suggestedPath = m_currentFilePath.isEmpty() ? tr("edited_image.png") : m_currentFilePath;
    const QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Image"),
        suggestedPath,
        tr("PNG Image (*.png);;JPEG Image (*.jpg *.jpeg);;BMP Image (*.bmp)")
        );

    if (fileName.isEmpty()) {
        return;
    }

    if (!m_currentImage.save(fileName)) {
        QMessageBox::warning(this, tr("儲存失敗"), tr("無法將圖片儲存至指定路徑。"));
        return;
    }

    m_currentFilePath = fileName;
    statusBar()->showMessage(tr("已儲存圖片：%1").arg(fileName));

}

void MainWindow::rotateClockwise90(){

    if (m_currentImage.isNull()) {
        return;
    }

    QTransform transform;
    transform.rotate(90.0);
    m_baseImage = m_baseImage.transformed(transform, Qt::SmoothTransformation);
    m_currentImage = m_baseImage;

    updateImageDisplay();
    statusBar()->showMessage(tr("已順時針旋轉 90 度（目前尺寸：%1 x %2）")
                                 .arg(m_currentImage.width())
                                 .arg(m_currentImage.height()));

}

void MainWindow::zoomIn(){

    if (m_currentImage.isNull()) {
        return;
    }
    m_scaleFactor *= 1.25;
    updateImageDisplay();

}

void MainWindow::zoomOut(){

    if (m_currentImage.isNull()) {
        return;
    }
    m_scaleFactor *= 0.8;
    updateImageDisplay();

}

void MainWindow::resetView(){

    if (m_currentImage.isNull()) {
        return;
    }
    m_scaleFactor = 1.0;
    updateImageDisplay();

}

void MainWindow::updateImageDisplay(){

    if (m_currentImage.isNull()) {
        return;
    }

    const QSize targetSize = m_currentImage.size() * m_scaleFactor;
    const QPixmap scaledPixmap = QPixmap::fromImage(m_currentImage)
                                     .scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_imageLabel->setPixmap(scaledPixmap);
    m_imageLabel->resize(scaledPixmap.size());
    statusBar()->showMessage(tr("縮放比例：%1%").arg(qRound(m_scaleFactor * 100.0)));

}

void MainWindow::updateActionsState(){

    const bool hasImage = !m_currentImage.isNull();
    m_saveAction->setEnabled(hasImage);
    m_rotateAction->setEnabled(hasImage);
    m_grayscaleAction->setEnabled(hasImage);
    m_brightnessAction->setEnabled(hasImage);
    m_zoomInAction->setEnabled(hasImage);
    m_zoomOutAction->setEnabled(hasImage);
    m_resetViewAction->setEnabled(hasImage);

}

void MainWindow::convertToGrayscale()
{
    if (m_baseImage.isNull()) {
        return;
    }

    m_baseImage = m_baseImage.convertToFormat(QImage::Format_Grayscale8);
    m_brightness = 0;
    m_currentImage = m_baseImage;

    updateImageDisplay();
    statusBar()->showMessage(tr("已轉換為灰階影像"));
}

void MainWindow::adjustBrightness()
{
    if (m_baseImage.isNull()) {
        return;
    }

    bool ok = false;
    const int delta = QInputDialog::getInt(
        this,
        tr("調整明亮度"),
        tr("明亮度偏移 (-100 ~ +100)："),
        m_brightness,
        -100,
        100,
        5,
        &ok
        );

    if (!ok) {
        return;
    }

    m_brightness = delta;

    QImage adjusted = m_baseImage.convertToFormat(QImage::Format_ARGB32);
    const int offset = qRound((m_brightness / 100.0) * 255.0);

    for (int y = 0; y < adjusted.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(adjusted.scanLine(y));
        for (int x = 0; x < adjusted.width(); ++x) {
            const QColor c(line[x]);
            const int r = qBound(0, c.red() + offset, 255);
            const int g = qBound(0, c.green() + offset, 255);
            const int b = qBound(0, c.blue() + offset, 255);
            line[x] = qRgba(r, g, b, c.alpha());
        }
    }

    m_currentImage = adjusted;
    updateImageDisplay();
    statusBar()->showMessage(tr("明亮度調整為：%1").arg(m_brightness));
}

MainWindow::~MainWindow() = default;
