#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

class QLabel;
class QScrollArea;
class QAction;
class QMenu;
class QToolBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void openImage();
    void saveImage();
    void rotateClockwise90();
    void zoomIn();
    void zoomOut();
    void resetView();
    void convertToGrayscale();
    void adjustBrightness();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createCentralWidget();
    void updateImageDisplay();
    void updateActionsState();

    QScrollArea *m_scrollArea = nullptr;
    QLabel *m_imageLabel = nullptr;
    QImage m_currentImage;
    QString m_currentFilePath;
    double m_scaleFactor = 1.0;

    QMenu *m_fileMenu = nullptr;
    QMenu *m_imageMenu = nullptr;
    QMenu *m_editMenu = nullptr;
    QToolBar *m_mainToolBar = nullptr;

    QAction *m_openAction = nullptr;
    QAction *m_saveAction = nullptr;
    QAction *m_exitAction = nullptr;

    QAction *m_rotateAction = nullptr;
    QAction *m_zoomInAction = nullptr;
    QAction *m_zoomOutAction = nullptr;
    QAction *m_resetViewAction = nullptr;

    QImage m_baseImage;
    int m_brightness = 0;

    QAction *m_grayscaleAction = nullptr;
    QAction *m_brightnessAction = nullptr;

};
#endif // MAINWINDOW_H
