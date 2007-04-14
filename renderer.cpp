/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>
            
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#include "renderer.h"
#include <QImage>
#include <QPainter>
#include <ksvgrenderer.h>
#include <kstandarddirs.h>

Renderer::Renderer()
{
    m_renderer = new KSvgRenderer(
        KStandardDirs::locate("appdata", "pictures/theme.svgz"), 0);
}

QPixmap Renderer::render(const QString& id)
{
    if (!m_cache.contains(id)) {
        QImage tmp(m_size, QImage::Format_ARGB32_Premultiplied);
        tmp.fill(0);
        
        QPainter p(&tmp);
        m_renderer->render(&p, id, QRectF(QPointF(0, 0), m_size));
        
        m_cache[id] = QPixmap::fromImage(tmp);
    }
    
    return m_cache.value(id);
}


void Renderer::resize(const QSize& size)
{
    m_size = size;
    m_cache.clear();
}
