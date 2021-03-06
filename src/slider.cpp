/***************************************************************************
 *            slider.cpp
 *
 *  Mon Feb 24 12:00:00 CEST 2014
 *  Copyright 2014 Lars Bisballe Jensen
 *  larsbjensen@gmail.com
 ****************************************************************************/
/*
 *  This file is part of Visutest.
 *
 *  Visutest is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Visutest is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Visutest; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#define DEBUG

#include "slider.h"

#include <stdio.h>
#include <QHBoxLayout>

Slider::Slider(QSettings *config, QString group, QString name,
	       int minValue, int maxValue, int stdValue, int step, QWidget *parent)
  : QWidget(parent)
{
  this->config = config;
  this->group = group;
  this->name = name;
  this->defaultValue = stdValue;
  
  QHBoxLayout *layout = new QHBoxLayout(this);
  valueSlider = new QSlider(Qt::Horizontal, this);
  valueSlider->installEventFilter(parent);
  valueSlider->setSingleStep(step);
  valueSlider->setMinimum(minValue);
  valueSlider->setMaximum(maxValue);
  valueLineEdit = new QLineEdit(this);
  valueLineEdit->setFocusPolicy(Qt::NoFocus);
  valueLineEdit->setReadOnly(true);
  valueLineEdit->setMaximumWidth(40);
  layout->addWidget(valueSlider);
  layout->addWidget(valueLineEdit);
  setLayout(layout);

  connect(valueSlider, &QSlider::valueChanged, this, &Slider::saveToConfig);

  if(!group.isEmpty())
    config->beginGroup(group);

  if(!config->contains(name)) {
    config->setValue(name, stdValue);
  }
  valueLineEdit->setText(QString::number(valueSlider->value())); // Hack to make sure the initial value is displayed, even if it's equal to minValue in which case valueChanged is never emitted on construction
  valueSlider->setValue(config->value(name, stdValue).toInt());
  if(!group.isEmpty())
    config->endGroup();
}

Slider::~Slider()
{
}

void Slider::resetToDefault()
{
  valueSlider->setValue(defaultValue);
}

void Slider::saveToConfig()
{
  printf("SLIDER VALUE: %d\n", valueSlider->value());
  valueLineEdit->setText(QString::number(valueSlider->value()));

  QString value = valueLineEdit->text();

  if(value.contains(",")) {
    value.replace(",", ".");
  }

  config->setValue((group.isEmpty()?"":group + "/") + name, value);

  qDebug("Key '%s' saved to config with value '%s'\n", name.toStdString().c_str(),
	 value.toStdString().c_str());
}
