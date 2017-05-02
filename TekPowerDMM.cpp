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

#include "TekPowerDMM.h"

#include <cstring>

#define TPDMM_UNUSED(x) (void) x

TekPowerDMM::TekPowerDMM()
    : m_mode(UnknownMode)
    , m_range(NoRange)
    , m_precision(0)
    , m_features(NoFeatures)
    , m_minMax(NoMinMax)
{
	std::memset(m_value, 0, sizeof(m_value));
}

void TekPowerDMM::processPayload(const char *data)
{
	static const uint8_t kKnownModeMask =
	    FTemperatureMode | CTemperatureMode | CapacitanceMode | OhmMode |
	    AmperimeterMode | VoltimeterMode;
	const Mode mode = static_cast<Mode>(data[10] & kKnownModeMask);
	if (mode != m_mode)
		setMode(mode);

	static const uint8_t kKnownRangeMask =
	    DiodeRange | ContinuityRange | MegaRange | KiloRange |
	    MilliRange | MicroRange;
	const Range range = static_cast<Range>(data[9] & kKnownRangeMask);
	if (range != m_range)
		setRange(range);

	static const uint8_t kKnownMinMaxMask = MinMinMax | MaxMinMax;
	const uint8_t minMax = data[8] & kKnownMinMaxMask;
	if (minMax != m_minMax)
		setMinMax(minMax);

	static const uint8_t kKnownFeaturesMask =
	    HoldFeature | RelativeFeature | ACFeature | DCFeature |
	    AutoRangeFeature;
	const uint8_t features = data[7] & kKnownFeaturesMask;
	if (features != m_features)
		setFeatures(features);

	const int precision = data[6] & 0x3;
	if (precision != m_precision)
		setPrecision(precision);

	setValue(data);
}

const char * TekPowerDMM::value() const
{
	return m_value;
}

uint8_t TekPowerDMM::features() const
{
	return m_features;
}

uint8_t TekPowerDMM::minMax() const
{
	return m_minMax;
}

TekPowerDMM::Mode TekPowerDMM::mode() const
{
	return m_mode;
}

TekPowerDMM::Range TekPowerDMM::range() const
{
	return m_range;
}

int TekPowerDMM::precision() const
{
	return m_precision;
}

void TekPowerDMM::setFeatures(uint8_t flags)
{
	m_features = flags;
}

void TekPowerDMM::setMinMax(uint8_t flags)
{
	m_minMax = flags;
}

void TekPowerDMM::setMode(TekPowerDMM::Mode value)
{
	m_mode = value;
}

void TekPowerDMM::setPrecision(int value)
{
	m_precision = value;
}

void TekPowerDMM::setValue(const char value[6])
{
	std::strncpy(m_value, value, 5);
}

void TekPowerDMM::setRange(TekPowerDMM::Range value)
{
	m_range = value;
}

