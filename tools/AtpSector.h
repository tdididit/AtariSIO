#ifndef ATPSECTOR_H
#define ATPSECTOR_H

/*
   AtpSector.h - data of an sector according to the ATP format

   Copyright (C) 2003, 2004 Matthias Reichl <hias@horus.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <iostream>

#include "RCPtr.h"
#include "RefCounted.h"
#include "ChunkReader.h"
#include "ChunkWriter.h"

class AtpSector: public RefCounted {
public:
	AtpSector(
		unsigned int id,
		unsigned int data_len,
		const unsigned char* data,
		unsigned int pos,
		unsigned int time_len,
		unsigned char status=255);

	virtual ~AtpSector();

	// get sector ID
	inline unsigned int GetID() const;

	// get absolute sector position on track (in usec)
	inline unsigned int GetPosition() const;

	// get time length of sector on track (in usec)
	inline unsigned int GetTimeLength() const;

	// get error status of sector
	inline unsigned char GetSectorStatus() const;

	// set error status of sector
	inline void SetSectorStatus(unsigned char status);

	// get lenght of data block
	inline unsigned int GetDataLength() const;

	// get data block of sector
	bool GetData(unsigned char* dest, unsigned int len) const;

	inline const unsigned char* GetDataPtr() const;

	// set data block of sector
	bool SetData(const unsigned char* src, unsigned int len);
	
	void Dump(std::ostream& os, unsigned int indentlevel=0) const;

	// create ATP "SECT" chunk from internal data
	RCPtr<ChunkWriter> BuildSectorChunk() const;

	// set internal data from ATP "SECT" chunk
	bool InitFromSectorChunk(RCPtr<ChunkReader> chunk, bool beQuiet);

private:
	friend class AtpTrack;
	AtpSector();
	inline void SetPositionAndTimeLength(unsigned int pos, unsigned int time_len);

private:
	unsigned int fID;
	unsigned int fDataLength;
	unsigned char* fSectorData;
	unsigned int fPosition;
	unsigned int fTimeLength;
	unsigned char fSectorStatus;
};

inline unsigned int AtpSector::GetPosition() const
{
	return fPosition;
}

inline unsigned int AtpSector::GetID() const
{
	return fID;
}

inline unsigned char AtpSector::GetSectorStatus() const
{
	return fSectorStatus;
}

inline void AtpSector::SetSectorStatus(unsigned char status)
{
	fSectorStatus = status;
}

inline unsigned int AtpSector::GetDataLength() const
{
	return fDataLength;
}

inline unsigned int AtpSector::GetTimeLength() const
{
	return fTimeLength;
}

inline const unsigned char* AtpSector::GetDataPtr() const
{
	return fSectorData;
}

inline void AtpSector::SetPositionAndTimeLength(unsigned int pos, unsigned int time_len)
{
	fPosition = pos;
	fTimeLength = time_len;
}

#endif
