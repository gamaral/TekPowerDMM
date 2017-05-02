/*
 * Copyright (c) 2017, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "DMMDisplay.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtWidgets/QMessageBox>

#include "QTekPowerDMM.h"

#include "ui_DMMDisplay.h"

#define TEKPOWER_VID 0x1A86
#define TEKPOWER_PID 0x7523

DMMDisplay::DMMDisplay(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui_DMMDisplay)
    , m_dmm(Q_NULLPTR)
{
	m_ui->setupUi(this);

	connectToDMM();

	startTimer(2000);
}

void DMMDisplay::clearDisplay()
{
	onFeaturesChanged(TekPowerDMM::NoFeatures);
	onMinMaxChanged(TekPowerDMM::NoMinMax);

	m_ui->lAmp->setEnabled(false);
	m_ui->lCelsius->setEnabled(false);
	m_ui->lFahrenheit->setEnabled(false);
	m_ui->lFarad->setEnabled(false);
	m_ui->lNano->setEnabled(false);
	m_ui->lOhm->setEnabled(false);
	m_ui->lVolt->setEnabled(false);

	m_ui->lCont->setEnabled(false);
	m_ui->lDiode->setEnabled(false);
	m_ui->lKilo->setEnabled(false);
	m_ui->lMega->setEnabled(false);
	m_ui->lMicro->setEnabled(false);
	m_ui->lMilli->setEnabled(false);

	onValueChanged(QStringLiteral("0"));

	delete m_dmm;
}

void DMMDisplay::connectToDMM()
{
	clearDisplay();

	QSerialPortInfo pinfo;
	for (QSerialPortInfo &p : QSerialPortInfo::availablePorts()) {
		qDebug("%s: %s (%x:%x)", qPrintable(p.portName().data()), qPrintable(p.description().data()), p.productIdentifier(), p.vendorIdentifier());
		if (TEKPOWER_VID == p.vendorIdentifier() && TEKPOWER_PID == p.productIdentifier()) {
			qDebug("TekPower DMM Found!");
			pinfo = p;
			break;
		}
	}

	if (pinfo.isNull()) {
		qWarning("TekPower DMM Not Found!");
		onValueChanged(QStringLiteral("404"));
		return;
	}

	QSerialPort *port = new QSerialPort(pinfo.portName());
	port->setBaudRate(QSerialPort::Baud9600);
	if (!port->open(QIODevice::ReadOnly)) {
		qWarning("Failed to open DMM serial port!");
		onValueChanged(QStringLiteral("401"));
		return;
	}

	m_dmm = new QTekPowerDMM(port, this);
	connect(m_dmm, SIGNAL(featuresChanged(uint8_t)), this, SLOT(onFeaturesChanged(uint8_t)));
	connect(m_dmm, SIGNAL(minMaxChanged(uint8_t)), this, SLOT(onMinMaxChanged(uint8_t)));
	connect(m_dmm, SIGNAL(modeChanged(TekPowerDMM::Mode, TekPowerDMM::Mode)), this, SLOT(onModeChanged(TekPowerDMM::Mode, TekPowerDMM::Mode)));
	connect(m_dmm, SIGNAL(rangeChanged(TekPowerDMM::Range, TekPowerDMM::Range)), this, SLOT(onRangeChanged(TekPowerDMM::Range, TekPowerDMM::Range)));
	connect(m_dmm, SIGNAL(valueChanged(QString)), this, SLOT(onValueChanged(QString)));
	port->setParent(m_dmm);
}

void DMMDisplay::onValueChanged(const QString &value)
{
	m_ui->lcdNumber->display(value);
}

void DMMDisplay::onFeaturesChanged(uint8_t flags)
{
	m_ui->lAC->setEnabled(flags & TekPowerDMM::ACFeature);
	m_ui->lAuto->setEnabled(flags & TekPowerDMM::AutoRangeFeature);
	m_ui->lDC->setEnabled(flags & TekPowerDMM::DCFeature);
	m_ui->lHold->setEnabled(flags & TekPowerDMM::HoldFeature);
	m_ui->lRel->setEnabled(flags & TekPowerDMM::RelativeFeature);
}

void DMMDisplay::onMinMaxChanged(uint8_t flags)
{
	m_ui->lMax->setEnabled(flags & TekPowerDMM::MaxMinMax);
	m_ui->lMin->setEnabled(flags & TekPowerDMM::MinMinMax);
}

void DMMDisplay::onModeChanged(TekPowerDMM::Mode prev, TekPowerDMM::Mode value)
{
	switch (prev) {
	case TekPowerDMM::FTemperatureMode:
		m_ui->lFahrenheit->setEnabled(false);
		break;
	case TekPowerDMM::CTemperatureMode:
		m_ui->lCelsius->setEnabled(false);
		break;
	case TekPowerDMM::CapacitanceMode:
		m_ui->lFarad->setEnabled(false);
		m_ui->lNano->setEnabled(false);
		break;
	case TekPowerDMM::OhmMode:
		m_ui->lOhm->setEnabled(false);
		break;
	case TekPowerDMM::AmperimeterMode:
		m_ui->lAmp->setEnabled(false);
		break;
	case TekPowerDMM::VoltimeterMode:
		m_ui->lVolt->setEnabled(false);
		break;
	case TekPowerDMM::UnknownMode:
		/* NOTHING */
		break;
	}

	switch (value) {
	case TekPowerDMM::FTemperatureMode:
		m_ui->lFahrenheit->setEnabled(true);
		break;
	case TekPowerDMM::CTemperatureMode:
		m_ui->lCelsius->setEnabled(true);
		break;
	case TekPowerDMM::CapacitanceMode:
		m_ui->lFarad->setEnabled(true);
		m_ui->lNano->setEnabled(true);
		break;
	case TekPowerDMM::OhmMode:
		m_ui->lOhm->setEnabled(true);
		break;
	case TekPowerDMM::AmperimeterMode:
		m_ui->lAmp->setEnabled(true);
		break;
	case TekPowerDMM::VoltimeterMode:
		m_ui->lVolt->setEnabled(true);
		break;
	case TekPowerDMM::UnknownMode:
		/* NOTHING */
		break;
	}
}

