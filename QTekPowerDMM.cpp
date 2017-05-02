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

#include <QTekPowerDMM.h>

#include <QtCore/QIODevice>

QTekPowerDMM::QTekPowerDMM(QIODevice *device, QObject *parent)
    : QObject(parent)
    , m_device(device)
{
	Q_ASSERT(m_device);
	connect(m_device, &QIODevice::readyRead, this, &QTekPowerDMM::onReadyRead);
}

bool QTekPowerDMM::isValid() const
{
	return m_device->isOpen();
}

void QTekPowerDMM::setValue(const char value[6])
{
	TekPowerDMM::setValue(value);

	QString v = QString::fromLatin1(TekPowerDMM::value());
	if (precision() > 0)
		v.insert(1 + precision(), '.');
	Q_EMIT valueChanged(v);
}

void QTekPowerDMM::setFeatures(uint8_t flags)
{
	TekPowerDMM::setFeatures(flags);
	Q_EMIT featuresChanged(flags);
}

void QTekPowerDMM::setMinMax(uint8_t flags)
{
	TekPowerDMM::setMinMax(flags);
	Q_EMIT minMaxChanged(flags);
}

void QTekPowerDMM::setMode(TekPowerDMM::Mode value)
{
	const Mode prev = mode();
	TekPowerDMM::setMode(value);
	Q_EMIT modeChanged(prev, value);
}

void QTekPowerDMM::setRange(TekPowerDMM::Range value)
{
	const Range prev = range();
	TekPowerDMM::setRange(value);
	Q_EMIT rangeChanged(prev, value);
}

void QTekPowerDMM::onReadyRead()
{
	QByteArray payload = m_device->readLine();
	if (payload.size() > 10) {
#ifdef VERBOSE
		qDebug(qPrintable(payload.toHex()));
#endif
		processPayload(payload.constData());
	}
}

