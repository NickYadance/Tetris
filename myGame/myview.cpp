#include "myview.h"
#include "boxgroup.h"
#include "onebox.h"
#include <QApplication>
#include <QIcon>
#include <QDebug>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <QTimer>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QFileInfo>
#include <QSlider>

MyView::MyView(QWidget *parent)
{
    initView();
}

void MyView::startGame()
{
    gameWelcomeText->hide();
    startButton->hide();
    optionButton->hide();
    helpButton->hide();
    exitButton->hide();
    maskWidget->hide();
    initGame();
}

void MyView::restartGame()
{
    maskWidget->hide();
    gameOverText->hide();
    finishButton->hide();
    restartButton->setPos(600, 150);

    // 销毁提示方块组和当前方块移动区域中的所有小方块
    nextBoxGroup->clearBoxGroup(true);
    boxGroup->clearBoxGroup();
    boxGroup->hide();
    foreach (QGraphicsItem *item, scene()->items(QRectF(199, 49, 202, 402), Qt::ContainsItemShape)) {
        // 先从场景中移除小方块，因为使用deleteLater()是在返回主事件循环后才销毁
        // 小方块的，为了在出现新的方块组时不发生碰撞，所以需要先从场景中移除小方块
        scene()->removeItem(item);
        OneBox *box = (OneBox*) item;
        box->deleteLater();
    }

    initGame();
}

void MyView::finishGame()
{
    gameOverText->hide();
    finishButton->hide();
    restartButton->setPos(600, 150);
    restartButton->hide();
    pauseButton->hide();
    showMenuButton->hide();
    gameScoreText->hide();
    gameLevelText->hide();

    topLine->hide();
    bottomLine->hide();
    leftLine->hide();
    rightLine->hide();

    nextBoxGroup->clearBoxGroup(true);
    boxGroup->clearBoxGroup();
    boxGroup->hide();

    foreach (QGraphicsItem *item, scene()->items(QRectF(199, 49, 202, 402), Qt::ContainsItemShape)) {
        OneBox *box = (OneBox*) item;
        box->deleteLater();
    }

    // 可能是在进行游戏时按下“主菜单”按钮
    maskWidget->show();
    gameWelcomeText->show();
    startButton->show();
    optionButton->show();
    helpButton->show();
    exitButton->show();
    scene()->setBackgroundBrush(QPixmap(":/background.png"));

    // 在2-5中添加的代码
//    backgroundMusic->setCurrentSource(Phonon::MediaSource(SOUNDPATH + "background.mp3"));
//    backgroundMusic->play();
}

void MyView::pauseGame()
{
    boxGroup->stopTimer();
    restartButton->hide();
    pauseButton->hide();
    showMenuButton->hide();
    maskWidget->show();
    gamePausedText->show();
    returnButton->show();
}

void MyView::returnGame()
{
    returnButton->hide();
    gamePausedText->hide();
    maskWidget->hide();
    restartButton->show();
    pauseButton->show();
    showMenuButton->show();
    boxGroup->startTimer(gameSpeed);
}

void MyView::clearFullRows()
{
    for (int y = 429; y > 50; y -= 20)
    {
        QList<QGraphicsItem*> list = scene()->items(QRectF(199, y, 202, 22), Qt::ContainsItemShape);
        if (list.count() == 10)
        {
            foreach (QGraphicsItem *item, list) {
                OneBox * box = (OneBox *)item;
                QGraphicsBlurEffect * blurEffect = new QGraphicsBlurEffect ;
                box->setGraphicsEffect(blurEffect);
                QPropertyAnimation * animation = new QPropertyAnimation(box, "scale");
                animation->setEasingCurve(QEasingCurve::OutBounce);
                animation->setDuration(400);
                animation->setStartValue(4);
                animation->setEndValue(0.25);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                connect(animation, SIGNAL(finished()), box, SLOT(deleteLater()));
//                box->deleteLater();
            }
            rows << y ;
        }
    }
    if (rows.count() > 0)
    {
//        moveBox();
        clearRowSound->play();
        QTimer::singleShot(400, this, SLOT(moveBox()));
    }
    else
    {
        boxGroup->createBox(newBox, nextBoxGroup->getCurrentShape());
        nextBoxGroup->clearBoxGroup(true);
        nextBoxGroup->createBox(hintBox);
    }
}

