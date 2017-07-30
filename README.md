# Tetris #
按照 `Qt及Qucik开发实战精解`一书写的简易俄罗斯方块游戏
# Review #
- Qt的图像视图框架

> 三大部分：图形项 **QGraphicsItem**、场景**QGraphicsScene**以及视图**QGraphicsView**


- 如何在qt空项目中让QOBJECT子类正确运行：
在.pro文件中添加
> greaterThan(QT_MAJOR_VERSION, 4): QT += widgets 

- Q_OBJECT宏有什么作用：
> 如果自定义类是QOBJECT的子类，并且需要利用到Qt的槽与信号机制则添加Q_OBJECT宏声明

- 如何实现QGraphicsItemGroup的组合旋转:

> setTransform(transform().rotate(90));

- 如何实现背景音乐的自动重复播放

> 将要播放的音乐放在QMediaPlayerList中，调用 setPlaybackMode(QMediaPlaylist::Loop)

- 使用QUrl引用资源文件

> QUrl("qrc:/background.mp3")

- 无法播放媒体文件：DirectShowPlayerService::doSetUrlSource: Unresolved error code 0x8007007b ()

> 由于qt会调用操作系统自带的播放设备，可能引起问题，安装第三方设备如 LAVFilters-0.65.exe 可解决

- Qt5不再支持phonon框架（可使用，需要手动安装）

> 使用Qt5自带的QtMediPlayer
