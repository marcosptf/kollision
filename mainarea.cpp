/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "mainarea.h"
#include "renderer.h"
#include "ball.h"

#include <kdebug.h>
#include <klocalizedstring.h>
#include <kmessagebox.h>
#include <QMouseEvent>

// for rand
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

struct Collision
{
    double square_distance;
    QPointF line;
};

MainArea::MainArea(QWidget* parent)
: KGameCanvasWidget(parent)
, m_man(0)
, m_death(false)
{
    m_renderer = new Renderer;
    m_renderer->resize(QSize(30, 30));
    
    srand(time(0));
    
    addBall("red_ball");
    addBall("red_ball");
    addBall("red_ball");
    addBall("red_ball");
    
    /*
    
    red->setPosition(QPointF(40.0, 40.0));
    red->setVelocity(QPointF(3.0, 0.1));
    
    blue->setPosition(QPointF(150.0, 40.0));
    blue->setVelocity(QPointF(-1.0, 0));*/
    /*
    Ball* ball = new Ball(this, m_renderer, "red_ball");
    ball->moveTo(30, 30);
    ball->show();*/
    
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_time.start();
    m_timer.start(0);
    
    setMouseTracking(true);
}

QPointF MainArea::randomPoint() const
{
    double x = (double)rand() * (width() - m_renderer->size().width()) / RAND_MAX;
    double y = (double)rand() * (height() - m_renderer->size().height()) / RAND_MAX;
    return QPointF(x, y);
}

QPointF MainArea::randomDirection(double val) const
{
    double angle = (double)rand() * 2 * M_PI / RAND_MAX;
    return QPointF(val * sin(angle), val * cos(angle));
}

Ball* MainArea::addBall(const QString& id)
{
    Ball* ball = new Ball(this, m_renderer, id);
    QPoint pos = randomPoint().toPoint();
    kDebug() << "ball at " << pos << endl;
    kDebug() << "size = " << size() << endl;
    ball->setPosition(pos);
    ball->setVelocity(randomDirection((double)rand() / RAND_MAX));
    
    ball->show();
    m_balls.push_back(ball);
    
    return ball;
}

bool MainArea::collide(const QPointF& a, const QPointF& b, double diam, Collision& collision)
{
    collision.line = b - a;
    collision.square_distance = collision.line.x() * collision.line.x()
                              + collision.line.y() * collision.line.y();
    return collision.square_distance <= diam * diam;
}

void MainArea::tick()
{
    int w = m_renderer->size().width();
    int h = m_renderer->size().height();
    Collision collision;
    
    foreach (Ball* ball, m_balls) {
        QPointF pos = ball->position();
        QPointF vel = ball->velocity();
        QPointF center = pos + QPoint(w, h) / 2.0;

        if (m_man && collide(center, m_man->position() + QPoint(w, h) / 2.0, w, collision)) {
            m_death = true;
        }
        
        // handle collisions with borders
        if (pos.x() <= 0) {
            vel.setX(fabs(vel.x()));
        }
        if (pos.x() >= width() - w) {
            vel.setX(-fabs(vel.x()));
        }
        if (pos.y() <= 0) {
            vel.setY(fabs(vel.y()));
        }
        if (!m_death) {
            if (pos.y() >= height() - h) {
                vel.setY(-fabs(vel.y()));        
            }
        }
        
        ball->setVelocity(vel);
        
        // handle collisions with other balls
        foreach (Ball* other, m_balls) {
            if (other == ball) {
                continue;
            }
            QPointF other_center = other->position() + QPoint(w, h) / 2.0;
            if (collide(center, other_center, w, collision)) {
                QPointF other_vel = other->velocity();
                
                // compute the parallel component of the
                // velocity with respect to the collision line
                double v_par = vel.x() * collision.line.x() 
                             + vel.y() * collision.line.y();
                double w_par = other_vel.x() * collision.line.x()
                             + other_vel.y() * collision.line.y();
                
                // switch those two parallel components
                if (w_par - v_par <= 0) {
                    QPointF drift = collision.line * (w_par - v_par) /
                                     collision.square_distance;
                    ball->setVelocity(ball->velocity() + drift);
                    other->setVelocity(other->velocity() - drift);
                }
            }
        }
    }
    
    foreach (Ball* ball, m_balls) {
        if (m_death) {
            // add fall
            ball->setVelocity(ball->velocity() + 
                QPointF(0, 0.002) * m_time.elapsed());
        }
        QPointF pos = ball->position();
        pos += ball->velocity() * m_time.elapsed();
        
        if (pos.y() >= 2.0 * height()) {
            delete ball;
            m_balls.removeAll(ball);
        }
        else {
            ball->setPosition(pos);
        }
    }
    
    m_time.restart();
}

void MainArea::mouseMoveEvent(QMouseEvent* e)
{
    if (!m_man) {
        m_man = new Ball(this, m_renderer, "blue_ball");
        m_man->show();
        kDebug() << "ball created" << endl;
    }
    
    if (!m_death) {
        m_man->setPosition(e->pos() - QPoint(m_renderer->size().width(),
                                        m_renderer->size().height()) / 2);
    }

}


#include "mainarea.moc"


