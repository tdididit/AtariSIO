/*
   FileIO.cpp - wrapper classes for handling file IO

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

#include <unistd.h>

#include "FileIO.h"
#include "AtariDebug.h"


FileIO::FileIO()
{
}

FileIO::~FileIO()
{
}

bool FileIO::ReadByte(unsigned char& byte)
{
	return ReadBlock(&byte, 1) == 1;
}

bool FileIO::WriteByte(const unsigned char& byte)
{
	return WriteBlock(&byte, 1) == 1;
}

bool FileIO::ReadWord(unsigned int& word)
{
	unsigned char buf[2];

	if (ReadBlock(buf,2) != 2) {
		return false;
	}
	word = buf[0] | (buf[1] << 8);
	return true;
}

bool FileIO::WriteWord(const unsigned int& word)
{
	unsigned char buf[2];

	buf[0] = word & 0xff;
	buf[1] = word >> 8;

	if (WriteBlock(buf,2) != 2) {
		return false;
	}
	return true;
}

bool FileIO::ReadBigEndianWord(unsigned int& word)
{
	unsigned char buf[2];

	if (ReadBlock(buf,2) != 2) {
		return false;
	}
	word = (buf[0] << 8) | buf[1];
	return true;
}

bool FileIO::WriteBigEndianWord(const unsigned int& word)
{
	unsigned char buf[2];

	buf[0] = word >> 8;
	buf[1] = word & 0xff;

	if (WriteBlock(buf,2) != 2) {
		return false;
	}
	return true;
}

bool FileIO::Unlink(const char* filename)
{
	return unlink(filename) == 0;
}

StdFileIO::StdFileIO()
	: super(), fFile(0)
{
}

StdFileIO::~StdFileIO()
{
	if (IsOpen()) {
		Close();
		AssertMsg(false,"file not closed at ~StdFileIO!");
	}
}

bool StdFileIO::OpenRead(const char* filename)
{
	if (IsOpen()) {
		Assert(false);
		return false;
	}

	fFile = fopen(filename,"rb");
	return IsOpen();
}

bool StdFileIO::OpenWrite(const char* filename)
{
	if (IsOpen()) {
		Assert(false);
		return false;
	}

	fFile = fopen(filename,"wb");
	return IsOpen();
}

bool StdFileIO::Close()
{
	if (!IsOpen()) {
		Assert(false);
		return false;
	}

	bool ret = (fclose(fFile) == 0);
	fFile = 0;
	return ret;
}


unsigned int StdFileIO::ReadBlock(void* buf, unsigned int len)
{
	if (!IsOpen()) {
		Assert(false);
		return 0;
	}
	return fread(buf, 1, len, fFile);
}

unsigned int StdFileIO::WriteBlock(const void* buf, unsigned int len)
{
	if (!IsOpen()) {
		Assert(false);
		return 0;
	}
	return fwrite(buf, 1, len, fFile);
}

unsigned int StdFileIO::GetFileLength()
{
	if (!IsOpen()) {
		Assert(false);
		return 0;
	}
	unsigned int current_pos = ftell(fFile);
	fseek(fFile, 0, SEEK_END);
	long len = ftell(fFile);
	fseek(fFile, current_pos, SEEK_SET);
	return len;
}

unsigned int StdFileIO::Tell()
{
	if (!IsOpen()) {
		Assert(false);
		return 0;
	}
	return ftell(fFile);
}

bool StdFileIO::Seek(unsigned int pos)
{
	if (!IsOpen()) {
		Assert(false);
		return false;
	}
	if (fseek(fFile, pos, SEEK_SET) != 0) {
		return false;
	}
	return true;
}

bool StdFileIO::IsOpen() const
{
	return fFile != 0;
}

#ifdef USE_ZLIB

GZFileIO::GZFileIO()
	: super(), fFile(0)
{
}

GZFileIO::~GZFileIO()
{
	if (IsOpen()) {
		Close();
		AssertMsg(false,"file not closed at ~GZFileIO!");
	}
}

bool GZFileIO::OpenRead(const char* filename)
{
	if (IsOpen()) {
		Assert(false);
		return false;
	}

	fFile = gzopen(filename,"rb");
	return IsOpen();
}

bool GZFileIO::OpenWrite(const char* filename)
{
	if (IsOpen()) {
		Assert(false);
		return false;
	}

	fFile = gzopen(filename,"wb");
	return IsOpen();
}

bool GZFileIO::Close()
{
	if (!IsOpen()) {
		Assert(false);
		return false;
	}

	bool ret = (gzclose(fFile) == 0);
	fFile = 0;
	return ret;
}


unsigned int GZFileIO::ReadBlock(void* buf, unsigned int len)
{
	if (!IsOpen()) {
		Assert(false);
		return 0;
	}
	return gzread(fFile, buf, len);
}

unsigned int GZFileIO::WriteBlock(const void* buf, unsigned int len)
{
	if (!IsOpen()) {
		Assert(false);
		return 0;
	}
	// workaround for const-warning with zlib
	return gzwrite(fFile, const_cast<void*>(buf), len);
}

unsigned int GZFileIO::GetFileLength()
{
	if (!IsOpen()) {
		Assert(false);
		return 0;
	}
	unsigned int current_pos = gztell(fFile);
	unsigned int len = current_pos;
	unsigned int s;

	unsigned char buf[256];

	while ( (s = gzread(fFile,buf,256)) ) {
		len += s;
	}

	gzseek(fFile, current_pos, SEEK_SET); 
	return len;
}

unsigned int GZFileIO::Tell()
{
	if (!IsOpen()) {
		Assert(false);
		return 0;
	}
	return gztell(fFile);
}

bool GZFileIO::Seek(unsigned int pos)
{
	if (!IsOpen()) {
		Assert(false);
		return false;
	}
	if (gzseek(fFile, pos, SEEK_SET) != (int)pos) {
		return false;
	}
	return true;
}

bool GZFileIO::IsOpen() const
{
	return fFile != 0;
}

#endif