void MyView::moveBox()
{
    for (int i = rows.count(); i > 0 ; --i) {
        int row = rows.at(i - 1);
        foreach (QGraphicsItem * item, scene()->items(QRectF(199, 49, 202, row - 47), Qt::ContainsItemShape)) {
            item->moveBy(0, 20);
        }
    }
    updateScore(rows.count());
    rows.clear();
    boxGroup->createBox(newBox, nextBoxGroup->getCurrentShape());
    nextBoxGroup->clearBoxGroup(true);
    nextBoxGroup->createBox(hintBox);
}

void MyView::aboutToFinish()
{

}

void MyView::keyPressEvent(QKeyEvent *event)
{
    if (pauseButton->isVisible())
    {
        boxGroup->setFocus();
    }
    else {
        boxGroup->clearFocus();
    }
    QGraphicsView::keyPressEvent(event);
}

void MyView::gameOver()
{
    pauseButton->hide();
    showMenuButton->hide();
    maskWidget->show();
    gameOverText->show();
    restartButton->setPos(370, 200);
    finishButton->show();
}

void MyView::initView()
{
    setRenderHint(QPainter::Antialiasing);
    setCacheMode(CacheBackground);
    setWindowTitle("方块游戏 By Nicky");
    setWindowIcon(QIcon(":/icon.png"));
    setMinimumSize(810, 510);
    setMaximumSize(810, 510);

    QGraphicsScene  *scene = new QGraphicsScene;
    scene->setSceneRect(5 , 5, 800, 500);
    scene->setBackgroundBrush(QPixmap(":/background.png"));
    setScene(scene);

    topLeft.setX(197);topLeft.setY(47);
    topRight.setX(403);topRight.setY(47);
    bottomLeft.setX(197);bottomLeft.setY(453);
    bottomRight.setX(403);bottomRight.setY(453);
    newBox.setX(300);newBox.setY(70);
    hintBox.setX(500);hintBox.setY(70);

    topLine = scene->addLine(QLine(topLeft, topRight));
    bottomLine = scene->addLine(QLine(bottomLeft, bottomRight));
    leftLine = scene->addLine(QLine(topLeft, bottomLeft));
    rightLine = scene->addLine(QLine(topRight, bottomRight));

    boxGroup = new BoxGroup;
    connect(boxGroup, SIGNAL(needNewBox()), this, SLOT(clearFullRows()));
    connect(boxGroup, SIGNAL(gameFinished()), this, SLOT(gameOver()));
    scene->addItem(boxGroup);
    nextBoxGroup = new BoxGroup;
    scene->addItem(nextBoxGroup);

    gameScoreText = new QGraphicsTextItem;
    gameScoreText->setFont(QFont("times", 20, QFont::Bold));
    gameScoreText->setPos(650, 350);
    gameLevelText = new QGraphicsTextItem;
    gameLevelText->setFont(QFont("times", 30, QFont::Bold));
    gameLevelText->setPos(20, 150);

    QGraphicsDropShadowEffect *textEffect = new QGraphicsDropShadowEffect;
    gameScoreText->setGraphicsEffect(textEffect);
    QGraphicsDropShadowEffect *textEffect22 = new QGraphicsDropShadowEffect;
    gameLevelText->setGraphicsEffect(textEffect22);

    scene->addItem(gameScoreText);
    scene->addItem(gameLevelText);

    topLine->hide();
    bottomLine->hide();
    leftLine->hide();
    rightLine->hide();
    gameScoreText->hide();
    gameLevelText->hide();

    QWidget * mask = new QWidget ;
    mask->setAutoFillBackground(true);
    mask->setPalette(QPalette(QColor(0, 0, 0, 80)));
    mask->resize(900, 600);
    maskWidget = scene->addWidget(mask);
    maskWidget->setPos(-50, -50);
    maskWidget->setZValue(1);

    // 选项面板
    QWidget *option = new QWidget;
    QPushButton *optionCloseButton = new QPushButton(tr("关   闭"), option);
    QPalette palette;
    palette.setColor(QPalette::ButtonText, Qt::black);
    optionCloseButton->setPalette(palette);
    optionCloseButton->move(120, 300);
    connect(optionCloseButton, SIGNAL(clicked()), option, SLOT(hide()));
    option->setAutoFillBackground(true);
    option->setPalette(QPalette(QColor(0, 0, 0, 180)));
    option->resize(300, 400);
    QGraphicsWidget *optionWidget = scene->addWidget(option);
    optionWidget->setPos(250, 50);
    optionWidget->setZValue(3);
    optionWidget->hide();

    // 帮助面板
    QWidget *help = new QWidget;
    QPushButton *helpCloseButton = new QPushButton(tr("关   闭"), help);
    helpCloseButton->setPalette(palette);
    helpCloseButton->move(120, 300);
    connect(helpCloseButton, SIGNAL(clicked()), help, SLOT(hide()));
    help->setAutoFillBackground(true);
    help->setPalette(QPalette(QColor(0, 0, 0, 180)));
    help->resize(300, 400);
    QGraphicsWidget *helpWidget = scene->addWidget(help);
    helpWidget->setPos(250, 50);
    helpWidget->setZValue(3);
    helpWidget->hide();

    QLabel *helpLabel = new QLabel(help);
    helpLabel->setText(tr("<h2><font color=white>Nicky Refer:yafeilinux"
                          "<br>www.yafeilinux.com</font></h2>"));
    helpLabel->setAlignment(Qt::AlignCenter);
    helpLabel->move(30, 150);


    // 游戏欢迎文本
    gameWelcomeText = new QGraphicsTextItem;
    gameWelcomeText->setHtml(tr("<font color=white>MyBox方块游戏</font>"));
    gameWelcomeText->setFont(QFont("Times", 30, QFont::Bold));
    gameWelcomeText->setPos(250, 100);
    gameWelcomeText->setZValue(2);
    scene->addItem(gameWelcomeText);

    // 游戏暂停文本
    gamePausedText = new QGraphicsTextItem;
    gamePausedText->setHtml(tr("<font color=white>游戏暂停中！</font>"));
    gamePausedText->setFont(QFont("Times", 30, QFont::Bold));
    gamePausedText->setPos(300, 100);
    gamePausedText->setZValue(2);
    gamePausedText->hide();
    scene->addItem(gamePausedText);

    // 游戏结束文本
    gameOverText = new QGraphicsTextItem;
    gameOverText->setHtml(tr("<font color=white>游戏结束！</font>"));
    gameOverText->setFont(QFont("Times", 30, QFont::Bold));
    gameOverText->setPos(320, 100);
    gameOverText->setZValue(2);
    gameOverText->hide();
    scene->addItem(gameOverText);

    // 游戏中使用的按钮

    QPushButton *button1 = new QPushButton(tr("开    始"));
    QPushButton *button2 = new QPushButton(tr("选    项"));
    QPushButton *button3 = new QPushButton(tr("帮    助"));
    QPushButton *button4 = new QPushButton(tr("退    出"));
    QPushButton *button5 = new QPushButton(tr("重新开始"));
    QPushButton *button6 = new QPushButton(tr("暂    停"));
    QPushButton *button7 = new QPushButton(tr("主 菜 单"));
    QPushButton *button8 = new QPushButton(tr("返回游戏"));
    QPushButton *button9 = new QPushButton(tr("结束游戏"));


    connect(button1, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(button2, SIGNAL(clicked()), option, SLOT(show()));
    connect(button3, SIGNAL(clicked()), help, SLOT(show()));
    connect(button4, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
    connect(button5, SIGNAL(clicked()), this, SLOT(restartGame()));
    connect(button6, SIGNAL(clicked()), this, SLOT(pauseGame()));
    connect(button7, SIGNAL(clicked()), this, SLOT(finishGame()));
    connect(button8, SIGNAL(clicked()), this, SLOT(returnGame()));
    connect(button9, SIGNAL(clicked()), this, SLOT(finishGame()));

    startButton = scene->addWidget(button1);
    optionButton = scene->addWidget(button2);
    helpButton = scene->addWidget(button3);
    exitButton = scene->addWidget(button4);
    restartButton = scene->addWidget(button5);
    pauseButton = scene->addWidget(button6);
    showMenuButton = scene->addWidget(button7);
    returnButton = scene->addWidget(button8);
    finishButton = scene->addWidget(button9);

    startButton->setPos(370, 200);
    optionButton->setPos(370, 250);
    helpButton->setPos(370, 300);
    exitButton->setPos(370, 350);
    restartButton->setPos(600, 150);
    pauseButton->setPos(600, 200);
    showMenuButton->setPos(600, 250);
    returnButton->setPos(370, 200);
    finishButton->setPos(370, 250);

    startButton->setZValue(2);
    optionButton->setZValue(2);
    helpButton->setZValue(2);
    exitButton->setZValue(2);
    restartButton->setZValue(2);
    returnButton->setZValue(2);
    finishButton->setZValue(2);

    restartButton->hide();
    finishButton->hide();
    pauseButton->hide();
    showMenuButton->hide();
    returnButton->hide();

    //音效
    bgMusic = new QMediaPlayer;
    clearRowSound = new QMediaPlayer ;

    bgMusicList= new QMediaPlaylist();
    bgMusicList->addMedia(QUrl("qrc:/background.mp3"));
    bgMusicList->setPlaybackMode(QMediaPlaylist::Loop);
    bgMusic->setPlaylist(bgMusicList);

    clearRowList= new QMediaPlaylist();
    clearRowList->addMedia(QUrl("qrc:/clearRow.mp3"));
    clearRowList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    clearRowSound->setPlaylist(clearRowList);

    QLabel *volumeLabel = new QLabel(QString("音乐"), option);
    QLabel *musicLabel = new QLabel(QString("音效"), option);
    QSlider *volumeSlider = new QSlider(Qt::Horizontal, option);
    QSlider *musicSlider = new QSlider(Qt::Horizontal, option);

    volumeSlider->move(100, 100);
    volumeSlider->setRange(0, 200);
    volumeSlider->setValue(100);
    musicSlider->move(100, 200);
    volumeLabel->move(60, 105);
    musicLabel->move(60, 205);

    connect(volumeSlider, SIGNAL(valueChanged(int)), bgMusic, SLOT(setVolume(int)));
    connect(button6, SIGNAL(clicked(bool)), bgMusic, SLOT(pause()));
    connect(button8, SIGNAL(clicked(bool)), bgMusic, SLOT(play()));
    //    startGame();
}

void MyView::initGame()
{
    restartButton->show();
    pauseButton->show();
    showMenuButton->show();
    gameScoreText->show();
    gameLevelText->show();
    topLine->show();
    bottomLine->show();
    leftLine->show();
    rightLine->show();
    boxGroup->show();

    boxGroup->createBox(newBox);
    boxGroup->setFocus();
    boxGroup->startTimer(INITSPEED);
    gameSpeed = INITSPEED;
    nextBoxGroup->createBox(hintBox);
    scene()->setBackgroundBrush(QPixmap(":/background01.png"));
    gameScoreText->setHtml(QString("<font color=red>Score: 0</font>"));
    gameLevelText->setHtml(QString("<font color=white>第<br>一<br>幕</font>"));

    bgMusic->play();
}

void MyView::updateScore(const int fullRowNum)
{
    int score = fullRowNum * 100;
    int currentScore = gameScoreText->toPlainText().toInt();
    currentScore += score;
    gameScoreText->setHtml(QString("<font color=red>%1</font>").arg(currentScore));
    if (currentScore < 500)
    {
        //do nothing
    }
    else if (currentScore < 1000)
    {
        gameLevelText->setHtml(QString("<font color=white>第<br>二<br>幕"));
        scene()->setBackgroundBrush(QPixmap(":/background02.png"));
        gameSpeed = 300 ;
        boxGroup->stopTimer();
        boxGroup->startTimer(gameSpeed);
    }
    else
    {
        //next
    }
}
