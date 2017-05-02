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

#ifndef QTEKPOWERDMM_H
#define QTEKPOWERDMM_H 1

#include <QtCore/QObject>
#include <TekPowerDMM.h>

class QIODevice;

class QTekPowerDMM : public QObject, public TekPowerDMM
{
	Q_OBJECT
public:
	QTekPowerDMM(QIODevice *device, QObject *parent = Q_NULLPTR);

	bool isValid() const;

Q_SIGNALS:
	void featuresChanged(uint8_t flags);
	void minMaxChanged(uint8_t flags);
	void modeChanged(TekPowerDMM::Mode prev, TekPowerDMM::Mode value);
	void rangeChanged(TekPowerDMM::Range prev, TekPowerDMM::Range value);
	void valueChanged(const QString value);

protected: /* change notifications */
	void setFeatures(uint8_t flags) Q_DECL_OVERRIDE;
	void setMinMax(uint8_t flags) Q_DECL_OVERRIDE;
	void setMode(Mode value) Q_DECL_OVERRIDE;
	void setRange(Range value) Q_DECL_OVERRIDE;
	void setValue(const char value[6]) Q_DECL_OVERRIDE;

protected Q_SLOTS:
	void onReadyRead();

private:
	QIODevice *m_device;
};

Q_DECLARE_METATYPE(TekPowerDMM::Mode);
Q_DECLARE_METATYPE(TekPowerDMM::Range);

#endif // QTEKPOWERDMM_H
