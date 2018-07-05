/*
  This file is part of the DMX library.
  Copyright (C) 2018  Arduino AG (http://www.arduino.cc/)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <RS485.h>

#include "DMX.h"

DMXClass::DMXClass() :
  _universeSize(0),
  _transmissionBegin(false)
{
}

int DMXClass::begin(int universeSize)
{
  if (universeSize < 1 || universeSize > DMX_MAX_SLOTS) {
    return 0;
  }

  _universeSize = universeSize;
  _transmissionBegin = false;

  RS485.begin(250000, SERIAL_8N2);
  RS485.beginTransmission();

  return 1;
}

void DMXClass::end()
{
  RS485.endTransmission();
  RS485.end();
}

int DMXClass::beginTransmission()
{
  _transmissionBegin = true;
  _address = 1;

  memset(_values, 0x00, _universeSize);

  return 1;
}

int DMXClass::write(int address, byte value)
{
  if (!_transmissionBegin || address < 1 || address > _universeSize) {
    return 0;
  }

  _values[address] = value;

  return 1;
}

int DMXClass::write(byte value)
{
  if (!write(_address, value)) {
    return 0;
  }

  _address++;

  return 1;
}

int DMXClass::writeAll(byte value)
{
  memset(_values + 1, value, _universeSize);

  return 1;
}

int DMXClass::endTransmission()
{
  RS485.sendBreakMicroseconds(88);
  delayMicroseconds(12);

  RS485.write(_values, _universeSize + 1);

  _transmissionBegin = false;

  return 1;
}

DMXClass DMX;
