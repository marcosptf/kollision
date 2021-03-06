/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
  Copyright (c) 2010 Brian Croom <brian.s.croom@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MAINAREA_H
#define MAINAREA_H

#include <QTimer>
#include <QTime>
#include <QList>
#include <QGraphicsScene>
#include <KGameRenderer>
#include <KgSound>
#include "animator.h"
#include "message.h"

class Renderer;
class Ball;
class Animation;
class QGraphicsSceneMouseEvent;
class QAction;

struct Collision;

class MainArea : public QGraphicsScene
{
Q_OBJECT
    QTimer m_timer;
    int m_lastTime;
    int m_lastGameTime;
    QTime m_time;

    /// time interval between two balls being added
    int m_ball_timeout;

    int m_size;
    KGameRenderer m_renderer;
    Animator m_animator;
    QFont m_msgFont;

    QList<Ball*> m_balls;
    QList<Ball*> m_fading;
    Ball* m_man;

    // Player's ball diameter
    int m_manBallDiameter;
    // Other balls' diameter
    int m_ballDiameter;

    /// the blue ball is dead
    bool m_death;

    /// the falling animation is over, we're waiting for a new game to start
    bool m_game_over;

    bool m_paused;
    int m_pauseTime;
    int m_penalty;

    QList<MessagePtr> m_welcomeMsg;
    QList<MessagePtr> m_pauseMsg;

    // Flag if automatic incremental ball size is enabled.
    bool m_increaseBallSize;

    // Flag if sound is enabled.
    bool m_soundEnabled;

    KgSound m_soundHitWall;
    KgSound m_soundYouLose;
    KgSound m_soundBallLeaving;
    KgSound m_soundStart;

    QAction * m_pauseAction;

    double radius() const;
    void setBallDiameter(int val);

    QPointF randomPoint() const;
    QPointF randomDirection(double val) const;

    Ball* addBall(const QString& id);
    bool collide(const QPointF& a, const QPointF& b,
                double diamA, double diamB, Collision& collision);

    Animation* writeMessage(const QString& text);
    Animation* writeText(const QString& text, bool fade = true);
    void displayMessages(const QList<QExplicitlySharedDataPointer<Message> >& msgs);
    void playSound(int sound);
    void onDeath();
    void setManPosition(const QPointF& p);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent*) Q_DECL_OVERRIDE;
public:
    MainArea();
    void start();
    void setPauseAction(QAction * action);
public slots:
    void tick();
    void increaseBallSize(bool enable);
    void enableSounds(bool enable);
    void abort();
    void togglePause();
signals:
    void starting();
    void gameOver(int);
    void changeBallNumber(int);
    void changeGameTime(int);
    void changeState(bool);
    void pause(bool);
};

#endif // MAINAREA_H
