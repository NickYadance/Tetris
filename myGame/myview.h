#ifndef MYVIEW_H
#define MYVIEW_H

#include <QObject>
#include <QtWidgets/QGraphicsView>
#include <QGraphicsTextItem>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>

#define INITSPEED 500

class BoxGroup;

class MyView :public QGraphicsView
{
    Q_OBJECT
public:
    explicit    MyView(QWidget * parent = 0);

public slots:
    void    startGame();
    void    restartGame();
    void    finishGame();
    void    pauseGame();
    void    returnGame();
    void    gameOver();
    void    clearFullRows();
    void    moveBox();
    void    aboutToFinish();
protected:
    void    keyPressEvent(QKeyEvent *event);

private:
    BoxGroup            *boxGroup;
    BoxGroup            *nextBoxGroup;
    QGraphicsLineItem   *topLine;
    QGraphicsLineItem   *bottomLine;
    QGraphicsLineItem   *leftLine;
    QGraphicsLineItem   *rightLine;
    qreal               gameSpeed;
    QList< int >        rows;

    void    initView();
    void    initGame();
    void    updateScore(const int fullRowNum = 0);

    QPoint topLeft ;
    QPoint topRight;
    QPoint bottomLeft;
    QPoint bottomRight;
    QPoint newBox;
    QPoint hintBox;

    QGraphicsTextItem *gameScoreText ;
    QGraphicsTextItem *gameLevelText ;
    QGraphicsTextItem *gameWelcomeText ;
    QGraphicsTextItem *gamePausedText ;
    QGraphicsTextItem *gameOverText ;

    QGraphicsWidget *maskWidget;
    QGraphicsWidget *startButton;
    QGraphicsWidget *finishButton;
    QGraphicsWidget *restartButton;
    QGraphicsWidget *pauseButton;
    QGraphicsWidget *optionButton;
    QGraphicsWidget *returnButton;
    QGraphicsWidget *helpButton;
    QGraphicsWidget *exitButton;
    QGraphicsWidget *showMenuButton;

    QMediaPlayer    *bgMusic;
    QMediaPlayer    *clearRowSound;

    QMediaPlaylist  *bgMusicList;
    QMediaPlaylist  *clearRowList;
};

#endif // MYVIEW_H
