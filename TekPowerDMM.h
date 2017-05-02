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

#ifndef TEKPOWERDMM_H
#define TEKPOWERDMM_H 1

#include <cstdint>

class TekPowerDMM
{
/*
 * TekPowerDMM is currently TP9605BT specific.
 */
public:
	enum Mode
	{
		FTemperatureMode = (1 << 0),
		CTemperatureMode = (1 << 1),
		CapacitanceMode  = (1 << 2),
		OhmMode          = (1 << 5),
		AmperimeterMode  = (1 << 6),
		VoltimeterMode   = (1 << 7),
		UnknownMode      = 0
	};

	enum Range
	{
		DiodeRange      = (1 << 2),
		ContinuityRange = (1 << 3),
		MegaRange       = (1 << 4),
		KiloRange       = (1 << 5),
		MilliRange      = (1 << 6),
		MicroRange      = (1 << 7),
		NoRange         = 0
	};

	enum Features
	{
		HoldFeature       = (1 << 1),
		RelativeFeature   = (1 << 2),
		ACFeature         = (1 << 3),
		DCFeature         = (1 << 4),
		AutoRangeFeature  = (1 << 5),
		NoFeatures        = 0
	};

	enum MinMax
	{
		MinMinMax  = (1 << 4),
		MaxMinMax  = (1 << 5),
		NoMinMax   = 0
	};

public:
	TekPowerDMM();

	uint8_t features() const;
	uint8_t minMax() const;
	Mode mode() const;
	int precision() const;
	Range range() const;
	const char * value() const;

protected:
	void processPayload(const char *data);

protected: /* change notifications */
	virtual void setFeatures(uint8_t flags);
	virtual void setMinMax(uint8_t flags);
	virtual void setMode(Mode value);
	virtual void setPrecision(int value);
	virtual void setRange(Range value);
	virtual void setValue(const char value[6]);

private:
	Mode m_mode;
	Range m_range;
	int m_precision;
	char m_value[6];
	uint8_t m_features;
	uint8_t m_minMax;
};

#endif // TEKPOWERDMM_H
