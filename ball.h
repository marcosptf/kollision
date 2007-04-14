/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef BALL_H
#define BALL_H

#include <kgamecanvas.h>
#include <QPointF>

class Renderer;

class Ball : public KGameCanvasPixmap
{
    QString m_id;
    QPointF m_position;
    QPointF m_velocity;
    double m_opacityF;
    double m_radius;
public:
    Ball(KGameCanvasAbstract* parent, Renderer* renderer, const QString& id);
    
    void update(Renderer* renderer);
    const QPointF& velocity() const { return m_velocity; }
    void setVelocity(const QPointF& vel) { m_velocity = vel; }
    
    const QPointF& position() const { return m_position; }
    void setPosition(const QPointF& p);
    
    double opacityF() const { return m_opacityF; }
    void setOpacityF(double val) { m_opacityF = val; setOpacity((int)(val * 255)); }
    
    double radius() const { return m_radius; }
};

#endif // BALL_H