void DMMDisplay::onRangeChanged(TekPowerDMM::Range prev, TekPowerDMM::Range value)
{
	switch (prev) {
	case TekPowerDMM::MegaRange:
		m_ui->lMega->setEnabled(false);
		break;
	case TekPowerDMM::KiloRange:
		m_ui->lKilo->setEnabled(false);
		break;
	case TekPowerDMM::MilliRange:
		m_ui->lMilli->setEnabled(false);
		break;
	case TekPowerDMM::MicroRange:
		m_ui->lMicro->setEnabled(false);
		break;
	case TekPowerDMM::DiodeRange:
		m_ui->lDiode->setEnabled(false);
		break;
	case TekPowerDMM::ContinuityRange:
		m_ui->lCont->setEnabled(false);
		break;
	case TekPowerDMM::NoRange:
		/* NOTHING */
		break;
	}

	switch (value) {
	case TekPowerDMM::MegaRange:
		m_ui->lMega->setEnabled(true);
		break;
	case TekPowerDMM::KiloRange:
		m_ui->lKilo->setEnabled(true);
		break;
	case TekPowerDMM::MilliRange:
		m_ui->lMilli->setEnabled(true);
		break;
	case TekPowerDMM::MicroRange:
		m_ui->lMicro->setEnabled(true);
		break;
	case TekPowerDMM::DiodeRange:
		m_ui->lDiode->setEnabled(true);
		break;
	case TekPowerDMM::ContinuityRange:
		m_ui->lCont->setEnabled(true);
		break;
	case TekPowerDMM::NoRange:
		/* NOTHING */
		break;
	}
}

void DMMDisplay::timerEvent(QTimerEvent *event)
{
	Q_UNUSED(event);

	if (!m_dmm || !m_dmm->isValid())
		connectToDMM();
}

