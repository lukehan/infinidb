/* Copyright (C) 2013 Calpont Corp.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

//
// $Id: command.cpp 2035 2013-01-21 14:12:19Z rdempsey $
// C++ Implementation: command
//
// Description: 
//
//
// Author: Patrick LeBlanc <pleblanc@calpont.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <unistd.h>

#include "bpp.h"
#include <typeinfo>

using namespace std;
using namespace messageqcpp;

namespace primitiveprocessor
{

Command::Command(CommandType c) : cmdType(c), fFilterFeeder(NOT_FEEDER) { }

Command::~Command() { };

void Command::createCommand(ByteStream &bs) 
{
	bs >> OID;
	bs >> tupleKey;
}

void Command::resetCommand(ByteStream &bs) { };

void Command::setMakeAbsRids(bool) { }

Command* Command::makeCommand(ByteStream &bs, CommandType *type, vector<SCommand>& cmds)
{
	Command* ret;
	uint8_t tmp8;

	bs.peek(tmp8);
	*type = (CommandType) tmp8;
	switch (*type) {
		case COLUMN_COMMAND:
			ret = new ColumnCommand();
			break;
		case DICT_STEP:
			ret = new DictStep();
			break;
		case PASS_THRU:
			ret = new PassThruCommand();
			break;
		case RID_TO_STRING:
			ret = new RTSCommand();
			break;
		case FILTER_COMMAND:
			ret = FilterCommand::makeFilterCommand(bs, cmds);
			break;
		default:
			throw logic_error("Command::makeCommand(): can't deserialize this bytestream");
	};
	ret->createCommand(bs);
	return ret;
}

void Command::setBatchPrimitiveProcessor(BatchPrimitiveProcessor *b) 
{
	bpp = b;
}

void Command::copyRidsForFilterCmd()
{
	if (fFilterFeeder == LEFT_FEEDER)
	{
		bpp->fFiltRidCount[0] = bpp->ridCount;
		for (uint64_t i = 0; i < bpp->ridCount; i++)
			bpp->fFiltCmdRids[0][i] = bpp->relRids[i];
	}
	else // if (fFilterFeeder == RIGHT_FEEDER)
	{
		bpp->fFiltRidCount[1] = bpp->ridCount;
		for (uint64_t i = 0; i < bpp->ridCount; i++)
			bpp->fFiltCmdRids[1][i] = bpp->relRids[i];
	}
}

bool Command::operator==(const Command &c) const
{
	const type_info &cType = typeid(c);

	if (cType != typeid(*this))
		return false;

	if (cType == typeid(ColumnCommand)) {
		const ColumnCommand *cc = dynamic_cast<const ColumnCommand *>(&c);
		const ColumnCommand *t = dynamic_cast<const ColumnCommand *>(this);
		if (*cc != *t)
			return false;
	}
	else if (cType == typeid(DictStep)) {
		const DictStep *ds = dynamic_cast<const DictStep *>(&c);
		const DictStep *t = dynamic_cast<const DictStep *>(this);
		if (*ds != *t)
			return false;
	}
	else if (cType == typeid(PassThruCommand)) {
		const PassThruCommand *pt = dynamic_cast<const PassThruCommand *>(&c);
		const PassThruCommand *t = dynamic_cast<const PassThruCommand *>(this);
		if (*pt != *t)
			return false;
	}
	else if (cType == typeid(RTSCommand)) {
		const RTSCommand *rts = dynamic_cast<const RTSCommand *>(&c);
		const RTSCommand *t = dynamic_cast<const RTSCommand *>(this);
		if (*rts != *t)
			return false;
	}
	else if (cType == typeid(FilterCommand)) {
		const FilterCommand *fc = dynamic_cast<const FilterCommand *>(&c);
		const FilterCommand *t = dynamic_cast<const FilterCommand *>(this);
		if (*fc != *t)
			return false;
	}
	else
		cerr << "unknown Command type\n";

	return true;
}

bool Command::operator!=(const Command &c) const
{
	return !(*this == c);
}

uint32_t Command::getOID() const
{
	return OID;
}

uint32_t Command::getTupleKey() const
{
	return tupleKey;
}

void Command::duplicate(Command *c)
{
	bpp = c->bpp;
	cmdType = c->cmdType;
	fFilterFeeder = c->fFilterFeeder;
	OID = c->OID;
	tupleKey = c->tupleKey;
}

void Command::getLBIDList(uint count, vector<int64_t> *out)
{
}

};
